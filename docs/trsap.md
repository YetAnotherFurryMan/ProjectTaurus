# Taurus Argument Parser Documentation

This is the documentation of trsap library which is command-line arguments\' parser written for consistancy of how Taurus tools manages arguments.

How does it work:
 - If the argument does not start with '-' then it is parsed as literal value (m_Id < 0). There is one exception '--' which is also returned as literal.
 - If the argument starts with single '-' then the next character is considered as the short form.
 - If the argument starts with '--' then the next word (characters following '--' to '=', ':' or null) is considered as the long form;
 - The value of argument can be passed after ':' or '=' characters, also in the short form we are not obligated to use any separator because the short form is always one character.
 - The second value (m_Value2) may be specified after the first value seperated by ':' or '='.

## C header:

 1. enum trsap_ArgType provides information about accepting values by argument in argument description.

``` C
typedef enum {
	TRSAP_ARG_TYPE_FLAG = 0,
	TRSAP_ARG_TYPE_VALUE,
	TRSAP_ARG_TYPE_VALUE_OPTIONAL,
	TRSAP_ARG_TYPE_VALUE2,
	TRSAP_ARG_TYPE_VALUE2_OPTIONAL
} trsap_ArgType;
```

 2. enum trsap_ArgStatus provides information about the status of parsing. The EOI means that we reached the end of input (for example argc == 0), OK* means that everything was succesfull for now and finally ERR* means that the given input included some sort of error.

``` C
typedef enum {
	TRSAP_ARG_STATUS_EOI = 0, // End Of Input
	TRSAP_ARG_STATUS_OK,
	TRSAP_ARG_STATUS_OK_VALUE,
	TRSAP_ARG_STATUS_OK_VALUE2,
	TRSAP_ARG_STATUS_ERR_SHORT,
	TRSAP_ARG_STATUS_ERR_LONG,
	TRSAP_ARG_STATUS_ERR_VALUE,
	TRSAP_ARG_STATUS_ERR_VALUE2
} trsap_ArgStatus;
```

 3. struct trsap_Desc is the structure of single argument description that (in an array) should be provided to the parsing function.

``` C
struct trsap_Desc{
	char m_Short;
	const char* m_Long;
	trsap_ArgType m_Type;
};
```

 4. struct trsap_Arg is the result of parsing single argument returned by the parser.

```C
struct trsap_Arg{
	int m_Id;
	const char* m_Value;
	const char* m_Value2;
	size_t m_ValueLen;
	size_t m_Value2Len;
	trsap_ArgStatus m_Status;
};
```

 5. trsap_chop is a function that shifts both argc and argv, then returns shifted element (like pop).

``` C
const char* trsap_chop(int* argc, const char*** argv);
inline const char* trsap_chop2(int* argc, const char* const** argv);
```

 6. trsap_next is a basic parsing function. Parses one argument. Warning: that function calls trsap_chop at least once, but can do it up to 3 times.

``` C
trsap_Arg trsap_next(size_t descc, trsap_Desc* descv, int* argc, const char*** argv);
inline trsap_Arg trsap_next2(size_t descc, trsap_Desc* descv, int* argc, const char* const** argv);
```

## C++ header:

Warning: trsap.hpp includes trsap.h inside of trs::ap namespace.

 1. ArgType is a C++ wrapper for trsap_ArgType.

``` C++
enum class trs::ap::ArgType {
	FLAG             = TRSAP_ARG_TYPE_FLAG,
	VALUE            = TRSAP_ARG_TYPE_VALUE,
	VALUE_OPTIONAL   = TRSAP_ARG_TYPE_VALUE_OPTIONAL,
	VALUE2           = TRSAP_ARG_TYPE_VALUE2,
	VALUE2_OPTIONAL	 = TRSAP_ARG_TYPE_VALUE2_OPTIONAL	
};
```

 2. ArgStatus is a C++ wrapper for trsap_ArgStatus.

``` C++
enum class trs::ap::ArgStatus {
	EOI        = TRSAP_ARG_STATUS_EOI,
	OK         = TRSAP_ARG_STATUS_OK,
	OK_VALUE   = TRSAP_ARG_STATUS_OK_VALUE,
	OK_VALUE2  = TRSAP_ARG_STATUS_OK_VALUE2,
	ERR_SHORT  = TRSAP_ARG_STATUS_ERR_SHORT,
	ERR_LONG   = TRSAP_ARG_STATUS_ERR_LONG,
	ERR_VALUE  = TRSAP_ARG_STATUS_ERR_VALUE,
	ERR_VALUE2 = TRSAP_ARG_STATUS_ERR_VALUE2
};
```

 3. Desc is a C++ wrapper for trsap_Desc, but uses ArgType instead of trsap_ArgType.

``` C++
struct trs::ap::Desc{
	char m_Short = 0;
	const char* m_Long = 0;
	ArgType m_Type = ArgType::FLAG;
};
```

 4. Arg is a C++ wrapper for trsap_Arg, but uses ArgStatus instead of trsap_ArgStatus.

``` C++
struct trs::ap::Arg{
	int m_Id = 0;
	const char* m_Value = 0;
	const char* m_Value2 = 0;
	size_t m_ValueLen = 0;
	size_t m_Value2Len = 0;
	ArgStatus m_Status = ArgStatus::EOI;
};
```

 5. chop is an inline wrapping function around trsap_chop.

``` C++ 
inline const char* trs::ap::chop(int* argc, const char*** argv);
inline const char* trs::ap::chop(int* argc, const char* const** argv);
```

 6. next is an inline wrapping function around trsap_next.

``` C++ 
inline trs::ap::Arg trs::ap::next(size_t descc, trs::ap::Desc* descv, int* argc, const char*** argv);
inline trs::ap::Arg trs::ap::next(size_t descc, trs::ap::Desc* descv, int* argc, const char* const** argv);
```

 7. getAll is an inline function that calls trs_next to the EOI or any ERR* and returns std::vector of parsed arguments.

``` C++
inline std::vector<trs::ap::Arg> trs::ap::getAll(size_t descc, trs::ap::Desc* descv, int* argc, const char*** argv);
inline std::vector<trs::ap::Arg> trs::ap::getAll(size_t descc, trs::ap::Desc* descv, int* argc, const char* const** argv);
```

