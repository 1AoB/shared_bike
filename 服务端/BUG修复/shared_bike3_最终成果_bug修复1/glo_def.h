#ifndef _GLO_DEF_H
#define _GLO_DEF_H

#include"Logger.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define	MAX_MESSAGE_LEN 367280

//#define LOG_DEBUG  printf
//#define LOG_WARN   printf
//#define LOG_ERROR  printf

	/* Global defition */
#define TRUE    1
#define FALSE   0

#define INVALID_U32  0xFFFF

/* Global type defintions */
	typedef unsigned char  u8;
	typedef unsigned short u16;
	typedef unsigned int   u32;     /* int == long */
	typedef signed char    i8;
	typedef signed short   i16;
	typedef signed int     i32;     /* int == long */
	typedef float          r32;
	typedef double         r64;
	typedef long double    r128;

	typedef unsigned char  BOOL;

	typedef u32            TBoolean;
	typedef i32            TDevid;

	//#ifdef __DCC__ /* For diab compiler environment */

	typedef unsigned long long u64;
	typedef signed long long i64;

#ifdef __cplusplus
}
#endif

#endif
