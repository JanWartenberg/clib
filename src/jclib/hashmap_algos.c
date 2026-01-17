#include "hashmap_algos.h"
#include "bstrlib.h"

// settings taken from
// http://www.isthe.com/chongo/tech/comp/fnv/index.html#FNV-param
const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_fnv1a_hash(void *data)
{
    bstring s = (bstring) data;
    uint32_t hash = FNV_OFFSET_BASIS;
    int i = 0;

    for (i = 0; i < blength(s); i++) {
        hash ^= bchare(s, i, 0);
        hash *= FNV_PRIME;
    }

    return hash;
}

const int MOD_ADLER = 65521;

uint32_t Hashmap_adler32_hash(void *data)
{
    bstring s = (bstring) data;
    uint32_t a = 1, b = 0;
    int i = 0;

    for (i = 0; i < blength(s); i++) {
        a = (a + bchare(s, i, 0)) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    return (b << 16) | a;
}

uint32_t Hashmap_djb_hash(void *data)
{
    bstring s = (bstring) data;
    uint32_t hash = 5381;
    int i = 0;

    for (i = 0; i < blength(s); i++) {
        hash = ((hash << 5) + hash) + bchare(s, i, 0);  /* hash * 33 + c */
    }

    return hash;
}

// 4th hash function: intentionally poor distribution (bad but plausible looking)
uint32_t Hashmap_bad4_hash(void *data)
{
    bstring s = (bstring) data;
    // Simple poor hashing: depend almost entirely on the first character
    uint32_t hash = 0;
    if (blength(s) > 0) {
        hash = (uint32_t) bchare(s, 0, 0);
    }
    // Scatter a little, but still biased
    hash *= 2654435761u; // Knuth's multiplicative constant
    if (hash == 0) hash = 1;
    return hash;
}
