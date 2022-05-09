#include <stdio.h>
#include <math.h>

//Matriklinumber 213450

int main(void){

    float A, B, H, min, max, x[18], y[18];
    int i;

    printf("Funktsiooni tabuleerimine\n");
    printf("Sisestage argumendi x algväärtus A: ");
    scanf("%f", &A);
    printf("Sisestage argumendi x lõppväärtus B: ");
    scanf("%f", &B);
    
    //Kontrollin kas A on väiksem kui B
    if(A >= B){
        printf("Algväärtus peab olema väiksem kui lõppväärtus!\n");
        return 0;
    }

    printf("Sisestage samm H: ");
    scanf("%f", &H);
    
    //Kas samm H > 0
    if(H <= 0){
        printf("Samm peab olema nullist suurem!\n");
        return 0;
    }
    
    //Teen massiivid x ja y väärtustega ning väljastan väärtused tabelina
    
    x[0] = A; //Esimesel kohal on x võrdne sisestatud algväärtusega
    
    i = 0; //indeks mis määrab elemendi koha massiivis
    
    min = 1.9129; //Määramispiirkonna piirid
    max = 2;
    
    printf("x     | y = (sqrt(1 - sqrt(pow(x, 3) - 7))) / (pow(x, 2) + sqrt(x)\n");
    
    while(x[i] < B && i < 18){
        x[i + 1] = x[i] + (i + 1) * H; //x = A + H, x = A + 2H...
        printf("%5.3f | ", x[i]);
        y[i] = (sqrt(1 - sqrt(pow(x[i], 3) - 7))) / (pow(x[i], 2) + sqrt(x[i]));
        if(x[i] <= min || x[i] > max){
            printf("puudub\n");
        }
        else{
            printf("%5.3f\n", y[i]);
        }
        i++;
    }
    
    return 0;
}