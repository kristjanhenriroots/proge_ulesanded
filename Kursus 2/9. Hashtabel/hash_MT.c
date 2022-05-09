#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>


#define BILLION 1000000000.0
#define NAMELEN 80


//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // can lock the data to one core, fixes race scenario, slow...

typedef struct names{
    char name[NAMELEN];
} data_t;

typedef struct hash{
	data_t *e;
	struct hash *next; // linked list
} hash_t;

typedef struct name_arr{
    int len;
    int true_size;
    int nhash;
    int conflicts;
    data_t *arr;
    hash_t *hash;
} name_arr_t;

typedef struct namekeep{
    short all_dates[101][13][32]; // need different structs, all threads need void pointers and its easier to typecast structs
} namekeep_t;

typedef struct frequency{
    unsigned short date_frequency[13][32]; // keeping birthday frequency in a LUT
} date_fr_t;

void dataCheck(int min, int max, int in){
    if(in < min || in > max){
        printf("Error with limits in data\n");
        exit(0);
    }
}

void readData(FILE *f, name_arr_t *names, short all_dates[101][13][32], unsigned short date_frequency[13][32], unsigned short months[13]){


    char lastname[NAMELEN];
    unsigned short day, month, year;
    int i = 0;

    enum limits{y_min = 1900, y_max = 1999, dm_min = 1, d_max = 31, m_max = 12};
    while(fscanf(f, "%s %s %*s %hd-%hd-%hd", names->arr[i].name, lastname, &year, &month, &day) > 3){
        if(i == names->len){
            names->len *= 2;
            data_t *p = realloc(names->arr, names->len * sizeof(data_t));
            if(p == NULL){
                printf("Error reallocating memory to namelist\n");
                exit(0);
            }
            names->arr = p;
        }
        strcat(names->arr[i].name, " "); 
        strcat(names->arr[i].name, lastname); // copying the name to one variable
        //printf("%d: %s %hd.%hd.%hd\n", i, names->arr[i].name, year, month, day);
        dataCheck(y_min, y_max, year);
        dataCheck(dm_min, m_max, month); // checking input dates
        dataCheck(dm_min, months[month], day);
        all_dates[year - y_min][month][day] += 1;
        date_frequency[month][day] += 1;
        i++;
    }
    names->true_size = i; // how many elements are actually in the array
    /*
    for(i = 1; i <= 12; i++){
        printf("%2d: ", i);
        for(int j = 1; j <= months[i]; j++)
            printf(" %4hd |", date_frequency[i][j]);
        printf("\n");
    }
    for(i = 0; i < 10; i++)
        printf("%s\n", names->arr[i].name);
    printf("true size is %d\n", names->true_size);
    */

}


void printDate(int in){ // indexes are dates, so printing directly would not allow dates like 01.01 so its converting them to char
    char out[2] = "0", nr, nrcheat[2];
    if(in < 10){
        nr = in + '0';
        nrcheat[0] = nr;
        strcat(out, nrcheat);
        printf("%s", out); 

    }
    else{
        printf("%2d", in); 
    }
}

void* minmax(void *arg){
    date_fr_t *fr = (date_fr_t *)arg; 
    unsigned short months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int min_val = 9999, max_val = 0;
    int month, day;
    for(month = 1; month <= 12; month++){
        for(day = 1; day <= months[month]; day++){
            if(fr->date_frequency[month][day] < min_val)
                min_val = fr->date_frequency[month][day];
            if(fr->date_frequency[month][day] > max_val)
                max_val = fr->date_frequency[month][day];
        }
    }
    printf("Most birthdays (%hd) were on:\n", max_val);
    for(month = 1; month <= 12; month++){
        for(day = 1; day <= months[month]; day++){
            if(max_val == fr->date_frequency[month][day]){
                printf("%d.%d\n", day, month);
                /*
                printDate(day);
                printf(".");
                printDate(month); // fancy printing broke
                printf("\n");
                */
            }
        }
    }

    printf("Least birthdays (%hd) were on:\n", min_val);
    for(month = 1; month <= 12; month++){
        for(day = 1; day <= months[month]; day++){
            if(min_val == fr->date_frequency[month][day]){
                printf("%d.%d\n", day, month);
                /*
                printDate(day);
                printf(".");
                printDate(month);
                printf("\n");
                */
            }
        }
    }
    return 0;
}

