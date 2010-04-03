
#ifndef HEADER_BSD_ASPRINTF_H
	#define HEADER_BSD_ASPRINTF_H
	
	#include "config.h"
	#include <stdarg.h>
	#include <stdlib.h>
	#include <errno.h>
	#include <stdarg.h>
	#include <stdlib.h>
	
	#ifndef HAVE_VASPRINTF
		int vasprintf(char**, const char*, va_list);
	#endif
	
	#ifndef HAVE_ASPRINTF
		int asprintf(char**, const char*, ...);
	#endif
#endif
