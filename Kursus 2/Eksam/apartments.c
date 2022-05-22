#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMELEN 50

typedef struct data{
    char linnaosa[NAMELEN];
    char address[NAMELEN];
    short rooms;
    float size;
    short floor, total_floors;
    int price;
} data_t;

typedef struct apartments{
    int len;
    FILE *f;
    data_t *arr;
} apartments_t;

int read_data(apartments_t *AP){
    AP->len = 0;
    int size = 10;                // guessing the file is 10 rows
    AP->arr = malloc(size * sizeof(data_t));
    if(AP->arr == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }

    int i = 0;
    while(fscanf(AP->f, "%s %s %hd %f %hd/%hd %d", 
                        AP->arr[i].linnaosa, AP->arr[i].address, &AP->arr[i].rooms, &AP->arr[i].size, &AP->arr[i].floor,
                        &AP->arr[i].total_floors, &AP->arr[i].price) > 6){
        i++;
        if(i == size){
            size *= 2;          // increasing size 2x
            //printf("reallocating memory\n");
            data_t *p = realloc(AP->arr, size * sizeof(data_t));
            if(p == NULL){
                printf("Error reallocating memory\n");
                exit(0);
            }
            AP->arr = p;
        }
    }
    if(i == 0){
        printf("The file is empty\n");
        exit(0);
    }
    printf("read lines %d\n", i);
    AP->len = i;
    /*
    for(int j = 0; j < i; j++){
        printf("%s %s %hd %f %hd/%hd %d\n", 
                        AP->arr[j].linnaosa, AP->arr[j].address, AP->arr[j].rooms, AP->arr[j].size, AP->arr[j].floor,
                        AP->arr[j].total_floors, AP->arr[j].price);
    }
    */
    fclose(AP->f);
    return 0;
}


void averages(apartments_t *AP){
    float average_rooms = 0, average_size = 0;
    int i;

    for(i = 0; i < AP->len; i++){
        average_rooms += AP->arr[i].rooms;
        average_size += AP->arr[i].size;
    }

    average_rooms /= AP->len;
    average_size /= AP->len; 


    printf("Average rooms: %.2f eur\nAverage size: %.3f m2\n", average_rooms, average_size);

    return;
}

void linnaosa_avg(apartments_t *AP){
    char buf[NAMELEN];
    printf("Enter linnaosa name: ");
    scanf("%s", buf);
    if(strlen(buf) < 3){                // 3 seems like a good number, can also be 1
        printf("Cmon now >:(\n");
        return;
    }

    int i, matches = 0;
    float average_price = 0, average_size = 0;
    for(i = 0; i < AP->len; i++){
        if(!strcmp(buf, AP->arr[i].linnaosa)){  // adding to statistics only if the linnaosa matches
            matches++;
            average_price += AP->arr[i].price;
            average_size += AP->arr[i].size;
        }
    }
    if(matches == 0){   // There were no matches
        printf("There were no matches for %s\n", buf);
        return;
    }

    average_price /= AP->len;
    average_size /= AP->len;

    float price_per_m2 = average_price / average_size;


    printf( "Average price for %s: %.2f eur\n"
            "Average size for %s: %.3f m2\n"
            "Average price per m2: %.2f eur\n", buf, average_price, buf, average_size, price_per_m2);



    return;
}

int comp(const void * a, const void * b){ // qsort to get ascending order for apartment prices
    data_t *ia = (data_t *)a;
    data_t *ib = (data_t *)b;

    return (ia->price - ib->price);
}

