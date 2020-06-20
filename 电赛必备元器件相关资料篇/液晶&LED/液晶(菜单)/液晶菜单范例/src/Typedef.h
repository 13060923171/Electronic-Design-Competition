#ifndef __TYPEDEF_H
#define __TYPEDEF_H

//#include <t89c51rd2.h>
#include <absacc.h>
#include <intrins.h>

#define	MAX(a,b)	(a>b?a:b)
#define MIN(a,b)	(a<b?a:b)

typedef unsigned char	uchar;
typedef unsigned int	uint;
typedef unsigned int	uint16;
typedef int		int16;
typedef char		int8;
typedef unsigned char	uint8;
typedef unsigned char	bool;
typedef unsigned long	uint32;
typedef	long		int32;
typedef	char		BOOL;

#define	false		0
#define	true		1

#ifndef NULL
#define	NULL		0
#endif


#endif
