#include "functions.c"


int main() 
{
    double a = 7.0;
    double x = 2.0;

    double *m1 = (double *)malloc(sizeof(double) * 3);
    if (m1 == NULL) return 1;
    m1[2] = 1;
    m1[1] = 2;
    m1[0] = 3;
    //f(x) = f_0 + f_1x + f_2xˆ2 => 1 + 2x + 3x^2 = 17 = 86 + 32 (2 - 5) + 3 (2 - 5) ^ 2
    double result1 = gorner_calculate(x, 3, m1);
    printf("%lf \n", result1);


    printf("\n\n");

    double *m2 = compute_coefficients(0.00001, a, 1, 3.0, 2.0, 1.0);
    for (int i = 2; i >= 0; i--)
    {
       printf("%lf ", m2[i]);
    }
    
    double result2 = gorner_calculate(x - a, 1, m2);
    printf("\n%lf \n", result2);

    free(m1);
    free(m2);
    
    return 0;
}