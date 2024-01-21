#ifndef ZR_ANIMABLE_HPP
#define ZR_ANIMABLE_HPP
#include "../Common.hpp"
#include "../Tools.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <optional>
/**
 * Cette classe encapsule les informations et méthodes nécessaires à l'affichage d'animations
 * */
class Animation
{
  public:
    Animation()
    {
    }
    /**
     * Constructeur.
     * @param pDuration : durée de l'animation en ms.
     * @param vecteur de rectangles définissants les différentes images de l'animation.
     */
    Animation(float pDuration, std::vector<sf::FloatRect> pFrames) : mFrames(pFrames), mDuration(pDuration)
    {
        // Si on a passé un ensemble d'images non vide..
        if (pFrames.size() > 0)
        { // .. alors on positionne l'image courante sur le début du vecteur.
            mCurrentFrame = mFrames.begin();
            // on positionne également le temps d'affichage d'une seule image.
            //  ce temps correspond au temps total de l'animation divisé par le nombre d'images
            //  de celle-ci.
            mFrameDuration = pDuration / mFrames.size();
        }
        else
        {
            // Si on a passé un ensemble d'image vide l'image courante est placé à la fin du vecteur vide.
            mCurrentFrame = mFrames.end();
        }
    }
    /**
     * Démarre l'animation
     */
    void start()
    {
        mRunning = true;
    }
    /** Pause l'animation
     */
    void pause()
    {
        mRunning = false;
    }
    /**
     * Stop l'animation
     */
    void stop()
    {
        mRunning = false;
        mDisplayTime = 0.f;
    }
    /**
     * Ajoute pDt au temps d'affichage, permet de décompter le temps d'affichage de chaque image de l'animation.
     * @param pDt : le temps à ajouter en ms.
     */
    void addDisplayTime(float pDt)
    {
        mDisplayTime += pDt;
    }
    /**
     * Récupère le temps d'affichage de l'image
     */
    float getDisplayTime() const
    {
        return mDisplayTime;
    }
    /**
     * Récupère le temps d'affichage cible de chaque image.
     */
    float getFrameDuration() const
    {
        return mFrameDuration;
    }
    /**
     * Avance l'image courante de l'animation à l'image suivante. Si l'animation est à la dernière image on passe
     * à la première image de l'animation.
     *
     * Si l'animation est en pause ou arrêtée ou ne contient qu'une seule image cette méthode ne fait rien.
     *
     */
    void nextFrame()
    {
        if (mFrames.size() > 1 && mRunning)
        {
            mCurrentFrame++;
            if (mCurrentFrame == mFrames.end())
            {
                mCurrentFrame = mFrames.begin();
            }
            mDisplayTime = 0.f;
        }
    }
    /**
     * Permet de récupérer le rectangle de l'image courante sous forme optionelle.
     * soit l'animation est valide et on retourne le rectangle ou sinon nullopt
     */
    std::optional<sf::FloatRect> getCurrentFrame()
    {
        if (mCurrentFrame != mFrames.end())
            return std::optional<sf::FloatRect>(*mCurrentFrame);
        else
            return std::nullopt;
    }

  private:
    // durée totale de l'animation.
    float mDuration{1.f};
    // durée d'affichage de chaque image de l'animation.
    float mFrameDuration{0.f};
    // durée d'affichage de l'image courante.
    float mDisplayTime{0.f};
    // animation en pause ou stoppée
    bool mRunning{true};
    // les rectangles définissants les images de l'animations
    std::vector<sf::FloatRect> mFrames;
    // itérateur sur l'image courante de l'animation.
    std::vector<sf::FloatRect>::iterator mCurrentFrame;
};
/**
 * Class component permet de rendre une entité "animable"
 * contient une map indexant l'ensemble des animations en fonction du type d'entité passé au constructeur.
 * cette map contient un liste d'animations à afficher en fonction de l'animation choisie.
 */
class Animable
{
  public:
    /**
     * Constructeur
     * @param pType : type d'animable
     */
    Animable(const std::string &pType)
    {
        if (pType == "player")
        {
            // Définition de l'ensemble des animations
            auto &standingAnimations = animations["standing"];

            standingAnimations.push_back({1.f, Tools::createAnimation(1, 1, {32.f, 23.f}, 27.f)});
            standingAnimations.push_back({500.f, Tools::createAnimation(0, 20, {32.f, 27.f}, 0.f)});

            auto &walkingAnimations = animations["walking"];

            walkingAnimations.push_back({500.f, Tools::createAnimation(1, 20, {32.f, 23.f}, 27.f)});
            walkingAnimations.push_back({1.f, Tools::createAnimation(0, 1, {32.f, 27.f}, 0.f)});

            currentAnimation = "standing";
        }
    }
    /**
     * Méthode de récupération de la liste d'animation courante.
     */
    std::optional<std::vector<Animation> *> getCurrentAnimationList()
    {
        auto animationIt = animations.find(currentAnimation);
        if (animationIt != animations.end())
        {
            return std::optional<std::vector<Animation> *>(&(animationIt->second));
        }
        else
            return std::nullopt;
    }
    /**
     * Récupération du nom de l'animation courante active
     */
    std::string getCurrentAnimationName() const
    {
        return currentAnimation;
    }
    /** Définition de l'animation courante.
     * @param animationName : nom de l'animation que l'on souhaite définir comme active.
     * si l'animation existe la méthode retourne vrai après avoir défini l'animation comme courante.
     * sinon la méthode retourne faux et rien n'est changé.
     */
    bool setCurrentAnimation(const std::string &animationName)
    {
        auto animOpt = animations.find(animationName);
        if (animOpt != animations.end())
        {
            currentAnimation = animationName;
            return true;
        }
        return false;
    }

  private:
    // Map de stockage des animations, il peut y avoir plusieurs animations pour un nom donné,
    // Dans ce cas l'ordre d'ajout dans le vecteur détermine l'ordre d'affichage.
    std::map<std::string, std::vector<Animation>> animations;
    // Nom de l'animation courante active, invariant : doit exister en tant qu'index dans la map.
    std::string currentAnimation;
};
#endif
