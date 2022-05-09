#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NAMELEN 20
#define MAXROW 60
#define MAX 30
#define NRAMOUNT 5
#define MINVAL 1
#define MAXVAL 30


// 70% bells and whistles 30% ülesanne

typedef struct people{ // suurus 20 * 2 + 2 * 5 + 2 = 52
    char name[NAMELEN * 2];
    unsigned short *numbers;
    unsigned short tickets;
}people_t;

typedef struct arr{
    FILE *f;
    unsigned int count; // mitu inimest on
    unsigned int ticket_max;
    people_t *array;
}data_t;

void read(data_t *loto){
    int peoplecount = 0;
    char *temp;
    temp = malloc(MAXROW); // will only store 1 row and be freed afterwards 
    if(temp == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }
    while(fgets(temp, MAXROW, loto->f) != NULL){
        peoplecount++;   
    }
    free(temp);
    loto->count = peoplecount;
    rewind(loto->f); // loen 2x
    unsigned int i = 0;
    char *last_buf;
    last_buf = malloc(MAXROW);
    if(last_buf == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }

    loto->array = malloc(peoplecount * (sizeof(people_t))); 
    if(loto->array == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }

    for(int k = 0; k < peoplecount; k++){
        loto->array[k].numbers = malloc(NRAMOUNT * sizeof(unsigned short));
        if(loto->array[k].numbers == NULL){
            printf("Error allocating memory\n");
            exit(0);
        }
    }

    int j = 0;
    while(i < peoplecount){
        fscanf(loto->f, "%s %s", loto->array[i].name, last_buf);
        for(j = 0; j < NRAMOUNT; j++)
            fscanf(loto->f, "%hd", &loto->array[i].numbers[j]);
        strcat(loto->array[i].name, " ");
        strcat(loto->array[i].name, last_buf); // väga loll ja laisk aga töötab :/
        i++;
    }
    free(last_buf);  
}

int compare(const void *a, const void *b) { // piletite jaoks
	people_t *ia = (people_t *)a;
    people_t *ib = (people_t *)b;
	return (int)(ib->tickets - ia->tickets);
}

int namesort(const void* a, const void* b){ // nimede jaoks
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return strcmp(ia, ib);
}
int numsort(const void * a, const void * b){ // loto numbrite jaoks
  if (*(double*)a > *(double*)b)
    return 1;
  else if (*(double*)a < *(double*)b)
    return -1;
  else
    return 0;  
}

void sort(data_t *loto){ // nimede sorteerimine tähestiku järjekorda
    qsort(loto->array->name, loto->count - 1, sizeof(loto->array[0]), namesort);
}


// leian iga inimese piletite arvu
void ticketcounter(data_t *loto){
    int j = 1, i = 0;
    loto->ticket_max = 1;
    while(i < loto->count - 1){
        while(1){
            if(strcmp(loto->array[i].name, loto->array[i + j].name) == 0)
                j++;
            else
                break;
        }
        loto->array[i].tickets = j;
        if(j > 1){
            i += j;
        }
        else{
            i++;
        }
        if(j + 1 > loto->ticket_max)
            loto->ticket_max = j;
        j = 1;  
    }
}

void overOne(data_t *loto){ // need kellel on rohkem kui 1 pilet
    qsort(loto->array, loto->count - 1, sizeof(people_t), compare);
}


void winningNum(unsigned short *NRs){ // suvalised numbrid mis ei kordu (ei tea täpselt kas tohivad korduda)
    srand(time(0));
    for(int i = 0; i < NRAMOUNT; i++){
		NRs[i] = MINVAL + rand() % (MAXVAL - MINVAL + 1);
        if(MAXVAL - MINVAL + 1 > NRAMOUNT){ // parandus
            for(int j = 0; j < i; j++){
                if(NRs[j] == NRs[i]){
                    i--;
                }
            } 
        }
    }
}

