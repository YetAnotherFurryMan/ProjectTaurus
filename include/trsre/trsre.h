#ifndef _TRSRE_H_
#define _TRSRE_H_

#include <stddef.h>

typedef enum{
	TRSRE_TOKEN_TYPE_LITERAL     = 0x00, // 0b0000
    TRSRE_TOKEN_TYPE_RANGE       = 0x02, // 0b0010
    TRSRE_TOKEN_TYPE_GROUP       = 0x04, // 0b0100
    TRSRE_TOKEN_TYPE_CHOOSE      = 0x06, // 0b0110
    TRSRE_TOKEN_TYPE_ANY         = 0x08, // 0b1000
    TRSRE_TOKEN_TYPE_NOT_LITERAL = 0x01, // 0b0001
    TRSRE_TOKEN_TYPE_NOT_RANGE   = 0x03, // 0b0011
    TRSRE_TOKEN_TYPE_NOT_GROUP   = 0x05, // 0b0101
    TRSRE_TOKEN_TYPE_NOT_CHOOSE  = 0x07, // 0b0111
    TRSRE_TOKEN_TYPE_EOE         = 0x09, // 0b1001
} trsre_TokenType;

typedef struct trsre_Token trsre_Token;

struct trsre_Token{
	trsre_TokenType m_Type;
	const char* m_Value;
	size_t m_Length;
	size_t m_Min;
	size_t m_Max;
};

trsre_Token trsre_getToken(const char* str);

#endif //_TRSRE_H_
