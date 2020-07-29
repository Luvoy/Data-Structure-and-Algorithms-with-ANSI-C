/* 测试各种哈希函数*/
#include <stdio.h>
typedef unsigned int hash_type;
#define HASH_MAX_POSITIVE_TAG 0x7FFFFFFF
hash_type SDBMHash(char *str)
{
    hash_type hash = 0;

    while (*str)
    {
        // equivalent to: hash = 65599*hash + (*str++);
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// RS Hash Function
hash_type RSHash(char *str)
{
    hash_type b = 378551;
    hash_type a = 63689;
    hash_type hash = 0;

    while (*str)
    {
        hash = hash * a + (*str++);
        a *= b;
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// JS Hash Function
hash_type JSHash(char *str)
{
    hash_type hash = 1315423911;

    while (*str)
    {
        hash ^= ((hash << 5) + (*str++) + (hash >> 2));
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// P. J. Weinberger Hash Function
hash_type PJWHash(char *str)
{
    hash_type BitsInUnignedInt = (hash_type)(sizeof(hash_type) * 8);
    hash_type ThreeQuarters = (hash_type)((BitsInUnignedInt * 3) / 4);
    hash_type OneEighth = (hash_type)(BitsInUnignedInt / 8);
    hash_type HighBits = (hash_type)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
    hash_type hash = 0;
    hash_type test = 0;

    while (*str)
    {
        hash = (hash << OneEighth) + (*str++);
        if ((test = hash & HighBits) != 0)
        {
            hash = ((hash ^ (test >> ThreeQuarters)) & (~HighBits));
        }
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// ELF Hash Function
hash_type ELFHash(char *str)
{
    hash_type hash = 0;
    hash_type x = 0;

    while (*str)
    {
        hash = (hash << 4) + (*str++);
        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
            hash &= ~x;
        }
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// BKDR Hash Function
hash_type BKDRHash(char *str)
{
    hash_type seed = 131; // 31 131 1313 13131 131313 etc..
    hash_type hash = 0;

    while (*str)
    {
        hash = hash * seed + (*str++);
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// DJB Hash Function
hash_type DJBHash(char *str)
{
    hash_type hash = 5381;

    while (*str)
    {
        hash += (hash << 5) + (*str++);
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}

// AP Hash Function
hash_type APHash(char *str)
{
    hash_type hash = 0;
    int i;

    for (i = 0; *str; i++)
    {
        if ((i & 1) == 0)
        {
            hash ^= ((hash << 7) ^ (*str++) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ (*str++) ^ (hash >> 5)));
        }
    }

    return (hash & HASH_MAX_POSITIVE_TAG);
}
hash_type JAVAHash(char *str)
{
    hash_type len = 0;
    char *temp = str;
    while (*temp)
    {
        len++;
        temp++;
    }
    hash_type seed = 31;
    hash_type ret = 0;
    hash_type pow;
    int i, j;
    for (i = 0; i < len; ++i)
    {
        pow = 1;
        for (j = len - 1 - i; j--;)
        {
            pow *= seed;
        }
        ret += *(str + i) * pow;
    }
    return ret & HASH_MAX_POSITIVE_TAG;
}
int main(int argc, char const *argv[])
{
    char *s = "abc";
    printf("%d\n", sizeof(unsigned long long int));
    printf("%lld\n", HASH_MAX_POSITIVE_TAG);
    printf("SDBMHash: %d\n", SDBMHash(s));
    printf("RSHash: %d\n", RSHash(s));
    printf("JSHash: %d\n", JSHash(s));
    printf("PJWHash: %d\n", PJWHash(s));
    printf("ELFHash: %d\n", ELFHash(s));
    printf("BKDRHash: %d\n", BKDRHash(s));
    printf("DJBHash: %d\n", DJBHash(s));
    printf("APHash: %d\n", APHash(s));
    printf("JAVAHash: %d\n", JAVAHash(s));
    return 0;
}
