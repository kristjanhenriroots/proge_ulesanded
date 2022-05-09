#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void gen(int n, int *M, int algus, int lopp){
	int i, j;
	for(i = 0; i < n; i++){
		M[i] = algus + rand() % (lopp - algus + 1);
        if(abs(algus + lopp) > n){
            for(j = 0; j < i; j++){
                if(M[j] == M[i]){
                    i--;
                }
            } 
        }
    }
}

void tryki(int n, int *M){
	int i;
	for(i = 0; i < n; i++)
		printf("%d\n", M[i]);
}

int main(void){
	int n, algus, lopp;
	printf("Mitu arvu: ");
	scanf("%d", &n);
	
	if (n < 2)
		return 0;
	
    int M[n];
    printf("Vahemik, nt 1-3: ");
    scanf("%d-%d", &algus, &lopp);

    if(lopp <= algus){
        return 0;
    }
	
    srand(time(NULL));
	gen(n, M, algus, lopp);
	tryki(n, M);
	
    return 0;
}