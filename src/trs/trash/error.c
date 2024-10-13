#include <trs/error.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct{
	const char* m_Msg;
	ErrorLevel m_Level;
} Error;

static Error errors[] = {
	{"No input files.", ERROR_LEVEL_FATAL},
	{"Out of memory.", ERROR_LEVEL_FATAL},
	{"Bad input file: %s", ERROR_LEVEL_FATAL},
	{"Too many files.", ERROR_LEVEL_FATAL},
	{"%s: Failed to open file: %s", ERROR_LEVEL_FATAL},
	{"Excepted '%s', but got '%s'.", ERROR_LEVEL_ERROR},
	{"Excepted %s, but got '%s'.", ERROR_LEVEL_ERROR},
	{"Unexpected token '%s'.", ERROR_LEVEL_ERROR},
	{"Unparsable number.", ERROR_LEVEL_ERROR},
	{"Stand alone identifier: %s", ERROR_LEVEL_WARNING},
	{"Stand alone number literal: %ld", ERROR_LEVEL_WARNING},
	{"Stand alone string leteral: \"%s\".", ERROR_LEVEL_WARNING}
};

#define ERRORS_LEN (sizeof(errors) / sizeof(Error))

void vyell_impl(size_t err, va_list args){
	if(err >= ERRORS_LEN){
		fprintf(stderr, "FATAL: Unknown error.");
		exit(2);
	}

	int quit = 1;
	Error e = errors[err];
	switch(e.m_Level){
		case ERROR_LEVEL_FATAL:
			fprintf(stderr, "FATAL: ");
			quit = 2;
			break;
		case ERROR_LEVEL_ERROR:
			fprintf(stderr, "ERROR: ");
			break;
		case ERROR_LEVEL_WARNING:
			fprintf(stderr, "WARNING: ");
			quit = 0;
			break;
		case ERROR_LEVEL_INFO:
			fprintf(stderr, "INFO: ");
			quit = 0;
			break;
		default:
			fprintf(stderr, "UNKNOWN: ");
			quit = 3;
	}

	vfprintf(stderr, e.m_Msg, args);
	fprintf(stderr, "\n");

	if(quit)
		exit(quit);
}

void yell_impl(size_t err, ...){
	va_list args;
	va_start(args, err);
	vyell_impl(err, args);
	va_end(args);
}

void yellp_impl(size_t err, const char* filename, size_t lineno, size_t colno, ...){
	fprintf(stderr, "%s:%ld:%ld: ", filename, lineno, colno);
	
	va_list args;
	va_start(args, colno);
	vyell_impl(err, args);
	va_end(args);
}

void set_error_level(size_t err, ErrorLevel level){
	if(err >= ERRORS_LEN){
		fprintf(stderr, "FATAL: Unknown error.");
		exit(1);
	}

	errors[err].m_Level = level;
}

