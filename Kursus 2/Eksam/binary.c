#include <stdio.h>
#include <stdlib.h>

typedef struct data{
    long long int num; // just in case the numbers get large
} data_t;

typedef struct fibonaccy{
    FILE *f;
    int len;
    data_t *arr;
} fibonaccy_t;

int read_data(fibonaccy_t *F){
    F->len = 1;
    int size = 10;                // guessing the file is 10 rows
    F->arr = malloc(size * sizeof(data_t));

    int i = 0;
    while(fscanf(F->f, "%lld", &F->arr[i].num) > 0){
        i++;
        if(i == size){
            size *= 2;          // increasing size 2x
            //printf("reallocating memory\n");
            data_t *p = realloc(F->arr, size * sizeof(data_t));
            if(p == NULL){
                printf("Error reallocating memory\n");
                exit(0);
            }
            F->arr = p;
        }
    }
    printf("read lines %d\n", i);
    F->len = i;
    /*
    for(int j = 0; j < i; j++){
        printf("%lld\n", F->arr[j].num);
    }
    */
    fclose(F->f);
    return 0;
}

int write_bin(fibonaccy_t *F){
    F->f = fopen("fibonaccy.bin", "wb");
    if(F->f == NULL){
        printf("Error opening binary file\n");
        exit(0);
    }
    fwrite(F->arr, sizeof(data_t) * F->len, 1, F->f);
    fclose(F->f);
    return 0;
}

void binary_check(fibonaccy_t *F){          // quick check function that reads in the same file and displays it
    F->f = fopen("fibonaccy.bin", "rb");    // verified that the file works

    int size = 10;                // guessing the file is 10 rows
    F->arr = malloc(size * sizeof(data_t)); // MAXROWS = 100000
    int i = 0;

    while(fread(&F->arr[i], sizeof(data_t), 1, F->f)){
        i++;
        if(i == size){
            size *= 2;          // increasing size 2x
            printf("reallocating memory\n");
            data_t *p = realloc(F->arr, size * sizeof(data_t));
            if(p == NULL){
                printf("Error reallocating memory\n");
                exit(0);
            }
            F->arr = p;
        }
    }
    for(int j = 0; j < i; j++){
        printf("%lld\n", F->arr[j].num);
    }
    return;
}

int main(int argc, char *argv[]){
    fibonaccy_t F;
    int filename = 1; // maybe user specified file on commandline
    if(argc > 1){ // argument present, let's try it
        F.f = fopen(argv[1], "r");
        if(F.f == NULL)
            filename = 0; // nope
    } else
        filename = 0;
    char buf[100];
    if(filename == 0){
        printf("Please enter filename: ");
        scanf("%s", buf);
        F.f = fopen(buf, "r");
        if(F.f == NULL)
            exit(0); // user supplied one didn't work either, quit
    }

    read_data(&F);
    write_bin(&F);
    free(F.arr);

    //binary_check(&F);
    //free(F.arr);
    return 0;
}