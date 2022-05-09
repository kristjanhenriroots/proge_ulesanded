#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMELEN 25 // nt maakonna nime pikkus
#define CODELEN 15 // jahiala koodi pikkus
#define MAXROW 100 // max 1 faili rida fgets jaoks

// loomade vanused
#define NA 200
#define YOUNG 201
#define MID 202
#define ADULT 203

typedef struct jahiandmed{ // jahiandmete hoidmine
    char code[CODELEN]; // jahiala kood
    int age; // vanus ja kogus
    int count;
}jdata_t;

typedef struct jahiandmed_arr{ // massiiv structidest, koos pikkusega
    unsigned int len;
    jdata_t *arr;
}animals_t;

typedef struct jahialad{ // jahialade hoidmine
    char code[CODELEN]; // jahiala kood ja maakond
    char county[NAMELEN];
}jahiala_t;

typedef struct jahialad_arr{ // massiiv structidest, koos pikkusega
    unsigned int len;
    jahiala_t *arr;
}area_t;

typedef struct statsistics{ // statistika hoidmiseks
    char county[NAMELEN];
    int ages[4]; // ei hoia erinevate vanuste arvu eraldi muutujates vaid massiivis
}numbers_t;

typedef struct table{
    unsigned int len;
    unsigned short year; // kasutaja etteantud aasta
    unsigned short reason; // hukkumise põhjus
    char separator; // strtok eraldaja, csv jaoks ;
    char species[NAMELEN]; // kasutaja etteantud liik
    numbers_t *numbers;
}stats_t;


int fileID(FILE *f){ //  saan aru kumb fail on juhul kui programmi sisenditeks pannakse suvalises järjekorras
    static short next, read = 0;
    char template[2][NAMELEN] = {"aasta", "kood"};
    char temp[MAXROW];
    fscanf(f, "%s", temp);
    if(read == 0){
        if(strncmp(temp, template[0], 5) == 0){ // jahiandmed
            read++;
            next = 1;
            return 1;
        }
        else if(strncmp(temp, template[1], 4) == 0){ // jahialad
            read++;
            next = 0;
            return 2;
        }
        else
            return 0;
    }
    else{
        if(strncmp(temp, template[next], 5 - next) == 0){ // teise faili lugemise korral peab olema esimesest failist erinev
            return next + 1;
        }
        else
            return 0;
        }
    return 0;
}

void readHunt(animals_t *animals, char *correct[], int counter){ // salvestan jahiandmed struktuuri
    strcpy(animals->arr[counter].code, correct[2]);
    animals->arr[counter].age = atoi(correct[4]); 
    animals->arr[counter].count = atoi(correct[6]);
}

void readArea(area_t *area, char *correct[], int counter){ // salvestan jahialad struktuuri
    strcpy(area->arr[counter].code, correct[0]);
    strcpy(area->arr[counter].county, correct[3]);
}

void fileopen(char *potentialFile, int argc, char *argv[], animals_t *animals, area_t *area, stats_t *stats){
    int filename = 1; // maybe user specified file on commandline
    FILE *f;
    static unsigned short filecount = 1;
    f = fopen(potentialFile, "r");
    if(f == NULL){
        if(argc > 1){ // argument present, let's try it
        f = fopen(argv[filecount], "r");
        if(f == NULL)
            filename = 0; // nope
        } else
            filename = 0;
    }
    
    char buf_in[NAMELEN];
    if(filename == 0){
        printf("Sisestage failinimi: ");
        scanf("%s", buf_in);
        f = fopen(buf_in, "r");
        if(f == NULL){
            printf("Faili ei leitud\n");
            exit(0); // user supplied one didn't work either, quit
        }
    }
    short type = fileID(f); // vaatan mis failiga on tegu
    int match = 0; // filtrile vastavate ridade arv   
    char *token;
    int i = 0;
    char buf[MAXROW];
    char line[MAXROW];
    char separator[1];
    strncpy(separator, &stats->separator, 1); // otse structist andmisega tekkisid pointeri probleemid, lihtne lahendus
    
    if(type == 2){ // jahialad fail sisestati
        int size = 341; // ei ole väga dünaamiline, kuid Eestis olevate jahialade arv ei tohiks eriliselt muutuda
        char *format[4];
        fgets(line, MAXROW, f); // võtan esimese rea ära

        area->arr = malloc(size * sizeof(jahiala_t));
        if(area->arr == NULL){
            printf("Error allocating memory\n");
            exit(0);
        }
        
        while(fgets(buf, MAXROW, f) != NULL){
            token = strtok(buf, separator);
            while(token != NULL){ // lõhun rea eraldi elementideks
                format[i++] = token;
                token = strtok(NULL, separator);
            }
            readArea(area, format, match); 
            match++;
            i = 0;
        }
        area->len = match;
    }
    else if(type == 1){ // jahiandmed sisestati
        int size = 10;
        char *format[7];
        fgets(line, MAXROW, f);
        animals->arr = malloc(size * sizeof(jdata_t)); // eeldan et failis on 10 rida
        if(animals->arr == NULL){
            exit(0);
        }
        while(fgets(buf, MAXROW, f) != NULL){
            token = strtok(buf, separator);
            while(token != NULL){
                format[i++] = token;
                token = strtok(NULL, separator);
            }
            if(stats->year == atoi(format[0]) && strcmp(stats->species, format[1]) == 0){
                if(stats->reason == 1){ // lugemist on kahel korral eraldi juhuks kui kasutaja soovib kõiki vanuseid
                    if(match == size){ // struct array on vaja suuremaks teha
                        size = size * 2;
                        jdata_t *p = realloc(animals->arr, size * sizeof(jdata_t)); 
                        if(p == NULL){
                            printf("Error reallocating memory\n");
                            exit(0);
                        }
                        animals->arr = p;
                    }
                    readHunt(animals, format, match); 
                    match++;
                }
                else if(stats->reason == atoi(format[3])){
                    if(match == size){
                        size = size * 2;
                        jdata_t *p = realloc(animals->arr, size * sizeof(jdata_t)); 
                        if(p == NULL){
                            printf("Error reallocating memory\n");
                            exit(0);
                        }
                        animals->arr = p;
                    }
                    readHunt(animals, format, match); 
                    match++;
                }
            }
            i = 0;
        }
        animals->len = match;
        printf("Filtrile vastavad %d rida\n", match);
    }
    else{
        printf("Error with file\n");
        exit(0);
    }
    fclose(f);
    if(match < 1){
        printf("Failis ei ole vastavaid andmeid!\n");
        exit(0);
    }
    filecount++;
}

