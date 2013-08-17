
#ifndef HEADER_FATAL_H
	#define HEADER_FATAL_H

	#include <sys/types.h>
	#include <stdarg.h>
	
	#include "include/log.h"
	#include "include/cleanup.h"
	
	void fatal(const char *fmt,...);
#endif
