#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define PROPERTIES_WIDTH 350
#define PROPERTIES_HEIGHT WINDOW_HEIGHT
#define VIEWPORT_WIDTH (WINDOW_WIDTH - PROPERTIES_WIDTH)
#define VIEWPORT_HEIGHT WINDOW_HEIGHT
#define DEFAULT_GRID_SPACING 75
#define GRID_UNIT_SEGMENT 10.f

#define PROP_A 55.f
#define PROP_B 35.f
#define PROP_C 25.f
#define PROP_D 10.f
#define PROP_E 25.f
#define PROP_F 50.f
#define PROP_G 18.f
#define PROP_H 13.f

#define PROP_INDEX_A 0
#define PROP_INDEX_B 1
#define PROP_INDEX_C 2
#define PROP_INDEX_D 3
#define PROP_INDEX_E 4
#define PROP_INDEX_F 5
#define PROP_INDEX_G 6
#define PROP_INDEX_H 7

#define FIGURE_PROPS_COUNT 8
#define ACCURACY 0.00001
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
