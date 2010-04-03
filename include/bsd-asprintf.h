
#ifndef INCLUDE_BSD_ASPRINTF_H
	#define INCLUDE_BSD_ASPRINTF_H
	
	#include "config.h"
	#include <stdarg.h>
	#include <stdio.h>
	
	#ifndef HAVE_VASPRINTF
		extern int vasprintf(char**, const char*, va_list);
	#endif
	
	#ifndef HAVE_ASPRINTF
		extern int asprintf(char**, const char*, ...);
	#endif
#endif
