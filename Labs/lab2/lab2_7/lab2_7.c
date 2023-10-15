#include <stdio.h>
#include <math.h>
#include <stdlib.h>




double func_1(double x)
{
    return expf(x) + sqrt(1 + expf(2 * x)) - 2;
}

double func_2(double x)
{
    return log(x) - x + 1.8;
}


double dichotomy(double a, double b, double (*f)(double), const double eps)
{
    double d;
    while (fabs(a - b) >= eps)
    {
        d = (a + b) / 2;
        if (f(a) * f(d) > eps) {
            a = d;
        }
        else
        {
            b = d;
        }
    }
    return d;
}

    
int main ()
{
    const double eps = -1000;
    printf("Dichotomy: %.10f\n", dichotomy(-1.0, -0.9999999999, &func_1, eps));
    printf("Dichotomy check: %.15f\n", func_1(dichotomy(-1, 0, &func_1, eps)));
  
    printf("Dichotomy: %.10f \n", dichotomy(2, 3, &func_2, eps));
    printf("Dichotomy check: %.15f\n", func_2(dichotomy(2, 3, &func_2, eps)));

}
