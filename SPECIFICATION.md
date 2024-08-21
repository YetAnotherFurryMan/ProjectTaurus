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
In addition to the vector the library provides an implementation for an associative vector.

#### Structures

 - [x] cvec_Header - A header of a vector that introduces the data.
    - .count (size_t) - the count of elements.
    - .element (size_t) - the size of a single element.
    - .size (size_t) - the amount of allocated memory.
    - .data (char[]) - the data (flexible array member).
 - [ ] cassoc_Header - A header of an associativ vector that introduces the data.
    - .values (cvec) - A pointer to the value vector.
    - .keys (cvec) - A pointer to the vector of key-hash pairs.

#### Functions

 - [x] void* cvec_new(size_t element, size_t n) - Allocates a new vector with n elements and returns a pointer to the data or NULL.
 - [x] void cvec_free(void* vec) - Frees the vector.
 - [x] size_t cvec_element(void* vec) - Returns .element of vec, inline.
 - [x] size_t cvec_length(void* vec) - Returns .count of vec, inline.
 - [x] size_t cvec_size(void* vec) - Returns .size of vec, inline.
 - [x] bool cvec_needsRealloc(void* vec, size_t n) - Returns true if vec needs reallocating for n additional elements.
 - [x] void* cvec_ensure(void* vec, size_t n) - Reallocates memory of the vec to fit the n new elements if needed and returns a new pointer to the data, or the vec otherwise.
 - [x] void* cvec_add(void* vec, void* val) - Appends the value behind val to the vec, realocates if needed and returns vec or a new pointer.
 - [x] void* cvec_addArray(void* vec, void* arr, size_t n) - Same as cvec_add but for arrays.
 - [x] void* cvec_insert(void* vec, size_t index, void* val) - Inserds the valueat index, or to the end of the vec, in case of reallocation behaves as cvec_add.
 - [x] void* cvec_insertArray(void* vec, size_t index, void* arr, size_t n) - Like addArray but insert.
 - [x] void cvec_remove(void* vec, size_t index) - Remove element at index.
 - [x] void* cvec_get(void* vec, size_t index) - Returns a pointer to the element at index or NULL.
 - [ ] void* cassoc_new(size_t value, size_t n) - Allocates a new associative vector with n elements.
 - [ ] void cassoc_free(void* vec) - Frees the vec.
 - [ ] void* cassoc_keys(void* vec, size_t* count) - Returns the keys and theirs amount (by argument).
 - [ ] void* cassoc_values(void* vec, size_t count) - Returns the values and theirs amount (by argument).
 - [ ] void* cassoc_set(const char* key, void* val) - Sets the value assocated with the key, inserts if needed.
 - [ ] void* cassoc_get(const char* key) - Returns a pointer to the value assocated with the key or NULL.

#### Macros

 - [x] cvsc_ensureN(void** VEC, size_t N) => * VEC = cvec_ensure(* VEC, N);
 - [x] cvec_addN(void** VEC, void* VAL) => * VEC = cvec_add(* VEC, VAL);
 - [x] cvec_addArrayN(void** VEC, void* ARR, size_t N) => * VEC = cvec_addArray(* VEC, ARR, N);
 - [ ] cvec_addAllN(void** VEC, ...) => * VEC = cvec_addArray(* VEC, ...);
 - [x] cvec_insertN(void** VEC, size_t IDX, void* VAL) => * VEC = cvec_insert(* VEC, IDX, VAL);
 - [x] cvec_insertArrayN(void** VEC, size_t IDX, void* ARR, size_t N) => * VEC = cvec_insertArray(* VEC, IDX, ARR, N);
 - [ ] cvec_insertAllN(void** VEC, size_t index, ...) => * VEC = cvec_insertArray(* VEC, ...);
 - [ ] cassoc_setN(void** VEC, void* KEY, void* VAL) => * VEC = cassoc_set(* VEC, KEY, VAL);

### C Area (toollib -> carea)

A simple memory area implementation. An area is a peace of preallocated memory (also reallocable) that you can free once and reuse.

#### Structures

 - [x] carea_Header - Holds information about the state of the area.
    - .size (size_t) - the amount of allocated memory inside the area.
    - .capacity (size_t) - the amount of preallocated memory.
    - .child (carea_Header*) - a next page of area or NULL.
    - .data (char[]) - the data.

#### Functions

 - [x] void* carea_new() - Creates a new area.
 - [x] void* carea_alloc(void* area, size_t n) - Returns a pointer to the end of area, if n is bigger than the capacity, a new area page is allocated. Returns 0 on error.
 - [x] void carea_free(void* area) - Sets the size of area to 0, if there were any new pages allocated, they will be freed permanently and the area will be expanded.
 - [x] void carea_freeHard(void* area) - Frees the area pernamently.