void analytics(data_t *loto, unsigned short *rig, short rigcheck, float jackpot){ // ebavajalik
    int i = 0, j = 0;
    double *values;
    double dbl_val; 
    values = malloc(loto->count * sizeof(double) * 10);
    if(values == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }
    for(i = 0; i < loto->count; i++){
        for(j = 1; j <= NRAMOUNT; j++){
            dbl_val = (double) loto->array[i].numbers[j - 1];
            values[i] += dbl_val / pow(100, j); 
        }
    }
    qsort(values, loto->count - 1, sizeof(values[0]), numsort);
    i = 0, j = 0;
    int same = 0;
    while(i < loto->count - 1){
        while(1){
            if(values[i] == values[j])
                j++;
            else
                break;
        }
        if(j > 1){
            i += j;
        }
        else{
            i++;
        }
        if(j + 1 > same)
            same = j;
        j = 1;  
    }
    printf("\nThe most popular numbers, found %d time(s):\n", same + 1);
    int printcount = 0, allcount;
    double previous;
    for(i = 0; i < loto->count - 1; i++){
        previous = 0;
        if(values[i] == values[i + same]){
            if(printcount < 10){
                for(j = 0; j < NRAMOUNT; j++){
                    values[i] = values[i] * 100 - previous;
                    printf("%2.0f ", values[i]);
                    previous = round(values[i]) * 100;
                }
                printf("\n");
                printcount++;
            }
            else
                allcount++;
            i += same;   
        }
    }
    if(allcount - printcount / NRAMOUNT - 1 > 0)
        printf("+%d as popular numbers\n", allcount);
    free(values);

    // finding winner
    static unsigned short *winningNRs;
    unsigned int *winnerLoc;
    winningNRs = malloc(NRAMOUNT * sizeof(short));
    if(winningNRs == NULL){
        printf("Error allocating memory for winning numbers\n");
        exit(0);
    }
    winnerLoc = malloc(1 * sizeof(int));
    if(winnerLoc == NULL){
        printf("Error allocating memory for winner locations\n");
        exit(0);
    }


    //rigging 
    if(rigcheck != 0){
        for(i = 0; i < NRAMOUNT; i++){
            winningNRs[i] = rig[i];
        }
    }
    else{
        winningNum(winningNRs); // random winning numbers
    }
    
    
    printf("\nThe winning numbers are : ");
    for(i = 0; i < NRAMOUNT; i++){
        printf("%2hd ", winningNRs[i]);
    }
    printf("\n\n");

    short next, size = 2, location = 0;
    int *p;
    // checking for winners
    for(i = 0; i < loto->count - 1; i++){
        if(winningNRs[0] == loto->array[i].numbers[0]){
            for(next = 1; next < NRAMOUNT; next++){
                if(winningNRs[next] != loto->array[i].numbers[next])
                    break;
                else if(next == NRAMOUNT - 1){
                    p = realloc(winnerLoc, size * sizeof(short));
                    if(p == NULL){
                        printf("Error reallocating memory for winner location array\n");
                        exit(0);
                    }
                    winnerLoc[location] = i;
                    size++;
                    location++;
                }
            }
            
        }
    }
    if(size == 2)
        printf("No winners this time, better luck next time!\n");
    else{
        printf("Congrats to the winner(s)!\n");
        for(i = 0; i < location; i++){
            printf("%s\n\ttickets: %hd\n", loto->array[winnerLoc[i]].name, loto->array[winnerLoc[i]].tickets);
            printf("\tPayout: %.0f\n", jackpot / (size - 2) * loto->array[winnerLoc[i]].tickets);
        }
    }
    free(winningNRs);
    free(winnerLoc);
}

void printMain(){ // käskluste printimine
    printf("1: Read data\n"
        "2: Print multiple ticket owners\n"
        "3: Rig the results :o\n"
        "4: Find a winner\n"
        "H: Help\n"
        "X: Exit\n");
}

int main(int argc, char *argv[]){
    data_t loto;
    unsigned short rig[NRAMOUNT];
    short inloop = 1, tamper = 0, filescan = 0;
    float jackpot;
    printMain();
    while(inloop){  
        char input[1];
        scanf("%c", &input[0]);
        switch(input[0]){
            case '1': 
                if(filescan > 0){
                    printf("File already read, doesn't support multiple files currently\n");
                    break;
                }

                int filename = 1; // maybe user specified file on commandline
                FILE *f;
                if(argc > 1 && filescan < 1){ // argument present, let's try it
                    f = fopen(argv[1], "r");
                    if(f == NULL)
                        filename = 0; // nope
                } else
                    filename = 0;
                char buf[MAX];
                if(filename == 0){
                    printf("Please enter filename: ");
                    scanf("%s", buf);
                    f = fopen(buf, "r");
                    if(f == NULL)
                        exit(0); // user supplied one didn't work either, quit
                }
                filescan++;
                loto.f = f;
                read(&loto);
                fclose(f);
                sort(&loto);
                ticketcounter(&loto);
                printf("Read %d lines\n", loto.count);
                break;
            case '2': 
                if(filescan < 1){
                    printf("Read data first\n");
                    break;
                }

                overOne(&loto);
                printf("\nTicketmax is %d\n\n", loto.ticket_max);
                for(int j = 0; j < loto.count - 1; j++){
                    if(loto.array[j].tickets > 1)
                        printf("%20s ticket count %2d\n", loto.array[j].name, loto.array[j].tickets);
                }
                break;
            case '3': 
                if(filescan < 1){
                    printf("Read data first\n");
                    break;
                }
                printf("Enter the winning numbers: ");
                for(int i = 0; i < NRAMOUNT; i++){
                    if(scanf("%hd", &rig[i]) < 1){
                        break;
                    }
                    if(rig[i] < 1 || rig[i] > 30){
                        printf("Every value has to be between %d and %d\n", MINVAL, MAXVAL);
                    }
                    tamper = 1;

                }
                break;
            case 'X': 
                inloop = 0;
                free(loto.array);
                break;
            case '4':
                if(filescan < 1){
                    printf("Read data first\n");
                    break;
                } 
                printf("Enter Jackpot amount: ");
                scanf("%f", &jackpot);
                if(jackpot < 1){
                    printf("jackpot has to be larger than 0\n");
                    break;
                }
                analytics(&loto, rig, tamper, jackpot);
                tamper = 0;
                break;
            case 'H': printMain(); // prindin käsklused uuesti
                break;
        }
    }
    return 0;
}