#include <stdio.h>

int main(void){

    int N, row, col, x, y;

    printf("Korrutustabel\n");
    printf("Sisestage suurus N: \n");
    scanf("%d", &N);

    //Teen maatriksi
    int kt[N][N];
    
    //Kontrollin kas sisestatud arv on suurem kui 0
    if(N <= 1){
        printf("Sisestage ühest suurem arv!\n");
        return 0;
    }
    
    //Jätan tühja rea enne maatriksit
    printf("\n");
    
    //Maatriksi tegemine
    for(x = 0; x < N; x++){
        for(y = 0; y < N; y++){
			kt[x][y] = (y + 1) * (x + 1);
		}
    }
    
    //Maatriksi printimine
    for(row = 0; row < N; row++){
        for(col = 0; col < N; col++){
            printf("%d     ", kt[row][col]);
        }
        printf("\n");
     }
    return 0;
}