#include <stdio.h>
#include <stdlib.h>
#include <math.h>


long double bpow(long double base, long long degree)
{
    long double result = 1.0L;
    while (degree > 0) {
        if(degree & 1)
        {
            result = (result * base);
        }
        base = (base * base);
        degree = degree >> 1;
    }
    return result;   
}

long double Euler_number_by_limit (long double epsilon)
{
    long double e = 1.0L, division = 1.0L;
    long long n = 1;
    while (division > epsilon)
    {
        n++;
        division = 1.0L / n;
    }
    printf("%lld\n", n);
    e = bpow(e + division, n);
    return e;
    
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("\nInvalid input\n");
        printf("Use: %s <epsilon>\n\n", argv[0]);
        return 1;
    }


    long double epsilon = strtold(argv[1], NULL);
    printf("%Lf\n\n", epsilon);

    printf("The Euler's number:\n\n");
    printf("By limit: %Lf\n", Euler_number_by_limit(epsilon));


    return 0;
}