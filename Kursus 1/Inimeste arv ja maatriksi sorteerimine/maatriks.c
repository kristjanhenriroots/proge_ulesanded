#include <stdio.h>
#include <stdlib.h>
#include <limits.h> //min ja max jaoks

int loe(const char *s);
void sort(int n, int mat[n][n], int idx[n]);
void tryki(int n, int mat[n][n], const char *s, int idx[n]);
void loemaatriks(int n, int mat[n][n]);

int main(void){
	int n; // elementide arv
	printf("Maatriksi sortimine\n");
	n = loe("Ridade arv");
	if(n < 2){
		printf("Liiga vähe!\n");
		return 0;
	}
	int mat[n][n];
	int idx[n];

	loemaatriks(n, mat);
	sort(n, mat, idx);
	tryki(n, mat,"\n\nUus järjekord:\n", idx);
	
	return 0;
}
int loe(const char *s){
	int x;
	if(scanf("%d", &x) < 1)
		exit(0);
	return x;
}
void loemaatriks(int n, int mat[n][n]){
	int i, j;
	for(i = 0; i < n; i++){	
		for(j = 0; j < n; j++){
			scanf("%d", &mat[i][j]);
		}	
	}
}

void sort(int n, int mat[n][n], int idx[n]){
	int vahemik[n], i, j, k;
	int max, min;
	//Loon vahemike massiivi
	for(i = 0; i < n; i++){
		max = INT_MIN;
		min = INT_MAX;
		for(j = 0; j < n; j++){
			if(mat[j][i] < min){
				min = mat[j][i];
			}
			if(mat[j][i] > max){
				max = mat[j][i];
			}
		}
		vahemik[i] = max - min;
	} 
	
	for(i = 0; i < n; i++){
		idx[i] = i; // algväärtustamine
	}
	//Indeksite sorteerimine vahemike kahanemise järjekorras
	for(i = 0; i < n; i++){
		for(j = i + 1; j < n; j++){
			if(vahemik[idx[i]] < vahemik[idx[j]]){
				k = idx[i];
				idx[i] = idx[j];
				idx[j] = k;
			} 
		}
	}
	
	printf("\nVahemikud kahanemise järjekorras\n");
	for(j = 0; j < n; j++){
			printf("%6d", vahemik[idx[j]]);
	}
}

void tryki(int n, int mat[n][n], const char *s, int idx[n]){
	printf(s);
	int i, j;
	for(i = 0; i < n; i++){	
		for(j = 0; j < n; j++){
			printf("%6d", mat[i][idx[j]]);
		}
		printf("\n");	
	}
}