

#ifndef _TYPEDEFS_H
#define _TYPEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <windows.h>
#include <windef.h>
	
//
// types
//
#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif

#ifndef dword
typedef unsigned long dword;
#endif


// boolean
#if ! (defined(_WINDEF_H) || defined(_WINDEF_))
typedef enum _BOOL {
	FALSE = 0,
	TRUE = 1
} BOOL;
#endif

// null
#ifndef NULL
#define NULL ((void *) 0)
#endif



#ifdef __cplusplus
}
#endif 




#endif

