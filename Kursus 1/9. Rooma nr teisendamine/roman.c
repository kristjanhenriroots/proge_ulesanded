#include <stdio.h>
#include <string.h>
#define MAX 100

int toRoman(int a), toArabic(char arr[MAX]), ar_val(char);

int main(void){
    
    int end = 0;
    printf("Rooma/Araabia numbrite teisendamine\n");
    while(end == 0){

        char ans[1];
        int check1 = 0, AtoR = 0;

        printf("\nKas soovite teisendada Rooma või Araabia numbriteks? R/A : ");
    
        while(check1 == 0){
            scanf("%c", &ans[0]);
            if(ans[0] == 'A' || ans[0] == 'a'){
                check1++;
            }
            else if(ans[0] == 'R' || ans[0] == 'r'){
                AtoR++; //Saan aru mis valiti
                check1++;
            }
            else{
                printf("\nKui soovite teisendada Rooma numbriteks (nt IV, IX, XVIII) sisestage R\n");
                printf("\nKui soovite teisendada Araabia numbriteks (nt 1, 2, 3) sisestage A\n\n");
                getchar();
            }
        }
        
        int Anr, check2 = 0, checkRnr = 0;
        char Rnr[MAX];
        
        while(check2 == 0){
            if(AtoR != 0){
                printf("\nSisestage teisendamiseks soovitud arv: ");
                scanf("%d", &Anr);
                if(Anr < 1 || Anr > 3999){
                    printf("\nNumber peab olema vahemikus 1 - 3999\n\n");
                    getchar();
                }
                else{
                    printf("\nArvule %d vastav Rooma number oleks: ", Anr);
                    toRoman(Anr);
                    printf("\n\n");
                    check2++;
                }
            }
            else{
                while(checkRnr == 0){
                    printf("\nSisestage teisendamiseks soovitud Rooma number: ");
                    scanf("%s", Rnr);
                    if(toArabic(Rnr) != 0){
                        printf("\nRooma numbrile %s vastav arvuline väärtus oleks: %d\n\n", Rnr, toArabic(Rnr));
                        checkRnr++;
                        check2++;
                    }
                } 
            }
        }
    
        //Lõpetan loopi
        char loop_end[1];
        int check3 = 0;

        while(check3 == 0){
            printf("Kas see on kõik? Y/N : ");
            getchar();
            scanf("%c", &loop_end[0]);
            if(loop_end[0] == 'Y' || loop_end[0] == 'y'){
                end++;
                check3++;
            }
            getchar();
            if(loop_end[0] == 'N' || loop_end[0] == 'n'){
                check3++;
            }
        }
    }
    
    
    return 0;
}

