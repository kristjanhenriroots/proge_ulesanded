#include <stdio.h>

int main(void){
    float x, d = 1, sum = 0;
    printf("Sisestage x: ");
    scanf("%f", &x);
    int i = 0;

    while(i <= 20){
        if(i % 2 == 0){
            sum = sum + d;
        }
       
        i = i + 1;
        d = d * x / i;
    }
    printf("%f", sum);
    return 0;
}