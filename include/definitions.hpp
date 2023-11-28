#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PROPERTIES_WIDTH 400
#define PROPERTIES_HEIGHT WINDOW_HEIGHT
#define VIEWPORT_WIDTH (WINDOW_WIDTH - PROPERTIES_WIDTH)
#define VIEWPORT_HEIGHT WINDOW_HEIGHT
#define DEFAULT_GRID_SPACING 75
#define GRID_UNIT_SEGMENT 1.f

#define PROP_R 5.f
#define PROP_r 1.1234f
#define PROP_delta_ange 3.
#define PROP_max_parts 128


#define PROP_INDEX_R 0
#define PROP_INDEX_r 1
#define PROP_INDEX_dt 2
#define PROP_INDEX_max_parts 3

#define FIGURE_PROPS_COUNT 4
#define ACCURACY 0.000001
#define ITEM_INDEND 12.f


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
