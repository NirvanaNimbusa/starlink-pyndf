      REAL FUNCTION VAL_ACOSR( BAD, ARG, STATUS )
*+
*  Name:
*     VAL_ACOSR

*  Purpose:
*     Evaluate the REAL arc-cosine function.

*  Language:
*     Starlink Fortran

*  Invocation:
*     RESULT = VAL_ACOSR( BAD, ARG, STATUS )

*  Description:
*     The routine evaluates the arc-cosine function for a single
*     argument of type REAL.  If a numerical error occurs, the value
*     VAL__BADR is returned and a STATUS value is set.

*  Arguments:
*     BAD = LOGICAL (Given)
*        Whether the argument value (ARG) may be "bad".
*     ARG = REAL (Given)
*        The REAL argument of the arc-cosine function.
*     STATUS = INTEGER (Given & Returned)
*        This should be set to SAI__OK on entry, otherwise the routine
*        returns immediately with the result VAL__BADR.  A STATUS
*        value will be set by this routine if a numerical error occurs.

*  Returned Value:
*     VAL_ACOSR = REAL
*        Returns the evaluated arc-cosine function result as a REAL
*        value.  The value VAL__BADR will be returned under error
*        conditions.

*  Copyright:
*     Copyright (C) 1987, 1991 Science & Engineering Research Council.
*     Copyright (C) 1995 Central Laboratory of the Research Councils.
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
*     R.F. Warren-Smith (STARLINK)
*     {enter_new_authors_here}

*  History:
*     15-AUG-1987 (RFWS):
*        Original version.
*     28-OCT-1991 (RFWS):
*        Added LIB$REVERT call.
*     7-NOV-1991 (RFWS):
*        Changed to use NUM_TRAP.
*     27-SEP-1995 (BKM):
*        Changed LIB$ESTABLISH and LIB$REVERT calls to NUM_HANDL and NUM_REVRT
*     27-SEP-1995 (BKM):
*        Changed LIB$ESTABLISH and LIB$REVERT calls to NUM_HANDL and NUM_REVRT
*     {enter_further_changes_here}

*  Bugs:
*     {note_any_bugs_here}

*-

*  Type Definitions:
      IMPLICIT NONE              ! No implicit typing

*  Global Constants:
      INCLUDE 'SAE_PAR'          ! Standard SAE constants

      INCLUDE 'PRM_PAR'          ! PRM_ public constants

      INCLUDE 'PRM_CONST'        ! PRM_ private constants

      INCLUDE 'PRM_ERR'          ! PRM_ error codes


*  Arguments Given:
      LOGICAL BAD                ! Bad data flag
      REAL ARG                 ! Function argument

*  Status:
      INTEGER STATUS             ! Error status

*  External References:
      EXTERNAL NUM_TRAP          ! Error handling routine

*  Global Variables:
      INCLUDE 'NUM_CMN'          ! Define NUM_ERROR flag


*  Internal References:
      INCLUDE 'NUM_DEC_CVT'      ! Declare NUM_ conversion functions

      INCLUDE 'NUM_DEC_R'      ! Declare NUM_ arithmetic functions

      INCLUDE 'NUM_DEF_CVT'      ! Define NUM_ conversion functions

      INCLUDE 'NUM_DEF_R'      ! Define NUM_ arithmetic functions


*.

*  Check status.  Return the function result VAL__BADR if not OK.
      IF( STATUS .NE. SAI__OK ) THEN
         VAL_ACOSR = VAL__BADR

*  If the bad data flag is set, check if the argument given is bad.
*  Return VAL__BADR if it is.
      ELSE IF( BAD .AND. ( ARG .EQ. VAL__BADR ) ) THEN
         VAL_ACOSR = VAL__BADR

*  Check if the argument value is acceptable.  If not, return the
*  result VAL__BADR and set a STATUS value.
      ELSE IF( .NOT. ( NUM_GER( ARG, VAL__M1R ) .AND. NUM_LER( ARG, VAL_
     :_P1R ) ) ) THEN
         VAL_ACOSR = VAL__BADR
         STATUS = PRM__ARGIN

*  If the argument value is acceptable then, if required, establish the
*  numerical error handler and initialise the common block error
*  status.
      ELSE
         IF( .FALSE. ) THEN
            CALL NUM_HANDL( NUM_TRAP )
            NUM_ERROR = SAI__OK
         ENDIF

*  Evaluate the function.
         VAL_ACOSR = NUM_ACOSR( ARG )

*  If an error handler is established, check if the numerical error
*  flag is set.  If so, return the result VAL__BADR and set STATUS to
*  NUM_ERROR.
         IF( .FALSE. ) THEN
            IF( NUM_ERROR .NE. SAI__OK ) THEN
               VAL_ACOSR = VAL__BADR
               STATUS = NUM_ERROR
            ENDIF

*  Remove the error handler.
            CALL NUM_REVRT
         ENDIF
      ENDIF

*  Exit routine.
      END
