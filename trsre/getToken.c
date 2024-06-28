#include <trsre/trsre.h>

#include <stdbool.h>
#include <string.h>

static bool isdec(char c){
	return c >= '0' && c <= '9';
}

static bool ishex(char c){
	return isdec(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

trsre_Token trsre_getToken(const char** str){
	trsre_Token tok = {0};
	
	if(!str || !**str)
		return tok;

	bool neg = false;
	if(**str == '^'){
		neg = true;
		(*str)++;
	}
	
	tok.m_Value = *str;
	tok.m_Min = tok.m_Max = 1;

	switch(**str){
		case '\\': 
		{
			(*str)++;
			if(!**str){
				tok.m_Type = TRSRE_TOKEN_TYPE_EOE;
				tok.m_Length = 1;
			} else if(ishex(**str)){
				(*str)++;
				tok.m_Length = 2;
				if(ishex(**str)){
					(*str)++;
					tok.m_Length++;
				}
			} else{
				(*str)++;
				tok.m_Length = 2;
			}
		} break;
		case '.':
		{
			(*str)++;
			tok.m_Type = TRSRE_TOKEN_TYPE_ANY;
			tok.m_Length = 1;
		} break;
		case '$':
		{
			(*str)++;
			tok.m_Type = TRSRE_TOKEN_TYPE_EOE;
			tok.m_Length = 1;
		} break;
		case '(': 
		{
			tok.m_Type = TRSRE_TOKEN_TYPE_GROUP;
			size_t c = 0;
			do{
				if(**str == '\\'){
					(*str)++;
					tok.m_Length++;
				} else if(**str == '('){
					c++;
				} else if(**str == ')'){
					c--;
				}

				(*str)++;
				tok.m_Length++;
			} while(**str && c);

			// Consume last )
			if(!c)
				tok.m_Length--;

			// Delete (
			tok.m_Value++;
			tok.m_Length--;
		} break;
		case '[': 
		{
			tok.m_Type = TRSRE_TOKEN_TYPE_CHOOSE;
			size_t c = 0;
			do {
				if(**str == '\\'){
					(*str)++;
					tok.m_Length++;
				} else if(**str == '['){
					c++;
				} else if(**str == ']'){
					c--;
				}

				(*str)++;
				tok.m_Length++;
			} while(**str && c);

			// Consume last ]
			if(!c)
				tok.m_Length--;

			// Delete [
			tok.m_Value++;
			tok.m_Length--;
		} break;
		default: 
		{
			(*str)++;
			tok.m_Length = 1;
		} break;
	}

	if(**str == '-' && tok.m_Type == TRSRE_TOKEN_TYPE_LITERAL){
		tok.m_Type = TRSRE_TOKEN_TYPE_RANGE;
		(*str)++;
		if(**str == '\\'){
			(*str)++;
			if(!**str){
				tok.m_Length += 2;
			} else if(ishex(**str)){
				(*str)++;
				tok.m_Length += 3;
				if(ishex(**str)){
					(*str)++;
					tok.m_Length++;
				}
			} else{
				(*str)++;
				tok.m_Length += 3;
			}
		} else if(**str){
			(*str)++;
			tok.m_Length += 2;
		} else{
			tok.m_Type = TRSRE_TOKEN_TYPE_LITERAL;
			(*str)--;
			//return tok;
		}
	}

	if(neg){
		// Negate only negation bit.
		// tok.m_Type = tok.m_Type | (((tok.m_Type & 1) + 1) & 1);
		// Yeach, kinda complicated. But xor works exacly the same way, mei dei boni!
		tok.m_Type = tok.m_Type ^ 1;
	}

	if(!**str)
		return tok;

	if(**str == ';'){
		(*str)++;
		return tok;
	}

	// parse +*? and ranges

	if(**str == '?'){
		(*str)++;
		tok.m_Min = 0;

		if(**str == ';')
			(*str)++;

		return tok;
	}

	bool min_set = isdec(**str);
	if(min_set){
		size_t min = 0;
		do{
			min *= 10;
			min += (**str) - '0';
			(*str)++;
		} while(**str && isdec(**str));
		tok.m_Min = min;
	}

	if(**str != '*' && **str != '+'){
		if(**str == ';')
			(*str)++;
		tok.m_Max = tok.m_Min;
		return tok;
	}

	if(**str == '*' && !min_set)
		tok.m_Min = 0;

	(*str)++;

	if(isdec(**str)){
		size_t max = 0;
		do{
			max *= 10;
			max += (**str) - '0';
			(*str)++;
		} while(**str && isdec(**str));
		tok.m_Max = max;
	} else{
		tok.m_NoMax = true;
	}

	if(**str == ';')
		(*str)++;

	return tok;
}
