#include <gchain/wallet.h>

int wallet_construct ( wallet **pp_wallet )
{
    
    // argument check
    if ( NULL == pp_wallet ) goto no_wallet;

    // initialized data
    wallet *p_wallet = malloc(sizeof(wallet));

    // error check
    if ( NULL == p_wallet ) goto no_mem;

    // construct a public private keypair
    if ( 0 == key_pair_construct(&p_wallet->p_public_key, &p_wallet->p_private_key) ) goto failed_to_construct_key_pair;

    // return a pointer to the caller
    *pp_wallet = p_wallet;

    // success
    return 1;

failed_to_construct_key_pair:
    free(p_wallet);
    return 0;   

no_mem:
    free(p_wallet);
    return 0;

no_wallet:
    return 0;
}
int wallet_from_key_pair ( wallet **pp_wallet, public_key p, private_key q );
int wallet_from_path     ( wallet **pp_wallet, const char *p_path );

int wallet_print ( wallet *p_wallet, const char *p_name )
{

    // argument check
    if ( NULL == p_wallet ) goto no_wallet;

    printf("Wallet: %s", p_name ? p_name : "???");
    print_public_key(p_wallet->p_public_key);

    // success
    return 1;

no_wallet:
    return 0;
}

int wallet_sign_transaction ( transaction **pp_transaction, wallet *p_wallet, int quantity, public_key *p_payee_public_key )
{

    // argument check
    //

    // initialized data
    transaction *p_transaction = NULL;
    char _buffer[1024] = { 0 };
    size_t len = 0; 

    transaction_construct(&p_transaction, quantity, p_wallet->p_public_key, p_payee_public_key);

    len = transaction_pack(_buffer, p_transaction);
    digital_signature_sign(p_wallet->p_private_key, p_wallet->p_public_key, _buffer, len, &p_transaction->p_signature);

    *pp_transaction = p_transaction;

    // success
    return 1;
    
failed_to_sign_transaction:
    return 0;

no_wallet:
    return 0;
}

