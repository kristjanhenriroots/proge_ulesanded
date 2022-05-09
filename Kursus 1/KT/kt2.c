#include <stdio.h>

float keskmine(int n, int *M, int x){
    int i, j = 0;
    int summa = 0;
    for(i = 0; i < n; i++){
        if(M[i] > x){
            summa = summa + M[i];
            j++;
        }
    }

    if(j == 0){
        return 0;
    }
    
    float keskmine = summa / j;


    
    return keskmine;
}

int main(void){
    int n, x;
    
    printf("Sisestage massiivi pikkus: ");
    scanf("%d", &n);
    
    printf("Sisestage massiiv: ");
    int M[n];
    int i;
    for(i = 0; i < n; i++){
        scanf("%d", &M[i]);
    }
    printf("Sisestage votmevaartus: ");
    scanf("%d", &x);
    float vastus = keskmine(n, M, x);
    printf("%f", vastus);
    return 0;
}

