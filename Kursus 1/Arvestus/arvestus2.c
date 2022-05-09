#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char string(int n, char input[n], char remove[1]){
    printf("input is %s\n", input);
    printf("remove %c\n", remove[0]);
    int i, index = -1;


    char myway[1];
    char eemalda[1];
    strcpy(eemalda, remove);

    for(i = 0; i < 40; i++){
        strcpy(myway, &input[i]);

        if(strcmp(myway, eemalda) == 0){
            index = i;
        }
        printf("%d\n", i);
    }
    printf("index is %d\n", index);

    if(index == -1){
        return index;
    }
    strncpy(&input[index], &remove[0], 1);
    printf("output is %s\n", input);

    return index;

}

int main(void){
    int n = 40;
    char remove[1] = "i";
    char teststring[40] = "See on test tekstiloik, vaatame mis saab";
    char *removeptr;
    char *stringptr;

    stringptr = teststring;
    removeptr = remove;

    string(n, stringptr, removeptr);
    return 0;
}


