      SUBROUTINE NUM_GETERR( STATUS )
*+
*  Name:
*     NUM_GETERR

*  Purpose:
*     Obtain the current numeric error status

*  Language:
*     Starlink Fortran

*  Invocation:
*     CALL NUM_GETERR( STATUS )

*  Description:
*     Obtain the current numerical processing error status and
*     place it in the inherited status variable.

*  Arguments:
*     STATUS = INTEGER (Given & Returned)
*        Current error status from NUM subsystem. It is not modified
*        if STATUS is not SAI__OK on entry.

*  Copyright:
*     Copyright (C) 2004 Central Laboratory of the Research Councils.
*     All Rights Reserved.

*  Licence:
*     This program is free software; you can redistribute it and/or
*     modify it under the terms of the GNU General Public License as
*     published by the Free Software Foundation; either version 2 of
*     the License, or (at your option) any later version.
*
*     This program is distributed in the hope that it will be
*     useful,but WITHOUT ANY WARRANTY; without even the implied
*     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
*     PURPOSE. See the GNU General Public License for more details.
*
*     You should have received a copy of the GNU General Public License
*     along with this program; if not, write to the Free Software
*     Foundation, Inc., 51 Franklin Street,Fifth Floor, Boston, MA
*     02110-1301, USA

*  Authors:
*     Tim Jenness (JAC, Hawaii)

*  History:
*     1-OCT-2004 (TIMJ):
*        Original version

*  Notes:
*     If you are only interested in checking whether a numeric operation
*     succeeded, you should use NUM_ISOK().

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! For good status

*  Global Variables:
      INCLUDE 'NUM_CMN'          ! Define the NUM_ERROR flag

*  Inherited Status:
      INTEGER STATUS

*.

*     Do nothing if already bad status
      IF (STATUS .NE. SAI__OK) RETURN

      STATUS = NUM_ERROR

      END
