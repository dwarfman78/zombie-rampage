#ifndef ZR_ANIMABLE_HPP
#define ZR_ANIMABLE_HPP
#include "../Common.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <optional>
class Animation
{
  public:
    Animation()
    {
    }
    Animation(float pDuration, std::vector<sf::FloatRect> pFrames)
    {
        mFrames = pFrames;
        mDuration = pDuration;

        if (pFrames.size() > 0)
        {
            mCurrentFrame = mFrames.begin();
            mFrameDuration = pDuration / mFrames.size();
        }
        else
        {
            mCurrentFrame = mFrames.end();
        }
    }

    void start()
    {
        mRunning = true;
    }

    void pause()
    {
        mRunning = false;
    }

    void stop()
    {
        mRunning = false;
        mDisplayTime = 0.f;
    }

    void addDisplayTime(float pDt)
    {
        mDisplayTime += pDt;
    }
    float getDisplayTime() const
    {
        return mDisplayTime;
    }
    float getFrameDuration() const
    {
        return mFrameDuration;
    }
    void nextFrame()
    {
        if (mFrames.size() > 1 && mRunning)
        {
            if (mCurrentFrame == mFrames.end())
                mCurrentFrame = mFrames.begin();
            else
                mCurrentFrame++;
            mDisplayTime = 0.f;
        }
    }
    std::optional<sf::FloatRect> getCurrentFrame()
    {
        // std::printf("animRec in animable : %f,%f,%f,%f", animRec.left, animRec.top, animRec.left + 32.f,
        //             animRec.top + 32.f);
        if (mCurrentFrame != mFrames.end())
            return std::optional<sf::FloatRect>(*mCurrentFrame);
        else
            return std::nullopt;
    }

  private:
    float mDuration{1.f};
    float mFrameDuration{0.f};
    float mDisplayTime{0.f};
    bool mRunning{true};
    std::vector<sf::FloatRect> mFrames;
    std::vector<sf::FloatRect>::iterator mCurrentFrame;
};

class Animable
{
  public:
    Animable(const std::string pType)
    {
        if (pType == "player")
        {
            // use of rvalue here seems to do the trick..
            animations["standing"] = {1.f, {sf::FloatRect({0.f, 0.f}, {32.f, 32.f})}};
            currentAnimation = "standing";
        }
    }
    std::optional<Animation *> getCurrentAnimation()
    {
        auto animationIt = animations.find(currentAnimation);
        if (animationIt != animations.end())
            return std::optional<Animation *>(&(animationIt->second));
        else
            return std::nullopt;
    }

  private:
    std::map<std::string, Animation> animations;
    std::string currentAnimation;
};
#endif
