#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long long int const BASE = 1000000000;

typedef struct uint1024_t
{
    unsigned long int digits[35];

} uint1024_t;

uint1024_t from_uint(unsigned int x)
{
    uint1024_t number;

    for (int i = 0; i < 35; i++)
    {
        number.digits[i] = 0;
    }

    if (x / BASE > 0)
    {
        number.digits[0] = x % BASE;
        number.digits[1] = x / BASE;
    }

    else
    {
        number.digits[0] = x;
    }

    return number;
}

uint1024_t add_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;

    unsigned long long int mod = 0;
    for (int i = 0; i <= 34; i++)
    {
        unsigned long long int sum = x.digits[i] + y.digits[i] + mod;
        result.digits[i] = sum % BASE;
        mod = sum / BASE;
    }

    return result;
}

uint1024_t subtr_op(uint1024_t x, uint1024_t y)
{
    uint1024_t result;
    int carry = 0;
    for (int i = 0; i <= 34; i++)
    {
        if (x.digits[i] >= y.digits[i] + carry)
        {
            result.digits[i] = x.digits[i] - y.digits[i] - carry;
            carry = 0;
        }
        else
        {
            result.digits[i] = x.digits[i] + BASE - y.digits[i] - carry;
            carry = 1;
        }
    }

    return result;
}

uint1024_t mult_op(uint1024_t x, uint1024_t y)
{
    uint1024_t res;
    unsigned long long int carry;

    memset(&res, 0, sizeof(int) * 35);
   

    for (int i = 0; i <= 17; i++)
    {
        for (int j = 0, over = 0; j <= 17 || over != 0; j++)
        {
            if (j <= 17)
            {
                carry = res.digits[i + j] + x.digits[i] * 1ll * y.digits[j] + over;
            }

            else
            {
                carry = res.digits[i + j] + over;
            }

            res.digits[i+j] = (int) (carry % BASE);
            over = (int) (carry / BASE);
        }
    }
    return res;
}


void scanf_value(uint1024_t *x)
{
    uint1024_t value;
    char string[312];
    scanf("%s", string);
    int j = 0;

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

        j++;
    }
    *x = value;
}
void printf_value(uint1024_t x)
{

    for (int i = 34; i >= 0; i--)
    {
        printf("%09d", x.digits[i]);
    }
}

int main()
{
    uint1024_t a;
    uint1024_t b;
    uint1024_t c;
    uint1024_t d;
    a = from_uint(0);
    b = from_uint(1);
    
    c = subtr_op(a, b);
    printf_value(c);
    return 0;
}