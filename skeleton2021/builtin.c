#include "builtin.h"

bool builtin_is_exit(pipeline pipe){
	return false;

}
bool builtin_is_cd(pipeline pipe){
	return false;
}

bool builtin_is_internal(pipeline pipe){
	return builtin_is_exit(pipe) || builtin_is_cd(pipe);
}

void builtin_exec(pipeline pipe){
}


