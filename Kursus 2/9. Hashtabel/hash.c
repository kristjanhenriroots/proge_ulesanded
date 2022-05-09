#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BILLION 1000000000.0 // for time calculation
#define NAMELEN 80
#define HASHCOEFF 3.4   // even though the conflict count is high it is still faster with a lower value
#define MULT 43         // however weird mistakes come in with values lower than 2

typedef struct names{
    char name[NAMELEN]; // firstname + lastname
} data_t;

typedef struct hash{ // tunnikoodist
	data_t *e;
	struct hash *next; // linked list
} hash_t;

typedef struct name_arr{
    int len;                // estimated size
    int true_size;          // true size
    int nhash;      
    int conflicts;
    data_t *arr;
    hash_t *hash;
} name_arr_t;

void dataCheck(int min, int max, int in){ // data validation
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
        if(i == names->len){ // need more memory
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
        dataCheck(y_min, y_max, year);
        dataCheck(dm_min, m_max, month); // checking input dates
        dataCheck(dm_min, months[month], day);
        all_dates[year - y_min][month][day] += 1; // hõretabelid
        date_frequency[month][day] += 1;
        i++;
    }
    names->true_size = i; // how many elements are actually in the array
}

unsigned int hash(char *s){ // hash calculation
	int i;
	unsigned int h = 0;
	for(i = 0; i < strlen(s); i++)
		h = h * MULT + (s[i] - ' ');
	return h;
}

