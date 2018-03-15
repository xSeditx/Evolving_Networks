#pragma once
#define SDL_MAIN_HANDLED
#include <SDL.h>

#define GetRandom( min, max )     ((rand() % (int)(((max) + 1) - (min))) + (min))  
#define RANDOM(x)                 ((rand() * (1.0 / (1.0 + RAND_MAX))) * x)
#define RANDOM_RANGE(x)           (RANDOM(x * 2) - (x))

#define RADIANS(angle)            (angle * .0174532925199444)

#define LOOP(x)                   for(int count = 0; count < (x); count++)
#define FOR_LOOP(x,c)             for(int x = 0; x < (c); x++)

#define SCREENWIDTH        1280 //680 
#define SCREENHEIGHT       960 //460 

extern float COS[360], SIN[360];
#define _COS(a) COS[static_cast<int>(a)]
#define _SIN(a) SIN[static_cast<int>(a)]

template <typename T>
static inline unsigned color_from_rgb(T r, T g, T b)
{
    return static_cast<unsigned>(b) + (static_cast<unsigned>(g) << 8u) + (static_cast<unsigned>(r) << 16u);
}


class WINDOW final
{

public:
    WINDOW() = default;
    ~WINDOW() = default;
    WINDOW(int, int, int, int, char*);


    int
        X,
        Y,
        WIDTH,
        HEIGHT;

    char *TITLE;


    SDL_Window             *HWND;
    SDL_Texture            *BACK_BUFFER;
    SDL_Renderer           *RENDER;

    Uint32                  *WINDOW_PIXELS;

    Uint32                  WINDOW_FORMAT;
    SDL_Event               EVENT;
    SDL_Surface            *WINDOW_SURFACE;
    SDL_PixelFormat        *MAPPING_FORMAT;
    SDL_Point               MOUSE_POSITION, MOUSE_VELOCITY;

    double                  MOUSE_ANGLE;
    struct
    {
        bool LEFT, RIGHT;
    } MOUSE_BUTTON;

    unsigned long           DRAW_COLOR;
    float FPS;
    void SET_DRAW_COLOR(unsigned long Col);

    double TIME;

    int   FRAME_COUNTER;
    int   CYCLE_COUNTER;

    int   CyclePerSecond;

    float CYCLE_TIMER;
    float TIMER;

};


extern   int       KEYBOARD_HANDLER(SDL_Keycode sym);

// FINDS THE NEX X/Y POSITION A DISTANCE FROM THE GIVEN X/Y AT A GIVEN ANGLE
static inline float NEWX(float x, float dist, float angle)
{
    return x + dist * _COS(angle);
}
static inline float NEWY(float y, float dist, float angle)
{
    return y + dist * _SIN(angle);
}

void      SYNC();
void      CLS();
void      PRINT_SCREEN(char *text);
void      SET_PIXEL(int, int, Uint32);
void      SET_PIXELII(int, int, Uint32);
void      SET_DRAW_COLOR(unsigned long Col);
void      LINE(int x1, int y1, int x2, int y2);
void      LINE2(int x, int y, float Angle, int Length);
void      CIRCLE(int x, int y, float radius);
void      FILLED_CIRCLE(int x, int y, float radius);
void      BOX(int X1, int Y1, int X2, int Y2);
void      FILLED_BOX(int X1, int Y1, int X2, int Y2);


// RETURNS ANGLE IN DEGREES
static inline float FindAngle(SDL_Point A, SDL_Point B)
{
    return atan2f(B.y - A.y, B.x - A.x) / 3.14159 * 180;
}

bool      Is_CLICK(int X, int Y);
bool      LOOP_GAME();
bool      SET_ACTIVE_WINDOW(WINDOW *active);

extern   WINDOW   *SCREEN;
extern   int       WINDOW_OFFSET;
extern   int       LEFT_BOUNDS;
extern   int       TOP_BOUNDS;
extern   int       RIGHT_BOUNDS;
extern   int       BOTTOM_BOUNDS;
