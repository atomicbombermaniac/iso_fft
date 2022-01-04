#include "includes_global.h"



#include "kiss_fftr.h"
#include "_kiss_fft_guts.h"


typedef struct data_processor {
    float sample_hz;
    int nfft;
    kiss_fft_cpx *cin;
    kiss_fft_cpx *cout;
    kiss_fft_cfg  kiss_fft_state;
}*data_processor_t;

data_processor_t data_processor_init(int nfft, int hz);
float data_processor_run(data_processor_t dfft);
void data_processor_close(data_processor_t dfft);

void get_current_fft(pa_simple *s,float *output, uint8_t number_of_freq);