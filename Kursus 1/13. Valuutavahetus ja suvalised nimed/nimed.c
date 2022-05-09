#include <stdio.h>
#include <stdlib.h>
#define NAMELEN 20

int loeandmed(int n, int sagedus[], char nimed[][NAMELEN], FILE *f){
	int i, k;
	for(i = 0; i < n; i++){
		if(fscanf(f, "%s %d", nimed[i], &k) < 2){
			break;
		}else{
			if(i == 0)
				sagedus[0] = k;
			else
				sagedus[i] = sagedus[i - 1] + k;
		}
	}
	return i;
}

// key on juhuslik arv vahemikus 0 kuni kõigi sageduste summa
int find(int n, int sagedus[], int key){
	int i;
	if(key < sagedus[0])
		return 0;
	for(i = 1; i < n; i++){
		if(key >= sagedus[i - 1] && key < sagedus[i]){
			return i;
		}
	}
	return 0; 
}

void gen(int sur, int sagedus_sur[], char surname[][NAMELEN], int fem, int sagedus_fem[], char female[][NAMELEN], int m, int sagedus_m[], char male[][NAMELEN], int bl, int sagedus_bl[], char black[][NAMELEN], int as, int sagedus_as[], char asian[][NAMELEN], int his, int sagedus_his[], char hispanic[][NAMELEN], int arv){
	int i;
	//ilus oleks võrrelda seda RAND_MAX-ga
	int maxval_sur = sagedus_sur[sur - 1]; // kõigi sageduste summa
	int maxval_fem = sagedus_fem[fem - 1]; 
	int maxval_m = sagedus_m[m - 1]; 
	int maxval_his = sagedus_his[his - 1]; 
	int maxval_as = sagedus_as[as - 1]; 
	int maxval_bl = sagedus_bl[bl - 1]; 

	int random_gend, random_race;
	for(i = 0; i < arv; i++){
		random_gend = rand() % 100 + 1;
		if(random_gend > 50){
			printf("Male: %s ", male[ find(m, sagedus_m, rand() % maxval_m) ]);
		}
		else{
			printf("Female: %s ", female[ find(fem, sagedus_fem, rand() % maxval_fem) ]);
		}
		random_race = rand() % 100 + 1;
		
		if(random_race <= 5){
			printf("%s :Asian\n", asian[ find(as, sagedus_as, rand() % maxval_as) ]);
		}
		else if(random_race <= 13){
			printf("%s :Black\n", black[ find(bl, sagedus_bl, rand() % maxval_bl) ]);
		}
		else if(random_race <= 17){
			printf("%s :Hispanic\n", hispanic[ find(his, sagedus_his, rand() % maxval_his) ]);
		}
		else{
			printf("%s :White\n", surname[ find(sur, sagedus_sur, rand() % maxval_sur) ]);
		}
	}
		
}

int main(void){
	FILE *f;
	int n_sur, n_fem, n_m, n_bl, n_as, n_his;
	f = fopen("white.txt", "r");
	if(f == NULL) // faili ei saanud avada
		return 0;
	if(fscanf(f, "%d", &n_sur) < 1) //pole kirjete arvu
		return 0;
	char surname[n_sur][NAMELEN];
	int sagedus_sur[n_sur];
	int sur = loeandmed(n_sur, sagedus_sur, surname, f);
	// sur palju tegelikult oli
	fclose(f);
	
	// loen eesnimed

	f = fopen("female.txt", "r");
	if(f == NULL) 
		return 0;
	if(fscanf(f, "%d", &n_fem) < 1) 
		return 0;
	char female[n_fem][NAMELEN];
	int sagedus_fem[n_fem];
	int fem = loeandmed(n_fem, sagedus_fem, female, f);
	// sur palju tegelikult oli
	fclose(f);

	f = fopen("male.txt", "r");
	if(f == NULL) 
		return 0;
	if(fscanf(f, "%d", &n_m) < 1) 
		return 0;
	char male[n_m][NAMELEN];
	int sagedus_m[n_m];
	int m = loeandmed(n_m, sagedus_m, male, f);
	// sur palju tegelikult oli
	fclose(f);
	
	f = fopen("black.txt", "r");
	if(f == NULL) 
		return 0;
	if(fscanf(f, "%d", &n_bl) < 1) 
		return 0;
	char black[n_bl][NAMELEN];
	int sagedus_bl[n_fem];
	int bl = loeandmed(n_bl, sagedus_bl, black, f);
	// sur palju tegelikult oli
	fclose(f);

	f = fopen("asian.txt", "r");
	if(f == NULL) 
		return 0;
	if(fscanf(f, "%d", &n_as) < 1) 
		return 0;
	char asian[n_as][NAMELEN];
	int sagedus_as[n_as];
	int as = loeandmed(n_as, sagedus_as, asian, f);
	// sur palju tegelikult oli
	fclose(f);

	f = fopen("hispanic.txt", "r");
	if(f == NULL) 
		return 0;
	if(fscanf(f, "%d", &n_his) < 1) 
		return 0;
	char hispanic[n_his][NAMELEN];
	int sagedus_his[n_his];
	int his = loeandmed(n_his, sagedus_his, hispanic, f);
	// sur palju tegelikult oli
	fclose(f);
	
	printf("Palju: ");
	int x;
	scanf("%d", &x);
	gen(sur, sagedus_sur, surname, fem, sagedus_fem, female, m, sagedus_m, male, bl, sagedus_bl, black, as, sagedus_as, asian, his, sagedus_his, hispanic, x);
	return 0;	
}
