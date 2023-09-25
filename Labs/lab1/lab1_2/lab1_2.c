


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
    printf("By limit: %.10Lf\n", Euler_number_by_limit(epsilon));



    return 0;
}