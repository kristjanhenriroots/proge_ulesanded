#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXRIDA 40
#define CUR 4

void ask(int n, char valuuta[][CUR], char wants[], double needed[]);
int kontroll(int n, char valuuta[][CUR], char find[]);
double math(double x, double amount);
void print(double finalCost, char wants[], double amount);
double tradeToEur(double price, double amount);
double tradeToResult(double price, double amount);
int mode(double cashIn, double cashOut);

int main(int argc, char *argv[]){
    //char has[CUR], char wants[CUR], double cashIn, double cashOut
	char valuuta[MAXRIDA][CUR];
	double hinnad[2][MAXRIDA]; // ostu ja müügihinnad on ühes massiivis
    if(argc < 5){
		printf("Rakendusele tuleb info käsurealt ette anda\n");
		printf("Kasutage formaati: LÄHTEVALUUTA LÕPPVALUUTA OSTUKOGUS MÜÜGIKOGUS\n");
		printf("NB üks kogustest jätke nulliks\n");
		return 0;
	}

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

    char wants[CUR], has[CUR];
    strcpy(wants, argv[2]); //Valuuta mida kasutaja soovib 
    strcpy(has, argv[1]); //Valuuta mis kasutajal on
    char* ptr;
    double cashIn, cashOut;
    cashIn = strtod(argv[3], &ptr);
    cashOut = strtod(argv[4], &ptr);

    if(cashIn < 0 || cashOut < 0){
        printf("Sisend ei saa olla negatiivne\n");
        return 0;
    }
    if(cashIn == 0 && cashOut == 0){
        printf("Mõlemad sisendid ei tohi olla 0 või on sisestus vales formaadis!\n");
        return 0;
    }

    int index_wants = kontroll(n, valuuta, wants); // otsin kus real valuuta on mis kasutajal on ja mida ta tahab
    int index_has = kontroll(n, valuuta, has);

    if(index_has == index_wants){ // kasutaja pani sama valuuta mõlemale väljale
        printf("%.2f %s = %.2f %s\n", cashIn, wants, cashIn, wants);
        printf("%.2f %s = %.2f %s :)\n", cashOut, wants, cashOut, wants);
        return 0;
    }

    int request = mode(cashIn, cashOut); // saan teada kas soovitakse osta või müüa

    double inEur;
    if(request == 0){
        if(index_wants == -1){ // kasutaja tahab osta mingi summa eurosid, müün seda valuutat mis tal on
            inEur = cashOut * hinnad[0][index_has];
            printf("%.2f %s on %.2f %s\n", cashOut, wants, inEur, has);
        }
        else{ // kasutaja soovib müüa ja osta mingi muu valuuta
            inEur = tradeToEur(hinnad[1][index_wants], cashOut);
            if(index_has == -1){ // kas tal on eurod?
                printf("%.2f %s on %.2f EUR\n", cashOut, wants, tradeToEur(hinnad[1][index_wants], cashOut));
                return 0;
            }
            else{ // ei ole eurod
                printf("%.2f %s on %.2f %s\n", cashOut, wants, tradeToResult(inEur, hinnad[0][index_has]), has);
            }
        }
    }
    else{ // kasutaja tahab müüa mingi summa 
        inEur = tradeToEur(hinnad[0][index_has], cashIn);
        if(index_wants == -1){ // kas ta tahab eurosid
            printf("%.2f %s on %.2f EUR\n", cashIn, has, tradeToEur(hinnad[0][index_has], cashIn));
            return 0;
        }
        if(index_has == -1){ // kas tal on eurod
            printf("%.2f EUR on %.2f %s\n", cashIn, tradeToResult(hinnad[0][index_wants], cashIn), wants);
            return 0;
        }
        else{ // mingid muud valuutad on mõlemad
            printf("%.2f %s on %.2f %s\n", cashIn, wants, tradeToResult(inEur, hinnad[1][index_wants]), has);

        }

    }  

	return 0;
}

int kontroll(int n, char valuuta[][CUR], char find[]){
    int i;
    if(strcmp(find, "EUR") == 0){
        return -1;
    }
    for(i = 0; i < n; i++){
        if(strcmp(valuuta[i], find) == 0){
            return i;
        }
    }
    printf("Valuuta kood on vale või puudub andmetest\n");
    exit(0);
}

int mode(double in, double out){
    if(in == 0)
        return 0; // Kasutaja ostab
    else
        return 1;  // Kasutaja müüb / vahetab
}

double tradeToEur(double price, double amount){
    return amount / price;
}
double tradeToResult(double price, double amount){
    return amount * price;
}