//
// Created by dwarf on 17/07/2021.
//

#ifndef ZR_APPLICATION_HPP
#define ZR_APPLICATION_HPP
#include "../include/components/Movable.hpp"
#include "../include/components/Playable.hpp"
#include "../include/components/Renderable.hpp"
#include "../include/systems/AllSystems.hpp"
#include "Common.hpp"
/**
 * Main class of the program.
 */
class Application : public entityx::EntityX
{
  public:
    /**
     * Constructor.
     * @param argc Arguments count.
     * @param arguments Program arguments.
     */
    Application(int argc, char **arguments);

    /**
     * Starts the application.
     */
    void start();

  private:
    sf::RenderWindow mWindow;
    bool mDebug{false};
};
#endif
