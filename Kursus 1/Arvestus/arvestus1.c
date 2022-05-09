#include <stdio.h>
#include <stdlib.h>
#define MAX 15 //max failinime pikkus



int kontroll(int N, int min){
    if(N < min)
        return 0;
    else 
        return 1;
}

void algvaartus(int n, int sum[3][n]){
    int i, j;
    for(i = 0; i < 3; i++){
        for(j = 0; j < n; j++){
            sum[i][j] = 0;
        }
    }
}

void find(int n, int maatriks[n][n], int sum[4][n]){
    int x, y;
    for(x = 0; x < n; x++){
        for(y = 0; y < n; y++){
            if(maatriks[x][y] == maatriks[y][x]){
                sum[0][x] += 1; // mitu on samad
                sum[1][x] += maatriks[x][y]; // peab summale arvet
                sum[2][x] = x; // indeks
            }
        }
    }
}

void find_diag(int n, int maatriks[n][n], int sum[2]){
    int i, x, y = 0;
    for(x = 0; x < n; x++){  
        if(maatriks[x][y] == maatriks[n - x][y]){
            sum[0] += 1; // mitu on samad
            sum[1] += maatriks[x][y]; // peab summale arvet
            i++;
        }
        y++;
    }
}

//Sain alguses valesti aru, et samade elementide summat on vaja ja sellega võrrelda
//sp on see ka leitud
void print(int n, int sum[3][n], int sum_diag[2], int X){
    int i;
    printf("Veerud ja read:\n");
    for(i = 0; i < n; i++){
        if(sum[0][i] > X){
            printf("i = %d, %d elementi on samad, summa on %d\n", sum[2][i], sum[0][i], sum[1][i]);
        }
    }
    printf("\n\nDiagonaalid:\n");
    for(i = 0; i < n; i++){
        if(sum_diag[0] > X){
            printf("%d elementi on samad\n", sum_diag[1]);
        }
    }

}




int main(void){

    FILE *sisend;
    char buf[200];
	printf("Sisesta faili nimi: ");
	scanf("%s", buf);
	sisend = fopen(buf, "r");
		if(sisend == NULL)
			return 0;
    
    
    
    int n;
    fscanf(sisend, "%d", &n);
    if(kontroll(n, 3) == 0){
        printf("Maatriks peab olema vähemalt 3x3");
        return 0;
    }
    int maatriks[n][n];

    int i, j;
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            fscanf(sisend, "%d", &maatriks[i][j]);
        }
    }

	fclose(sisend); //vabastame ressursi
    int X;
    printf("Sisestage väärtus X: ");
    scanf("%d", &X);
    if(kontroll(X, 1) == 0){
        printf("X peab olema suurem nullist\n");
        return 0;
    }
    
    for(i = 0; i < n; i++){
        for(j = 0; j < n; j++){
            printf("%d ", maatriks[i][j]);
        }
        printf("\n");
    }
    
    int sum[3][n];
    int sum_diag[2] = {0, 0};
    algvaartus(n, sum);
    find(n, maatriks, sum);
    find_diag(n, maatriks, sum_diag);
    print(n, sum, sum_diag, X);



    return 0;
}