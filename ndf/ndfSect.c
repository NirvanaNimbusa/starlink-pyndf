#include "sae_par.h"
#include "dat_par.h"
#include "ndf1.h"
#include "ndf.h"
#include "mers.h"

void ndfSect_( int indf1, int ndim, const hdsdim lbnd[],
              const hdsdim ubnd[], int *indf2, int *status ){
/*
*+
*  Name:
*     ndfSect

*  Purpose:
*     Create an NDF section.

*  Synopsis:
*     void ndfSect( int indf1, int ndim, const hdsdim lbnd[],
*                   const hdsdim ubnd[], int *indf2, int *status )

*  Description:
*     This function creates a new NDF section which refers to a selected
*     region of an existing NDF (or NDF section). The region may be larger
*     or smaller in extent than the initial NDF.

*  Parameters:
*     indf1
*        Identifier for the initial NDF.
*     ndim
*        Number of dimensions for the new section.
*     lbnd
*        Lower pixel-index bounds of the section.
*     ubnd
*        Upper pixel-index bounds of the section.
*     *indf2
*        Returned holding the identifier for the new section.
*     *status
*        The global status.

*  Notes:
*     -  The number of section dimensions need not match the number of
*     dimensions in the initial NDF. Pixel-index bounds will be padded with
*     1's as necessary to identify the pixels to which the new section
*     should refer.
*     -  The array components of sections which extend beyond the pixel-
*     index bounds of the initial NDF will be padded with bad pixels.
*     -  If the section bounds extend beyond the bounds of the associated
*     base NDF and any of the NDF's axis arrays have defined values, then
*     these values will be extrapolated as necessary.
*     -  If this function is called with "status" set, then a value of
*     NDF__NOID will be returned for the "indf2" parameter, although no
*     further processing will occur. The same value will also be returned
*     if the function should fail for any reason. The NDF__NOID constant is
*     defined in the header file "ndf.h".

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
   NdfACB *acb1;         /* Pointer to initial NDF in the ACB */
   NdfACB *acb2;         /* Pointer to new NDF in the ACB */

/* Set an initial value for the "indf2" parameter. */
   *indf2 = NDF__NOID;

/* Check inherited global status. */
   if( *status != SAI__OK ) return;

/* Ensure the NDF library has been initialised. */
   NDF_INIT( status );

/* Import the initial NDF identifier. */
   ndf1Impid( indf1, &acb1, status );

/* Check the bounds of the region to select for validity. */
   ndf1Vbnd( ndim, lbnd, ubnd, status );

/* Create an ACB entry for the new NDF. */
   if( *status == SAI__OK ) ndf1Cut( acb1, ndim, lbnd, ubnd, &acb2, status );

/* Export an identifier for the new NDF. */
   *indf2 = ndf1Expid( ( NdfObject * ) acb2, status );

/* If an error occurred, then set a value of NDF__NOID for the "indf2"
   parameter, report context information and call the error tracing
   function. */
   if( *status != SAI__OK ) {
      *indf2 = NDF__NOID;
      errRep( " ", "ndfSect: Error creating an NDF section.", status );
      ndf1Trace( "ndfSect", status );
   }

/* Restablish the original AST status pointer */
   NDF_FINAL

}