void write_html(apartments_t *new_AP){
    printf("Starting html file making\n");
    FILE *f;
    f = fopen("apartments.html", "w");
    if(f == NULL){
        printf("Error opening file\n");
        exit(0);
    }

    fprintf(f,      "<!DOCTYPE html>\n\n"          // print anything til table data
                    "<html lang=\"et\">\n\n"
                    "<head>\n"
                    "\t<meta charset=\"utf-8\" />\n"
                    "\t<title>Hello!</title>\n"
                    "</head>\n\n"
                    "<body>\n"
                    "\t<div>\n"
                    "\t\t<table style = \"margin-left:auto;margin-right:auto;\">\n"
                    "\t\t\t<thead>\n"
                    "\t\t\t\t<tr>\n"
                    "\t\t\t\t\t<th>nr</th>\n"
                    "\t\t\t\t\t<th>Linnaosa</th>\n"
                    "\t\t\t\t\t<th>Aadress</th>\n"
                    "\t\t\t\t\t<th>Tubade arv</th>\n"
                    "\t\t\t\t\t<th>Suurus m2</th>\n"
                    "\t\t\t\t\t<th>Korrus</th>\n"
                    "\t\t\t\t\t<th>Korruste arv</th>\n"
                    "\t\t\t\t\t<th>Hind</th>\n"
                    "\t\t\t\t</tr>\n"
                    "\t\t\t</thead>\n"
                    "\t\t\t<tbody>\n"
            );




    for(int i = 0; i < new_AP->len; i++){
        fprintf(f,  "\t\t\t\t<tr>\n"
                    "\t\t\t\t\t<td>%d</td>\n"
                    "\t\t\t\t\t<td>%s</td>\n"
                    "\t\t\t\t\t<td>%s</td>\n"
                    "\t\t\t\t\t<td>%hd</td>\n"
                    "\t\t\t\t\t<td>%.3f</td>\n"
                    "\t\t\t\t\t<td>%hd</td>\n"
                    "\t\t\t\t\t<td>%hd</td>\n"
                    "\t\t\t\t\t<td>%.2d</td>\n"
                    "\t\t\t\t</tr>\n",
                    i + 1, new_AP->arr[i].linnaosa, new_AP->arr[i].address, new_AP->arr[i].rooms, new_AP->arr[i].size, new_AP->arr[i].floor,
                        new_AP->arr[i].total_floors, new_AP->arr[i].price
                );
    }

    fprintf(f,      "\t\t\t</tbody>\n"
                    "\t\t</table>\n"
                    "\t</div>\n"
                    "</body>\n"
                    "</html>"
            );
    fclose(f);
    printf("HTML file created successfully\n");
}

void linnaosa_asc(apartments_t *AP){
    apartments_t new_AP;
    int size = 5; // guessing there might be 5 matches
    new_AP.arr = malloc(size * sizeof(data_t));
    if(new_AP.arr == NULL){
        printf("Error allocating memory\n");
        exit(0);
    }

    char buf[NAMELEN];
    printf("Enter linnaosa name: ");
    scanf("%s", buf);
    if(strlen(buf) < 3){                // 3 seems like a good number, can also be 1
        printf("Cmon now >:(\n");
        return;
    }

    int i, j = 0;
    for(i = 0; i < AP->len; i++){
        if(!strcmp(buf, AP->arr[i].linnaosa)){  // seeing if we have a match
            if(j == size){                      // need more memory
                size *= 2;                      // increasing size 2x
                //printf("reallocating memory\n");
                data_t *p = realloc(new_AP.arr, size * sizeof(data_t));
                if(p == NULL){
                    printf("Error reallocating memory\n");
                    exit(0);
                }
                new_AP.arr = p;
            }
            new_AP.arr[j] = AP->arr[i];
            j++;
        }
    }
    if(j == 0){
        printf("There were no matches\n");
        return;
    }
    new_AP.len = j;
    qsort(new_AP.arr, j, sizeof(data_t), comp);
    for(i = 0; i < j; i++){
        printf("%3d. %20s \t%-30s %3hd %3.1f %hd/%hd %10d\n", 
                        i + 1, new_AP.arr[i].linnaosa, new_AP.arr[i].address, new_AP.arr[i].rooms, new_AP.arr[i].size, new_AP.arr[i].floor,
                        new_AP.arr[i].total_floors, new_AP.arr[i].price);
    }
    getchar();
    printf("Make an HTML file? (y/n) :)\n");
    char input;
    scanf("%c", &input);
    switch(input){
        case 'y':
            write_html(&new_AP);
            break;
        default:
            break;
    }
    free(new_AP.arr);
    return;
}

void helper_print(){
    printf( "1. Print average rooms / sizes\n"
            "2. Print linnaosa average prices and price per m2\n"
            "3. Search by linnaosa price ascending\n"
            "H. Help\n"
            "X. Exit\n");
}


int main(int argc, char *argv[]){
    apartments_t AP;

    int filename = 1; // maybe user specified file on commandline
    if(argc > 1){ // argument present, let's try it
        AP.f = fopen(argv[1], "r");
        if(AP.f == NULL)
            filename = 0; // nope
    } else
        filename = 0;
    char buf[100];
    if(filename == 0){
        printf("Please enter filename: ");
        scanf("%s", buf);
        AP.f = fopen(buf, "r");
        if(AP.f == NULL)
            exit(0); // user supplied one didn't work either, quit
    }
    read_data(&AP);
    printf("file read succeessfully\n");
    helper_print();
    int inloop = 1;
    char input;
    while(inloop){
        scanf("%c", &input);
        switch(input){
            case '1':
                averages(&AP);
                break;
            case '2':
                linnaosa_avg(&AP);
                break;
            case '3':
                linnaosa_asc(&AP);
                break;
            case 'H':
                helper_print();
                break;
            case 'X':
                inloop = 0;
                break;
        }
    }

    free(AP.arr);
    return 0;
}