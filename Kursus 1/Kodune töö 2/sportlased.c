#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 10 // Max nime pikkus

int readN();
void readArr(int n, char names[][LEN], float score[]);
void giveValues(int N, char allNames[N][LEN], float score[2][N]);
int allList(int n, char names[n][LEN], int N, char allNames[N][LEN], float allScore[2][N], int uniqueNames, int counter, float score[n]);
void print(int N, char allNames[N][LEN], float allScore[2][N], int uniqueNames, int which);
void orderScore(int N, float allScore[2][N], int uniqueNames, int idx[uniqueNames]);
void printWinners(int N, int nx, char allNames[nx][LEN], float allScore[2][N], int idx[nx]);

int main(void){
    int n1, n2, n3;

    printf("Sportlaste tulemused\n");
    n1 = readN(); // info lugemine ja massiivide loomine

    char names1[n1][LEN];
    float score1[n1];
    readArr(n1, names1, score1);

    n2 = readN();
    
    char names2[n2][LEN];
    float score2[n2];
    readArr(n2, names2, score2);

    n3 = readN();
    
    char names3[n3][LEN];
    float score3[n3];
    readArr(n3, names3, score3);

    int N = n1 + n2 + n3; // Maksimaalne suurus juhul kui kõik nimed on erinevad
    char allNames[N][LEN]; // Hoiab kõiki nimesid
    float allScore[2][N]; // Mitmemõõtmeline, hoiab kõiki tulemusi ning mitmel võistlusel osaleti
    giveValues(N, allNames, allScore);

    int uniqueNames = 0; // näitab mitu võistlejat on
    
    uniqueNames = allList(n1, names1, N, allNames, allScore, uniqueNames, 0, score1);
    uniqueNames = allList(n2, names2, N, allNames, allScore, uniqueNames, 1, score2);
    uniqueNames = allList(n3, names3, N, allNames, allScore, uniqueNames, 2, score3);

    int idx[uniqueNames]; // indeksmassiiv kahaneva järjekorra jaoks
    
    orderScore(N, allScore, uniqueNames, idx); // punktide järgi järjestus
    print(N, allNames, allScore, uniqueNames, 0); // prindi kõikides osalejad
    printWinners(N, uniqueNames, allNames, allScore, idx); // prindi top 3
    print(N, allNames, allScore, uniqueNames, 1); // prindi läbikukkujad

    return 0;
}

int readN(){ // loeb massiivi suuruse
    int n;
	if(scanf("%d", &n) < 1)
		exit(0);
	return n;
}

void readArr(int n, char names[][LEN], float score[]){ // loeb osalejate ja punktide massiivid
    int i;
	for(i = 0; i < n; i++){		
		if(scanf("%s %f", names[i], &score[i])< 2)
			exit(0);
	}
}

void giveValues(int N, char allNames[N][LEN], float score[2][N]){ // algväärtustan massiivid
    int i;
    for(i = 0; i < N; i++){
        strcpy(allNames[i], "1"); // nii saan aru millal nimed said otsa või prindib vale asja
        score[0][i] = 0; // tava punktiväärtus
        score[1][i] = 1; // loeb mitmel võistlusel osaleti
    }
}

int allList(int n, char names[n][LEN], int N, char allNames[N][LEN], float allScore[2][N], int uniqueNames, int counter, float score[n]){
    
    int i, j;
    int breakcount; // Saan aru kas nimi on juba olemas

    for(i = 0; i < n; i++){
        breakcount = 0;
        for(j = 0; j < uniqueNames; j++){
            if(strcmp(names[i], allNames[j]) == 0){ // nimi on juba olnud, punktid tuleb liita
                allScore[0][j] += score[i];
                allScore[1][j]++; // osales veel ühel võistlusel
                breakcount++;
                break;
            }
        }
        if(breakcount == 0){ // Nime ei leitud, lisan massiivi
            strcpy(allNames[uniqueNames], names[i]);
            allScore[0][uniqueNames] += score[i];
            uniqueNames++; // uus nimi lisatud
        }
    }

    // Keeruline tsükkel, juhul kui midagi on valesti
    /*if(counter == 2){
        printf("\nUnique names: %d\n", uniqueNames);
        for(i = 0; i < uniqueNames; i++){
            printf("%3d. %8s - %5.1f - times seen: %3.0f\n", i + 1, allNames[i], allScore[0][i], allScore[1][i]);
        }
        printf("\n\n");
    }*/

    if(uniqueNames == N){
        printf("Mitte ükski võistleja ei lõpetanud kõiki võistlusi\n");
        exit(0);
    }
    
    return uniqueNames;
}

void orderScore(int N, float allScore[2][N], int uniqueNames, int idx[uniqueNames]){ // punktide järjestus
    int i, j, k;
    for(i = 0; i < uniqueNames; i++){ // indeksmassiivi algväärtustamine
        idx[i] = i;
    }
    for(i = 0; i < uniqueNames; i++){ // punktid kahanevas järjekorras indeksmassiiviga
		for(j = i + 1; j < uniqueNames; j++){
			if(allScore[0][idx[i]] < allScore[0][idx[j]]){
				k = idx[i];
				idx[i] = idx[j];
				idx[j] = k;
			} 
		}
	}
}



void print(int N, char allNames[N][LEN], float allScore[2][N], int uniqueNames, int which){ // prindi võitjad/kaotajad

    int i, j = 0, min, max;
    int printed = 0;
    // kumba prindib, info saab samast kohast, allScore[1], mis on 1-3
    if(which == 0){
        min = 2;
        max = 999;
        printf("\nKõikidel võistlustel osalesid:");
    }
    else{
        min = 0;
        max = 3;
        printf("\nVõistlustel ebaõnnestusid:");
    }
    for(i = 0; i < uniqueNames; i++){
        if(allScore[1][i] > min && allScore[1][i] < max){
            if(j > 0){
                printf(",");
            }
            printf(" %s", allNames[i]);
            j++;
            printed++;
        }
        if(i + 1 == uniqueNames && printed == 0){ // Kui keegi ei ebaõnnestunud prindib - lihtsalt
            printf(" -\n");
        }
    }
    printf("\n\n");
}


void printWinners(int N, int nx, char allNames[nx][LEN], float allScore[2][N], int idx[nx]){ // prindin võitjad

    int i, j, place = 1, max = 3;
    float previous = 0;

    for(i = 0; i < nx; i++){ // nx on uniqueNames, kõik vaatab läbi
        if(allScore[1][idx[i]] >= 3 && place < 4 && allScore[0][idx[i]] != previous){ // kas oli ikka kõigil võistlustel ja juba ei ole prinditud
            printf("%2d. koht: ", place);
            printf("%6s", allNames[idx[i]]);
            for(j = i + 1; j < nx; j++){ // vaatan kas kellelgi oli veel sama palju punkte
                if(allScore[0][idx[i]] == allScore[0][idx[j]] && allScore[1][idx[j]] >= 3){
                    printf(", %s", allNames[idx[j]]);
                }
            }
            printf(" tulemusega %5.1f\n", allScore[0][idx[i]]); // prindi punktid
            place++;
            previous = allScore[0][idx[i]]; // kontroll järgmiseks korraks
        }
        if(i + 1 == nx && place <= 3){ // kui massiiv sai otsa ja kõikidel kohtadel pole veel kedagi
            for(j = place; j <= max; j++){
                printf("%2d. koht: -\n", j); // prindib nii mitu korda kui mitu puudu on
            }
        }
    }
    printf("\n");
}
