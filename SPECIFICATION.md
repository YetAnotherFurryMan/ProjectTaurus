# Taurus Project Specification Description

 - [x] written in C/C++/D
 - [ ] rewritten in Taurus

## Taurus Argument Parser (toollib -> trsap)

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
 - [x] inline functions like *2 have theirs alternatives, but without '2' if C++ has no problem with overwritting them
 - [x] std::vector<trs::ap::Arg> trs::ap::getAll(std::size_t descc, trs::ap::Desc* descv, int* argc, const char** argv) - Returns all parsed arguments (to the end or to first failure) (inline)
 - [x] std::vector<trs::ap::Arg> trs::ap::getAll(std::size_t descc, trs::ap::Desc* descv, int* argc, const char* const* argv) - Returns result of the function above ^. (inline)

## CSV parser (toollib -> csv)

A simple library for CSV parsing.

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
 - [x] char* csv_strrow(csv_Row row, char delimeter) - Stringifies the row (without encoding). Caller owns the memory.

### C++ wrapper
 - [x] all C strucs have wrappers
 - [x] all C functions have wrappers
 - [x] inline functions like \*2 have theirs alternatives, but without '2' if C++ has no problem with overwritting them
 - [x] inline Row fgetrow(std::istream& f, char delimeter) - Same as csv_fgetrow but uses std::istream instead of FILE\*
 - [x] inline std::string to_string(const Row& row, char delimeter) - to_string implementation for csv::Row

## Taurus Typed CSV (toollib -> tcsv)

A simple library for typed CSV parsing.

### Types
 - [ ] num - signed integer
 - [ ] float - signed floating-point number
 - [ ] char - single character
 - [ ] seq - aka string
 - [ ] list(T) - a list of T typed elements
 - [ ] T[N] - an array of size N and T typed elements
 - [ ] if '!' starts the type, the column is encoded

### Notation
 - [ ] Numbers follows [0-9]+
 - [ ] Floats follows ([0-9]+(\.[0-9]+))|(\.[0-9]+)
 - [ ] Chars follows .
 - [ ] Seqs follows \".+\"
 - [ ] Lists' items are seperated by semicommas and surrounded by prentisies
 - [ ] Arrays' items are seperated by semicommas. 

### Enumerations
 - [ ] tcsv_PrimaryType - a list of basic types and collections
    - TCSV_PRIMARY_TYPE_NUM = 0
    - TCSV_PRIMARY_TYPE_FLOAT
    - TCSV_PRIMARY_TYPE_CHAR
    - TCSV_PRIMARY_TYPE_SEQ
    - TCSV_PRIMARY_TYPE_LIST
    - TCSV_PRIMARY_TYPE_ARRAY

### Structures
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

### Functions
 - [ ] tcsv_Shape tcsv_fgetshape(FILE* f) - Returns a shape described in first row or {0}.
 - [ ] tcsv_Row tscv_fgetRow(FILE* f, tcsv_Shape* s) - Returns a row that matches shape s or {0}.
 - [ ] char* tscv_strshape(tscv_Shape* s) - Returns stringified shape.
 - [ ] char* tcsv_strrow(tscv_Row* r) - Returns stringified row.

### C++ wrapper
TODO

## Taurus Regular Expresion library (trsre)

A toolkit/library for regex (BRE/ERE + Taurus Regular Expresion Extension) tokenizing, compiling and interpreting.

### IEEE POSIX Basic Regular Expresion
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

### IEEE POSIX Extended Regular Expresion
 - [ ] Back-compability with BRE
 - [ ] '?' - Matches the preceding element zero or one time.
 - [ ] '+' - Matches the preceding element one or more times.
 - [ ] '|' - The choice operator matches either the expression before or the expression after the operator.

### Taurus Regular Expresion Extension
 - [ ] ';' - Forses the end of expresion.
 - [ ] '\-N' - Defines a back-reference expresion relative to current position.
 - [ ] '\+N' - Defines a forward-reference expresion relative to current position.
 - [ ] 'A-B' - Equivalent for '[A-B]'.
 - [ ] '"X"=()' - Defines a names expresion with name X. Also '"X"=Y;' is correct.
 - [ ] '"X"' - Defines a reference expresion, where X is a name.

### Escape sequences
 - [ ] '\w' - 
 - [ ] '\n' -
 - [ ] '\t' -
 - [ ] '\v' - 
 - [ ] '\f' -
 - [ ] '\e' -
 - [ ] '\xXX' -
 - [ ] '\0XXX' - 
 - [ ] '\p' - punctation.

