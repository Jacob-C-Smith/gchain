#pragma once
#include <stdio.h>

#include <core/rsa.h>
#include <core/sha.h>
#include <core/digital_signature.h>

#include <gchain/transaction.h>

struct wallet_s
{
    public_key *p_public_key;
    private_key *p_private_key;
};

typedef struct wallet_s wallet;

int wallet_construct     ( wallet **pp_wallet );
int wallet_from_key_pair ( wallet **pp_wallet, public_key p, private_key q );
int wallet_from_path     ( wallet **pp_wallet, const char *p_path );
int wallet_sign_transaction ( transaction **pp_transaction, wallet *p_wallet, int quantity, public_key *p_payee_public_key );

int wallet_print ( wallet *p_wallet, const char *p_name );
int wallet_pack ( void *p_buffer, wallet *p_wallet );