void getFilters(stats_t *stats){ // kasutajaga suhtlemine
    char second[NAMELEN];
    printf("Enter year: ");
    scanf("%hd", &stats->year);
    if(stats->year < 0){ // stats->year == unsigned short
        printf("Negatiivne aasta?\n");
        exit(0);
    }
    printf("Enter species: ");
    scanf("%s %s", stats->species, second); // fgets oleks lihtsam ja parem, aga ei hakka segama fgets ja scanf
    strcat(stats->species, " ");
    strcat(stats->species, second);
    printf("Sisestage põhjus\n");
    printf("100 - Muu\n101 - Küttimine\n102 - Liikluses hukkunud\n103 - Loendamine\n  1 - Kõik\n");
    scanf("%hd", &stats->reason);
    if(stats->reason != 100 && stats->reason != 101 && stats->reason != 102 && stats->reason != 103 && stats->reason != 1){
        printf("Vigane sisend\n");
        exit(0);
    }

}

int namesort(const void* a, const void* b){ // nimed tähestiku järjekorda
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return strcmp(ia, ib);
}

int findIndex(stats_t *stats, char county[]){
    for(int i = 0; i < stats->len; i++){
        if(strcmp(stats->numbers[i].county, county) == 0)
            return i;
    }   
    return -1;
}

void killCount(animals_t *animals, area_t *area, stats_t *stats){ // statistika liitmine
    int found = 0;
    stats->len = 0;
    stats->numbers = malloc(15 * sizeof(numbers_t)); // 15 maakonda on, kui kõiki andmetes ei leidu jääb mälu üle, kuid ei hakka dünaamiliseks muutma
    for(int z = 0; z < 15; z++)
        memset(stats->numbers[z].ages, 0, 4 * sizeof(int)); // et += töötaks
    int i, j, index;
    for(i = 0; i < animals->len; i++){
        for(j = 0; j < area->len; j++){
            if(strcmp(animals->arr[i].code, area->arr[j].code) == 0){ // andmeid liidetakse juurde kuni maakonnad on samad
                index = findIndex(stats, area->arr[j].county);
                if(index == -1){ // leiti uus maakond
                    strcpy(stats->numbers[stats->len].county, area->arr[j].county);
                    stats->len++;
                }
                if(animals->arr[i].age == NA) // vaatan mis vanusele andmed juurde liita
                    stats->numbers[stats->len - 1].ages[0] += animals->arr[i].count;
                else if(animals->arr[i].age == YOUNG)
                    stats->numbers[stats->len - 1].ages[1] += animals->arr[i].count;
                else if(animals->arr[i].age == MID)
                    stats->numbers[stats->len - 1].ages[2] += animals->arr[i].count;
                else if(animals->arr[i].age == ADULT)
                    stats->numbers[stats->len - 1].ages[3] += animals->arr[i].count;
                found++;
            }
        }
        if(found == 0){
            printf("Jahiala %s ei leitud!\nAndmed puudulikud.\n", animals->arr[i].code);
            exit(0);
        }
        found = 0;
    }
}

void countyPrint(stats_t *stats){ // prindib väljundi
    int i, j;
    char header[5][NAMELEN] = { {"Maakond"}, {"Teadmata"}, {"Noor"}, {"Keskmine"}, {"Täiskasvanud"} };
    printf(" %5s | %5s | %5s | %5s | %5s\n", header[4], header[3], header[2], header[1], header[0]);
    for(i = 0; i < stats->len; i++){
        for(j = 3; j >= 0; j--){
            printf("%10d ", stats->numbers[i].ages[j]);
        }
        printf("    %-15s\n", stats->numbers[i].county);
    }
}


int main(int argc, char *argv[]){
    animals_t animals = {0, NULL}; 
    area_t area = {0, NULL};
    stats_t stats = {0, 0, 0, ';', "0", NULL}; 
    //                         ^   määran ära eraldaja
    getFilters(&stats); // küsin kasutaja käesst filtrid
    fileopen("jahiala.csv", argc, argv, &animals, &area, &stats); // avan mõlemad failid
    fileopen("jahiandmed.csv", argc, argv, &animals, &area, &stats);
    killCount(&animals, &area, &stats); // panen kokku statistika
    free(animals.arr); // algandmeid ei ole enam vaja
    free(area.arr);
    qsort(stats.numbers->county, stats.len, sizeof(stats.numbers[0]), namesort); // maakonnad tähestiku järjekorda
    countyPrint(&stats); // väljastan tulemused
    free(stats.numbers);
    return 0;
}