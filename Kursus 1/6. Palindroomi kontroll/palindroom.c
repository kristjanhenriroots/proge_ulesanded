#include <stdio.h>
#include <string.h>
#define MAX 256 //Peaks piisama

int main(void){

    char words[MAX];
    printf("Palindroomi kontroll\n");
    printf("Sisestage ");
    
    int check = 0;
    while(check == 0){
        printf("sõna või sümbolid kontrolliks: ");
        scanf("%s", words);
        
        //Vaatan kui pikk sisestatu on
        size_t pikkus = strlen(words);
    
        if(pikkus > 2){
            check++;
        }
        else{
            printf("\nEi lähe arvesse, sisestage vähemalt 2 sümbolit!\n\nSisestage SOBIV ");
        }
    }

    size_t pikkus = strlen(words);

    //Massiivide võrdlemine
    int m;
    int n = 0;
    for(m = pikkus - 1; m >= 0; m--){ //Pikkus - 1 ei töötanud??
        if(words[n] != words[m]){
            printf("\nTegemist ei ole palindroomiga.\n");
            return 0;
        }
        n++;
    }
    
    printf("\nTegemist on palindroomiga!\n");
   
    return 0;
}