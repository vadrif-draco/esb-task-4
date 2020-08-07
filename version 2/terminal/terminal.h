#ifndef TYPEDEFS
#define TYPEDEFS
#include "..\typedefs.h"
#endif

ST_transaction_t startTransaction();
void decline(uint8_t *reason, ST_transaction_t *transaction);