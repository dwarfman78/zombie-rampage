#ifndef ZR_CONSTANTS
#define ZR_CONSTANTS
// Number of time the game logic is updated each second.
constexpr unsigned short TICK_RATE = 1;
// Frequence of game logic update. 1/TICK_RATE seconds. 1e6 microseconds.
constexpr unsigned int WINDOW_SIZE = 1000000 / TICK_RATE;
#endif
