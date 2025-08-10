#include <stdio.h>

#include <core/rsa.h>
#include <core/sha.h>

struct block_s
{
    sha256_hash _prev;
    transaction *p_transaction; 
    timestamp _timestamp;
};

typedef struct block_s block;

int construct_block(block **pp_block, sha256_hash prev, )
{

    // argument check
    if ( NULL == pp_block ) goto no_block;

    // initialized data
    block *p_block = malloc(sizeof(block));

    // error check
    if ( NULL == p_block ) goto no_mem;

    // populate the block struct
    *p_block = (block)
    {
        ._prev = prev
    };

    // return a pointer to the caller
    *pp_block = p_block;

    // success
    return 1;

    no_block:
        return 0;
    no_mem:
        return 0;
}

int pack_block ( void *p_buffer, block *p_block )
{

    // error check
    if ( NULL == p_block ) goto no_block;

    // initialized data
    void *p_base = p_buffer;

    // pack the block struct
    p_buffer += pack_pack(p_buffer, "%u32", p_block->_prev);


    // success
    return p_buffer - p_base;

    no_block:
        return 0;
    no_pack:
        return 0;
}