#include <stdio.h>
#include <string.h>

#define NAMELEN 30
#define MAXROWS 100
#define MAXDATA 20

typedef struct measureData{ // too nested, but easier to sort
    short time[2];
    float temp;
}measure_t;

typedef struct cityData{
    char city[NAMELEN];
    short date[3];
    short datacount;
    measure_t measure[MAXDATA];
}city_t;

typedef struct dataArr{
    short len;
    city_t arr[MAXROWS];
}arr_t;

int findCity(char *name, arr_t *data){
    for(int i = 0; i < data->len; i++){
        if(strcmp(name, data->arr[i].city) == 0)
            return i;
    }
    return -1;
}

int compare(const void * a, const void * b){
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}


int main(void){
    arr_t data;
    FILE *f = fopen("test.txt", "r");
	if (f == NULL) {
		return 0;
	}
    int i, j;
    fscanf(f, "%hd", &data.len);
    for(i = 0; i < data.len; i++){
        fscanf(f, "%s %hd.%hd.%hd %hd", data.arr[i].city, &data.arr[i].date[0], &data.arr[i].date[1], &data.arr[i].date[2], &data.arr[i].datacount);
        for(j = 0; j < data.arr[i].datacount; j++){
            fscanf(f, "%hd:%hd %f", &data.arr[i].measure[j].time[0], &data.arr[i].measure[j].time[1], &data.arr[i].measure[j].temp);
        }
    }

    char cityName[NAMELEN];
    printf("Enter city name: ");
    scanf("%s", cityName);
    int check = findCity(cityName, &data);
    if(check < 0){
        printf("City not found\n");
        return 0;
    }
    measure_t a;
    for(i = 0; i < data.arr[check].datacount; ++i){ // ei saanud qsorti tööle õigeaegselt
        for(j = i + 1; j < data.arr[check].datacount; ++j){
            if(data.arr[check].measure[i].temp < data.arr[check].measure[j].temp){
                a = data.arr[check].measure[i];
                data.arr[check].measure[i] = data.arr[check].measure[j];
                data.arr[check].measure[j] = a;

            }

        }

    }
    fclose(f);
    


    FILE *html = fopen("html.html", "w"); // html fail peaks brauseris avanema ka
	if(html == NULL) {
		return 0;
	}
    fprintf(html, "<!DOCTYPE html>\n<html>\n<body><h1>%s</h1><table>", data.arr[check].city);
    for(i = 0; i < data.arr[check].datacount; i++)
        fprintf(html, "<tr><th>%2hd.%2hd.%2hd</th><th>%2hd:%2hd</th><th>%.1fC</th></tr>\n", data.arr[check].date[0], data.arr[check].date[1], data.arr[check].date[2], data.arr[check].measure[i].time[0], data.arr[check].measure[i].time[1], data.arr[check].measure[i].temp);

    fprintf(html, "</table>\n</body>\n</html>\n");

    fclose(html);


    return 0;
}