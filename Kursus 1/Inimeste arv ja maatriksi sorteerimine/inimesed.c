#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TEXTLEN 6
int loe(const char *s);
void sort(int n, int inimesed[], int idx[]);
void tryki(int n, int inimesed[], char kell[][TEXTLEN], const char *s, int idx[]);
void loemassiivid(int n, int inimesed[], char kell[][TEXTLEN]);
int main(void){
	int n; // elementide arv
	printf("Inimeste arv kahanevas järjekorras\n");
	n = loe("Ridade arv");
	if(n < 2){
		printf("Liiga vähe!\n");
		return 0;
	}
	int inimesed[n];
	char kell[n][TEXTLEN];
	int idx[n];
	loemassiivid(n, inimesed, kell);
	sort(n, inimesed, idx);
	tryki(n, inimesed, kell, "uus järjekord:\n", idx);
	return 0;
}
int loe(const char *s){
	int x;
	if(scanf("%d", &x) < 1)
		exit(0);
	return x;
}
void loemassiivid(int n, int inimesed[], char kell[][TEXTLEN]){
	int i;
	for(i = 0; i < n; i++){		
		if(scanf("%s %d", kell[i], &inimesed[i])< 2)
			exit(0);
	}
}
void sort(int n, int inimesed[], int idx[]){
	int i, j, k;
	for(i = 0; i < n; i++)
		idx[i] = i; // algväärtustamine
	for(j = n - 1; j > 0; j--)
		for(i = 0; i < j; i++)
			if(inimesed[idx[i]] < inimesed[idx[j]]){ // kahanev järjekord
				k = idx[i];
				idx[i] = idx[j];
				idx[j] = k;
			}
}

void tryki(int n, int inimesed[], char kell[][TEXTLEN], const char *s, int idx[]){
	printf(s);
	for(int i = 0; i < n; i++)
		printf("%2d  %s\n", inimesed[idx[i]], kell[idx[i]]);
}