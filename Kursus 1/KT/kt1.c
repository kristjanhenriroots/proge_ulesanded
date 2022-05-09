#include <stdio.h>
#include <math.h>

int ValidateTime(int hh , int mm , int ss)
{
    int ret=0;
    
    if(hh>24)   ret=1;
    if(mm>60)   ret=1;
    if(ss>60)   ret=1;
    
    return ret;
}



//ei tööta
int main(void){
    int N, i, j, k;
    int ret=0,hour=0,min=0,sec=0;
    printf("Sisestage väärtus N: ");
    scanf("%d", &N);
    char arr[N][20];
    if(N <= 0){
        return 0;
    }
    printf("Sisestage kellaajad: ");
    for(i = 0; i < N; i++){
        scanf("%s", &arr[i][20]);
        //printf("%s", arr[i][20]);
        getchar();
    }
    //sscanf(arr[1][20] , "%d:%d:%d" , arr[i][0], arr[i][1], arr[i][2]);

    
    for(j = 1; j <= N; j++){
        
        for(k = 0; k < 8; k++){
            while(arr[N][k] != '\0'){...}
        }
    }
    
    return 0;
}