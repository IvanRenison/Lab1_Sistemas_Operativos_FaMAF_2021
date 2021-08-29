#include <assert.h>
#include <stdio.h>

#include "builtin.h"

bool builtin_is_exit(pipeline pipe){
	assert(pipe != NULL);

	return false;
}

bool builtin_is_cd(pipeline pipe){
	return false;
}

bool builtin_is_internal(pipeline pipe){
	return builtin_is_exit(pipe) || builtin_is_cd(pipe);
}

void builtin_exec(pipeline pipe){
//	prinf("builtin_exec\n");
}
