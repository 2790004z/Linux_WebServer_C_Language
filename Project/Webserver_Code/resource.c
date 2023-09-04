#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "resource.h"
#include "mime.h"

// Search resource
int searchResource(char const* path){
	return access(path, R_OK);
}

// Identify type
int identifyType(char const* path, char* type){
	char* suffix = strchr(path, '.');
	if (suffix == NULL){
		printf("Process:%d. Thread:%ld>>>>>> Can not get the file extension.\n", getpid(), syscall(SYS_gettid));
		return -1;
	}

	for(size_t i =0; i<sizeof(s_mime)/sizeof(s_mime[0]); i++){
		if (!strcasecmp(suffix, s_mime[i].suffix)){
			strcpy(type, s_mime[i].type);
			return 0;
		}
	}

	printf("Process:%d. Thread:%ld>>>>>> Unable to parse resource type: %s.\n", getpid(), syscall(SYS_gettid), suffix);
	return -1;
}
