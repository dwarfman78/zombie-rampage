#ifndef ZR_APPLICATION_SERVER_HPP
#define ZR_APPLICATION_SERVER_HPP
#include "../include/systems/AllSystems.hpp"
#include "Common.hpp"
/**
 * Main class of the program server side.
 */
class ApplicationServer : public entityx::EntityX {
public:
  /**
   * Constructor.
   * @param argc Arguments count.
   * @param arguments Program arguments.
   */
  ApplicationServer(int argc, char **arguments);

  /**
   * Starts the application.
   */
  void start();
};
#endif