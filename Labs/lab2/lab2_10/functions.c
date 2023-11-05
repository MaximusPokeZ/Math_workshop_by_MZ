#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


double gorner_calculate(int x, int N, double *mas)
{
    double res = 0;
    for (int i = 0; i < N; i++)
    {
        res = res * x + mas[i];
    }
    return res;
}


void derivative_of_polynomial(int *N, double **m)
{
    for (int i = 0; i < *N - 1; i++)
    {
        (*m)[i] = (*m)[i] * ((*N) - (i + 1)); //берем производную и умножаем каждый коэф
    }
    (*m)[(*N) - 1] = 0; 
    *N = (*N) -1;
    
}
double* compute_coefficients(double epsilon, double a, int count_coef, ...) 
{
    double* result_coefficients = (double*) malloc((count_coef) * sizeof(double));
    if (result_coefficients == NULL) { return NULL; }
    double* coefs = (double*) malloc((count_coef) * sizeof(double));
    if (coefs == NULL) 
    { 
        free(result_coefficients); 
        return NULL; 
    }

    va_list args;
    va_start(args, count_coef);
    for (int i = 0; i < count_coef; i++) 
    {
        coefs[i] = va_arg(args, double);
    }
    va_end(args);

    int N = count_coef;
    double fact_n = 1;
    for (int i = 0; i < count_coef && N > 0; i++) 
    {
        double f_n_a = gorner_calculate(a, N, coefs);
        derivative_of_polynomial(&N, &coefs);
        result_coefficients[N] = f_n_a / fact_n;
        fact_n = fact_n * (i + 1);
    }
    free(coefs);
    return result_coefficients;
}