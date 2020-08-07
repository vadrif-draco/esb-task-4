#ifndef TYPEDEFS
#define TYPEDEFS
#include "..\typedefs.h"
#endif

void initializeServer();
void showServerStatus();
int8_t getAccountIndex(uint8_t pan[10]);
void verifyTransaction(ST_transaction_t *transaction);