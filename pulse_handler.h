#include "includes_global.h"


#define BUF_SIZE 256
#define LONG_BUF BUF_SIZE*70


/* The sample type to use */
    static const pa_sample_spec ss = {
        .format = PA_SAMPLE_S16LE,
        .rate = 44100,
        .channels = 2
    };
   


void buf_push(uint8_t * big_buf, uint8_t * data);

pa_simple * init_rec_stream(void);

uint8_t * read_audio(pa_simple * s);