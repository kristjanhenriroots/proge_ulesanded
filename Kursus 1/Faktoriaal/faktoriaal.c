#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(void){
    
    int x, MP, Ans;
    
    printf("Faktoriaali leidja\n");
    printf("Sisestage number millest soovite faktoriaali võtta: ");
    scanf("%d", &x);

    //Kontrollin kas sisestatud number on suurem nullist
    if(x <= 0){
        printf("Sisestage nullist suurem number!\n");
        return 0;
    }

    //Funktsioon faktoriaali leidmiseks, lisan muutuja Multiplier = MP
    //Liidan kordajale 1 senikaua kuni see on väiksem kui sisestatud arvust 
    Ans = 1;
    for (MP = 1; MP <= x; MP++){
        Ans = Ans * MP;
        }
    printf("Sisestatud arvu faktoriaal on %u", Ans);
    return 0;
}