### Taurus Lisp (toollib -> trslisp)

A light-weight library for parsing and evaluating list-based command-first expresions (lisp).

#### Syntax

 - A list is defined inside prentises e.g. (a b c)
 - A pair is defined by dot e.g. (a . b)
 - There is special nil value.
 - Command-first, means that in (a b c) case the 'a' is the command and 'b', 'c' are the arguments.
 - Vectors (and matrixes) are defined in square brackets e.g. [1 2 3] or [[1 2 3] [4 5 6] [7 8 9]]
 - Strings are defined in quotes e.g. 'Hello World'
 - (a . (b . (c . nil))) = (a b c)
 - All lists are evaluated like calls, vectors and strings are always values.
 - Symbols begins with $ (instead of ') and represent both in-script defined functions and variables e.g. ($a b c) or ($f (a b c))
 - However when $ is followed by a number from 0 to 9 it means the index of argument passed to the function. When the argument wasn't passed it is equal to nil. $$ is a list of arguments with indexes greater than 9, $$n where 9 >= n >= 0 means all arguments that follows n, so $$0 is a list of all arguments passed to the function. E.g. ($f (a $1) (b $2 $3) ($f $$4))

#### Types

 - [ ] trslisp_FunctionType (??) - A function pointer that can be called from lisp.

#### Enumerations

 - [ ] trslisp_AST - A list of possible AST.
    - TRSLISP_LIST
    - TRSLISP_VALUE
 - [ ] trslisp_Type - A list of types.
    - TRSLISP_INT
    - TRSLISP_FLOAT
    - TRSLISP_SEQ
    - TRSLISP_VEC
 - [ ] trslisp_Error - A list of possible state errors.
    - TRSLISP_OK = 0
    - TRSLISP_EXPEXTED_LIST_END
    - TRSLISP_EXPECTED_VEC_END
    - TRSLISP_EXPECTED_SEQ_END
    - TRSLISP_UNKNOWN_COMMAND
    - TRSLISP_COMMAND_FAILED
    - TRSLISP_EOM  - End Of Memeory

#### Structures

 - [ ] trslisp_Lisp - The state.
    - .data (void*) - A pointer to prealocated memory.
    - .size (size_t) - The capacity of memory.
    - .realloc (void*(size_t)) - A function pointer to allocator, NULL if VM is not allowed to reallocate memory.
    - .error (trslisp_Error) - Set on error.
 - [ ] trslisp_Value - A value litteral.
    - .type (trslisp_AST) - AST, always on top.
    - .valueType (trslisp_Type) - the value type.
    - .value (void*) - Pointer to the data.
 - [ ] trslisp_List - A list AST.
    - .type (trslisp_AST) - AST, always on top.
    - .command (const char*) - The list command (first element).
    - .count (size_t) - A count of elements of the list.
    - .data (void*) - The elements.

#### Functions

 - [ ] trslisp_Lisp trslisp_new(void* data, size_t size, void*(size_t) realloc) - Initializes new state, inline.
 - [ ] trslisp_Value trslisp_eval(trslisp_Lisp* st, const char* src) - Evaluates src and returns value.
 - [ ] trslisp_Value trslisp_evalFile(trslisp_Lisp* st, FILE* file) - Evaluates the file and returns value.

### Taurus Simple Evaluated Values (toollib -> tsev)

A simple and very light-weight library for evaluating simple config files.

#### Types

 - [ ] int - a signed integer
 - [ ] seq - aka string

#### Syntax

 - [ ] A name is a sequense that matches \[a-zA-Z_]\[a-zA-Z0-9_]*
 - [ ] A value is int if matches \[0-9]+ otherwise it is a seq
 - [ ] Defining/overriding NAME = VALUE
 - [ ] Basic algebra with ints (+, -, /, *, %)
 - [ ] Concatencion with plus sign (+)
 - [ ] Evaluation priority may be changed with preanteces e.g. (1 + 2) * 3
 - [ ] Comments with double-slash (//)

#### Enumerations

 - [ ] tsev_Type - A list of types
    - TSEV_INT
    - TSEV_SEQ

#### Structures

 - [ ] tsev_Value - A single value.
    - .name (const char*) - A value name.
    - .value (void*) - A value (not a ptr if .type = INT else char*)
    - .type (tsev_Type) - A type.

#### Functions

 - [ ] size_t tsev_eval(const char* str, tsev_Value* out) - Evaluates str and returns a list of values (by out) with last element 0ed out and its size.
 - [ ] size_t tsev_evalFile(FILE* file, tsev_Value* out)  - Reads file and calls tsev_eval.
 - [ ] void tsev_free(tsev_Value* vals) - Frees values.

