#include <stdio.h>

void piramide(int n, int altura){
    for(int i = 0; i < n; i++){

        for(int j = 0; j < altura; j++){

            for(int k = 0; k < (altura)*n - i*(altura) - j; k++){
                printf(" ");
            }
            for(int k = 0; k <= j*2; k++){
                printf("*");
            }

            for(int x = 0; x < i; x++){

                for(int k = 0; k < ((altura)*2)-1-(2*j); k++){
                    printf(" ");
                }
                for(int k = 0; k <= j*2; k++){
                    printf("*");
                }
            }
            printf("\n");
        }
     }
}

int main(void){
    
    int n = 5;
    int altura = 8; // 1, 3, 5, 7, 9, 11, 13, 15 (8*2-1)

    piramide(n, altura);


    return 0;
}