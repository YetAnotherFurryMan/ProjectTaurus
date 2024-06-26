#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <trsre/trsre.h>

typedef struct{
	const char* re;
	const trsre_Token tok;
} Case;

const Case cases[120] = {
	{"a",         {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}},
	{"a?",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}},
	{"a+",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}},
	{"a*",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}},
	{"a10+",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a+10",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}},
	{"a10+20",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"a10*",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a*10",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}},
	{"a10*20",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"(aa)",      {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}},
	{"(aa)?",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}},
	{"(aa)+",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}},
	{"(aa)*",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}},
	{"(aa)10+",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)+10",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}},
	{"(aa)10+20", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"(aa)10*",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)*10",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}},
	{"(aa)10*20", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"[ab]",      {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}},
	{"[ab]?",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}},
	{"[ab]+",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}},
	{"[ab]*",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}},
	{"[ab]10+",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]+10",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}},
	{"[ab]10+20", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"[ab]10*",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]*10",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}},
	{"[ab]10*20", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"a-z",       {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}},
	{"a-z?",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}},
	{"a-z+",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}},
	{"a-z*",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}},
	{"a-z10+",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z+10",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}},
	{"a-z10+20",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
	{"a-z10*",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z*10",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}},
	{"a-z10*20",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
	{"a;",         {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}},
	{"a?;",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}},
	{"a+;",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}},
	{"a*;",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}},
	{"a10+;",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a+10;",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}},
	{"a10+20;",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"a10*;",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a*10;",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}},
	{"a10*20;",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"(aa);",      {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}},
	{"(aa)?;",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}},
	{"(aa)+;",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}},
	{"(aa)*;",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}},
	{"(aa)10+;",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)+10;",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}},
	{"(aa)10+20;", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"(aa)10*;",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)*10;",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}},
	{"(aa)10*20;", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"[ab];",      {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}},
	{"[ab]?;",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}},
	{"[ab]+;",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}},
	{"[ab]*;",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}},
	{"[ab]10+;",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]+10;",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}},
	{"[ab]10+20;", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"[ab]10*;",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]*10;",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}},
	{"[ab]10*20;", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"a-z;",       {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}},
	{"a-z?;",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}},
	{"a-z+;",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}},
	{"a-z*;",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}},
	{"a-z10+;",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z+10;",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}},
	{"a-z10+20;",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
	{"a-z10*;",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z*10;",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}},
	{"a-z10*20;",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
	{"aa",         {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, false}},
	{"a?a",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, false}},
	{"a+a",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 1, true}},
	{"a*a",        {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 1, true}},
	{"a10+a",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a+10a",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 1, 10, false}},
	{"a10+20a",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"a10*a",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 1, true}},
	{"a*10a",      {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 0, 10, false}},
	{"a10*20a",    {TRSRE_TOKEN_TYPE_LITERAL, "a", 1, 10, 20, false}},
	{"(aa)a",      {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, false}},
	{"(aa)?a",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, false}},
	{"(aa)+a",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 1, true}},
	{"(aa)*a",     {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 1, true}},
	{"(aa)10+a",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)+10a",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 1, 10, false}},
	{"(aa)10+20a", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"(aa)10*a",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 1, true}},
	{"(aa)*10a",   {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 0, 10, false}},
	{"(aa)10*20a", {TRSRE_TOKEN_TYPE_GROUP, "aa", 2, 10, 20, false}},
	{"[ab]a",      {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, false}},
	{"[ab]?a",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, false}},
	{"[ab]+a",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 1, true}},
	{"[ab]*a",     {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 1, true}},
	{"[ab]10+a",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]+10a",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 1, 10, false}},
	{"[ab]10+20a", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"[ab]10*a",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 1, true}},
	{"[ab]*10a",   {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 0, 10, false}},
	{"[ab]10*20a", {TRSRE_TOKEN_TYPE_CHOOSE, "ab", 2, 10, 20, false}},
	{"a-za",       {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, false}},
	{"a-z?a",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, false}},
	{"a-z+a",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 1, true}},
	{"a-z*a",      {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 1, true}},
	{"a-z10+a",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z+10a",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 1, 10, false}},
	{"a-z10+20a",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
	{"a-z10*a",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 1, true}},
	{"a-z*10a",    {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 0, 10, false}},
	{"a-z10*20a",  {TRSRE_TOKEN_TYPE_RANGE, "a-z", 3, 10, 20, false}},
};

#define CASE_COUNT (sizeof(cases) / sizeof(Case))

int main(){
	size_t passed = 0;
	for(size_t casei = 0; casei < CASE_COUNT; casei++){
		const trsre_Token tok = cases[casei].tok;
		const char* re = cases[casei].re;

		trsre_Token t = trsre_getToken(&re);
		char* val = malloc(t.m_Length + 1);
		memcpy(val, t.m_Value, t.m_Length);
		val[t.m_Length] = 0;

		if(
			t.m_Type == tok.m_Type
			&& t.m_Length == tok.m_Length
			&& t.m_Min == tok.m_Min
			&& t.m_Max == tok.m_Max
			&& t.m_NoMax == tok.m_NoMax
			&& strcmp(tok.m_Value, val) == 0
		) passed++;
		else fprintf(stderr, "FAILED(case %ld): Tokens dismatch\n  E: {%d, \"%.*s\", %ld, %ld, %ld, %s}\n  G: {%d, \"%.*s\", %ld, %ld, %ld, %s}\n", casei, 
				tok.m_Type, (int)tok.m_Length, tok.m_Value, tok.m_Length, tok.m_Min, tok.m_Max, (tok.m_NoMax?"true":"false"),
				t.m_Type, (int)t.m_Length, t.m_Value, t.m_Length, t.m_Min, t.m_Max, (t.m_NoMax?"true":"false"));
	}

	if(passed != CASE_COUNT)
		printf("FAILED\n");
	else
		printf("OK\n");

	return 0;
}
