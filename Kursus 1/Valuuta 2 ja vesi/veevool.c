#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define YEARS 100 // max aastate arv 99
#define MONTHS 13 // 1 võrra liiga suured, kõik massiivi elemendid on -1 algväärtusega
#define DAYS 32 // hiljem kui loetud andmetega üle kirjutan näen millal kuu lõppeb

// Sisendi formaat: ./nim FAILINIMI eraldi / koos

void giveVal(float data[YEARS][MONTHS][DAYS]);
void findAvg(float data[YEARS][MONTHS][DAYS], float monthAvg[12], int delta, int firstYear, int mode, int counter);
void printData(float monthAvg[12], int firstYear, int programMode, int delta, int counter);


int readData(float data[YEARS][MONTHS][DAYS], int num, char *input[], int delta[2]){ // loen faili
    if(num < 3){
        printf("Sisend peab tulema käsurealt!\n");
        printf("Formaat: ./nim FAILINIMI koos/eraldi\n");
        printf("koos kui soovite kõikide aastate histogrammi, eraldi kui iga aasta eraldi\n");
        exit(0);
    }

    char allowed[2][7] = {"eraldi", "koos"};
    int mode;
    if(strcmp(allowed[0], input[2]) == 0) // kasutaja soovib eraldi histogramme
        mode = 0;
    else if(strcmp(allowed[1], input[2]) == 0) // 1 histogramm
        mode = 1;
    else{
        printf("Viimane sisend peab olema kas eraldi või koos\n");
        exit(0);
    }

	FILE *sisend;
	
	sisend = fopen(input[1], "r");
	if(sisend == NULL){
		printf("Faili ei saanud avada\n");	
		char buf[200];
		printf("Sisesta faili nimi: ");
		scanf("%s", buf);
		sisend = fopen(buf, "r");
		if(sisend == NULL)
			exit(0);
	}
    /*Jälgin koguaeg mis eelmise kuu ja aasta väärtus on, siis kui muutub muudan indeksit maatriksis*/
    int aasta, kuu, paev, previousY = 0, previousM = 0;
    float value;
    int i = 0, j = 0, k = 0, first = 0;
    while(1){
        if(fscanf(sisend, "%d.%d.%d %f", &paev, &kuu, &aasta, &value) < 4){
            break;
        }
        if(first == 0){
            delta[0] = aasta; // saan algaasta
            previousY = aasta;
            previousM = kuu;
            first++;
        }
        if(previousM != kuu){ // kuu muutus
            j++;
            k = 0;
        }
        if(previousY != aasta){ // vaatan kui kuu või aasta vahetuvad, siis liigun maatriksis edasi
            i++;
            j = 0;
        }
        
        previousY = aasta;
        previousM = kuu;
        delta[1] = aasta; // saan viimase aasta
        data[i][j][k] = value; // salvestan andmed
        k++;
    }
    return mode;
}


int main(int argc, char *argv[]){
    float data[YEARS][MONTHS][DAYS];
    giveVal(data); // algväärtustan, lihtsam kui midagi läheb valesti
    int span[2];
    int mode = readData(data, argc, argv, span);
    int delta = span[1] - span[0]; // Saan teada mitu aastat infot on failis    
    float monthAvg[12]; // kuu keskmiste massiiv
    int counter, limit = delta;
    
    if(mode == 1)
        limit = 1;
    for(counter = 0; counter < limit; counter++)
        findAvg(data, monthAvg, delta, span[0], mode, counter);
    return 0;
}

void giveVal(float data[YEARS][MONTHS][DAYS]){ // algväärtustan
    for(int i = 0; i < YEARS; i++)
        for(int j = 0; j < MONTHS; j++)
            for(int k = 0; k < DAYS; k++)
                data[i][j][k] = -1;
}

void findAvg(float data[YEARS][MONTHS][DAYS], float monthAvg[12], int delta, int firstYear, int mode, int counter){
    int i, j, k, limit = 0;
    int dayCount[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // salvestan mitu teatud kuu päeva oli kokku, et keskmine leida

    if(mode == 1) // kas kasutaja tahtis eraldi või koos, sellega määran mitmest aastast keskmine arvutatakse
        limit = delta; // koos - arvutatakse kõik aastad
    else
        limit = counter + 1; // eraldi - arvutatakse 1 aasta

    for(i = counter; i < limit; i++){
        for(j = 0; j < 12; j++){
            for(k = 0; data[i][j][k] != -1; k++){
                monthAvg[j] += data[i][j][k]; // liidan kõik sama kuu vooluhulgad kokku
            }
            dayCount[j] += k;
        }
    }
    /*
    printf("Daycounts\n");
    for(i = 0; i < 12; i++){
        printf("%d = %d\n", i + 1, dayCount[i]);
    }
    printf("\nTotal\n");
    for(i = 0; i < 12; i++){
        printf("%d = %.2f\n", i + 1, monthAvg[i]);
    }
    */

    for(i = 0; i < 12; i++){
        monthAvg[i] = monthAvg[i] / dayCount[i]; // keskmise arvutamine
    }
    
    /*
    printf("\nAvg\n");
    for(i = 0; i < 12; i++){
        printf("%d = %.2f\n", i + 1, monthAvg[i]);
    }
    */
    printData(monthAvg, firstYear, mode, delta, counter);
}

void printData(float monthAvg[12], int firstYear, int mode, int delta, int counter){
    char Months[13][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    int i, amount;
    static int tooBig = 1; // histogrammi skaala, tavaline on 1 : 1, peab jääma samaks uuesti funktsiooni kutsudes niiet static int
    if(mode == 1) 
        printf("\n%d - %d. %d aasta keskmine\n", firstYear, firstYear + delta, delta); 
    else
        printf("\n%d\n", firstYear + counter); 

    
    for(i = 0; i < 12; i++){
        amount = monthAvg[i];
        // Muudab ära histogrammi skaala, nt Tartu ja Oore omades oli kordades rohkem, ei mahtunud ära
        if(amount > 20 && i == 0 && counter == 0){ // muudab ainult alguses, et keskel skaala ei erineks
            if(amount > 150)
                tooBig = 10;
            else
                tooBig = 5; // saab muuta suhet, panin igaks juhuks et ära mahuks kõik
        }
        printf("%s %6.2f : ", Months[i], monthAvg[i]);
        while(amount > 0){
            printf("X");
            amount = amount - tooBig;
        }
        printf("\n");
    }
    printf("X = %d\n", tooBig);

}
