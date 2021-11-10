#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const unsigned long int BASE = 1000000000;

typedef struct uint1024_t
{
    unsigned long int digits[35];
    int size;

} uint1024_t;

uint1024_t from_uint(unsigned int x)
{
    uint1024_t number;

    for (int i = 0; i <= 35; i++)
    {
        number.digits[i] = 0;
    }

    if (x / BASE > 0)
    {
        number.digits[34] = x % BASE;
        number.digits[33] = x / BASE;
    }

    else
    {
        number.digits[34] = x;
    }

    return number;
}

void printf_value(uint1024_t x)
{
    uint1024_t number = x;

    for (int i = 0; i <= 34; i++)
    {
        printf("%09d", x.digits[i]);
    }
}

void scanf_value(uint1024_t *x)
{
    uint1024_t value;
    char string[312];
    scanf("%s", string);
    int j = 34;

    for (int i = strlen(string); i > 0; i -= 9)
    {
        string[i] = 0;

        if (i >= 9)
        {
            value.digits[j] = atoi(string + i - 9);
        }

        else
        {
            value.digits[j] = atoi(string);
        }

        j--;
    }
    *x = value;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{

    uint1024_t result;

    for (int i = 34; i >= 0; i--)
    {
        result.digits[i] = x.digits[i] + y.digits[i];
    }
    for (int i = 34; i >= 0; i--)
    {
        if (result.digits[i] >= 1000000000)
        {
            result.digits[i] = result.digits[i] - BASE;
            result.digits[i - 1] += 1;
        }
    }

    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    int count = 0;

    for (int i = 34; i >= 0; i--)
    {
        if (x.digits[i] >= y.digits[i])
        {
            result.digits[i] = x.digits[i] - y.digits[i] + count;
            count = 0;
        }

        else
        {
            result.digits[i] = x.digits[i] + BASE - y.digits[i];
            count--;
        }
    }

    return result;
}


int main()
{
    uint1024_t a;
    uint1024_t b;
    uint1024_t c;

   //ФУНКЦИИ, КОТОРЫЕ ПОТРЕБУЮТСЯ БУДУТ ЗДЕСЬ

    return 0;
}