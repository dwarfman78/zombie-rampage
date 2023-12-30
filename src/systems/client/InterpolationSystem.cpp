#include "../../../include/systems/client/InterpolationSystem.hpp"
void InterpolationSystem::configure(entityx::EventManager &event_manager)
{
    event_manager.subscribe<NetworkEvent>(*this);
}
void InterpolationSystem::update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt)
{
    // We need at least one world state in the buffer.
    if (mWorldStateBuffer.size() >= 1)
    {
        // Get the next world state in the queue.
        auto &worldState = mWorldStateBuffer.front();

        // Send event for debugging purpose.
        InterpolationEvent ie;
        ie.currentWorldState = worldState;
        events.emit<InterpolationEvent>(ie);

        // We need at least two entities (one is the player + other players)
        if (worldState.size() > 1)
        {
            // We are going to interpolate all entities in one pass.
            // so we keep a counter of how many entities are interpolated.
            int nInterpolated = 0;

            // For each entity key is server side entityId
            // value is a tuple of components (renderable,movable,networkable...)
            for (auto &[key, value] : worldState)
            {
                // We convert distant entityId to localEntityId.
                auto localEntityId = mDistantToLocalEntitiesIds[key];

                // Check if local entity is still valid and is not the player entity.
                if (entities.valid(localEntityId) && !entities.has_component<Playable>(localEntityId))
                {
                    // Unwrap the tuple.
                    auto &[stateRenderable, stateMovable, stateNetworkable] = value;

                    // Retrieve current local entity in order to get components.
                    auto entity = entities.get(localEntityId);

                    // Get component handles from entity. Handles act like a reference.
                    auto renderable = entity.component<Renderable>();
                    auto movable = entity.component<Movable>();

                    // Get the vector between local entity position and server entity position.
                    sf::Vector2f vOldNewPos = stateRenderable.mPos - renderable->mPos;

                    // Sqr Length of that vector.
                    float vOldNewPosReference = vOldNewPos.lengthSq();

                    // If not interpolated already and
                    // there's a significant distance between local and server entity position we
                    // need to interpolate.
                    if (!stateNetworkable.interpolated && vOldNewPosReference >= 9)
                    {
                        // Check if we have already started to interpolate this entity but
                        // it has not yet reached the target.
                        if (!stateNetworkable.interpolating)
                        {
                            stateNetworkable.interpolating = true;

                            // Sqr length of distant entity acceleration is our target acceleration
                            // to be applied along the vOldNewPosVector.
                            float referenceAcceleration = stateMovable.mAcceleration.lengthSq();

                            // If the distant entity has stopped
                            if (!referenceAcceleration > 0)
                            {
                                // we keep the last referenceAcceleration.
                                referenceAcceleration = movable->mAcceleration.lengthSq();
                            }

                            // Maybe both current and next state have 0 acceleration.
                            if (referenceAcceleration > 0)
                            {
                                // We check if distant acceleration and the vector
                                // between local and distant entity are in the same
                                // direction if not we have gone too far.
                                if (vOldNewPos.dot(stateMovable.mAcceleration) < 0)
                                {
                                    // If it is a deceleration
                                    if (stateMovable.mAcceleration.lengthSq() < movable->mAcceleration.lengthSq())
                                    {
                                        movable->mAcceleration = stateMovable.mAcceleration;
                                    }
                                    stateNetworkable.interpolated = true;
                                    nInterpolated++;
                                }
                                else
                                {
                                    // k is the ratio between the lengthSqr of the vector
                                    // between local and distant entity (vOldNewPos) and the acceleration
                                    // vector of the distant entity.
                                    // When k is applied to vOldNewPos we retrieve a vector with the
                                    // same length of the distant acceleration vector but along vOldNewPos
                                    float k = 0.f;

                                    // We have two cases possible, if the two states are closer
                                    // or farther regarding the length of the acceleration vector.
                                    // we need k superior to 1.
                                    if (vOldNewPosReference > referenceAcceleration)
                                    {
                                        k = vOldNewPosReference / referenceAcceleration;
                                    }
                                    else
                                    {
                                        k = referenceAcceleration / vOldNewPosReference;
                                    }

                                    // New acceleration applied to local entity.
                                    sf::Vector2f newAcc;

                                    // Need to sqrt k for the ratio to be correct.
                                    newAcc = vOldNewPos * (float)(sqrt(k));

                                    // Define the new acceleration along vOldNewPos.
                                    movable->mAcceleration = newAcc;
                                }
                            }
                            else
                            {
                                stateNetworkable.interpolated = true;
                                movable->mAcceleration = {0.f, 0.f};
                                nInterpolated++;
                            }
                        }
                    }
                    else
                    {
                        stateNetworkable.interpolated = true;
                        movable->mAcceleration = {0.f, 0.f};
                        nInterpolated++;
                    }
                }
            }
            // when every entity (except the player) is interpolated,
            // we discard current worldState, front of queue will be
            // the next received worldState.
            if (nInterpolated == worldState.size() - 1)
            {
                std::cout << "Next worldStep... "
                          << "Remaining worldSteps in buffer : " << mWorldStateBuffer.size() << std::endl;
                mWorldStateBuffer.pop();
            }
        }
        else
        {
            while (mWorldStateBuffer.size() > 0)
            {
                mWorldStateBuffer.pop();
            }
        }
    }
}
void InterpolationSystem::receive(const NetworkEvent &event)
{
    if (event.type == NetworkEvent::Type::WorldState)
    {
        if (event.serverTick > mLastServerTick)
        {
            mWorldStateBuffer.push(event.worldState);
            mLastServerTick = event.serverTick;
        }
    }
    if (event.type == NetworkEvent::Type::EntityCreation)
    {
        mDistantToLocalEntitiesIds[event.serverEntityId] = event.clientEntityId;
        mLocalToDistantEntitiesIds[event.clientEntityId] = event.serverEntityId;
    }
    if (event.type == NetworkEvent::Type::EntitySuppression)
    {
        auto localId = mDistantToLocalEntitiesIds[event.serverEntityId];
        mLocalToDistantEntitiesIds.erase(localId);
        mDistantToLocalEntitiesIds.erase(event.serverEntityId);
    }
}