int leapCheck(int year){
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

void* days_without_births(void * arg){
    namekeep_t *namekeep = (namekeep_t *)arg;
    unsigned short months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int year, month, day, daycount = 0;
    for(year = 0; year < 100; year++){
        //pthread_mutex_lock(&lock);
        for(month = 1; month <= 12; month++){
            for(day = 1; day < months[month]; day++){
                if(month == 2 && day == 29){ // leap year check
                    if(leapCheck(year) == 0)
                        break;

                }
                if(namekeep->all_dates[year][month][day] < 1)
                    daycount++;
            }
        }
        //pthread_mutex_unlock(&lock);
    }
    printf("Days without births between 1900 and 1999: %d\n", daycount);
    return 0;
}

int namesort(const void* a, const void* b){ // qsort alpabetical order
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return strcmp(ia, ib);
}

void* most_frequent_name_qsort(void *arg){
    name_arr_t *names = (name_arr_t *) arg;
    qsort(names->arr, names->true_size, sizeof(names->arr[0].name), namesort);
    int i, maxval = 1, tempval = 0, j, index;
    for(i = 0; i < names->true_size; i++){
        if(strcmp(names->arr[i].name, names->arr[i + 1].name) == 0){
            j = i + 1;
            while(strcmp(names->arr[i].name, names->arr[j].name) == 0){
                j++;
            }
            tempval = j - i;
        }
        if(tempval > maxval){
            index = i;
            maxval = tempval;
        }
        tempval = 0;
    }
    printf("Found most common name: %s : present %d times\n", names->arr[index].name, maxval);
    return NULL;
}


int main(int argc, char *argv[]){
    struct timespec data_start, data_end, calc_start, calc_end, total_start, total_end;

    int filename = 1; // maybe user specified file on commandline
    FILE *f;
    if(argc > 1){ // argument present, let's try it
        f = fopen(argv[1], "r");
        if(f == NULL)
            filename = 0; // nope
    } else
        filename = 0;
    char buf[100];
    if(filename == 0){
        printf("Please enter filename: ");
        scanf("%s", buf);
        f = fopen(buf, "r");
        if(f == NULL)
            exit(0); // user supplied one didn't work either, quit
    }
    clock_gettime(CLOCK_REALTIME, &total_start); // start timer
    pthread_t Thread1, Thread2, Thread3;
    name_arr_t names;
    names.len = 600;
    names.arr = malloc(names.len * sizeof(data_t)); // guessing that the file is names.len rows, reallocating if exceeded

    namekeep_t all_dates;
    date_fr_t fr;
    unsigned short months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    clock_gettime(CLOCK_REALTIME, &data_start); // start timer
    readData(f, &names, all_dates.all_dates, fr.date_frequency, months);
    clock_gettime(CLOCK_REALTIME, &data_end); // stop timer
    
    clock_gettime(CLOCK_REALTIME, &calc_start); // start timer
    pthread_create(&Thread1, NULL, days_without_births, &all_dates);
    //pthread_join(Thread1, NULL);

    pthread_create(&Thread2, NULL, most_frequent_name_qsort, &names);
    //pthread_join(Thread2, NULL);

    pthread_create(&Thread3, NULL, minmax, &fr);


    //pthread_join(Thread1, NULL);
    //pthread_join(Thread2, NULL);
    pthread_join(Thread2, NULL); // wait for the longest process before stopping times and exiting


    //pthread_join(Thread2, NULL);

    clock_gettime(CLOCK_REALTIME, &calc_end); // stop timer

    clock_gettime(CLOCK_REALTIME, &total_end); // stop timer

    double total_time_spent = (total_end.tv_sec - total_start.tv_sec) + (total_end.tv_nsec - total_start.tv_nsec) / BILLION;
    double data_time_spent = (data_end.tv_sec - data_start.tv_sec) + (data_end.tv_nsec - data_start.tv_nsec) / BILLION;
    double calc_time_spent = (calc_end.tv_sec - calc_start.tv_sec) + (calc_end.tv_nsec - calc_start.tv_nsec) / BILLION;

    printf("\nTime spent reading data: %.4f ms\n", data_time_spent * 1000);
    printf("Time spent calculating data: %.4f ms\n", calc_time_spent * 1000);
    printf("Total program time: %.4f ms\n", total_time_spent * 1000);


    return 0;
}