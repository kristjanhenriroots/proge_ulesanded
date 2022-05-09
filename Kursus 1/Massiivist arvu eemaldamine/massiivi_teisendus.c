#include <stdio.h>

int main(void){
    
    int n, i, a, loop;
    float x;
    
    printf("Massiivi teisendus\n");
    printf("Sisestage massiivi pikkus: \n");
    scanf("%d", &n);
    
    //Kontrollin kas massiivi pikkus on suurem kui 0
    if(n <= 0){
        printf("Pikkus peab olema suurem kui 0\n");
        return 0;
    }
    
    float arvud[n];

    printf("Sisestage massiivi elemendid: \n");
    
    for(int i = 0 ; i < n; scanf("%f", &arvud[i++]));
    
    printf("Sisestage arv mis eemaldada: \n");
    scanf("%f", &x);
    
    //Kontrollin kas kasutaja sisestatud arv asub antud massiivis
    int leitud = 0;
    for(a = 0; a < n; a++){
        if(x == arvud[a]){
            leitud = 1;
        }
    }

    if(leitud == 0){
        printf("Sisestatud arvu ei eksisteeri massiivis!");
        return 0;
    }
    
    //Uue massiivi leidmine
    int new_n = n-1;
    float newarvud[new_n];
    int xstatus = 0;
    
    for(i = 0; i <= n; i++){
        if(arvud[i] < x || arvud[i] > x){
            newarvud[i - xstatus] = arvud[i];
        }
        else{
            xstatus = 1;
        }
    }

    printf("\nUus massiiv on: ");
    for(loop = 0; loop < new_n; loop++){
        printf("%.2f ", newarvud[loop]);
    }
    return 0;
}