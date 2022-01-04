#include "includes_global.h"
#include "pulse_handler.h"
#include "fft_handler.h"


#ifndef M_PI
#define M_PI 3.14159265358979324
#endif

#define N 16


data_processor_t dfft = NULL;
float hz = 0;

uint8_t big_buf_fft[LONG_BUF];

void get_current_fft(pa_simple *s,float *output, uint8_t number_of_freq)
{

    //int16_t *pb_big=&big_buf[0];

    dfft = data_processor_init(LONG_BUF / 4, 1000000);
    if (!dfft)
    {
        printf("data_processor_init error\n");
        exit(-1);
    }

    //big_buf = read_audio(void);
    int16_t *pb_big = read_audio(s);


    //resetting all input data for fft

    int i_i = LONG_BUF / 4;
    while (i_i < dfft->nfft)
    {
        dfft->cin[i_i].i = 0;
        dfft->cin[i_i++].r = 0;
    }

    for (int kk = 0; kk < dfft->nfft; kk++)
    {
        dfft->cin[kk].i = 0;
        dfft->cin[kk].r = ((pb_big[kk]) + (65535 / 2)) / 65535.0;
    }

    i_i = dfft->nfft / 4;
    hz = data_processor_run(dfft);

    for (i_i = 0; i_i < number_of_freq; i_i++)
    {
        *output = dfft->cout[i_i+1].r;
        output++;
    }

    data_processor_close(dfft);
}