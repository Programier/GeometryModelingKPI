#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PROPERTIES_WIDTH 350
#define PROPERTIES_HEIGHT WINDOW_HEIGHT
#define VIEWPORT_WIDTH (WINDOW_WIDTH - PROPERTIES_WIDTH)
#define VIEWPORT_HEIGHT WINDOW_HEIGHT

#if defined(_WIN32)
#define PLATFORM_WINDOWS 1
#else
#define PLATFORM_WINDOWS 0
#endif


#if defined(__linux__)
#define PLATFORM_LINUX 1
#else
#define PLATFORM_LINUX 0
#endif


#define delete_copy_and_move(name)                                                                                     \
    name(const name&)            = delete;                                                                             \
    name(name&&)                 = delete;                                                                             \
    name& operator=(name&&)      = delete;                                                                             \
    name& operator=(const name&) = delete;
