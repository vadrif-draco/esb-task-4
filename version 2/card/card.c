#include "..\terminal\terminal.h"
#include "..\server\server.h"
#include "..\card\card.h"

void validateCard(ST_transaction_t *t)
{
    uint16_t transYear = (t->transData.transactionDate[6] - '0') * 1000 +
                         (t->transData.transactionDate[7] - '0') * 100 +
                         (t->transData.transactionDate[8] - '0') * 10 +
                         (t->transData.transactionDate[9] - '0');

    uint16_t expiryYear = 2000 +
                          (t->cardHolderData.cardExpirationDate[3] - '0') * 10 +
                          (t->cardHolderData.cardExpirationDate[4] - '0');

    uint8_t transMonth = (t->transData.transactionDate[3] - '0') * 10 +
                         (t->transData.transactionDate[4] - '0');

    uint8_t expiryMonth = (t->cardHolderData.cardExpirationDate[0] - '0') * 10 +
                          (t->cardHolderData.cardExpirationDate[1] - '0');

    if (transYear > expiryYear || (transYear == expiryYear && transMonth > expiryMonth))
        decline("Card has expired", t);
}