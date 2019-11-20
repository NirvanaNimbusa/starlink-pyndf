#include <stdlib.h>
#include "sae_par.h"
#include "dat_par.h"
#include "ndf1.h"
#include "ndf_err.h"
#include "ary.h"
#include <string.h>
#include "mers.h"

void ndf1Awsbn( hdsdim lbndw, hdsdim ubndw, int iax, NdfACB *acb, int *status ){
/*
*+
*  Name:
*     ndf1Awsbn

*  Purpose:
*     Set new bounds for an axis width array.

*  Synopsis:
*     void ndf1Awsbn( hdsdim lbndw, hdsdim ubndw, int iax, NdfACB *acb,
*                     int *status )

*  Description:
*     This function sets new pixel-index bounds for an NDF's axis width
*     array, retaining the existing array values (if they exist). The
*     existing values will be extrapolated if the new bounds extend beyond
*     the old ones.

*  Parameters:
*     lbndw
*        New lower pixel-index bound for the axis.
*     ubndw
*        New upper pixel-index bound for the axis.
*     iax
*        Zero-based index of the NDF axis whose width array bounds are to
*        be changed.
*     acb
*        Pointer to the NDF entry in the ACB.
*     *status
*        The global status.

*  Notes:
*     -  This function may only be used to change the bounds of an axis
*     width array for a base NDF. It will return without action if the NDF
*     supplied is a section.
*     -  When changing the pixel-index bounds of an NDF, this function
*     should be called prior to making any changes to the NDF's main data
*     array, as it depends on this array's bounds matching the initial axis
*     width array bounds.

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
   Ary *tiary;           /* Temporary ARY_ system identifier */
   AryPlace *place;      /* ARY_ system placeholder */
   HDSLoc *tloc = NULL;  /* Locator to temporary component */
   NdfACB *acbs;         /* ACB for temporary NDF section */
   NdfDCB *dcb;          /* Pointer to data object entry in the DCB */
   char form[ NDF__SZFRM + 1 ];    /* Array storage form */
   char tname[ DAT__SZNAM + 1 ];   /* Temporary component name */
   char type[ NDF__SZTYP + 1 ];    /* Array numeric type */
   hdsdim lbnd[ NDF__MXDIM ];      /* NDF lower bounds */
   hdsdim lbnds[ NDF__MXDIM ];     /* Lower bounds of temporary section */
   hdsdim ubnd[ NDF__MXDIM ];      /* NDF upper bounds */
   hdsdim ubnds[ NDF__MXDIM ];     /* Upper bounds of temporary section */
   int i;                /* Loop counter for dimensions */
   int inside;           /* New bounds inside old ones? */
   int ndim;             /* Number of NDF dimensions */
   size_t el;            /* Number of mapped elements */
   void *pntr;           /* Pointer to mapped values */
   void *tpntr;          /* Pointer to mapped temporary array */

/* Check inherited global status. */
   if( *status != SAI__OK ) return;

/* Check that the NDF is not a section. There is nothing to do if it
   is. */
   if( !acb->cut ) {

/* Obtain an index to the data object entry in the DCB. */
      dcb = acb->dcb;

/* Ensure that information about the axis width array is available in
   the DCB. */
      ndf1Daw( iax, dcb, status );
      if( *status == SAI__OK ) {

/* Check whether the axis width array exists. If not, then no
   modifications will be needed to the data object, but the array"s
   default storage form in the DCB must be updated to take account of
   the new bounds, if necessary. */
         if( !dcb->awid[ iax ] ) {
            ndf1Cbfrm( 1, &lbndw, &ubndw, dcb->awfrm[ iax ],
                       sizeof( dcb->awfrm[ iax ] ), status );

/* If the array does exist, then obtain the bounds and number of
   dimensions of the NDF data object from the ARY_ system identifier
   for its main data array, held in the DCB. */
         } else {
            aryBound( dcb->did, NDF__MXDIM, lbnd, ubnd, &ndim, status );
            if( *status == SAI__OK ) {

/* Determine whether the new axis bounds lie inside the current ones. */
               inside = ( ( lbndw >= lbnd[ iax ] ) && ( ubndw <= ubnd[ iax ] ) );

/* If so, then simply set appropriate new bounds for the axis width
   array. */
               if( inside ) {
                  arySbnd( 1, &lbndw, &ubndw, dcb->awid[ iax ], status );

/* If the new bounds lie outside the existing bounds, then the new axis
   width array will have to hold values which are an extrapolation of
   the current one. */
               } else {

/* Obtain the width array's numeric type and storage form and convert
   the storage form to take account of the new array bounds if
   necessary. */
                  aryType( dcb->awid[ iax ], type, status );
                  aryForm( dcb->awid[ iax ], form, status );
                  ndf1Cbfrm( 1, &lbndw, &ubndw, form, sizeof( form ), status );

/* Generate the pixel index bounds of an NDF section which has the
   required bounds along the current axis, leaving other axis bounds
   unchanged. */
                  if( *status == SAI__OK ) {
                     for( i = 0; i < ndim; i++ ){
                        lbnds[ i ] = lbnd[ i ];
                        ubnds[ i ] = ubnd[ i ];
                     }
                     lbnds[ iax ] = lbndw;
                     ubnds[ iax ] = ubndw;

/* Create a new ACB entry describing the section. */
                     ndf1Cut( acb, ndim, lbnds, ubnds, &acbs, status );

/* Generate a temporary component name for use in the parent axis
   structure and obtain an ARY_ system placeholder for a new array with
   this name. */
                     ndf1Tcnam( dcb->aloc[ iax ], tname,
                                sizeof( tname ), status );
                     aryPlace( dcb->aloc[ iax ], tname, &place, status );

/* Test the array storage form against each valid value in turn and
   take the appropriate action. */

/* PRIMITIVE:
   =========
   Create a primitive array. */
                     if( !strcmp( form, "PRIMITIVE" ) ) {
                        aryNewp( type, 1, &ubndw, &place, &tiary, status );

/* Map the axis array of the NDF section for reading (this causes any
   necessary extrapolation of values to take place), and the new array
   for writing, and copy the values across. */
                        ndf1Awmap( iax, acbs, type, "READ", &pntr, &el, status );
                        aryMap( tiary, type, "WRITE", &tpntr, &el, status );
                        ndf1Move( type, el, pntr, tpntr, status );

/* Unmap the arrays when done. */
                        ndf1Awump( iax, acbs, status );
                        aryUnmap( tiary, status );

/* SIMPLE:
   ======
   Create a simple array. */
                     } else if( !strcmp( form, "SIMPLE" ) ) {
                        aryNew( type, 1, &lbndw, &ubndw, &place, &tiary,
                                status );

/* Map the axis array of the NDF section for reading (this causes any
   necessary extrapolation of values to take place), and the new array
   for writing, and copy the values across. */
                        ndf1Awmap( iax, acbs, type, "READ", &pntr, &el, status );
                        aryMap( tiary, type, "WRITE", &tpntr, &el, status );
                        ndf1Move( type, el, pntr, tpntr, status );

/* Unmap the arrays when done. */
                        ndf1Awump( iax, acbs, status );
                        aryUnmap( tiary, status );

/* If an unsupported array storage form was encountered, then report an
   error. */
                     } else {
                        *status = NDF__FATIN;
                        msgSetc( "BADFORM", form );
                        errRep( " ", "Invalid axis array storage form "
                                "'BADFORM' encountered in the NDF_ system "
                                "Data Control Block (internal programming "
                                "error).", status );
                     }

/* Annul the ACB entry for the temporary NDF section and the identifier
   for the temporary array. */
                     ndf1Anl( &acbs, status );
                     aryAnnul( &tiary, status );
                  }

/* Delete the original axis width array. */
                  aryDelet( dcb->awid + iax, status );

/* Obtain a locator to the temporary array and rename it to become the
   new axis width array. */
                  datFind( dcb->aloc[ iax ], tname, &tloc, status );
                  datRenam( tloc, "WIDTH", status );

/* Import the array into the ARY_ system and store the identifier in
   the DCB. */
                  aryImprt( tloc, dcb->awid + iax, status );

/* Annul the locator afterwards. */
                  datAnnul( &tloc, status );
               }

/* Note if the DCB axis width array information is now correct. */
               dcb->kaw[ iax ] = ( *status == SAI__OK );
            }
         }
      }
   }

/* Call error tracing function and exit. */
   if( *status != SAI__OK ) ndf1Trace( "ndf1Awsbn", status );

}

