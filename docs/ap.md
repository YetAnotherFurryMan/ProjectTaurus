# Taurus Toollib Argument Parser Documentation

This is the documentation of trsap library which is command-line arguments\' parser written for consistancy of how Taurus tools manages arguments.

How does it work:
 - If the argument does not start with '-' then it is parsed as literal value (m_Id < 0). There is one exception '--' which is also returned as literal.
 - If the argument starts with single '-' then the next character is considered as the short form.
 - If the argument starts with '--' then the next word (characters following '--' to '=', ':' or null) is considered as the long form;
 - The value of argument can be passed after ':' or '=' characters, also in the short form we are not obligated to use any separator because the short form is always one character.
 - The second value (m_Value2) may be specified after the first value seperated by ':' or '='.

## C header:

 1. enum ap_ArgType provides information about accepting values by argument in argument description.

``` C
typedef enum {
	AP_ARG_TYPE_FLAG = 0,
	AP_ARG_TYPE_VALUE,
	AP_ARG_TYPE_VALUE_OPTIONAL,
	AP_ARG_TYPE_VALUE2,
	AP_ARG_TYPE_VALUE2_OPTIONAL
} ap_ArgType;
```

 2. enum ap_ArgStatus provides information about the status of parsing. The EOI means that we reached the end of input (for example argc == 0), OK* means that everything was succesfull for now and finally ERR* means that the given input included some sort of error.

``` C
typedef enum {
	AP_ARG_STATUS_EOI = 0, // End Of Input
	AP_ARG_STATUS_OK,
	AP_ARG_STATUS_ERR_SHORT,
	AP_ARG_STATUS_ERR_LONG,
	AP_ARG_STATUS_ERR_VALUE,
	AP_ARG_STATUS_ERR_VALUE2
} ap_ArgStatus;
```

 3. struct ap_Desc is the structure of single argument description that (in an array) should be provided to the parsing function.

``` C
struct ap_Desc{
	char m_Short;
	const char* m_Long;
	ap_ArgType m_Type;
};
```

 4. struct ap_Arg is the result of parsing single argument returned by the parser.

```C
struct ap_Arg{
	int m_Id;
	const char* m_Value;
	const char* m_Value2;
	size_t m_ValueLen;
	size_t m_Value2Len;
	ap_ArgStatus m_Status;
};
```

 5. ap_chop is a function that shifts both argc and argv, then returns shifted element (like pop).

``` C
const char* ap_chop(int* argc, const char*** argv);
inline const char* ap_chop2(int* argc, const char* const** argv);
```

 6. ap_next is a basic parsing function. Parses one argument. Warning: that function calls ap_chop at least once, but can do it up to 3 times.

``` C
ap_Arg ap_next(size_t descc, ap_Desc* descv, int* argc, const char*** argv);
inline ap_Arg ap_next2(size_t descc, ap_Desc* descv, int* argc, const char* const** argv);
```

## C++ header:

Warning: trsap.hpp includes trsap.h inside of toollib::ap namespace.

 1. ArgType is a C++ wrapper for ap_ArgType.

``` C++
enum class toollib::ap::ArgType {
	FLAG             = AP_ARG_TYPE_FLAG,
	VALUE            = AP_ARG_TYPE_VALUE,
	VALUE_OPTIONAL   = AP_ARG_TYPE_VALUE_OPTIONAL,
	VALUE2           = AP_ARG_TYPE_VALUE2,
	VALUE2_OPTIONAL	 = AP_ARG_TYPE_VALUE2_OPTIONAL	
};
```

 2. ArgStatus is a C++ wrapper for ap_ArgStatus.

``` C++
enum class toollib::ap::ArgStatus {
	EOI        = AP_ARG_STATUS_EOI,
	OK         = AP_ARG_STATUS_OK,
	OK_VALUE   = AP_ARG_STATUS_OK_VALUE,
	OK_VALUE2  = AP_ARG_STATUS_OK_VALUE2,
	ERR_SHORT  = AP_ARG_STATUS_ERR_SHORT,
	ERR_LONG   = AP_ARG_STATUS_ERR_LONG,
	ERR_VALUE  = AP_ARG_STATUS_ERR_VALUE,
	ERR_VALUE2 = AP_ARG_STATUS_ERR_VALUE2
};
```

 3. Desc is a C++ wrapper for ap_Desc, but uses ArgType instead of ap_ArgType.

``` C++
struct toollib::ap::Desc{
	char m_Short = 0;
	const char* m_Long = 0;
	ArgType m_Type = ArgType::FLAG;
};
```

 4. Arg is a C++ wrapper for ap_Arg, but uses ArgStatus instead of ap_ArgStatus.

``` C++
struct toollib::ap::Arg{
	int m_Id = 0;
	const char* m_Value = 0;
	const char* m_Value2 = 0;
	size_t m_ValueLen = 0;
	size_t m_Value2Len = 0;
	ArgStatus m_Status = ArgStatus::EOI;
};
```

 5. chop is an inline wrapping function around ap_chop.

``` C++ 
inline const char* toollib::ap::chop(int* argc, const char*** argv);
inline const char* toollib::ap::chop(int* argc, const char* const** argv);
```

 6. next is an inline wrapping function around ap_next.

``` C++ 
inline toollib::ap::Arg toollib::ap::next(size_t descc, toollib::ap::Desc* descv, int* argc, const char*** argv);
inline toollib::ap::Arg toollib::ap::next(size_t descc, toollib::ap::Desc* descv, int* argc, const char* const** argv);
```

 7. getAll is an inline function that calls trs_next to the EOI or any ERR* and returns std::vector of parsed arguments.

``` C++
inline std::vector<toollib::ap::Arg> toollib::ap::getAll(size_t descc, toollib::ap::Desc* descv, int* argc, const char*** argv);
inline std::vector<toollib::ap::Arg> toollib::ap::getAll(size_t descc, toollib::ap::Desc* descv, int* argc, const char* const** argv);
```

