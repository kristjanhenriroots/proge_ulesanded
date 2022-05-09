#include <stdio.h>
#include <string.h>
#define MAX 120
#define SML 60

int main(void){

    int size = 5, check = 0;
    
    printf("Toote kalkulaator\n");
    

    
    printf("Sisestage tooted\n");
    printf("Kasutage formaati TOODE tühik HIND. Nt: leib 3 sai 4.5\n");
    printf(">: ");

    
    char string[MAX];
    
    fgets(string, MAX, stdin);
    
    printf("\n%s\n", string);
    
    //Teen uue massiivi korrektsete indeksitega, peale igat tühikut uus element kuni end of line ehk enter, millal token = NULL
    char *token = strtok(string, " ");
    char *correct[MAX], *copy_correct[MAX];
    int i = 0;
    while (token != NULL){
        correct[i++] = token;
        printf("\n%s\n", token);
        token = strtok(NULL, " ");
    }
    
    //Teen koopia, sest massiivist eraldamine tegi originaali katki millegipärast
    memcpy(copy_correct, correct, sizeof(copy_correct));

    int n;
    for (n = 0; n < 8; n++){
        printf("correct[%s]\n", correct[n]);
        printf("copy_correct[%s]\n", copy_correct[n]);
        printf("\n");
    }
    
    //Teen 2 eraldi massiivi lihtsamaks tootlemiseks
    
    char *product[SML];
    char *price[SML];
    

    int posx, posy, x = 0, y = 1;
    
    for(posx = 0; posx < 4; posx++){
        product[posx] = correct[x];
        printf("%s\n", product[posx]);
        x = x + 2;
    }
    for(posy = 0; posy < 4; posy++){
        price[posy] = copy_correct[y];
        printf("%s\n", product[posy]);
        y = y + 2;
    }
    int m;
    printf("\n");
    printf("%s\n", product[0]);
    for (m = 0; m < 4; m++){
       
        printf("hind[%s]\n", price[m]);
        printf("toode[%s]\n", product[m]);
        printf("\n");

    }

    
    double price_f[SML];
    int d;
    for(d = 0; d < SML; d++){
        price_f[d] = (float) *price[i];
        printf("%f\n", price_f[d]);
    }
    
    int qwwerty;
    for(qwwerty = 0; qwwerty < 4; qwwerty++){
        printf("%f", price_f[qwwerty]);
    }
    
    
    
    return 0;
}