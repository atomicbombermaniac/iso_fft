#include "includes_global.h"
#include "pulse_handler.h"

uint8_t big_buf[LONG_BUF];

void buf_push(uint8_t *big_buf, uint8_t *data)
{
    int j = 0;
    // move each of the 10 segments one segment to the left
    for (j = 0; j < LONG_BUF / BUF_SIZE - 1; j++)
    {
        // memcpy(&big_buf[j*BUF_SIZE], &big_buf[(j+1)*BUF_SIZE], BUF_SIZE);
        for (int i = 0; i < BUF_SIZE; i++)
        {
            big_buf[i + BUF_SIZE * j] = big_buf[i + BUF_SIZE + BUF_SIZE * j];
            // printf("big_buf[%d] = big_buf[%d]\n",i+BUF_SIZE*j, i+BUF_SIZE+BUF_SIZE*j);
            // big_buf[i+BUF_SIZE+BUF_SIZE*j] = big_buf[i+BUF_SIZE*j] ;
        }
    }

    // for(int i = 0; i < BUF_SIZE ; i++){
    //     big_buf[i+(BUF_SIZE*(LONG_BUF/BUF_SIZE-1))] = data[i]; //copy new data into segment 10
    // }
    memcpy(&big_buf[(LONG_BUF / BUF_SIZE - 1) * BUF_SIZE], &data[0], BUF_SIZE);
}

void buf_fill(uint8_t *big_buf, uint8_t *data)
{

    int j = 0;
    // move each of the 10 segments one segment to the left
    for (j = 0; j < LONG_BUF / BUF_SIZE - 1; j++)
    {
        memcpy(&big_buf[j * BUF_SIZE], &data[0], BUF_SIZE);
    }
}

pa_simple *init_rec_stream(void)
{

    int error;
    pa_simple *s = NULL;

    /* Create the recording stream */
    if (!(s = pa_simple_new(NULL, "ISO Spectrum ", PA_STREAM_RECORD, NULL, "record", &ss, NULL, NULL, &error)))
    {
        fprintf(stderr, __FILE__ ": pa_simple_new() failed: %s\n", pa_strerror(error));
    }

    return s;
}

uint8_t *read_audio(pa_simple *s)
{
    int error;
    uint8_t buf[BUF_SIZE];
    for (int i = 0; i < 5; i++)
    {
        pa_simple_read(s, buf, sizeof(buf), &error);
        
        //this pushes new data into the ring buffer
        buf_push(&big_buf, &buf);

        //this just clones into the ring buffer to fill it
        //faster response time but kills the highest fft component..ish
        //buf_fill(&big_buf, &buf);
    }
    return &big_buf[0];
}