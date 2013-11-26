#ifndef __BASIC_STRUCT_H__
#define __BASIC_STRUCT_H__

#ifndef uint32_t

#ifdef WIN32
#include <BaseTsd.h>
typedef UINT32 uint32_t;
#else
#include <bits/types.h>
typedef __uint32_t uint32_t;
#endif

#endif // uint32_t

typedef struct tag_Header 
{
	uint32_t cmd;
	uint32_t length;
	uint32_t error;
	uint32_t para1;
	uint32_t para2;
} Header;


#endif
