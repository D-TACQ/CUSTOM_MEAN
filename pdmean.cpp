/* pdmean: Pipe, Decimate Mean - inline decimating mean .. for scaling */

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define NCHAN 8
#define DECIM 1024         // must be power of two
#define SHR   10           // divide by 1024
#define ADC   short

void process(ADC* buffer, long* sum, ADC* decim)
{
	int ic;
	int isam;
	ADC* bp = buffer;

	for (isam = 0; isam < DECIM; ++isam, bp += NCHAN){
		for (ic = 0; ic < NCHAN; ++ic){
			sum[ic] += bp[ic];
		}
	}
	for (ic = 0; ic < NCHAN; ++ic ){
		decim[ic] = sum[ic] >> SHR;
	}
}


int main(int argc, char* argv[])
{
	ADC * buffer = new ADC[NCHAN*DECIM];
	long* sum = new long[NCHAN];
	ADC * decim = new ADC[NCHAN];

	int rc = setvbuf(stdin, (char*)buffer, _IOFBF, sizeof(ADC)*NCHAN*DECIM);
	assert(rc == 0);

	while(1){
		memset(sum, 0, sizeof(long)*NCHAN);

		int nwords = fread(buffer, sizeof(ADC), NCHAN*DECIM, stdin);
		if (nwords == NCHAN*DECIM){
			process(buffer, sum, decim);
			fwrite(decim, sizeof(ADC), NCHAN, stdout);
		}else{
			fprintf(stderr, "ERROR fread returns %ld\n", nwords);
			return -1;
		}		
	}
	return 0;
}


