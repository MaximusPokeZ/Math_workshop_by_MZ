#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printHelp() {
    printf("Choose an action:\n");
    printf("-h: Print natural numbers within 100 that are multiples of the specified number.\n");
    printf("-p: Check if a number is prime or composite.\n");
    printf("-s: Divide the number into individual digits and print them.\n");
    printf("-a: Calculate the sum of natural numbers from 1 to the specified number.\n");
    printf("-f: Calculate the factorial of the specified number.\n");
    printf("-help: Display this help message.\n\n");
}

void printInfo() {
    printf("\nCreated by Maxim Zaslavtsev\n");
    printf("Laboratory work #1_1\n\n");
}

int main(int argc, char* argv[])
{

    if (argc != 3)
    {
        if (argc == 2 && !(strcmp(argv[1], "-help"))) {
            printHelp();
            return 0;
        }
        else if (argc == 2 && !(strcmp(argv[1], "-info"))) {
            printInfo();
            return 0;
        }
        printf("\nInvalid input\n");
        printf("Use: %s <number> <flag> (use %s -help to list all functions)\n\n", argv[0], argv[0]);
        return 1;
    }

    int number = atoi(argv[1]);
    char flag = argv[2][1];

    printf("%c\n", flag);
    printf("%d\n", number);





    return 0;


}