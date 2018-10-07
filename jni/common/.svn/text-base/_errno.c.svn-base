
#include "_errno.h"

char* strerror_custom(int errno_arg)
{
	int i;

	if(errno_arg < 1000)
		return NULL;

	for(i=0; errcodestrs[i].str != NULL; i++){
		if(errcodestrs[i].code == errno_arg)
			return errcodestrs[i].str;
	}

	return NULL;
}