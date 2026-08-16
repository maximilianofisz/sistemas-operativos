#include <unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) { 

    // Soy Abraham
    printf("Soy Abraham\n");
    pid_t pid0 = fork();

    if (pid0 == 0) {
        // Soy Homero
        printf("Soy Homero\n");
        pid_t pid1 = fork();
        
        if (pid1 == 0) {
            //Soy Bart
            printf("Soy Bart\n");
            exit(0);
        }
        else {
            // Sigo siendo Homero
            pid_t pid2 = fork();
        
            if (pid2 == 0) {
                //Soy Lisa
                printf("Soy Lisa\n");
                exit(0);
            }
            else {
                // Sigo siendo Homero
                pid_t pid3 = fork();
            
                if (pid3 == 0) {
                    //Soy Maggie
                    printf("Soy Maggie\n");
                    exit(0);
                }
                else {
                    wait(NULL);
                }
                wait(NULL);
            }
            wait(NULL);
        }
    }
    else {
        // Sigo siendo abraham
        wait(NULL);
    }
    exit(0);
}