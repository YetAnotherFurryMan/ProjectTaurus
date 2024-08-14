# Taurus Project Specification Description

 - [x] written in C/C++
 - [ ] rewritten in Taurus

## Build system

Auto-generated Makefile and build.ninja form buildsystem.cpp

## Toollib

A set of libraries writen in C (with C++ wrappers if it makes sense) for writing tools like generators, compilers, etc.

### Argument Parser (toollib -> ap)

A library for parsing command-line argumants.

#### Enumerations

 - [x] ap_Type - Describes type of an argument
    - AP_ARG_TYPE_FLAG
	- AP_ARG_TYPE_VALUE
	- AP_ARG_TYPE_VALUE_OPTIONAL
	- AP_ARG_TYPE_VALUE2
	- AP_ARG_TYPE_VALUE2_OPTIONAL
 - [x] ap_Status - Describes state of parsing
    - AP_ARG_STATUS_EOI
    - AP_ARG_STATUS_OK
    - AP_ARG_STATUS_ERR_SHORT
    - AP_ARG_STATUS_ERR_LONG
	- AP_ARG_STATUS_ERR_VALUE
	- AP_ARG_STATUS_ERR_VALUE2 

#### Structures

 - [x] ap_Desc - Description of an argument 
    - .short (char) and .long (const char*) fields describes name
    - .type (ap_ArgType) field specifies count and form of values passed with argument
 - [x] ap_Arg - Result of ap_next()
    - .id (int) field is an index of matching ap_Desc, -1 if the argument is stand-alone valure
    - .value (const char*) field is the first value of the argument if specified or 0, with size stored in .valueLen (size_t)
    - .value2 (const char*) field is the second value of the argument if specified or 0, with size stored in .value2Len (size_t)
    - .status (ap_ArgStatus) field specifies status (an error or succes) of parsing

#### Functions

 - [x] const char* ap_chop(int* argc, const char*** argv) - Returns 1st argument or 0 and shifts argc and argv
 - [x] ap_Arg ap_next(size_t descc, argap_Desc* descv, int* argc, const char*** argv) - Returns one parsed arguments. Calls ap_chop
 - [x] const char* ap_chop2(int* argc, const char* const** argv) -- Returns result of ap_chop (inline)
 - [x] ap_Arg ap_next2(size_t descc, argap_Desc* descv, int* argc, const char* const** argv) - Return result of ap_next (inline)

#### C++ wrapper

 - [x] all C enums have wrappers
 - [x] all C strucs have wrappers
 - [x] all C functions have wrappers
 - [x] inline functions like * 2 have theirs alternatives, but without '2' if C++ has no problem with overwritting them
 - [x] std::vector<toollib::ap::Arg> toollib::ap::getAll(std::size_t descc, toollib::ap::Desc* descv, int* argc, const char** argv) - Returns all parsed arguments (to the end or to first failure) (inline)
 - [x] std::vector<toollib::ap::Arg> toollib::ap::getAll(std::size_t descc, toollib::ap::Desc* descv, int* argc, const char* const* argv) - Returns result of the function above ^. (inline)

### CSV parser (toollib -> csv)

A simple library for CSV parsing.

#### Structures

 - [x] csv_Row - Result of csv_fgetrow and csv_parseRow. Represents a single CSV row.
    - .count (size_t) field is the count of values found and parsed in the row.
    - .values (char**) field is an array of parsed values (tokens).