void createhash(name_arr_t *E){ // hash creation
	E->nhash = E->true_size * HASHCOEFF;
	E->hash = calloc(E->nhash, sizeof(hash_t));
	E->conflicts = 0;
	int i, x;
	for(i = 0; i < E->len; i++){
		x = hash(E->arr[i].name) % E->nhash;
		if(E->hash[x].e == NULL){
			E->hash[x].e = &E->arr[i];
			E->hash[x].next = NULL;
		}else{
			hash_t *hh = malloc(sizeof(hash_t));
			hh->next = E->hash[x].next;
			hh->e = &E->arr[i];
			E->hash[x].next = hh;
			E->conflicts++;
		}
	}	
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

void minmax(unsigned short date_frequency[13][32], unsigned short months[13]){
    int min_val = 9999, max_val = 0;
    int month, day;
    for(month = 1; month <= 12; month++){ // finding min and max values
        for(day = 1; day <= months[month]; day++){
            if(date_frequency[month][day] < min_val)
                min_val = date_frequency[month][day];
            if(date_frequency[month][day] > max_val)
                max_val = date_frequency[month][day];
        }
    }
    printf("Most birthdays (%hd) were on:\n", max_val); // going through again and printing incase there are multiple
    for(month = 1; month <= 12; month++){
        for(day = 1; day <= months[month]; day++){
            if(max_val == date_frequency[month][day]){
                printDate(day);
                printf(".");
                printDate(month);
                printf("\n");
            }
        }
    }

    printf("Least birthdays (%hd) were on:\n", min_val);
    for(month = 1; month <= 12; month++){
        for(day = 1; day <= months[month]; day++){
            if(min_val == date_frequency[month][day]){
                printDate(day);
                printf(".");
                printDate(month);
                printf("\n");
            }
        }
    }

}

int leapCheck(int year){ // leap year check
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        return 1;
    return 0;
}

void days_without_births(short all_dates[101][13][32], unsigned short months[13]){
    int year, month, day, daycount = 0;
    for(year = 0; year < 100; year++){
        for(month = 1; month <= 12; month++){
            for(day = 1; day < months[month]; day++){
                if(month == 2 && day == 29){ // leap year check
                    if(leapCheck(year) == 0) // if its not a leap year the 29th is skipped
                        break;

                }
                if(all_dates[year][month][day] < 1) // found a day without births
                    daycount++;
            }
        }
    }
    printf("Days without births between 1900 and 1999: %d\n", daycount);
}

int namesort(const void* a, const void* b){ // qsort alpabetical order
    const char *ia = (const char *)a;
    const char *ib = (const char *)b;
    return strcmp(ia, ib);
}

// finding most frequent name with qsort and hash
void most_frequent_name_qsort(name_arr_t *names){
    int i, maxval = 1, tempval = 0, j, index;
    for(i = 0; i < names->true_size; i++){
        if(strcmp(names->arr[i].name, names->arr[i + 1].name) == 0){ // go through and check how many in a row are the same
            j = i + 1;
            while(strcmp(names->arr[i].name, names->arr[j].name) == 0){
                j++;
            }
            tempval = j - i;
        }
        if(tempval > maxval){ // new record?
            index = i;
            maxval = tempval;
        }
        tempval = 0;
    }
    printf("Found most common name: %s : present %d times\n", names->arr[index].name, maxval);
}

void most_frequent_name_hash(name_arr_t *names){ // using hash
    int i, maxval = 1, tempval = 1;
    char namebuf[NAMELEN];
    hash_t *hh;
    printf("Conflict count %d\n", names->conflicts);
    for(i = 0; i < names->nhash; i++){
        if(names->hash[i].e != NULL && names->hash[i].next != NULL){
            hh = &names->hash[i];
            hh = hh->next; // compare it with the next one until there aren't any more and check if they're the same
            // broken without strlen checking?
            while(hh != NULL && strlen(hh->e->name) > 0 && strcmp(names->hash[i].e->name, hh->e->name) == 0){
                tempval++;
                hh = hh->next;
            }
            
        }
        if(tempval > maxval){
            maxval = tempval;
            strcpy(namebuf, names->hash[i].e->name);
        }
        tempval = 1;
    }
    printf("Found most common name: %s : present %d times\n", namebuf, maxval);
}


int main(int argc, char *argv[]){
    struct timespec data_start, data_end, calc_start, calc_end, total_start, total_end; // timing the program

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
    int input;
    enum userin{qsort_ = 1, hash_table = 2};
    printf("1: qsort\n2: hash table\n");
    scanf("%d", &input);
    if(input != qsort_ && input != hash_table)
        exit(0);
    clock_gettime(CLOCK_REALTIME, &total_start); // start timer
    name_arr_t names;
    names.len = 600;
    names.arr = malloc(names.len * sizeof(data_t)); // guessing that the file is names.len rows, reallocating if exceeded

    short all_dates[101][13][32]; // hõretabel
    unsigned short date_frequency[13][32]; // keeping birthday frequency in a LUT
    unsigned short months[13] = {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    clock_gettime(CLOCK_REALTIME, &data_start); // start timer
    readData(f, &names, all_dates, date_frequency, months);
    clock_gettime(CLOCK_REALTIME, &data_end); // stop timer
    
    clock_gettime(CLOCK_REALTIME, &calc_start); // start timer
    minmax(date_frequency, months);
    days_without_births(all_dates, months);
    switch(input){
        case qsort_:
            qsort(names.arr, names.true_size, sizeof(names.arr[0].name), namesort);
            most_frequent_name_qsort(&names);
            break;
        case hash_table:
            createhash(&names);
            most_frequent_name_hash(&names);
            break;
    }

    clock_gettime(CLOCK_REALTIME, &calc_end); // stop timer

    clock_gettime(CLOCK_REALTIME, &total_end); // stop timer

    double total_time_spent = (total_end.tv_sec - total_start.tv_sec) + (total_end.tv_nsec - total_start.tv_nsec) / BILLION;
    double data_time_spent = (data_end.tv_sec - data_start.tv_sec) + (data_end.tv_nsec - data_start.tv_nsec) / BILLION;
    double calc_time_spent = (calc_end.tv_sec - calc_start.tv_sec) + (calc_end.tv_nsec - calc_start.tv_nsec) / BILLION;

    printf("\nTime spent reading data: %.4f ms\n", data_time_spent * 1000);
    printf("Time spent calculating data: %.4f ms\n", calc_time_spent * 1000);
    printf("Total program time: %.4f ms\n", total_time_spent * 1000);
    free(names.arr);
    if(input == hash_table)
        free(names.hash);

    return 0;
}