# CSV

This is the documentation of csv library which is Taurus's extension made for .csv file parsing.

How does it work:
The library provides functions to operate on each individual row (line) in .csv file. 
If you need to use new-line symbol or the delimeter you may need to encode the row using 
csv_encode function. The rows are not encoded by default. After parsing encoded row you 
may need to call csv_decode, because the row are not decoded by default too.

## C header

 1. struct csv_Row is the structure of singe csv row (line) where m_Count is how many columns did we parse and m_Values is a dynamic array of these columns.

``` C
struct csv_Row{
	size_t m_Count;
	char** m_Values;
};
```

 2. csv_fgetrow is a function that reads one single line from file f and parses it to csv_Row. Returns zeroed structure on failure. You should call csv_freeRow when you do not need the data anymore.

``` C
csv_Row csv_fgetrow(FILE* f, char delimeter);
```

 3. csv_parseRow is a function that parses the line (uses delimeter as separator of columns). Because it uses strtok you should prefer to call csv_parseRow2 when you do not need the line.

``` C
csv_Row csv_parseRow(const char* line, char delimeter);
csv_Row csv_parseRow2(char* line, char delimeter);
```

 4. csv_freeRow is a functions that calls free for each value in row.

``` C
void csv_freeRow(csv_Row row);
```

 5. csv_encode is an encoding function that replaces delimeter with '\d', new-line with '\n', carriage return with '\r' and '\' with '\\'. The result is saved back to row, so the data may be reallocated and invalidated.

``` C
void csv_encode(csv_Row* row, char delimeter);
```

 6. csv_decode is an encoding function that reverses the result of csv_encode.

``` C
void csv_decode(csv_Row* row, char delimeter);
```

 7. csv_strrow stringifies the row back to .csv format. Adds new-line to the end.

``` C
char* csv_strrow(const csv_Row row, char delimeter);
```

## C++ header

Warning: csv.hpp includes csv.h inside of csv namespace.

 1. Row is a C++ wrapper around csv_Row. csv_freeRow is automaticly called in destructor.

``` C++
struct csv::Row{
	size_t m_Count;
	char** m_Values;

	Row(size_t count, char** values);
	Row(csv_Row row);
	~Row();
};
```

 2. fgetrow is an inline wrapping function around csv_fgetrow. Also the second one is reimplementation, so it do not call to csv_fgetrow at all.

``` C++ 
inline csv::Row csv::fgetrow(FILE* f, char delimeter);
inline csv::Row csv::fgetrow(std::istream& f, char delimeter);
```  
 
 3. parseRow is an inline wrapping function around csv_csv_parseRow.
  
``` C++  
inline csv::Row csv::parseRow(const char* line, char delimeter);
inline csv::Row csv::parseRow(char* line, char delimeter);
```

 4. encode is an inline wrapping function around csv_encode.

``` C++
inline void csv::encode(csv::Row& row, char delimeter);
```

 5. decode is an inline wrapping function around csv_decode.

``` C++
inline void csv::decode(csv::Row& row, char delimeter);
```

 6. strrow is an inline wrapping function around csv_strrow.

``` C++
inline char* csv::strrow(const csv::Row& row, char delimeter);
```

 7. to_string is an inline function that stringifies the row. Calls strrow.

``` C++
inline std::string csv::to_string(const csv::Row& row, char delimeter = '|');
```