#### Functions

 - [x] csv_Row csv_fgetrow(FILE* f, char delimeter) - Returns a single parsed line and moves the cursor. Caller should call csv_free.
 - [x] csv_Row csv_parseRow2(char* line, char delimeter) - Parses the row. Caller should call csv_free.
 - [x] csv_Row csv_parseRow(const char* line, char delimeter) - Copies the line and calls csv_parseRow2.
 - [x] void csv_freeRow(csv_Row row) - Frees csv_Row values.
 - [x] void csv_encode(csv_Row* row, char delimeter) - Encodes each value in row. (uses '\' as escape character) (reallocates memory)
 - [x] void csv_decode(csv_Row* row, char delimeter) - Decodes each value in row. (uses '\' as escape character)
 - [x] char* csv_strrow(csv_Row row, char delimeter) - Stringifies the row (without encoding). Caller owns the memory.

#### C++ wrapper

 - [x] all C strucs have wrappers
 - [x] all C functions have wrappers
 - [x] inline functions like \*2 have theirs alternatives, but without '2' if C++ has no problem with overwritting them
 - [x] inline Row fgetrow(std::istream& f, char delimeter) - Same as csv_fgetrow but uses std::istream instead of FILE\*
 - [x] inline std::string to_string(const Row& row, char delimeter) - to_string implementation for csv::Row

### Taurus Typed CSV (toollib -> tcsv)

A simple library for typed CSV parsing.

#### Types

 - [ ] num - signed integer
 - [ ] float - signed floating-point number
 - [ ] char - single character
 - [ ] seq - aka string
 - [ ] list(T) - a list of T typed elements
 - [ ] T[N] - an array of size N and T typed elements
 - [ ] if '!' starts the type, the column is encoded

#### Notation

 - [ ] Numbers follows [0-9]+
 - [ ] Floats follows ([0-9]+(\.[0-9]+))|(\.[0-9]+)
 - [ ] Chars follows .
 - [ ] Seqs follows \".+\"
 - [ ] Lists' items are seperated by semicommas and surrounded by prentisies
 - [ ] Arrays' items are seperated by semicommas. 

#### Enumerations

 - [ ] tcsv_PrimaryType - a list of basic types and collections
    - TCSV_PRIMARY_TYPE_NUM = 0
    - TCSV_PRIMARY_TYPE_FLOAT
    - TCSV_PRIMARY_TYPE_CHAR
    - TCSV_PRIMARY_TYPE_SEQ
    - TCSV_PRIMARY_TYPE_LIST
    - TCSV_PRIMARY_TYPE_ARRAY

#### Structures

 - [ ] tcsv_Type - Represents a column type.
    - .primary (tcsv_PrimaryType)
    - .count (size_t) - Only when primary is an array type, means the amount of elements.
    - .type (tcsv_Type) - Only when primary is an array or a list, means the type of items.
 - [ ] tcsv_Shape - Result of tcsv_fgetshape. Represents a table's row structure.
    - .count (size_t) field is an amount of columns
    - .types (tcsv_Type[]) field is a list of types
    - .encoding (bool[]) field is a list that describes which columns are encoded.
 - [ ] tcsv_Row - Result of tcsv_fgetrow. Represents a table's row.
    - .count (size_t)
    - .columns (void**) - an array of values, if the type is a list or a sequence the first 32bits are its size and then a pointer to the data (sizeof(void*)).

#### Functions

 - [ ] tcsv_Shape tcsv_fgetshape(FILE* f) - Returns a shape described in first row or {0}.
 - [ ] tcsv_Row tscv_fgetRow(FILE* f, tcsv_Shape* s) - Returns a row that matches shape s or {0}.
 - [ ] char* tscv_strshape(tscv_Shape* s) - Returns stringified shape.
 - [ ] char* tcsv_strrow(tscv_Row* r) - Returns stringified row.

#### C++ wrapper

TODO

### Taurus Regular Expresion library (toollib -> re)

A toolkit/library for regex (BRE/ERE + Taurus Regular Expresion Extension) tokenizing, compiling and interpreting.

#### IEEE POSIX Basic Regular Expresion

 - [ ] '^' - Matches starting position of input.
 - [ ] '.' - Matches any character.
 - [ ] '[]' - A bracket expresion. Matches one of the characters inside. 
              Slash escaping is not alowed, ']' is allowed only at 1st position. 
              '-' between two characters means any character in between.
 - [ ] '[\^]' - A negated bracked expresion.
 - [ ] '$' - Matches ending position of input.
 - [ ] '()' - Defines a marked expresion.
 - [ ] '\N' - Defines a back-reference expresion, were N is a digit from 1-9. Matches Nth marked expresion.
 - [ ] '*' - Kleene star.
 - [ ] '{M,N}' - Matches the preceding element at least M and not more than N times.
 - [ ] '\\' - Escape character.

#### IEEE POSIX Extended Regular Expresion

 - [ ] Back-compability with BRE
 - [ ] '?' - Matches the preceding element zero or one time.
 - [ ] '+' - Matches the preceding element one or more times.
 - [ ] '|' - The choice operator matches either the expression before or the expression after the operator.

#### Taurus Regular Expresion Extension

 - [ ] ';' - Forses the end of expresion.
 - [ ] '\-N' - Defines a back-reference expresion relative to current position.
 - [ ] '\+N' - Defines a forward-reference expresion relative to current position.
 - [ ] 'A-B' - Equivalent for '[A-B]'.
 - [ ] '"X"=()' - Defines a names expresion with name X. Also '"X"=Y;' is correct.
 - [ ] '"X"' - Defines a reference expresion, where X is a name.

#### Escape sequences

 - [ ] '\w' - 
 - [ ] '\n' -
 - [ ] '\t' -
 - [ ] '\v' - 
 - [ ] '\f' -
 - [ ] '\e' -
 - [ ] '\xXX' -
 - [ ] '\0XXX' - 
 - [ ] '\p' - punctation.

#### Enumerations

 - [ ] re_TokenType - describes type of token, where NOT indicates that the first bit (of a byte) is set
    - RE_TOKEN_TYPE_LITERAL       = 0x00 // 0b0000
    - RE_TOKEN_TYPE_ANY           = 0x01 // 0b0001
    - RE_TOKEN_TYPE_BRACKET       = 0x02 // 0b0010
    - RE_TOKEN_TYPE_MARK          = 0x03 // 0b0011
    - RE_TOKEN_TYPE_REFERENCE     = 0x04 // 0b0100
    - RE_TOKEN_TYPE_RANGE         = 0x05 // 0b0101
    - RE_TOKEN_TYPE_SOE           = 0x06 // 0b0110 - Start Of Expresion
    - RE_TOKEN_TYPE_NOT_LITERAL   = 0x08 // 0b1000
    - RE_TOKEN_TYPE_EOE           = 0x09 // 0b1001 - End Of Expresion
    - RE_TOKEN_TYPE_NOT_BRACKET   = 0x0A // 0b1010
    - RE_TOKEN_TYPE_NOT_MARK      = 0x0B // 0b1011
    - RE_TOKEN_TYPE_NOT_REFERENCE = 0x0C // 0b1100
    - RE_TOKEN_TYPE_NOT_RANGE     = 0x0D // 0b1101
 - [ ] re_Standard - specifies POSIX/Taurus standard/extension
    - RE_STD_BRE = 0
    - RE_STD_ERE
    - RE_STD_TRSRE

#### Structures

 - [ ] re_Token - Result of re_getToken, represents a single expression.
    - .type (re_TokenType) - the type of token.
	- .value (const char*) - the value of token.
	- .length (size_t) - the length of value.
	- .min (size_t) - the minimal expected amount of maches (default - 1, ? - 0, + - 1, * - 0).
	- .max (size_t) - the maximal expected amount of matches, zero if infinity (default - 1, ? - 1, + - 0, * - 0).

#### Functions

 - [ ] re_Token re_getToken(const char** str, re_Standard std) - Returns next fount experession without rcursive parsing or {0}, moves str to the end of the token.
 - [ ] void re_compile(const char* str, char[256]** table, size_t* size, re_Standard std) - Compiles regex in str into table for re_fsm.
 - [ ] size_t re_fsm(const char[256]* table, size_t size, size_t state, char c) - Returns next state of final state mashine described in table.
 - [ ] const char* re_strtype(re_TokenType type) - stringifies the token type.
 - [ ] char[4] re_strtype3(re_TokenType type) - strinifies the token type into 3-letter abrivation, zero-terminated.

#### C++ wrapper

TODO

### C Vector (toollib -> cvec)

A simple vector implementation (like dynamic array not math Xd). User gets a pointer to the data, not the header itself.

#### Structures

 - [ ] cvec_Header - A header of a vector that introduces the data.
    - .count (size_t) - the count of elements.
    - .element (size_t) - the size of a single element.
    - .capacity (size_t) - the amount of allocated memory.
    - .data (char[]) - the data (flexible array member).

#### Functions

 - [ ] void* cvec_new(size_t element, size_t n) - Allocates a new vector with n elements and returns a pointer to the data.
 - [ ] void cvec_free(void* vec) - Frees the vector.
 - [ ] size_t cvec_length(void* vec) - Returns .count of vec, inline.
 - [ ] size_t cvec_capacity(void* vec) - Returns .capacity of vec, inline.
 - [ ] bool cvsc_needsRealloc(void* vec, size_t n) - Returns true if vec needs reallocating for n additional elements.
 - [ ] void* cvec_ensure(void* vec, size_t n) - Reallocates memory of the vec to fit the n new elements if needed and returns a new pointer to the data, or the vec otherwise.
 - [ ] void* cvec_add(void* vec, void* val) - Appends the value behind val to the vec, realocates if needed and returns vec or a new pointer.
 - [ ] void* cvec_addArray(void* vec, void* arr, size_t n) - Same as cvec_add but for arrays.

#### Macros
 - [ ] cvsc_ensureN(void** VEC, size_t N) => * VEC = cvec_ensure(* VEC, N);
 - [ ] cvec_addN(void** VEC, void* VAL) => * VEC = cvec_add(* VEC, VAL);
 - [ ] cvec_addArrayN(void** VEC, void* ARR, size_t N) => * VEC = cvec_addArray(* VEC, ARR, N);

### C Area (toollib -> carea)

A simple memory area implementation. An area is a peace of preallocated memory (also reallocable) that you can free once and reuse.

#### Structures

 - [ ] carea_Area - Holds information about the state of the area.
    - .capacity (size_t) - the amount of preallocated memory. 
    - .size (size_t) - the amount of allocated memory inside the area.
    - .data (void*) - the data.

#### Functions

 - [ ] carea_Area carea_new(size_t cap) - Creates a new area.
 - [ ] void* carea_alloc(carea_Area* area, size_t n) - Returns a pointer to the end of area, if n is bigger than the capacity, a new area page is allocated. Returns 0 on error.
 - [ ] void carea_free(carea_Area* area) - Sets the size of area to 0, if there were any new pages allocated, they will be freed permanently and the area will be expanded.
 - [ ] void carea_freeHard(carea_Area* area) - Frees the area pernamently.

