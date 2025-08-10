#include <gchain/transaction.h>

int transaction_construct (transaction **pp_transaction, int qty, public_key *p_payer, public_key *p_payee)
{

    // argument check
    if ( NULL == pp_transaction ) goto no_transaction;

    // initialized data
    transaction *p_transaction = malloc(sizeof(transaction));

    // error check
    if ( NULL == p_transaction ) goto no_mem;

    // populate the transaction struct
    *p_transaction = (transaction)
    {
        .qty = qty,
        .p_payer = p_payer,
        .p_payee = p_payee
    };

    // return a pointer to the caller
    *pp_transaction = p_transaction;

    // success
    return 1;

    no_transaction:
        return 0;
    no_mem:
        return 0;
}

int transaction_pack ( void *p_buffer, transaction *p_transaction )
{

    // error check
    if ( NULL == p_transaction ) goto no_transaction;

    // initialized data
    void *p_base = p_buffer;

    // pack the transaction struct
    p_buffer += pack_pack(p_buffer, "%u32", p_transaction->qty);
    
    // pack the public keys
    p_buffer += public_key_pack(p_buffer, p_transaction->p_payer);
    p_buffer += public_key_pack(p_buffer, p_transaction->p_payee);

    // success
    return p_buffer - p_base;

    no_transaction:
        return 0;
    no_pack:
        return 0;
}

int transaction_print ( transaction *p_transaction )
{

    // argument check
    if ( NULL == p_transaction ) goto no_transaction;

    char _buf[1024] = { 0 };
    size_t len = transaction_pack(_buf, p_transaction);

    printf("Transaction: %d\n", p_transaction->qty);
    print_public_key(p_transaction->p_payer);
    print_public_key(p_transaction->p_payee);

    if ( digital_signature_verify(
        p_transaction->p_payer, 
        _buf, 
        len, 
        p_transaction->p_signature
    ) ) 
        log_info("Valid block\n");
    else
        log_error("Invalid block\n");
        
    // success
    return 1;

    no_transaction:
        return 0;
}
