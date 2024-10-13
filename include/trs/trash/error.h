#ifndef _ERROR_H_
#define _ERROR_H_

#include <stddef.h>
#include <stdarg.h>

#define FE_LEVEL_MIN 0
#define FE_NO_INPUT_FILES 0
#define FE_OUT_OF_MEMORY 1
#define FE_BAD_INPUT_FILE 2
#define FE_TOO_MANY_FILES 3
#define FE_FAILED_TO_OPEN_FILE 4

#define E_LEVEL_MIN 5
#define E_EXCEPTED 5
#define E_EXCEPTED_NAME 6
#define E_UNEXPECTED_TOKEN 7
#define E_UNPARSABLE_NUMBER 8

#define W_LEVEL_MIN 9
#define W_STAND_ALONE_ID 9
#define W_STAND_ALONE_NUM 10
#define W_STAND_ALONE_STR 11

typedef enum{
	ERROR_LEVEL_FATAL,
	ERROR_LEVEL_ERROR,
	ERROR_LEVEL_WARNING,
	ERROR_LEVEL_INFO
} ErrorLevel;

void yell_impl(size_t err, ...);
void vyell_impl(size_t err, va_list args);
void yellp_impl(size_t err, const char* filename, size_t lineno, size_t colno, ...);
void set_error_level(size_t err, ErrorLevel level);

#ifndef DEBUG
	#define yell(...)  yell_impl(__VA_ARGS__)
	#define vyell(...) vyell_impl(__VA_ARGS__)
	#define yellp(...) yellp_impl(__VA_ARGS__)
#else
	#include <stdio.h>
	#define yell(...)  do{ printf("%s:%d: yell:\n", __FILE__, __LINE__); yell_impl(__VA_ARGS__); } while(0)
	#define vyell(...) do{ printf("%s:%d: vyell:\n", __FILE__, __LINE__); vyell_impl(__VA_ARGS__); } while(0)
	#define yellp(...) do{ printf("%s:%d: yellp:\n", __FILE__, __LINE__); yellp_impl(__VA_ARGS__); } while(0)
#endif

#endif // _ERROR_H_