//Rooma => Araabia
int toArabic(char arr[MAX]){
    int i = 0, ans = 0;

    //Kestab kuni terve massiiv on läbi käidud
    while(arr[i]){
        
        /*Kontrollin kas sisestatud täht kuulub Rooma numbrite hulka, tavaline väärtus (value) on -1 niiet kui antud täht ei ole Rooma number on selle väärtus nullist väiksem*/
        if(ar_val(arr[i]) < 0){ 
            printf("\nTegemist ei ole Rooma numbriga!\n");
            printf("\nSobilikud on ainult: I, V, X, L, C, D, M\n\n");
            return 0;
        }

        //Üle ühe arv peab olema väiksem kui see millega võrreldakse. Väldib nt XVIIX 
        if((strlen(arr) - i) > 2){
            if(ar_val(arr[i]) < ar_val(arr[i + 2])){
                printf("Sisend on vales järjekorras!\n\n");
                return 0;
            }
           
        }
        
        //Vaatan mitu samasugust on kõrvuti, I, X, C, M max 3, teisi ainult 1
        if((ar_val(arr[i]) == 1) || (ar_val(arr[i]) == 10) || (ar_val(arr[i]) == 100) || (ar_val(arr[i]) == 1000)){
            if((ar_val(arr[i])) == (ar_val(arr[i + 1])) && (ar_val(arr[i])) == (ar_val(arr[i + 2])) && (ar_val(arr[i])) == (ar_val(arr[i + 3]))){
                printf("\nLiiga palju samu numbreid kõrvuti!\n\n");
                return 0;
            }
        }
        else{
            if(ar_val(arr[i]) == ar_val(arr[i + 1])){
                printf("\nLiiga palju samu numbreid kõrvuti!\n\n");
                return 0;
            }
        }
        
        /*Kui eelnev arv on väiksem kui järgnev, siis kontrollin mis arv on järgmine ning mis arv sellele eelneb. Nt X ees võib olla ainult I mitte V, C ees võib olla ainult X jne*/
        if(ar_val(arr[i]) < ar_val(arr[i + 1])){
            if(ar_val(arr[i + 1]) == 5){
                 if(ar_val(arr[i]) != 1){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
            if(ar_val(arr[i + 1]) == 10){
                if(ar_val(arr[i]) != 1){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
            if(ar_val(arr[i + 1]) == 50){
                if(ar_val(arr[i]) != 10){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
            if(ar_val(arr[i + 1]) == 100){
                if(ar_val(arr[i]) != 10){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
            if(ar_val(arr[i + 1]) == 500){
                if(ar_val(arr[i]) != 100){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
            if(ar_val(arr[i + 1]) == 1000){
                if(ar_val(arr[i]) != 100){
                    printf("Sisend on vales järjekorras!\n\n");
                    return 0;
                }
            }
        }
        /*Teisendus. Kui järgnev arv on väiksem või võrdne siis liidan, kui järgneb suurem arv siis lahutan sellest väiksema Nt: IV = 5-1*/
        if(ar_val(arr[i]) >= ar_val(arr[i+1])){
            ans = ans + ar_val(arr[i]);
        }
        else{
            ans = ans + (ar_val(arr[i+1]) - ar_val(arr[i]));
            i++;
        }
        i++;
    }
    
    return ans;
}

int ar_val(char c){
    
    int value = 0;

    //Rooma numbrite väärtuste tabel, vahetan ära tähe selle arvulise väärtusega ning tagastan selle
    switch(c){
        case 'I': value = 1; break;
        case 'i': value = 1; break;
        case 'V': value = 5; break;
        case 'v': value = 5; break;
        case 'X': value = 10; break;
        case 'x': value = 10; break;
        case 'L': value = 50; break;
        case 'l': value = 50; break;
        case 'C': value = 100; break;
        case 'c': value = 100; break;
        case 'D': value = 500; break;
        case 'd': value = 500; break;
        case 'M': value = 1000; break;
        case 'm': value = 1000; break;
        case '\0': value = 0; break;
        default: value = -1; //Saan aru kas kasutati sobivat Rooma numbrit, kui mitte siis väljastab -1
    }

    return value;
}

//Araabia => Rooma
int toRoman(int a){

    /*Vaatan kui suur arv on ning lahutan sellest suurima väärtuse mille saan ning prindin vastava tähe, kuniks järelejäänud arv on 0*/
    while(a > 0){
        if(a >= 1000){
            printf("M");
            a = a - 1000;
        }
        else if(a >= 900){
            printf("CM");
            a = a - 900;
        }
        else if(a >= 500){
            printf("D");
            a = a - 500;
        }
        else if(a >= 400){
            printf("CD");
            a = a - 400;
        }
        else if(a >= 100){
            printf("C");
            a = a - 100;
        }
        else if(a >= 90){
            printf("XC");
            a = a - 90;
        }
        else if(a >= 50){
            printf("L");
            a = a - 50;
        }
        else if(a >= 40){
            printf("XL");
            a = a - 40;
        }
        else if(a >= 10){
            printf("X");
            a = a - 10;
        }
        else if(a >= 9){
            printf("IX");
            a = a - 9;
        }
        else if(a >= 5){
            printf("V");
            a = a - 5;
        }
        else if(a >= 4){
            printf("IV");
            a = a - 4;
        }
        else if(a >= 1){
            printf("I");
            a = a - 1;
        }
    }

    return a;
}