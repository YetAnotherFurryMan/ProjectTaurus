#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <trsre/trsre.h>

typedef struct{
	const char* re;
	const size_t count;
	const trsre_Token* toks;
} Case;

const Case cases[180] = {
	{".",          1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, false}}},
	{".?",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, false}}},
	{".+",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, true}}},
	{".*",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, true}}},
	{".10+",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}}},
	{".+10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 10, false}}},
	{".10+20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}}},
	{".10*",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}}},
	{".*10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 10, false}}},
	{".10*20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}}},
	{"$",          1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, false}}},
	{"$?",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, false}}},
	{"$+",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, true}}},
	{"$*",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, true}}},
	{"$10+",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}}},
	{"$+10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 10, false}}},
	{"$10+20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}}},
	{"$10*",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}}},
	{"$*10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 10, false}}},
	{"$10*20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}}},
	{"a",          1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a?",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}}},
	{"a+",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}}},
	{"a*",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}}},
	{"a10+",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}}},
	{"a+10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}}},
	{"a10+20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}}},
	{"a10*",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}}},
	{"a*10",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}}},
	{"a10*20",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}}},
	{"(aa)",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}}},
	{"(aa)?",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}}},
	{"(aa)+",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}}},
	{"(aa)*",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}}},
	{"(aa)10+",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}}},
	{"(aa)+10",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}}},
	{"(aa)10+20",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}}},
	{"(aa)10*",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}}},
	{"(aa)*10",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}}},
	{"(aa)10*20",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}}},
	{"[ab]",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}}},
	{"[ab]?",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}}},
	{"[ab]+",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}}},
	{"[ab]*",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}}},
	{"[ab]10+",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}}},
	{"[ab]+10",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}}},
	{"[ab]10+20",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}}},
	{"[ab]10*",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}}},
	{"[ab]*10",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}}},
	{"[ab]10*20",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}}},
	{"a-z",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}}},
	{"a-z?",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}}},
	{"a-z+",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}}},
	{"a-z*",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}}},
	{"a-z10+",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}}},
	{"a-z+10",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}}},
	{"a-z10+20",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}}},
	{"a-z10*",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}}},
	{"a-z*10",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}}},
	{"a-z10*20",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}}},
	{".;",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, false}}},
	{".?;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, false}}},
	{".+;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, true}}},
	{".*;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, true}}},
	{".10+;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}}},
	{".+10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 10, false}}},
	{".10+20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}}},
	{".10*;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}}},
	{".*10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 10, false}}},
	{".10*20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}}},
	{"$;",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, false}}},
	{"$?;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, false}}},
	{"$+;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, true}}},
	{"$*;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, true}}},
	{"$10+;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}}},
	{"$+10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 10, false}}},
	{"$10+20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}}},
	{"$10*;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}}},
	{"$*10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 10, false}}},
	{"$10*20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}}},	
	{"a;",         1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a?;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}}},
	{"a+;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}}},
	{"a*;",        1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}}},
	{"a10+;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}}},
	{"a+10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}}},
	{"a10+20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}}},
	{"a10*;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}}},
	{"a*10;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}}},
	{"a10*20;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}}},
	{"(aa);",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}}},
	{"(aa)?;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}}},
	{"(aa)+;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}}},
	{"(aa)*;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}}},
	{"(aa)10+;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}}},
	{"(aa)+10;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}}},
	{"(aa)10+20;", 1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}}},
	{"(aa)10*;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}}},
	{"(aa)*10;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}}},
	{"(aa)10*20;", 1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}}},
	{"[ab];",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}}},
	{"[ab]?;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}}},
	{"[ab]+;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}}},
	{"[ab]*;",     1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}}},
	{"[ab]10+;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}}},
	{"[ab]+10;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}}},
	{"[ab]10+20;", 1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}}},
	{"[ab]10*;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}}},
	{"[ab]*10;",   1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}}},
	{"[ab]10*20;", 1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}}},
	{"a-z;",       1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}}},
	{"a-z?;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}}},
	{"a-z+;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}}},
	{"a-z*;",      1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}}},
	{"a-z10+;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}}},
	{"a-z+10;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}}},
	{"a-z10+20;",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}}},
	{"a-z10*;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}}},
	{"a-z*10;",    1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}}},
	{"a-z10*20;",  1, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}}},
	{".a",         2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".?a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".+a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".*a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".10+a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".+10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".10+20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".10*a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".*10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{".10*20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_ANY, ".", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$a",         2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$?a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$+a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$*a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$10+a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$+10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$10+20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$10*a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$*10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"$10*20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_EOE, "$", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"aa",         2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a?a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a+a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a*a",        2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a10+a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a+10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a10+20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a10*a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a*10a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a10*20a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)?a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)+a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)*a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)10+a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)+10a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)10+20a", 2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)10*a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)*10a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"(aa)10*20a", 2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]?a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]+a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]*a",     2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]10+a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]+10a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]10+20a", 2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]10*a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]*10a",   2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"[ab]10*20a", 2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-za",       2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z?a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z+a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z*a",      2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z10+a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z+10a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z10+20a",  2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z10*a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z*10a",    2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
	{"a-z10*20a",  2, (trsre_Token[]){{TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}, {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}}},
};

#define CASE_COUNT (sizeof(cases) / sizeof(Case))

int main(){
	size_t passed = 0;
	for(size_t casei = 0; casei < CASE_COUNT; casei++){
		const trsre_Token* toks = cases[casei].toks;
		size_t count = cases[casei].count;
		const char* re = cases[casei].re;

		trsre_Token t = trsre_getToken(&re);
		size_t i = 0;
		while(t.m_Value && i < cases[casei].count){
			char* val = malloc(t.m_Length + 1);
			memcpy(val, t.m_Value, t.m_Length);
			val[t.m_Length] = 0;

			trsre_Token tok = toks[i];

			if(
				t.m_Type == tok.m_Type
				&& t.m_Length == tok.m_Length
				&& t.m_Min == tok.m_Min
				&& t.m_Max == tok.m_Max
				&& t.m_NoMax == tok.m_NoMax
				&& strcmp(tok.m_Value, val) == 0
			) count--;
			else fprintf(stderr, "FAILED(case %ld): Tokens dismatch\n  E: {%d, \"%.*s\", %ld, %ld, %ld, %s}\n  G: {%d, \"%.*s\", %ld, %ld, %ld, %s}\n", casei, 
						 tok.m_Type, (int)tok.m_Length, tok.m_Value, tok.m_Length, tok.m_Min, tok.m_Max, (tok.m_NoMax?"true":"false"),
						 t.m_Type, (int)t.m_Length, t.m_Value, t.m_Length, t.m_Min, t.m_Max, (t.m_NoMax?"true":"false"));
			i++;
			t = trsre_getToken(&re);
		}
		
		if(count == 0 && t.m_Value == 0)
			passed++;
		else
			fprintf(stderr, "FAILED(case %ld): Bad token\n  G: {%d, \"%.*s\", %ld, %ld, %ld, %s}\n", casei, 
					t.m_Type, (int)t.m_Length, t.m_Value, t.m_Length, t.m_Min, t.m_Max, (t.m_NoMax?"true":"false"));
	}

	if(passed != CASE_COUNT)
		printf("FAILED\n");
	else
		printf("OK\n");

	return 0;
}
