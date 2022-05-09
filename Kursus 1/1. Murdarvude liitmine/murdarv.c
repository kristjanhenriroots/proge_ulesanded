#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//Suurima ühisteguri SUT leidmine läbi Eukleidese algoritmi
int SUT(int a, int b){
    if(a == 0){
        return b;
    }
    else{
        return SUT(b % a, a);
    }  
}

int main(void){
    int  L1, L2, N1, N2, Lx, Nx, finalLx, finalNx, taandatudLx, absLx, absNx, A, absSUT;

    //Leian murdude lugejad ja nimetajad L1, L2, N1 ja N2
    printf("Murdarvude liitmine\n");
    printf("Miinusmärgilise murru jaoks lisa miinusmärk lugejasse!\n");
    printf("Sisestage esimesse murru lugeja: ");
    scanf("%d", &L1);
    printf("Sisestage esimesse murru nimetaja: ");
    scanf("%d", &N1);
    printf("Sisestage teise murru lugeja: ");
    scanf("%d", &L2);
    printf("Sisestage teise murru nimetaja: ");
    scanf("%d", &N2);
    
    //Kontrollin kas miinusmärk pandi lugejasse kui üldse on
    if((N1 < 0) || (N2 < 0)){
        printf("Palun sisestage miinus lugejasse!\n");
        return 0;
    }
    
    //Kontrollin kas tegemist on murdarvuga. Kuskil on 0 või L ja N kattuvad, absoluutväärtus vältimaks märgivigu kontrollimises
    if((abs(L1) == abs(N1)) || (abs(L2) == abs(N2))){
        printf("Tegemist ei ole murdarvuga!\n");
        return 0;
    }

    if((N1 == 0) || (N2 == 0) || (L1 == 0) || (L2 == 0)){
        printf("Tegemist ei ole murdarvuga!\n");
        return 0;
    }

    //Vaatan kas murdude nimetajad on samad
    if(N1 == N2){
       Nx = N1;
       Lx = L1 + L2;
    }
    //Teen uue nimetaja Nx ning arvutan lugeja Lx
    else {
        Nx = N1 * N2;
        Lx = (Nx / N1 * L1) + (Nx / N2 * L2);
    }
    
    //Niisama lihtsama debuggimise pärast
    printf("\nTaandamata liidetud murd on %d/%d\n", Lx, Nx);
    

    //Suurima ühisteguri SUT leidmine Eukleidese algoritmiga
 
    int a = Nx; 
    int b = Lx;
    
    absSUT = abs(SUT(a, b));
    printf("Suurim ühistegur on %d\n", absSUT);

    //Jagan algsed väärtused suurima ühisteguri absoluutvaartusega
    finalNx = Nx / absSUT;
    finalLx = Lx / absSUT;

    //leian täisosa ning taandatud Lx ja Nx
    taandatudLx = finalLx % finalNx;
    A = finalLx / finalNx;

    //Segaarvu jaoks, et kahte miinust ei näitaks ning kui täisosa ei ole siis ei näitaks 0
    absNx = abs(finalNx);
    absLx = abs(absNx - abs(taandatudLx) - abs(finalNx));
    
    //Kontrollin kas äkki tuleb täisarv vastuseks mitte murd, siis ei näita 0/1 murdu
    if(absLx == 0 || finalLx == 0 || taandatudLx == 0){
        printf("Taandatud lõppvastus on %d\n", A);
    }
    else{
        if(A == 0){
        printf("Taandatud lõppvastus on %d/%d\n", taandatudLx, absNx);
        return 0;
        }
        else{
        printf("Taandatud lõppvastus on %d %d/%d\n", A, absLx, absNx);
        return 0;
        }
    }
    
        
}