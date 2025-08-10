/** !
 * GSDK demo program
 * 
 * @file main.c
 * 
 * @author [whoami]
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gsdk
#include <gsdk.h>
#include <core/log.h>

// gchain
#include <gchain/transaction.h>
// #include <gchain/block.h>
// #include <gchain/chain.h>
#include <gchain/wallet.h>

// data
char *_p_names[128] = { "Alice", "Bob", "Charlie", "Dave", "Eve", "Frank" };
wallet *_p_wallets[128] = { NULL };

// Entry point
int main ( int argc, const char *argv[] )
{

    // initialized data
    transaction *p_transaction = NULL;

    // construct wallets
    for (size_t i = 0; i < 2; i++)
        wallet_construct(&_p_wallets[i]),
        wallet_print(_p_wallets[i], _p_names[i]);

    // sign a transaction
    wallet_sign_transaction(&p_transaction, _p_wallets[0], 100, _p_wallets[1]->p_public_key);

    // print a transaction
    transaction_print(p_transaction);

    // Success
    return EXIT_SUCCESS;
}
