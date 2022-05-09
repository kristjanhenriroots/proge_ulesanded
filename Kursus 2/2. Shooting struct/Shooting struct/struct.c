#include <stdio.h>
#include <string.h>

#define NAMELEN 20 // max nime pikkus
#define MAX 1000 // max osalejate arv
#define SHOTCOUNT 10 // mitu laskekorda oli
#define TOP 5 // mitu kohta printida, hetkel top 5

typedef struct competition{
    char name[NAMELEN];
    int score;
    int bullseyecount;
    int shotdata[SHOTCOUNT];
} competitor_t;

typedef struct datalist{
    int n;
    competitor_t list[MAX];
} competitors_t;

int listcheck(char name_check[], int score_check, int bscount_check, competitors_t *data){ // kontroll kas võisleja on juba andmetes
    if(score_check < 0 || score_check > 100){ // negatiivne summa või üle 100 punkti
        printf("\nVigased andmed! Võistlejal %s on %d punkti!\n", name_check, score_check);
        return 0;
    }
    for(int i = 0; i < data->n; i++){
        if(strcmp(name_check, data->list[i].name) == 0){ // Leiti sama nimi
            if(score_check > data->list[i].score){ // Parem tulemus, vahetan ära structis
                data->list[i].score = score_check;
                data->list[i].bullseyecount = bscount_check;
                return 0;
            }
            else if((score_check = data->list[i].score) && (data->list[i].bullseyecount < bscount_check)){ // Sama tulemus, vaatan täistabamuste arvu
                data->list[i].score = score_check;
                data->list[i].bullseyecount = bscount_check;
                return 0;
            }
            else{ // Võistleja saavutas juba parema / sama tulemuse
                printf("%s saavutas juba tulemuse %dp %d täistabamusega\n", name_check, data->list[i].score, data->list[i].bullseyecount);
                return 0;
            }
        }
    }
    return 1; // uus nimi sisestati
}

void addtoList(char name[], int score, int bscount, competitors_t *data, int shots[]){ // saaks ka ühe reaga aga ei saanud tööle "nested" structi sisse kirjutamist
    strcpy(data->list[data->n].name, name);
    data->list[data->n].score = score;
    data->list[data->n].bullseyecount = bscount;
    for(int i = 0; i < SHOTCOUNT; i++)
        data->list[data->n].shotdata[i] = shots[i];
    data->n++;
}

void readFile(competitors_t *data){ // failist lugemine
	FILE *sisend;

    while(1){
        char buf[200];
        printf("Sisesta faili nimi: ");
        scanf("%s", buf);
        sisend = fopen(buf, "r");
        if(sisend != NULL)
            break;
        printf("Sellist faili ei eksisteeri!\n");
    }
    
    int n, counter, bs_counter, result, shots[SHOTCOUNT], addedNames = 0;
    char name_check[NAMELEN];

    fscanf(sisend, "%d", &n);
    for(int i = 0; i < n; i++){
        counter = 0;
        bs_counter = 0;
        fscanf(sisend, "%s", name_check);
        for(int j = 0; j < SHOTCOUNT; j++){
            fscanf(sisend, "%d", &shots[j]);
            if(shots[j] == 10)
                bs_counter++;
            counter += shots[j];
        }
        result = listcheck(name_check, counter, bs_counter, data); //kontrollin kas nimi või parem tulemus on olemas
        if(result == 1){ // Uued nimed
            addtoList(name_check, counter, bs_counter, data, shots);
            addedNames++;
        }
    }
    printf("\n%d lisatud nimekirja\n", addedNames);
}

int readManual(competitors_t *data){ // käsitsi sisestamine
    int nr, shots[SHOTCOUNT], addedNames = 0;
    
    printf("Manuaalne sisestus\n");
    printf("Mitu nime? ");
    scanf("%d", &nr);
    
    if(nr < 1)
        return 0;
    printf("Sisestage andmed: nimi tulemused\n");
    
    int counter, bs_counter, result = 0;
    char name_check[NAMELEN];
    
    for(int i = 0; i < nr; i++){
        counter = 0;
        bs_counter = 0;
        scanf("%s", name_check);
        for(int j = 0; j < SHOTCOUNT; j++){
            scanf("%d", &shots[j]);
            if(shots[j] == 10)
                bs_counter++;
            counter += shots[j];
        }
        result = listcheck(name_check, counter, bs_counter, data);
        if(result == 1){ // Uued nimed
            addtoList(name_check, counter, bs_counter, data, shots);
            addedNames++;
        }
    }
    printf("\n%d lisatud nimekirja\n", addedNames);
    return 0;
}

int winnerPrint(competitors_t *data){ // sorteerin ja prindin top 5
    if(data->n == 0){
        printf("No data\n");
        return 0;
    }

    int i, j, k, idx[data->n - 1];
    for(i = 0; i < data->n; i++) // loon indeksmassiivi ja algväärtustan
        idx[i] = i;
    for(i = 0; i < data->n; i++){
		for(j = i + 1; j < data->n; j++){
			if(data->list[idx[i]].score < data->list[idx[j]].score){ // sorteerin punktisumma järgi ja kui leiab sama siis ka täistabamuste järgi
				k = idx[i];
				idx[i] = idx[j];
				idx[j] = k;
			}
            else if(data->list[idx[i]].score == data->list[idx[j]].score && data->list[idx[i]].bullseyecount < data->list[idx[j]].bullseyecount){
                k = idx[i];
				idx[i] = idx[j];
				idx[j] = k;
            }
		}
	}
    for(i = 0; i < TOP; i++){
        printf("\n%2d. %10s tulemusega %3dp, %2d täistabamusega\n", i + 1, data->list[idx[i]].name, data->list[idx[i]].score, data->list[idx[i]].bullseyecount);
    }
    return 0;
}

void printMain(){ // käskluste printimine
    printf("1: Loe failist\n"
        "2: Manuaalne tulemuste lisamine\n"
        "3: Prindi top 5\n"
        "4: Kustuta tulemused\n"
        "H: Helper\n"
        "X: Välju\n");
}

int main(void){
    competitors_t data = {.n = 0};
    int inloop = 1;
    printMain();
    while(inloop){  
        char input[1];
        scanf("%c", &input[0]);
        switch(input[0]){
            case '1': readFile(&data);
                break;
            case '2': readManual(&data);
                break;
            case '3': winnerPrint(&data);
                break;
            case 'X': inloop = 0;
                break;
            case '4': data.n = 0;
                break;
            case 'H': printMain(); // prindin käsklused uuesti
                break;
        }
    }
    return 0;
}