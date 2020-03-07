#if !defined( HDS1_TYPES_INCLUDED ) /* hds1_types.h already included? */
#define HDS1_TYPES_INCLUDED 1
/*
*+
*  Name:
*     hds1_types.h

*  Type of Module:
*     C include file.

*  Purpose:
*     Define private but derived HDS-specific data types.

*  Description:
*     This file defines the private types that are used in the HDS
*     build but have no reason to be public.

*  Authors:
*     TIMJ: Tim Jenness (JAC, Hawaii)
*     ./make-hds-types program

*  History:
*     21-Oct-2005 (TIMJ):
*        Original version of C program (via auto-generation).
*     TODAY (./make-hds-types):
*        Generated
*     No further changes -- do not edit this file

*-
*/

#include <stddef.h>

#include <inttypes.h>

/* Standard HDS integer type. Only used internally */
typedef int hdsi32_t;

/* Standard HDS 64 bit integer (internal) */
typedef int64_t hdsi64_t;
typedef uint64_t hdsu64_t;
#define HDS_INT_BIG_S "ld"
#define HDS_INT_BIG_U "lu"

/* Private types and sizes relating to dimensions */
typedef int FORTRAN_INDEX_TYPE;
#define SIZEOF_HDSDIM 8
#define HDSDIM_IS_UNSIGNED 0

/* Decide whether Fortran dims should be copied element by element */
#define HDS_COPY_FORTRAN_DIMS 1

#endif /* _INCLUDED */

