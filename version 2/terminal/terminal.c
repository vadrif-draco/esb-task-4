#include "..\server\server.h"
#include "..\card\card.h"
#include "terminal.h"
#include <stdio.h>

ST_transaction_t startTransaction()
{
    ST_transaction_t transaction;
    transaction.transData.maxTransAmount = 5000.0;
    transaction.transStat = -1; // Not declined and not approved

    printf("\nPlease enter card data:\n");

    printf("    Please enter the card holder name: ");
    getchar(); // in case a newline is left behind
    gets(transaction.cardHolderData.cardHolderName);

    printf("    Please enter the primary account number: ");
    scanf(" %s", &transaction.cardHolderData.primaryAccountNumber);

    printf("    Please enter card expiry date (mm/yy): ");
    scanf(" %s", &transaction.cardHolderData.cardExpirationDate);

    printf("\nPlease enter terminal data:\n");

    printf("    Please enter the transaction amount: ");
    scanf(" %f", &transaction.transData.transAmount);

    // compare request with maximum transaction amount
    if (transaction.transData.transAmount > transaction.transData.maxTransAmount)
    {
        decline("Requested transaction amount is greater than maximum allowed", &transaction);
        return transaction;
    }

    printf("    Please enter transaction date (dd/mm/yyyy): ");
    scanf(" %s", &transaction.transData.transactionDate);

    printf("\nVerifying data from the server, please wait...\n");

    return transaction;
}

// Functionality of declining transactions
void decline(uint8_t *reason, ST_transaction_t *transaction)
{
    printf("\nThe transaction is DECLINED! \nReason: %s.\n", reason);
    transaction->transStat = DECLINED;
}