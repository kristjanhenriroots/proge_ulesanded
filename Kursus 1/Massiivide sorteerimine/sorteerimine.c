#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256

int main() {
    
    char string1[MAX], string2[MAX];
    
    printf("Sorteerimine\n");
    printf("Sisestage esimene massiiv: \n");
    fgets(string1, MAX, stdin);
    
    //Teen massiivi korrektsete indeksitega, peale igat tühikut uus element kuni end of line ehk enter, millal token = NULL
    char *token1 = strtok(string1, " ");
    char *M1_str[MAX];
    int i = 0, n1 = 0;
    
    while (token1 != NULL){
        M1_str[i++] = token1;
        token1 = strtok(NULL, " ");
        n1++; //Saan elementide arvu
    }
    
    //Kontrollin kas pikkus on rohkem kui 1
    if(n1 < 2){
        printf("Massiivis peab olema vähemalt 2 elementi!\n");
        return 0;
    }
    
    //Teen char array float omaks
    float M1[n1];
    int a, b;
    for(a = 0; a < n1; a++){
        M1[a] = atof(M1_str[a]);
    }

    //Kontrollin kas massiiv on sorteeritud
    int check1 = 0, control1 = 1, status1 = 0;
    
    while(check1 < n1 - 1){
        if(M1[check1] <= M1[control1]){
            check1++;
            control1++;
        }
        else{
            status1++;
            break;
        }
    }

    //Kui ei ole siis pakun automaatselt seda teha
    char ans[1];
    int gx1, gy1, og1, newog1, correct1 = 0, vf1;
    
    if(status1 != 0){
        printf("Tegemist ei ole sorteeritud massiiviga!\n");
        printf("Kas soovite massiivi sorteerida automaatselt? Y/N\n");
        while (correct1 == 0){
            scanf("%c", &ans[0]);
            getchar(); //Teen puhvri tühjaks, muidu hüppab fgets-st üle
            if(ans[0] == 'N'){
                printf("\nSelge.\n");
                return 0;
            }
            if(ans[0] != 'Y' && ans[0] != 'N'){
                printf("Sisestage Y või N (jah või ei)\n");
            }
            if(ans[0] == 'Y'){
                printf("Automaatne sorteerimine\n\n");
                for(gx1 = 0; gx1 < n1; gx1++){
                printf("[%.2f] ", M1[gx1]);
                }
                printf("\n=>\n");
                for(og1 = 0; og1 < n1; og1++){ 
                    for(newog1 = og1 + 1; newog1 < n1; newog1++){ //Kirjutan üle niikaua kuni leiab väikseima
                        if(M1[newog1] < M1[og1]){
                            vf1 = M1[og1];
                            M1[og1] = M1[newog1];
                            M1[newog1] = vf1;
                        }
                    }
                }
                for(gy1 = 0; gy1 < n1; gy1++){
                printf("[%.2f] ", M1[gy1]);
                }
                printf("\n");
                break;
            }
        }
    }
   
    //Täpselt sama asi uuesti teise massiiviga
    printf("\nSisestage teine massiiv: \n");
    fgets(string2, MAX, stdin);
    
    char *token2 = strtok(string2, " ");
    char *M2_str[MAX];
    int j = 0, n2 = 0;
    
    while (token2 != NULL){
        M2_str[j++] = token2;
        token2 = strtok(NULL, " ");
        n2++;
    }
    
    if(n2 < 2){
        printf("Massiivis peab olema vähemalt 2 elementi!\n");
        return 0;
    }
    
    float M2[n2];
    
    for(b = 0; b < n2; b++){
        M2[b] = atof(M2_str[b]);
    }
    
    int check2 = 0, control2 = 1, status2 = 0;
    
    while(check2 < n2 - 1){
        if(M2[check2] <= M2[control2]){
            check2++;
            control2++;
        }
        else{
            status2++;
            break;
        }
    }

    char ans2[1];
    int gx2, gy2, og2, newog2, correct2 = 0, vf2;
    if(status2 != 0){
        printf("Tegemist ei ole sorteeritud massiiviga!\n");
        printf("Kas soovite massiivi sorteerida automaatselt? Y/N\n");
        while (correct2 == 0){
            scanf("%c", &ans2[0]);
            getchar(); 
            if(ans2[0] == 'N'){
                printf("\nSelge.\n");
                return 0;
            }
            if(ans2[0] != 'Y' && ans2[0] != 'N'){
                printf("Sisestage Y või N (jah või ei)\n");
            }
            if(ans2[0] == 'Y'){
                printf("Automaatne sorteerimine\n\n");
                for(gx2 = 0; gx2 < n2; gx2++){
                printf("[%.2f] ", M2[gx2]);
                }
                printf("\n=>\n");
                for(og2 = 0; og2 < n2; og2++){ 
                    for(newog2 = og2 + 1; newog2 < n2; newog2++){ 
                        if(M2[newog2] < M2[og2]){
                            vf2 = M2[og2];
                            M2[og2] = M2[newog2];
                            M2[newog2] = vf2;
                        }
                    }
                }
                for(gy2 = 0; gy2 < n2; gy2++){
                printf("[%.2f] ", M2[gy2]);
                }
                break;
            }
        }
    }

    //Vaatan kas antud massiivid on ühesugused
    int test, counter = 0;
    if(n1 == n2){
        for(test = 0; test < n1; test++){
            if(M1[test] == M2[test]){
                counter++;
            }
        }
    }
   
    if(counter == n1){
        printf("\nAntud massiivid on ühesugused!\n");
        return 0;
    }
   
    //Teen uue massiivi
    int posM1 = 0, posM2 = 0, posM3 = -1, equal = 0, n3 = n1 + n2;
    float M3[n3];
   
    while(posM3 <= n3){
        posM3++;
        if(posM1 < n1 && posM2 < n2){
            if(M1[posM1] == M2[posM2]){ //Kui sama nr on 2tk siis jätan uude ainult 1tk
                M3[posM3] = M1[posM2];
                posM2++;
                posM1++;
                equal++;
            }
            else{
                if(M1[posM1] > M2[posM2]){
                    M3[posM3] = M2[posM2];
                    posM2++;
                }

                else{
                    M3[posM3] = M1[posM1];
                    posM1++;
                }
            }
            
        }
        else{
            if(posM1 >= n1){
                M3[posM3] = M2[posM2];
                posM2++;
            }

            else{
                M3[posM3] = M1[posM1];
                posM1++;
            }
        }
    }

    int z;
    printf("\n\nSorteeritud ning liidetud massiiv on: ");
    for(z = 0; z < n3 - equal; z++){
        printf("[%.2f] ", M3[z]);
    }
    return 0;
}