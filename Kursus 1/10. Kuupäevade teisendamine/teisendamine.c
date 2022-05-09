#include <stdio.h>

//Tunninäited liigaasta ning päevade arvu kuus leidmiseks
int liigaasta(int aasta){
	
    if((aasta % 4 == 0 && aasta % 100 != 0) || aasta % 400 == 0)
		return 1;
	else
		return 0;
}

int paevade_arv_kuus(int k, int a){
	if(k == 1 || k == 3 || k == 5 || k == 7 || k == 8 || k == 10 || k == 12)
		return 31;
	if(k == 4 || k == 6 || k == 9 || k == 11)
		return 30;
	if(k == 2)
		return 28 + liigaasta(a);
	return 0; // kõigis muudes "kuudes" on 0 päeva
}

int main(void){

    int X, p = 1, k = 1, a = 1900;

    printf("Kuupäeva teisendamine\n");
    printf("Sisestage päevade arv: ");
    scanf("%d", &X);
    
    if(X < 0){
        printf("Sisestage nullist suurem arv!\n");
        return 0;
    }

    //hakkan lahutama kuni X = 0
    while(X > 0){
        if(X >= 365){
            X = X - 365 - (liigaasta(a));//Kuna 1900 ei olnud liigaasta siis töötab nii ka, juhul kui sisestatakse täpselt 365
            a = a + 1;
        }
        if(X >= 31){
            if(k == 12){
                X = X - paevade_arv_kuus(k, a);
                a = a + 1;
                k = 1;
            }
            else{
                X = X - paevade_arv_kuus(k, a);
                k = k + 1;
            }
        }
        else{
            if(X + 1 > paevade_arv_kuus(k, a)){
                X = X - paevade_arv_kuus(k, a);
                k = k + 1;
            }
            else{
                if(X != 0){
                    p = X + 1;
                }
                X = 0;
            }
        }
    }
    
    printf("Kuupäev oleks siis: %d.%d.%d", p, k, a);
    
    return 0;
}