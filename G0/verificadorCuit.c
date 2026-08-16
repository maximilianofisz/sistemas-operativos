#include <stdio.h>   // Library for input/output
#include <string.h>


int main(int argc, char *argv[]) {
    int sum = 0;
    char *cuit = argv[1];
    int verification_vector[] = {5,4,3,2,7,6,5,4,3,2};
    int verificator_digit = cuit[strlen(cuit) -1] - '0';
    for (int i = 0; i <= 9; i++) {
        int current_digit = cuit[i] - '0'; //parse to int?
        int x = current_digit * verification_vector[i];
        sum += x;
    }
    int remainder = sum % 11;
    int calculated_verificator_digit = 11 - remainder;

    if (verificator_digit == calculated_verificator_digit) {
        printf("OK!\n");
    }
    else {
        printf("NOK: verificator digit should be: %d\n", calculated_verificator_digit);
    }

    return 0;        // Program ends successfully
}