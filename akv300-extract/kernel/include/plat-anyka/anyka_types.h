/** @file
 * @brief Define the register operator for system
 *
 * Copyright (C) 2006 Anyka (GuangZhou) Software Technology Co., Ltd.
 * @author 
 * @date 2006-01-16
 * @version 1.0
 */

#ifndef _ANYKA_TYPES_H_
#define _ANYKA_TYPES_H_

/** @defgroup ANYKA_CPU  
 *    @ingroup M3PLATFORM
 */
/*@{*/

/* preliminary type definition for global area */
typedef    unsigned char          T_U8;       /* unsigned 8 bit integer */
typedef    unsigned short         T_U16;      /* unsigned 16 bit integer */
typedef    unsigned long          T_U32;      /* unsigned 32 bit integer */
typedef    signed char            T_S8;       /* signed 8 bit integer */
typedef    signed short           T_S16;      /* signed 16 bit integer */
typedef    signed long            T_S32;      /* signed 32 bit integer */
typedef    void                   T_VOID;     /* void */

#define    T_U8_MAX             ((T_U8)0xff)                 // maximum T_U8 value
#define    T_U16_MAX            ((T_U16)0xffff)              // maximum T_U16 value
#define    T_U32_MAX            ((T_U32)0xffffffff)          // maximum T_U32 value
#define    T_S8_MIN             ((T_S8)(-127-1))             // minimum T_S8 value
#define    T_S8_MAX             ((T_S8)127)                  // maximum T_S8 value
#define    T_S16_MIN            ((T_S16)(-32767L-1L))        // minimum T_S16 value
#define    T_S16_MAX            ((T_S16)(32767L))            // maximum T_S16 value
#define    T_S32_MIN            ((T_S32)(-2147483647L-1L))   // minimum T_S32 value
#define    T_S32_MAX            ((T_S32)(2147483647L))       // maximum T_S32 value

/* basal type definition for global area */
typedef T_S8                    T_CHR;      /* char */
typedef T_U8                    T_BOOL;     /* BOOL type */

typedef T_VOID *                T_pVOID;    /* pointer of void data */
typedef const T_VOID *          T_pCVOID;   /* const pointer of void data */

typedef T_S8 *                  T_pSTR;     /* pointer of string */
typedef const T_S8 *            T_pCSTR;    /* const pointer of string */


typedef T_U16                   T_WCHR;     /**< unicode char */
typedef T_U16 *                 T_pWSTR;    /* pointer of unicode string */
typedef const T_U16 *           T_pCWSTR;   /* const pointer of unicode string */


typedef T_U8 *                  T_pDATA;    /* pointer of data */
typedef const T_U8 *            T_pCDATA;   /* const pointer of data */

typedef T_U32                   T_COLOR;    /* color value */

typedef T_U32                   T_HANDLE;   /* a handle */

#define        AK_FALSE            0
#define        AK_TRUE             1
#undef AK_NULL
#define        AK_NULL             ((T_pVOID)(0))

#define        AK_EMPTY
/*@}*/


#endif    //  _ANYKA_TYPES_H_

