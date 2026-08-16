#include <stdio.h>   // Library for input/output
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[50];
    int age;
    char cuit[50];
} Info;

int verify_cuit(char *cuit) {
    int sum = 0;
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
        printf("CUIT OK!\n");
    }
    else {
        printf("CUIT NOK: verificator digit should be: %d\n", calculated_verificator_digit);
    }

    return 1;        // Program ends successfully
}

int main(int argc, char *argv[]) {

    Info *info = NULL;
    int number_of_entries = 0;

    while (1) { // infinite loop wait for user input
        char selected_fn[20];
        scanf("%s", selected_fn);

        if (strcmp(selected_fn, "agregarInformacionUsuario") == 0) {
            char name[20];
            printf("Input name:");
            scanf("%s", name);

            int age;
            printf("Input age:");
            scanf("%d", &age);

            char cuit[20];
            printf("Input CUIT:");
            scanf("%s", cuit);

            int valid_age = age > 17 && age < 100;
            int valid_cuit = verify_cuit(cuit);

            if (!valid_age || !valid_cuit) {
                printf("Please validate the information submitted and try again!\n");
            }
            else {
                if (info == NULL) {
                    info = malloc(sizeof(Info));
                }
                else {
                    info = realloc(info, (number_of_entries + 1) * sizeof(Info));
                }

                info[number_of_entries].age = age;
                strcpy(info[number_of_entries].name, name);
                strcpy(info[number_of_entries].cuit, cuit);

                number_of_entries++;
            }
        }

        if (strcmp(selected_fn, "buscarInfomacionUsuario") == 0) { 
            int found = 0;
            char cuit[20];
            printf("Input CUIT:");
            scanf("%s", cuit);

            if (info == NULL) {
                printf("No information loaded yet!\n");
            }
            else {
                for (int j = 0; j < number_of_entries; j++) {
                    if (strcmp(cuit, info[j].cuit) == 0) {
                        printf("Name: %s, age: %d, CUIT: %s\n", info[j].name, info[j].age, info[j].cuit);
                        found = 1;
                    }
                }
                if (!found) {
                    printf("User not found!\n");
                }
            }

        }

        if (strcmp(selected_fn, "verInformacionUsuario") == 0) {
            if (info == NULL) {
                printf("No information loaded yet!\n");
            }
            else {
                for (int j = 0; j < number_of_entries; j++) {
                    printf("Name: %s, age: %d, CUIT: %s\n", info[j].name, info[j].age, info[j].cuit);
                }
            }
        }

        if (strcmp(selected_fn, "exit") == 0) {
            break;
        }
    }

    if (info != NULL) {
        free(info);
    }

    return 0;        // Program ends successfully
}