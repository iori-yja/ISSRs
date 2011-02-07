/*****************************************************************************
 *   type.h:  Type definition Header file for NXP LPC230x Family 
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *   2009.05.09  ver 2.00	 Nemui changed 
 *   2009.06.06  ver 3.00	 Nemui changed   LPC-USB Setting
 *   2009.10.24  ver 4.00	 Nemui changed   FatFs Setting
******************************************************************************/
#ifndef __TYPE_H
#define __TYPE_H

#include "integer.h"

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef EOF
#define EOF -1
#endif

/* LPC-USB Setting */
typedef unsigned char		U8;		/**< unsigned 8-bit */
typedef unsigned short int	U16;	/**< unsigned 16-bit */
typedef unsigned int		U32;	/**< unsigned 32-bit */

/* some other useful macros */
#define MIN(x,y)	((x)<(y)?(x):(y))	/**< MIN */
#define MAX(x,y)	((x)>(y)?(x):(y))	/**< MAX */

#endif  /* __TYPE_H */
