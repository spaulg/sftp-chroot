
AC_PREREQ(2.59)

# Initialise
AC_INIT([sftp-chroot], [0.1], [spaulger@codezen.co.uk])

# Config.h header
AC_CONFIG_HEADERS([config.h])
AC_CANONICAL_HOST

# Start automake
AM_INIT_AUTOMAKE([subdir-objects])

# Define parameters that can be passed
AC_PREFIX_DEFAULT([/])

# Default variables
CFLAGS="-O2"

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O()

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h stdio.h syslog.h signal.h sys/wait.h stdint.h pthread.h fcntl.h arpa/inet.h],
		  [], AC_MSG_FAILURE([Missing header files are required to successfully compile. Compilation without these header files.], [1]))

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_REALLOC

AC_CHECK_FUNCS(
	strlcat \
	strlcpy \
	strnvis \
	vasprintf \
	asprintf \
)

AC_CACHE_CHECK([for size_t], ac_cv_have_size_t, [
	AC_TRY_COMPILE(
		[
#include <sys/types.h>
		],
		[ size_t foo; foo = 1235; ],
		[ ac_cv_have_size_t="yes" ],
		[ ac_cv_have_size_t="no" ]
	)
])
if test "x$ac_cv_have_size_t" = "xyes" ; then
	AC_DEFINE(HAVE_SIZE_T, 1, [define if you have size_t data type])
fi

AC_CACHE_CHECK([whether va_copy exists], ac_cv_have_va_copy, [
	AC_TRY_LINK(
		[#include <stdarg.h>
		 va_list x,y;],
	    	[va_copy(x,y);],
		[ ac_cv_have_va_copy="yes" ],
		[ ac_cv_have_va_copy="no" ]
	)
])
if test "x$ac_cv_have_va_copy" = "xyes" ; then
	AC_DEFINE(HAVE_VA_COPY, 1, [Define if va_copy exists])
fi


# Check for some target-specific stuff
case "$host" in
*-*-openbsd*)
	AC_DEFINE(HAVE_ATTRIBUTE__SENTINEL__, 1, [OpenBSD's gcc has sentinel])
	AC_DEFINE(HAVE_ATTRIBUTE__BOUNDED__, 1, [OpenBSD's gcc has bounded])
	;;
esac

AC_SUBST([CFLAGS])

# Produce make files
AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
