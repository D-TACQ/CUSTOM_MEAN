/* pdmean: Pipe, Decimate Mean - inline decimating mean .. for scaling */

/* spad: scratch pad : data with instrumentation eg SAMPLE#
 * include in the output but DO NOT AVERAGE!
 */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DEF_NCHAN 8
#define DEF_DECIM 1024         // must be power of two
#define SHR   10           // divide by 1024
#define ADC   short

namespace G {
	int decim = DEF_DECIM;
	int nchan = DEF_NCHAN;    // channels are int16
	int nspad = 0;        // spads are int32
}

#define NCHAN	G::nchan
#define DECIM	G::decim
#define NSPAD	G::nspad
#define BUFFER_WIDTH   (G::nchan+G::nspad*2)
#define BUFFER_SAMPLES (G::decim * BUFFER_WIDTH)
#define BUFFER_SZ      (BUFFER_SAMPLES*sizeof(ADC))

void process(ADC* buffer, long* sum, ADC* decim)
{
	int ic;
	int isam;
	ADC* bp = buffer;

	for (isam = 0; isam < DECIM; ++isam, bp += BUFFER_WIDTH){
		for (ic = 0; ic < NCHAN; ++ic){
			sum[ic] += bp[ic];
		}
	}
	// scale the sums
	for (ic = 0; ic < NCHAN; ++ic ){
		decim[ic] = sum[ic] >> SHR;
	}
	// copy the spads across
	bp -= BUFFER_WIDTH;
	long* lbp = (long*)(bp+NCHAN);
	long* ldecim = (long*)(decim+NCHAN);

	for (int ispad = 0; ispad < NSPAD; ++ispad){
		ldecim[ispad] = lbp[ispad];
	}
}

void ui(int argc, char* argv[])
{
	if (argc > 1)
		if (argv[1][0] == 'h' || argv[1][0] == '?' ){
			fprintf(stderr, "pdmean [decim [nchan [nspad]]]\n");
			exit(1);
		}
		G::decim = atoi(argv[1]);
		if (G::decim == 0){
			fprintf(stderr, "pdmean [decim [nchan [nspad]]]\n");
			exit(1);
		}
	if (argc > 2)
		G::nchan = atoi(argv[2]);
	if (argc > 3)
		G::nspad = atoi(argv[3]);
}

int main(int argc, char* argv[])
{
	ui(argc, argv);
	ADC * buffer = new ADC[BUFFER_SAMPLES];
	long* sum = new long[NCHAN];
	ADC * decim = new ADC[BUFFER_WIDTH];

	int rc = setvbuf(stdin, (char*)buffer, _IOFBF, BUFFER_SZ);
	assert(rc == 0);

	while(1){
		memset(sum, 0, sizeof(long)*NCHAN);

		int nwords = fread(buffer, sizeof(ADC), BUFFER_SAMPLES, stdin);
		if (nwords == BUFFER_SAMPLES){
			process(buffer, sum, decim);
			fwrite(decim, sizeof(ADC), BUFFER_WIDTH, stdout);
			fflush(stdout);
		}else{
			fprintf(stderr, "ERROR fread returns %ld\n", nwords);
			return -1;
		}		
	}
	return 0;
}


