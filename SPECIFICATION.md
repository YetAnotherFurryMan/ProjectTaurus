# Taurus Project Specification Description

 - [x] written in C/C++
 - [ ] rewritten in Taurus

## Taurus Argument Parser

A library for parsing command-line argumants.

### Enumerations:
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

### Structures:
 - [x] trsap_Desc - Description of an argument 
    - .short (char) and .long (const char*) fields describes name
    - .type (trsap_ArgType) field specifies count and form of values passed with argument
 - [x] trsap_Arg - Result of trsap_next()
    - .id (int) field is an index of matching trsap_Desc, -1 if the argument is stand-alone valure
    - .value (const char*) field is the first value of the argument if specified or 0
    - .value2 (const char*) field is the second value of the argument if specified or 0
    - .state (trsap_ArgState) field specifies status (an error or succes) of parsing

### Functions
 - [x] const char* trsap_chop(int* argc, const char*** argv) - Returns 1st argument or 0 and shifts argc and argv
 - [x] trsap_Arg trsap_next(size_t descc, argap_Desc* descv, int* argc, const char*** argv) - Returns one parsed arguments. Calls trsap_chop

### C++ wrapper
 - [x] all C enums have wrappers
 - [x] all C strucs have wrappers
 - [x] all C functions have wrappers
 - [x] std::vector<trs::ap::Arg> trs::ap::getAll(std::size_t descc, trs::ap::Desc* descv, int* argc, const char** argv) - Returns all parsed arguments (to the end or to first failure)

