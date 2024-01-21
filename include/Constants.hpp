#ifndef ZR_CONSTANTS
#define ZR_CONSTANTS
constexpr unsigned int SCREEN_WIDTH =960; 
constexpr unsigned int SCREEN_HEIGHT = 540;
// Number of time the game logic is updated each second.
constexpr unsigned short TICK_RATE = 32;
// Frequence of game logic update. 1/TICK_RATE seconds. 1e6 microseconds.
constexpr unsigned int WINDOW_SIZE = 1000000 / TICK_RATE;
constexpr unsigned int WINDOW_SIZE_MILLI = WINDOW_SIZE / 1000.f;
#endif
