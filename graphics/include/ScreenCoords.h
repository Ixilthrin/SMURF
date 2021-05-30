#ifndef GENG_SCREEN_COORDS_H
#define GENG_SCREEN_COORDS_H

// gui screen orientations
#define GUI_LOWER_LEFT 1
#define GUI_UPPER_LEFT 2 
#define GUI_UPPER_RIGHT 3 
#define GUI_LOWER_RIGHT 4 
#define GUI_LOWER_CENTER 5 
#define GUI_UPPER_CENTER 6
#define GUI_DEAD_CENTER 7

struct ScreenCoords
{
    int left;
    int right;
    int bottom;
    int top;
};

#endif
