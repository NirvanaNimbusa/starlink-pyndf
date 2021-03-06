#include "sae_par.h"
#include "dat_par.h"
#include "ndf1.h"
#include "ary.h"
#include "ndf.h"
#include "mers.h"

void ndfSsary_( Ary *ary1, int indf, Ary **ary2, int *status ){
/*
*+
*  Name:
*     ndfSsary

*  Purpose:
*     Create an array section, using an NDF section as a template.

*  Synopsis:
*     void ndfSsary( Ary *ary1, int indf, Ary **ary2, int *status )

*  Description:
*     This function creates a "similar section" from an array (whose ARY_
*     system identifier is supplied) using an existing NDF section as a
*     template.  An identifier for the array section is returned and this
*     may subsequently be manipulated using the ARY_ system functions
*     (SUN/11).  The new array section will bear the same relationship to
*     its base array as the NDF template does to its own base NDF.
*     Allowance is made for any pixel-index shifts which may have been
*     applied, so that the pixel-index system of the new array section
*     matches that of the NDF template.
*
*     This function is intended for use when an array which must match
*     pixel-for-pixel with an NDF is stored in an NDF extension; if an NDF
*     section is obtained, then this function may be used to obtain a
*     pixel-by-pixel matching section from the array.

*  Parameters:
*     ary1
*        The ARY_ system identifier for the array, or array section, from
*        which the new section is to be drawn.
*     indf
*        NDF_ system identifier for the template NDF section (may also be a
*        base NDF).
*     *ary2
*        Returned holding the ARY_ system identifier for the new array
*        section.
*     *status
*        The global status.

*  Notes:
*     -  This function will normally generate an array section. However, if
*     the input array is a base array and the input NDF is a base NDF with
*     the same pixel-index bounds, then there is no need to generate a
*     section in order to access the required part of the array. In this
*     case, a base array identifier will be returned instead.
*     -  It is the caller's responsibility to annul the ARY_ system
*     identifier issued by this function (e.g. by calling "aryAnnul") when
*     it is no longer required. The NDF_ system will not perform this task
*     itself.
*     -  The new array generated by this function will have the same number
*     of dimensions as the array from which it is derived. If the template
*     NDF has fewer dimensions, then the pixel-index bounds of the extra
*     array dimensions are preserved unchanged. If the NDF has more
*     dimensions, then the extra ones are ignored.
*     -  This function takes account of the transfer windows of the array
*     and NDF supplied and will restrict the transfer window of the new
*     array section so as not to grant access to regions of the base array
*     which were not previously accessible through both the input array and
*     the NDF section.
*     -  If this function is called with "status" set, then a value of NULL
*     will be returned for the "ary2" parameter, although no further
*     processing will occur. The same value will also be returned if the
*     function should fail for any reason. The NULL constant is defined in
*     the header file "ary.h".

*  Copyright:
*     Copyright (C) 2018 East Asian Observatory
*     All rights reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or modify
*     it under the terms of the GNU General Public License as published by
*     the Free Software Foundation; either version 2 of the License, or (at
*     your option) any later version.
*
*     This program is distributed in the hope that it will be useful,but
*     WITHOUT ANY WARRANTY; without even the implied warranty of
*     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*     General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  Authors:
*     RFWS: R.F. Warren-Smith (STARLINK)
*     DSB: David S. Berry (EAO)

*  History:
*     3-APR-2019 (DSB):
*        Original version, based on equivalent Fortran function by RFWS.

*-
*/

/* Local Variables: */
   NdfACB *acb;          /* Pointer to the NDF entry in the ACB */

/* Set an initial value for the "ary2" parameter. */
   *ary2 = NULL;

/* Check inherited global status. */
   if( *status != SAI__OK ) return;

/* Ensure the NDF library has been initialised. */
   NDF_INIT( status );

/* Import the NDF identifier. */
   ndf1Impid( indf, &acb, status );

/* Obtain the array section, using the NDF's data array as a template. */
   if( *status == SAI__OK ) arySsect( ary1, acb->did, ary2, status );

/* If an error occurred, then report context information and call the
   error tracing function. */
   if( *status != SAI__OK ) {
      errRep( " ", "ndfSsary: Error creating an array section using an NDF "
              "section as a template.", status );
      ndf1Trace( "ndfSsary", status );
   }

/* Restablish the original AST status pointer */
   NDF_FINAL

}

