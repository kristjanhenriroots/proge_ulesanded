#include <stdio.h>
#include <stdlib.h>

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

int kontroll(int p, int k, int a){
    if(p <= 0 || paevade_arv_kuus(k, a) < p || k <= 0 || k > 12)
        printf("Kuupäev ei sobi!\n");
    else
        return 1;
    return 0;
}

/*Arvutamiseks teen kuupäevad päevade arvuks, algaasta on 0 ja sisestatud aasta on absoluutväärtusega juhul kui sisestatakse  midagi B.C*/
int inDays(int p, int k, int a){
    int value = 0, i, j;
    
    for(i = 0; i < abs(a); i++)//Kõik täis aastad nullist kuni sisestatuni
        value = value + 365 + liigaasta(i);
    
    for(j = 1; j < k; j++)//Plus kõik täis kuud viimases aastas
        value = value + paevade_arv_kuus(j, a);
    
    value = value + p; //Plus päevad

    return value;
}

int main(void){

    int p1, p2, k1, k2, a1, a2, one, two, ans;

    printf("Kahe kuupäeva vahe\n");
    printf("Kasutage formaati DD.MM.YYYY\n");
    printf("Sisestage esimene kuupäev: ");
    scanf("%d.%d.%d", &p1, &k1, &a1);
    
    if(kontroll(p1, k1, a1) == 0)
        return 0;
    
    printf("Sisestage teine kuupäev: ");
    scanf("%d.%d.%d", &p2, &k2, &a2);

    if(kontroll(p2, k2, a2) == 0)
        return 0;
    
    one = inDays(p1, k1, a1);
    two = inDays(p2, k2, a2);

    ans = abs(one - two);

    printf("Kuupäevade vahe on %d päeva", ans);

    return 0;
}