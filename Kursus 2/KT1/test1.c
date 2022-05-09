#include <stdio.h>

/* 
    asjad mis peaksid paremini olema:
    kuupäevad char mitte short, pärast printimisel on kole muidu
    hetkel on võimalik 1 kuupäev per linn, kuid mitu mõõtmistulemust
    binaarfaili asemel on kasutusel tavaline txt aja puuduse tõttu
*/
#define NAMELEN 30
#define MAXROWS 100
#define MAXDATA 20

typedef struct cityData{
    char city[NAMELEN];
    short date[3];
    short datacount;
    short measureData[MAXDATA][3];
}city_t;

typedef struct dataArr{
    short len;
    city_t arr[MAXROWS];
}arr_t;

int checkData(arr_t *data, int i, int mode, int j){
    if(mode == 1){
        if(data->arr[i].date[0] < 0 || data->arr[i].date[0] > 31)
            return 0;
        if(data->arr[i].date[1] < 0 || data->arr[i].date[1] > 12)
            return 0;
        if(data->arr[i].date[2] < 0 || data->arr[i].date[2] > 50000) // using short, being wary of limits
            return 0;
        if(data->arr[i].datacount < 0 || data->arr[i].datacount > MAXDATA)
            return 0;
        return 1;
    }
    else{
        if(data->arr[i].measureData[j][0] < 0 || data->arr[i].measureData[j][0] > 23)
            return 0;
        if(data->arr[i].measureData[j][1] < 0 || data->arr[i].measureData[j][1] > 59)
            return 0;
        if(data->arr[i].measureData[j][3] < -500000 || data->arr[i].measureData[j][3] > 50000)
            return 0;
        return 1;
    }
    
}

int main(void){
    arr_t data;
    printf("How many cities? : ");
    scanf("%hd", &data.len);
    if(data.len > 100 || data.len < 1){
        printf("Unsupported size, must be between 1 - 100\n");
        return 0;
    }
    int i = 0, j, check;
    printf("Insert data in format\n");
    printf("Cityname dd.mm.yyyy measurecount (cannot exceed 20)\n");
    printf("Then use format for measure data as many times as the input measurecount\n");
    printf("hh:mm temp\n\n");

    while(i < data.len){
        scanf("%s %hd.%hd.%hd %hd", data.arr[i].city, &data.arr[i].date[0], &data.arr[i].date[1], &data.arr[i].date[2], &data.arr[i].datacount);
        check = checkData(&data, i, 1, 0);
        if(check == 0){
            printf("Data incorrect\n");
            return 0;
        }
        for(j = 0; j < data.arr[i].datacount; j++){
            scanf("%hd:%hd %hd", &data.arr[i].measureData[j][0], &data.arr[i].measureData[j][1], &data.arr[i].measureData[j][2]);
            
            check = checkData(&data, i, 2, j);
            if(check == 0){
                printf("Data incorrect\n");
                return 0;
            }

        }
        i++;
    }

    printf("data read\n\n\n\n");

    /*for(i = 0; i < data.len; i++){
        printf("%s %hd.%hd.%hd %hd\n", data.arr[i].city, data.arr[i].date[0], data.arr[i].date[1], data.arr[i].date[2], data.arr[i].datacount);
        for(j = 0; j < data.arr[i].datacount; j++)
            printf("%hd:%hd %hd\n", data.arr[i].measureData[j][0], data.arr[i].measureData[j][1], data.arr[i].measureData[j][2]);
        printf("\n");
    }*/

    FILE *f;
    f = fopen("test.txt", "w");
    if(f == NULL){
        printf("Couldn't open file\n");
        return 0;
    }
    fprintf(f, "%d\n", data.len);
    for(i = 0; i < data.len; i++){
        fprintf(f, "%s %hd.%hd.%hd %hd\n", data.arr[i].city, data.arr[i].date[0], data.arr[i].date[1], data.arr[i].date[2], data.arr[i].datacount);
        for(j = 0; j < data.arr[i].datacount; j++)
            fprintf(f, "%hd:%hd %hd\n", data.arr[i].measureData[j][0], data.arr[i].measureData[j][1], data.arr[i].measureData[j][2]);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}