### Enumerations
 - [ ] trsre_TokenType - describes type of token, where NOT indicates that the first bit (of a byte) is set
    - TRSRE_TOKEN_TYPE_LITERAL       = 0x00 // 0b0000
    - TRSRE_TOKEN_TYPE_ANY           = 0x01 // 0b0001
    - TRSRE_TOKEN_TYPE_BRACKET       = 0x02 // 0b0010
    - TRSRE_TOKEN_TYPE_MARK          = 0x03 // 0b0011
    - TRSRE_TOKEN_TYPE_REFERENCE     = 0x04 // 0b0100
    - TRSRE_TOKEN_TYPE_RANGE         = 0x05 // 0b0101
    - TRSRE_TOKEN_TYPE_SOE           = 0x06 // 0b0110 - Start Of Expresion
    - TRSRE_TOKEN_TYPE_NOT_LITERAL   = 0x08 // 0b1000
    - TRSRE_TOKEN_TYPE_EOE           = 0x09 // 0b1001 - End Of Expresion
    - TRSRE_TOKEN_TYPE_NOT_BRACKET   = 0x0A // 0b1010
    - TRSRE_TOKEN_TYPE_NOT_MARK      = 0x0B // 0b1011
    - TRSRE_TOKEN_TYPE_NOT_REFERENCE = 0x0C // 0b1100
    - TRSRE_TOKEN_TYPE_NOT_RANGE     = 0x0D // 0b1101
 - [ ] trsre_Standard - specifies POSIX/Taurus standard/extension
    - TRSRE_STD_BRE = 0
    - TRSRE_STD_ERE
    - TRSRE_STD_TRSRE

### Structures
 - [ ] trsre_Token - Result of trsre_getToken, represents a single expression.
    - .type (trsre_TokenType) - the type of token.
	- .value (const char*) - the value of token.
	- .length (size_t) - the length of value.
	- .min (size_t) - the minimal expected amount of maches (default - 1, ? - 0, + - 1, * - 0).
	- .max (size_t) - the maximal expected amount of matches, zero if infinity (default - 1, ? - 1, + - 0, * - 0).

### Functions
 - [ ] trsre_Token trsre_getToken(const char** str, trsre_Standard std) - Returns next fount experession without rcursive parsing or {0}, moves str to the end of the token.
 - [ ] void trsre_compile(const char* str, char[256]** table, size_t* size, trsre_Standard std) - Compiles regex in str into table for trsre_fsm.
 - [ ] size_t trsre_fsm(const char[256]* table, size_t size, size_t state, char c) - Returns next state of final state mashine described in table.
 - [ ] const char* trsre_strtype(trsre_TokenType type) - stringifies the token type.
 - [ ] char[4] trsre_strtype3(trsre_TokenType type) - strinifies the token type into 3-letter abrivation, zero-terminated.

### C++ wrapper

TODO

## Taurus Project Manager

A tool for managing projects.

### Common argument meaning
 - name(n) [V]
 - language(l) [V]
 - module(m) [V]
 - project(p) [V]

### Subprograms
 - [ ] init - Initializes project inside the directory it was executed.
 - [ ] build - Generates build system.
 - [ ] module
    - [ ] list - Prints all registered modules.
    - [ ] add - Registers an existing module.
    - [ ] init - Initializes a new module.
 - [ ] project
    - [ ] list - Prints all registered subprojectss.
    - [ ] add - Registers an existing subproject.
    - [ ] init - Initializes a new subproject.
 - [ ] language
    - [ ] list - Pronts all registered languages.
    - [ ] add - Registers a new language.
 - [ ] test - Executes tests.
    - [ ] list - Prints all registered tests.
    - [ ] add - Registers an existing test.
    - [ ] init - Initializes a new test.
 - [ ] checkout - Verifies and tries to fix the project if needed.
    - [ ] remove - Tries to remove module, project or language from the project configuration.
    - [ ] module - Tries to modify module configuration.

### Build system
 - [ ] Supports GNU Make
 - [ ] Supports Ninja
 - [ ] Supports Taurus Build System
 - [ ] Debbug target
 - [ ] Release target

### Tests
 - [ ] Prints "OK" on pass.
 - [ ] Prints "FAILED" on failure.
 - [ ] Success information on stdout.

