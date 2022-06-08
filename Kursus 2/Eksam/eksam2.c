#include <stdio.h>
#include <stdlib.h>

typedef struct numbers{
    long int num;
} number_t;

typedef struct num_data{
    int rows;
    number_t *x;
} numbers_t;

void read_binary(FILE *f, numbers_t *N){
    int count = 5, i = 0;                
    N->x = malloc(count * sizeof(number_t));

    while(fread(&N->x[i], sizeof(number_t), 1, f)){
        i++;
        if(i == count){
            count = count * 2;         
            number_t *p = realloc(N->x, count * sizeof(number_t));
            if(p == NULL)
                exit(0);
            N->x = p;
        }
    }
    if(i == 0){ // file empty
        exit(0);
    }
    N->rows = i;
    fclose(f);
}

void write_text(numbers_t *N){
    FILE *f;
    f = fopen("int.txt", "w");
    if(f == NULL){
        printf("Failed to open file\n");
        exit(0);
    }

    for(int i = 0; i < N->rows; i++)
        fprintf(f, "%ld\n", N->x[i].num);
    fclose(f);
}

int main(int argc, char **argv){
    numbers_t N;
    FILE *f;
    if(argc > 1){
        f = fopen(argv[1], "rb");
        if(f == NULL){
            printf("Failed to open file\n");
            return 0;
        }
    }
    else
        return 0;

    read_binary(f, &N);
    write_text(&N);
    free(N.x);
    return 0;
}

