#ifndef ZR_ACTIONABLE_HPP
#define ZR_ACTIONABLE_HPP
#include "../Common.hpp"
#include "../Usings.hpp"
struct Actionable
{
    enum class Action
    {
        RUNUP,
        RUNDOWN,
        RUNLEFT,
        RUNRIGHT
    };

    Actionable()
    {
        actions[Actionable::Action::RUNLEFT] = false;

        actions[Actionable::Action::RUNRIGHT] = false;

        actions[Actionable::Action::RUNDOWN] = false;

        actions[Actionable::Action::RUNUP] = false;
    }
    bool isRunning()
    {

        return actions[Actionable::Action::RUNLEFT] || actions[Actionable::Action::RUNRIGHT] ||
               actions[Actionable::Action::RUNDOWN] || actions[Actionable::Action::RUNUP];
    }

    std::map<Action, bool> actions;
    ::Vec2f lookAt;
};
#endif
