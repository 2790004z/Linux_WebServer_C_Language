#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include "signals.h"

// Initialize signal
int initSignals(void){
	printf("Process:%d>>>>>> Ignore most of the signals.\n", getpid());

	for(int signum = 1; signum <= 64; ++signum){
		if (signum != SIGINT && signum != SIGTERM) signal(signum, SIG_IGN);   
	}
	return 0;
}
