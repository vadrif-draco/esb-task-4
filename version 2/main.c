#include "terminal\terminal.h"
#include "server\server.h"
#include "card\card.h"

#ifndef TYPEDEFS
#include "typedefs.h"
#endif

#include <stdio.h>
#include <conio.h>

/* --------------------------------------------------------------- DISCLAIMER ---------------------------------------------------------------
  
   *** Version 1 comments [DEPRECATED]:
   
   1. I believe that storing transaction logs alongside each account will be more efficient than saving and sorting them according to PAN.
   2. I implemented the checks based on the order of input, which is not the same as the order provided in the given flowchart for the task.
     
   Bonus features implemented:
  
   1. Account balances are updated after transaction is approved
   2. Approved transactions details are *dynamically* stored per account (thus eliminating the need of sorting transactions)



   *** Version 2 comments:

   The following updates have been implemented to version 1:
   1. Code has been distributed over eight files as opposed to the two of version 1. 
   2. Deviation #2 has been removed, application logic now operates based on the order provided in the given flowchart for the task.
   3. All usages of the function "atoi()" have been replaced with manually written string-into-integer conversion methods.
  
------------------------------------------------------------------------------------------------------------------------------------------ */

uint8_t decision = 'y'; // Used for determining whether to continue looping or exit application

int main()
{
    // Database initialization
    initializeServer();

    while (decision == 'y' || decision == 'Y')
    {
        // The following function queries the terminal to start the transaction process and take input from user.
        ST_transaction_t newTransaction = startTransaction();

        // The following function queries the server to verify the transaction (checks PAN validity and checks balance)
        // There is a possibility that the transaction has already been declined due to going over the terminal's limit...
        // ...hence a check is required before the verification process is triggered
        if (newTransaction.transStat != DECLINED)
            verifyTransaction(&newTransaction);

        printf("\nDo you want to continue (y/n)?: ");
        scanf(" %c", &decision);
    }

    // Displaying the final status of the database before terminating
    showServerStatus();
    getch();
}
