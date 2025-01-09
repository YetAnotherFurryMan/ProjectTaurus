#include <trs/error.h>

#include <stdio.h>
#include <stdarg.h>

void error_logError(error_Location loc, error_EIDX errno, ...){
	if(errno >= g_errorMax)
		errno = 0;

	if(loc.filename)
		fprintf(stderr, "%s:%ld:%ld: ERROR: ", loc.filename, loc.lineno, loc.colno);
	else
		fputs("ERROR: ", stderr);

	va_list args;
	va_start(args, errno);

	vfprintf(stderr, g_errorStrings[errno], args);

	va_end(args);
}

void error_logWarning(error_Location loc, error_EIDX errno, ...){
	if(errno >= g_errorMax){
		error_logError(loc, EIDX_INTERNAL_BAD_EIDX, errno);
		return;
	}

	if(loc.filename)
		fprintf(stderr, "%s:%ld:%ld: WARNING: ", loc.filename, loc.lineno, loc.colno);
	else
		fputs("WARNING: ", stderr);

	va_list args;
	va_start(args, errno);

	vfprintf(stderr, g_errorStrings[errno], args);

	va_end(args);
}

void error_logInfo(error_Location loc, error_EIDX errno, ...){
	if(errno >= g_errorMax){
		error_logError(loc, EIDX_INTERNAL_BAD_EIDX, errno);
		return;
	}
	
	if(loc.filename)
		fprintf(stderr, "%s:%ld:%ld: INFO: ", loc.filename, loc.lineno, loc.colno);
	else
		fputs("INFO: ", stderr);

	va_list args;
	va_start(args, errno);

	vfprintf(stderr, g_errorStrings[errno], args);

	va_end(args);
}

