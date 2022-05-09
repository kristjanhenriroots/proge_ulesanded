#include <stdio.h>
#include <string.h>

#define MAXRIDA 40
#define CUR 4

void ask(int n, char valuuta[][CUR], char wants[], double needed[]);
int kontroll(int n, char valuuta[][CUR], char wants[]);
double math(double x, double amount);
void print(double finalCost, char wants[], double amount);

int main(void){
	char valuuta[MAXRIDA][CUR];
	double hinnad[2][MAXRIDA]; // ostu ja müügihinnad on ühes massiivis

    //Tunnikood
	FILE *sisend;
	
	sisend = fopen("valuuta.txt", "r");
	if(sisend == NULL){
		printf("faili ei saanud avada\n");	
		char buf[200];
		printf("Sisesta faili nimi: ");
		scanf("%s", buf);
		sisend = fopen(buf, "r");
		if(sisend == NULL)
			return 0;
	}
	int n = 0;
	while(1){ //igavene tsükkel
		if(fscanf(sisend, "%s %lg %lg", valuuta[n], &hinnad[0][n], &hinnad[1][n]) < 2){ // lugemine ebaõnnestus
			//kas failiformaat ei sobi
			//või faililõpp
			break; //katkestame igavese tsükli
		}else
			n++;
		if(n >= MAXRIDA){ //failis rohkem andmeid kui mahub
			printf("ei saanud tervet faili lugeda\n");
			break;
		}
	}
	fclose(sisend); //vabastame ressursi

    char wants[CUR]; //Valuuta mida kasutaja soovib
    
    /*Loon väikse massiivi needed, mis hoiab soovitud valuuta indeksit nimekirjas
    ja soovitud kogust, nii saan void tüüp funktsioonist "ask" mõlemad väärtused kätte*/
    double needed[2] = {0, 0}; 
    
    ask(n , valuuta, wants, needed);
    
    int index = needed[0]; //nimetan ümber ajutise massiivi needed liikmed, indeksi teen int tüübiks
    double amount = needed[1];
    
    double x = hinnad[1][index]; //vahetuskurss soovitud valuutale
    double finalCost = math(x, amount); //lõpphind
    print(finalCost, wants, amount);
	return 0;
}

void ask(int n, char valuuta[][CUR], char wants[], double needed[]){
    printf("Valuutavahetus\n");
    while(1){
        printf("Sisestage valuuta kood: ");
        scanf("%s", wants);
        needed[0] = kontroll(n, valuuta, wants); //Vaatan kas valuuta kood on olemas, jätan meelde indeksi
        if(needed[0] == -1){
            printf("\nValuuta kood on vale või puudub andmetest\n\n");
        }
        else{
            break;
        }

    }
    while(1){
        printf("Sisestage soovitud summa valuutas %s: ", wants);
        scanf("%lg", &needed[1]);
        if(needed[1] <= 0){
            printf("\nLiiga väike summa!\n\n");
        }
        else{
            break;
        }
    }
}

int kontroll(int n, char valuuta[][CUR], char wants[]){
    int i;
    for(i = 0; i < n; i++){
        if(strcmp(valuuta[i], wants) == 0){
            return i;
        }
    }
    return -1;
}

double math(double x, double amount){
    double price = amount / x;
    return price;
}

void print(double finalCost, char wants[], double amount){
    printf("%.2f %s vahetus maksab %.2f€\n", amount, wants, finalCost);
}