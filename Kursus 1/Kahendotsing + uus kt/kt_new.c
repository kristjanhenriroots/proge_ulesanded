#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Matriklinumber 213450

float loe(const char *);
float loePiir(const char *, float);
void arvuta(float, float, float);
void prindi(int, float, float);

int main(void){
    float A, B, H;
	A = loe("x algväärtus A");
	B = loePiir("x lõppväärtus B", A);
	H = loePiir("samm H", 0);
    arvuta(A, B, H);
    return 0;
}

float loe(const char *nimi){
	float x;
	printf("Palun sisestage %s: ", nimi);
	if(scanf("%g", &x) == 1)
		return x;
	else{
		printf("Lugemine ei õnnestunud\n");
		exit(0);
	}
}

float loePiir(const char *nimi, float piir){
	float x;
	do{
		x = loe(nimi);
	}while(x <= piir);
	return x;
}

void arvuta(float A, float B, float H){
    
    prindi(0,0,0);//prindin tabeli päise, muutujaga n annan edasi kui vastus määramispiirkonnast väljas
    
    float x[18], y[18];
    x[0] = A; //Esimesel kohal on x võrdne sisestatud algväärtusega
    int i = 0; //indeks mis määrab elemendi koha massiivis
    float min = 1.9129; //Määramispiirkonna piirid
    float max = 2;

    while(x[i] < B && i < 18){
        x[i + 1] = x[0] + (i + 1) * H; //x = A + H, x = A + 2H...
        if(x[i] <= min || x[i] > max){
            prindi(2, x[i], 0);
        }
        else{
            y[i] = (sqrt(1 - sqrt(pow(x[i], 3) - 7))) / (pow(x[i], 2) + sqrt(x[i]));
            prindi(1, x[i], y[i]);
        }
        i++;
    }
}

void prindi(int n, float a, float b){
    if(n == 0){
        printf("x     | y = (sqrt(1 - sqrt(pow(x, 3) - 7))) / (pow(x, 2) + sqrt(x)\n");
    }
    else{
        printf("%5.3f | ", a);
        if(n == 2){
            printf("puudub\n");
        }
        else{
            printf("%5.3f\n", b);
        }
    }
}