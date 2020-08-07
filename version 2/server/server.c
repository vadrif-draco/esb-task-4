#include "..\terminal\terminal.h"
#include "..\server\server.h"
#include "..\card\card.h"

#include <stdlib.h>
#include <stdio.h>

ST_accountBalance_t dbAccounts[10]; // Used for emulating a database server - data hardcoded in main

// Dedicated equality checker for this application, compares two strings of exactly 10 characters length
uint8_t stringsEqual(uint8_t s1[10], uint8_t s2[10])
{
    for (int i = 0; i < 10; i++)
    {
        if (s1[i] != s2[i])
            return 0; // if any characters turn out to be different, return false (0)
    }
    return 1; // if loop is done, then all characters were equal, so return true (1)
}

// Account validator and index getter
int8_t getAccountIndex(uint8_t pan[10])
{
    for (int i = 0; i < 10; i++)
    {
        if (stringsEqual(pan, dbAccounts[i].primaryAccountNumber))
            return i; // account found, return its index
    }
    return -1; // account not found
}

// Setting the initial states of the accounts in the database server
void initializeServer()
{
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
}

void verifyTransaction(ST_transaction_t *transaction)
{
    // check #1: expiry - first check years, if fine, check months provided that the years are equal.
    validateCard(transaction);
    if (transaction->transStat == DECLINED)
        return;

    // check #3: search in database for account associated with provided PAN
    int8_t index = getAccountIndex(transaction->cardHolderData.primaryAccountNumber);
    if (index == -1)
    {
        decline("Provided PAN is invalid", transaction);
        return;
    }

    // check #4: if account exists, check if balance is sufficient for transaction
    else if (transaction->transData.transAmount > dbAccounts[index].balance)
    {
        decline("Insufficient balance", transaction);
        return;
    }

    // if all checks pass, transaction is approved and stored, and balance is updated

    // approving...
    printf("\nThe transaction is APPROVED!\n");
    transaction->transStat = APPROVED;

    // storing transaction log with associated account...
    dbAccounts[index].transactionLogs =
        realloc(dbAccounts[index].transactionLogs, ++dbAccounts[index].numOfTransactions * sizeof(ST_transaction_t));

    dbAccounts[index].transactionLogs[dbAccounts[index].numOfTransactions - 1] = *transaction;

    // updating account balance after transaction...
    dbAccounts[index].balance -= transaction->transData.transAmount;
}

// Extra functionality for showing the data stored in the server
void showServerStatus()
{
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
            printf("There are no transactions associated with this account.\n");
    }
}