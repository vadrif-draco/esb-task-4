#include "typedefs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>

/* --------------------------------------------------------------- DISCLAIMER ---------------------------------------------------------------
  
   Some deviations from the task requirements:
  
   1. I believe that storing transaction logs alongside each account will be more efficient than saving and sorting them according to PAN.
   2. I implemented the checks based on the order of input, which is not the same as the order provided in the given flowchart for the task.
    - That is *not* due to my inability of implementing it as required, it just made more sense for me to do it this way.
  
   Bonus features implemented:
  
   1. Account balances are updated after transaction is approved
   2. Approved transactions details are *dynamically* stored per account (thus eliminating the need of sorting transactions)
  
------------------------------------------------------------------------------------------------------------------------------------------ */

// Required global variables 
ST_accountBalance_t dbAccounts[10]; // Used for emulating a database server - data hardcoded in main
uint8_t decision = 'y';             // Used for determining whether to continue looping or exit application

// Account validator and index getter 
int8_t getAccountIndex(uint8_t pan[10])
{
    for (int i = 0; i < 10; i++)
    {
        if (!strcmp(pan, dbAccounts[i].primaryAccountNumber))
            return i; // account found, return its index
    }
    return -1; // account not found
}

// Functionality of declining transactions 
void decline(uint8_t *reason, ST_transaction_t transaction)
{
    printf("\nThe transaction is DECLINED! \nReason: %s.\n", reason);
    printf("\nDo you want to continue (y/n)?: ");
    scanf(" %c", &decision);
    transaction.transStat = DECLINED;
}

int main()
{
    // Database initialization
    dbAccounts[0] = (ST_accountBalance_t){"123456789", 100.00, 0};
    dbAccounts[1] = (ST_accountBalance_t){"234567891", 6000.00, 0};
    dbAccounts[2] = (ST_accountBalance_t){"567891234", 3250.25, 0};
    dbAccounts[3] = (ST_accountBalance_t){"456789123", 1500.12, 0};
    dbAccounts[4] = (ST_accountBalance_t){"258649173", 500.00, 0};
    dbAccounts[5] = (ST_accountBalance_t){"654823719", 2100.00, 0};
    dbAccounts[6] = (ST_accountBalance_t){"971362485", 0.00, 0};
    dbAccounts[7] = (ST_accountBalance_t){"793148625", 1.00, 0};
    dbAccounts[8] = (ST_accountBalance_t){"123123456", 10.12, 0};
    dbAccounts[9] = (ST_accountBalance_t){"456789321", 0.55, 0};

    while (decision == 'y')
    {
        ST_transaction_t newTransaction;
        newTransaction.transData.maxTransAmount = 5000.0;

        printf("\nPlease enter card data:\n");

        printf("    Please enter the card holder name: ");
        getchar(); // in case a newline is left behind
        gets(newTransaction.cardHolderData.cardHolderName);

        printf("    Please enter the primary account number: ");
        scanf(" %s", &newTransaction.cardHolderData.primaryAccountNumber);

        // check #1: search in database for account associated with provided PAN ------------------------------------------------------------------------------ 1
        int8_t index = getAccountIndex(newTransaction.cardHolderData.primaryAccountNumber);
        if (index == -1)
        {
            decline("Provided PAN is invalid", newTransaction);
            continue;
        }

        printf("    Please enter card expiry date (mm/yy): ");
        scanf(" %s", &newTransaction.cardHolderData.cardExpirationDate);

        printf("\nPlease enter terminal data:\n");
        printf("    Please enter the transaction amount: ");
        scanf(" %f", &newTransaction.transData.transAmount);

        // check #2: compare request with maximum transaction amount ------------------------------------------------------------------------------------------ 2
        if (newTransaction.transData.transAmount > newTransaction.transData.maxTransAmount)
        {
            decline("Requested transaction amount is greater than maximum allowed", newTransaction);
            continue;
        }

        // check #3: compare request with account balance ----------------------------------------------------------------------------------------------------- 3
        if (newTransaction.transData.transAmount > dbAccounts[index].balance)
        {
            decline("Insufficient balance", newTransaction);
            continue;
        }

        printf("    Please enter transaction date (dd/mm/yyyy): ");
        scanf(" %s", &newTransaction.transData.transactionDate);

        // check #4: expiry ----------------------------------------------------------------------------------------------------------------------------------- 4

        /* first check years, if fine, check months provided that the years are equal.

        for number conversion debugging purposes

        printf("%d %d %d %d",
                atoi(newTransaction.transData.transactionDate + 6),
                2000 + atoi(newTransaction.cardHolderData.cardExpirationDate + 3),
                atoi(newTransaction.transData.transactionDate + 3),
                atoi(newTransaction.cardHolderData.cardExpirationDate));

        */

        if (atoi(newTransaction.transData.transactionDate + 6) > 2000 + atoi(newTransaction.cardHolderData.cardExpirationDate + 3))
        {
            decline("Card has expired", newTransaction);
            continue;
        }
        else if (atoi(newTransaction.transData.transactionDate + 6) == 2000 + atoi(newTransaction.cardHolderData.cardExpirationDate + 3))
            if (atoi(newTransaction.transData.transactionDate + 3) > atoi(newTransaction.cardHolderData.cardExpirationDate))
            {
                decline("Card has expired", newTransaction);
                continue;
            }

        // if nothing goes wrong, then transaction is approved 
        newTransaction.transStat = APPROVED;

        // storing transaction log with associated account 
        dbAccounts[index].transactionLogs = realloc(dbAccounts[index].transactionLogs, ++dbAccounts[index].numOfTransactions * sizeof(ST_transaction_t));
        dbAccounts[index].transactionLogs[dbAccounts[index].numOfTransactions - 1] = newTransaction;

        // updating account balance after transaction 
        dbAccounts[index].balance -= newTransaction.transData.transAmount;

        printf("\nThe transaction is APPROVED!\nDo you want to continue (y/n)?: ");
        scanf(" %c", &decision);
    }

    // Displaying the final status of the database before terminating 
    printf("\n\n------------------ Current data in database ------------------");
    for (int i = 0; i < 10; i++)
    {
        printf("\nAccount PAN: %s", dbAccounts[i].primaryAccountNumber);
        printf("\nAccount Balance: %f", dbAccounts[i].balance);
        printf("\nTransaction Logs: ");

        if (dbAccounts[i].numOfTransactions > 0)
        {
            printf("\n");
            for (int j = 0; j < dbAccounts[i].numOfTransactions; j++)
            {
                printf(" %d. On %s, an amount of $", (j + 1), dbAccounts[i].transactionLogs[j].transData.transactionDate);
                printf("%f was withdrawn from this account.\n", dbAccounts[i].transactionLogs[j].transData.transAmount);
            }
        }
        else
            printf("There are no transactions associated with this account.");

        printf("\n");
    }
    getch();
}
