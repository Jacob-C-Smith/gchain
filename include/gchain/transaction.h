#pragma once
#include <stdio.h>

#include <core/rsa.h>
#include <core/sha.h>
#include <core/digital_signature.h>

struct transaction_s
{
    int qty;
    public_key *p_payer;
    public_key *p_payee;
    i2048 *p_signature;
};

typedef struct transaction_s transaction;

int transaction_construct (transaction **pp_transaction, int qty, public_key *p_payer, public_key *p_payee);
int transaction_pack ( void *p_buffer, transaction *p_transaction );
int transaction_print ( transaction *p_transaction );
