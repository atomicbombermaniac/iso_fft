#include "draw_iso.h"
#include <math.h>

typedef struct point
{
    /* data */
    float x;
    float y;
    float z;
} point;

float scale_data2(float x)
{
    //return (x - (0.2 * (x * x)));
    return log2(x+1);
}

void setRandomCol(SDL_Renderer *renderer)
{

    uint8_t r, g, b;

    while (1)
    {
        r = (uint8_t)random() % 0xFF;
        g = (uint8_t)random() % 0xFF;
        b = (uint8_t)random() % 0xFF;
        if (100 < r ||
            100 < g ||
            100 < b)
        {
            break;
        }
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    //SDL_RenderPresent(renderer); // copy to screen
    //usleep(1000000);
}
#define GRID_TALL 60
void draw_iso(SDL_Renderer *renderer, float *input)
{

    //SDL_SetRenderDrawColor(renderer, 0, 242, 242, 255);
    const int tall = GRID_TALL;
    const int nr = NR_OF_FREQ;
    const int spacing_x = 8;
    const int spacing_y = 8;
    static point grid[GRID_TALL][NR_OF_FREQ];
    volatile int i, j, f;

    //resetting grid, just in case
    for (i = 0; i < tall; i++)
    {
        for (j = 0; j < nr; j++)
        {
            grid[i][j].x = 0;
            grid[i][j].y = 0;
        }
    }

    //carry the values downstream
    i = tall;
    while (i--)
    { //vertical
        for (j = 0; j < nr; j++)
        { //horizontal
            grid[i][j].z = grid[i - 1][j].z;
        }
    }

    for (i = 0; i < tall; i++)
    { //vertical
        for (j = 0; j < nr; j++)
        { //horizontal
            grid[i][j].x = (j - (nr / 2)) * spacing_x;
            //grid[i][j].y = (i-(tall/2)) * spacing_y ;
            grid[i][j].y = (i)*spacing_y;
            if (i == 0)
            {
                grid[0][j].z = (30 * scale_data2(30 * input[j]));
            }
        }
    }

    static float inc = 0.7;
    const float upper_limit = 0.95;
    const float lower_limit = 0.70;
    static uint8_t direction = 0;
    const float rate = 0.001;

    if (direction)
    {
        inc += rate;
        if (inc > upper_limit)
        {
            direction = 0;
        }
    }
    else
    {
        inc -= rate;
        if (inc < lower_limit)
        {
            direction = 1;
        }
    }

    volatile point td_grid[GRID_TALL][NR_OF_FREQ];

    for (i = 0; i < tall; i++)
    { //vertical
        for (j = 0; j < nr; j++)
        { //horizontal

            //td_grid[i][j].x = inc * grid[i][j].x - inc * grid[i][j].y + WINDOW_SIZE_W/2;
            //td_grid[i][j].y = (1 - inc) * grid[i][j].x + (1 - inc) * grid[i][j].y - grid[i][j].z + WINDOW_SIZE_H/2;

            td_grid[i][j].x = inc * grid[i][j].x * (spacing_x / 10.0 + grid[i][j].y * grid[i][j].y * 0.0001) + WINDOW_SIZE_W / 2;
            td_grid[i][j].y = /*(spacing_y/100 + i / 20.0) */ (0.001 * i * i) * grid[i][j].y - grid[i][j].z - grid[i][j].z * (i / 10.0) * inc + WINDOW_SIZE_H / 7;
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 254, 0, 255);

    for (i = 0; i < tall; i++)
    { //vertical
        for (j = 0; j < nr; j++)
        { //horizontal

            SDL_RenderDrawPoint(renderer, td_grid[i][j].x, td_grid[i][j].y);
            if (j)
            {
                //draw blur
                //int b = 4;
                //while(--b){
                //    SDL_SetRenderDrawColor(renderer, 1, 254 - b*20, 1, 255);
                //    SDL_RenderDrawLine(renderer, td_grid[i][j - 1].x, td_grid[i][j - 1].y - b , td_grid[i][j].x, td_grid[i][j].y - b );
                //}

                //draw the actual line
                //SDL_SetRenderDrawColor(renderer, 10, 254, 10, 255);
                SDL_RenderDrawLine(renderer, td_grid[i][j - 1].x, td_grid[i][j - 1].y, td_grid[i][j].x, td_grid[i][j].y);

                //draw blur
                //SDL_SetRenderDrawColor(renderer, 1, 25, 100, 255);
                //SDL_RenderDrawLine(renderer, td_grid[i][j - 1].x, td_grid[i][j - 1].y + 3, td_grid[i][j].x, td_grid[i][j].y + 3);
            }
            if (i)
            {
                //draw blur
                //SDL_SetRenderDrawColor(renderer, 1, 25, 1, 255);
                //SDL_RenderDrawLine(renderer, td_grid[i - 1][j].x - 1, td_grid[i - 1][j].y, td_grid[i][j].x - 1, td_grid[i][j].y);

                //draw the actual line
                //SDL_SetRenderDrawColor(renderer, 10, 254, 10, 255);
                SDL_RenderDrawLine(renderer, td_grid[i - 1][j].x, td_grid[i - 1][j].y, td_grid[i][j].x, td_grid[i][j].y);

                //draw blur
                //SDL_SetRenderDrawColor(renderer, 1, 25, 1, 255);
                //SDL_RenderDrawLine(renderer, td_grid[i - 1][j].x + 1, td_grid[i - 1][j].y, td_grid[i][j].x + 1, td_grid[i][j].y);
            }
        }
    }
}
