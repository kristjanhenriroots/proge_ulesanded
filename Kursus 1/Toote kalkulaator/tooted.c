#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAX 120

int main(void){

    
    char string[MAX];

    printf("Toote kalkulaator\n");
    printf("Sisestage tooted\n");
    printf("Kasutage formaati TOODE tühik HIND. Nt: leib 3 sai 4.5\n");
    printf("NB! Toodete kogust ei ole vaja sisestada\n");
    printf(">: ");
    fgets(string, MAX, stdin);
    
   //Teen massiivi korrektsete indeksitega, peale igat tühikut uus element kuni end of line ehk enter, millal token = NULL
    char *token = strtok(string, " ");
    char *correct[MAX];
    int i = 0, len = 0;
    
    while (token != NULL){
        correct[i++] = token;
        token = strtok(NULL, " ");
        len++; //Saan elementide arvu
    }
    

    //Kontrollin kas pikkus on rohkem kui 1 toode
    if(len < 4){
        printf("Massiivis peab olema vähemalt 2 elementi!\n");
        return 0;
    }
    
    //Teen char array float omaks
    int half = len / 2;
    float hinnad[half];
    int a, b = 1;
    for(a = 0; a < half; a++){
        hinnad[a] = atof(correct[b]);
        b = b + 2;
    }
    
    //Vaatan kas hinnad on positiivsed
    int p;
    for(p = 0; p < half; p++){
        if(hinnad[p] < 0){
            printf("Hind ei saa olla negatiivne!\n");
            return 0;
        }
    }

    //Leian kõige kallima toote
    int c, d, index1, index2, index3;
    float exp = 0, cheap = 9999999999999;
    for(c = 0; c < half; c++){
        if(hinnad[c] > exp){
            exp = hinnad[c];
            index1 = c;
        }
    }
    printf("\nKalleim toode on: %s", correct[index1 * 2]);
    
    //Leian kõige odavama
    for(d = 0; d < half; d++){
        if(hinnad[d] < cheap){
            cheap = hinnad[d];
            index2 = d;
        }
    }
    printf("\nOdavaim toode on: %s\n", correct[index2 * 2]);

    //Keskmise hinna arvutamine
    float average, sum = 0;
    for(index3 = 0; index3 < half; index3++){
        sum = sum + hinnad[index3];
    }
    
    average = sum / half;
    printf("Keskmine hind on: %.2f\n", average);
    
    //Otsin lähima toote keskmisele
    float dis = 99999999999;
    int e, f;
    
    for(e = 0; e < half; e++){
        if(fabs(hinnad[e] - average) < dis){
            dis = fabs(hinnad[e] - average);
        }
    }

    printf("Keskmisele lähim toode on: ");
    for(f = 0; f < half; f++){
        if(fabs(hinnad[f] - average) == dis){
            printf("%s ", correct[f * 2]);
        }
    }
    
    return 0;
}