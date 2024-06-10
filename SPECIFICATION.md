# Taurus Project Specification Description

 - [x] written in C/C++
 - [ ] rewritten in Taurus

## Taurus Argument Parser (trsap)

A library for parsing command-line argumants.

### Enumerations
 - [x] trsap_Type - Describes type of an argument
    - TRSAP_ARG_TYPE_FLAG
	- TRSAP_ARG_TYPE_VALUE
	- TRSAP_ARG_TYPE_VALUE_OPTIONAL
	- TRSAP_ARG_TYPE_VALUE2
	- TRSAP_ARG_TYPE_VALUE2_OPTIONAL
 - [x] trsap_State - Describes state of parsing
    - TRSAP_ARG_STATUS_EOI
    - TRSAP_ARG_STATUS_OK
	- TRSAP_ARG_STATUS_OK_VALUE
	- TRSAP_ARG_STATUS_OK_VALUE2
    - TRSAP_ARG_STATUS_ERR_SHORT
    - TRSAP_ARG_STATUS_ERR_LONG
	- TRSAP_ARG_STATUS_ERR_VALUE
	- TRSAP_ARG_STATUS_ERR_VALUE2

### Structures
 - [x] trsap_Desc - Description of an argument 
    - .short (char) and .long (const char*) fields describes name
    - .type (trsap_ArgType) field specifies count and form of values passed with argument
 - [x] trsap_Arg - Result of trsap_next()
    - .id (int) field is an index of matching trsap_Desc, -1 if the argument is stand-alone valure
    - .value (const char*) field is the first value of the argument if specified or 0, with size stored in .valueLen (size_t)
    - .value2 (const char*) field is the second value of the argument if specified or 0, with size stored in .value2Len (size_t)
    - .state (trsap_ArgState) field specifies status (an error or succes) of parsing

### Functions
 - [x] const char* trsap_chop(int* argc, const char*** argv) - Returns 1st argument or 0 and shifts argc and argv
 - [x] trsap_Arg trsap_next(size_t descc, argap_Desc* descv, int* argc, const char*** argv) - Returns one parsed arguments. Calls trsap_chop
 - [x] const char* trsap_chop2(int* argc, const char* const** argv) -- Returns result of trsap_chop (inline)
 - [x] trsap_Arg trsap_next2(size_t descc, argap_Desc* descv, int* argc, const char* const** argv) - Return result of trsap_next (inline)

### C++ wrapper
 - [x] all C enums have wrappers
 - [x] all C strucs have wrappers
 - [x] all C functions have wrappers
 - [x] inline functions like *\2 have theirs alternatives, but without '2' if C++ has no problem with overwritting them
 - [x] std::vector<trs::ap::Arg> trs::ap::getAll(std::size_t descc, trs::ap::Desc* descv, int* argc, const char** argv) - Returns all parsed arguments (to the end or to first failure) (inline)
 - [x] std::vector<trs::ap::Arg> trs::ap::getAll(std::size_t descc, trs::ap::Desc* descv, int* argc, const char* const* argv) - Returns result of the function above ^. (inline)

## CSV parser (csv)

A simple library for CSV parsing.

### Enumerations
There is none.

### Structures
 - [x] csv_Row - Result of csv_fgetrow and csv_parseRow. Represents a single CSV row.
    - .count (size_t) field is the count of values found and parsed in the row.
    - .values (char**) field is an array of parsed values (tokens).

### Functions
 - [x] csv_Row csv_fgetrow(FILE* f, char delimeter) - Returns a single parsed line and moves the cursor. Caller should call csv_free.
 - [x] csv_Row csv_parseRow2(char* line, char delimeter) - Parses the row. Caller should call csv_free.
 - [x] csv_Row csv_parseRow(const char* line, char delimeter) - Copies the line and calls csv_parseRow2.
 - [x] void csv_freeRow(csv_Row row) - Frees csv_Row values.
 - [x] void csv_encode(csv_Row* row, char delimeter) - Encodes each value in row. (uses '\' as escape character) (reallocates memory)
 - [x] void csv_decode(csv_Row* row, char delimeter) - Decodes each value in row. (uses '\' as escape character)
 - [x] char* csv_srtrow(csv_Row row, char delimeter) - Stringifies the row (without encoding). Caller owns the memory.

### C++ wrapper


