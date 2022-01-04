/*       DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                   Version 2, December 2004

Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

Everyone is permitted to copy and distribute verbatim or modified
copies of this license document, and changing it is allowed as long
as the name is changed.

           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

 0. You just DO WHAT THE FUCK YOU WANT TO.


    main.c

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "includes_global.h"
#include "keyboard.h"

#include "pulse_handler.h"
#include "fft_handler.h"
#include "draw_iso.h"

#include <math.h>

pa_simple *s = NULL;

float scale_data(float x)
{
    //return (x - (0.2 * (x * x)));
    return log2(x+1);
}

// draw a damn line towards up
void DrawLine(SDL_Renderer *renderer, int x, int y, int height)
{

    if (x < 0 && y < 0 && height < 0)
        return;

    SDL_SetRenderDrawColor(renderer, 242, 242, 242, 255);
    SDL_RenderDrawLine(renderer, x, y, x, (y - height));
    SDL_RenderDrawLine(renderer, x + 1, y, x + 1, (y - height));
    SDL_RenderDrawLine(renderer, x + 2, y, x + 2, (y - height));
}

// draw a damn line towards up
void DrawLineColor(SDL_Renderer *renderer, int x, int y, int height, uint8_t r, uint8_t g, uint8_t b)
{

    if (x < 0 && y < 0 && height < 0)
        return;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderDrawLine(renderer, x, y, x, (y - height));
    SDL_RenderDrawLine(renderer, x + 1, y, x + 1, (y - height));
    SDL_RenderDrawLine(renderer, x + 2, y, x + 2, (y - height));
    SDL_RenderDrawLine(renderer, x + 3, y, x + 3, (y - height));
    SDL_RenderDrawLine(renderer, x + 4, y, x + 4, (y - height));
    SDL_RenderDrawLine(renderer, x + 5, y, x + 5, (y - height));
    SDL_RenderDrawLine(renderer, x + 6, y, x + 6, (y - height));
}

uint8_t minimum_of(uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t min = 255;

    if (r < min)
        min = r;

    if (g < min)
        min = g;

    if (b < min)
        min = b;

    return min;
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO); // init video

    SDL_Window *window; // main window
    // SDL_Surface *surface;   //master surface to blit to
    SDL_Renderer *renderer; // master renderer, if rendering used

    window = SDL_CreateWindow("ISO fft",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              WINDOW_SIZE_W,
                              WINDOW_SIZE_H,
                              SDL_WINDOW_SHOWN);
    if (0 == window)
    {
        printf("ERROR! Failed to create window: %s\n", SDL_GetError());
        exit(-1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (0 == renderer)
    {
        printf("ERROR! Failed to create renderer: %s\n", SDL_GetError());
        exit(-1);
    }

    if (!(s = init_rec_stream()))
    {
        printf("Failed to init audio. Quitting.\n");
        exit(-1);
    }

    while (1)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer); // fill the scene with whatever color was set

        const uint8_t nr_of_freq = NR_OF_FREQ;
        float output[nr_of_freq];
        get_current_fft(s, &output[0], nr_of_freq);

        int i_i = nr_of_freq;
        uint8_t r, g, b;
        while (i_i-- > 1)
        {
            while (1)
            {
                r = (uint8_t)random() % 0xFF;
                g = (uint8_t)random() % 0xFF;
                b = (uint8_t)random() % 0xFF;
                if (200 < r ||
                    200 < g ||
                    200 < b)
                {
                    if (10 > minimum_of(r, g, b))
                    {
                        break;
                    }
                }
            }
            DrawLineColor(renderer, i_i * 8, 800, abs((int)(200 * scale_data(20 * output[i_i]))), r, g, b);
        }

        draw_iso(renderer, output);

        SDL_RenderPresent(renderer); // copy to screen
        updateInput();
        // usleep(70000);

        //just drop frames do delay, or else the entire audio data will lag in time
        int i;
        for (i = 0; i < 3; i++)
        {
            get_current_fft(s, &output[0], nr_of_freq);
        }
    }

    return 0;
};
