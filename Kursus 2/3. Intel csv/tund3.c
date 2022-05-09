#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX 1000
//#define LUTSIZE 256;

typedef enum { s_init = 0, s_text = 1, s_int = 2, s_dec = 3, s_code = 4 } state_t;
// text a-zA-Z
// int 0-9
// dec 0-9.0-9
// code a-zA-Z0-9.

typedef enum { i_other = 0, i_letter = 1, i_number = 2, i_decimal = 3 } input_t;


void extract(char input[], int len, int comma){
    
    char LUT[256] = {
        0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      3,      0,      2,      2,      2,
        2,      2,      2,      2,      2,      2,      2,      0,      0,      0,
        0,      0,      0,      0,      1,      1,      1,      1,      1,      1,
        1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
        1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
        0,      0,      0,      0,      0,      0,      1,      1,      1,      1,
        1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
        1,      1,      1,      1,      1,      1,      1,      1,      1,      1,
        1,      1,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0
	};	

    if(comma != 46){
        LUT[comma] = 3;
        //LUT[46] = 0;
    }


	//next state based on NS[input][current state]
	char NS[4][5] = {
		{ 0, 0, 0, 0, 0 }, // i_other always takes us to i_init
		{ 1, 1, 4, 4, 4 }, // i_letter takes us to text or code, keeps at text, code
		{ 2, 4, 2, 3, 4 }, // i_num takes us to int or code, keeps at int, dec, code
		{ 3, 4, 3, 4, 4 }  // i_dec takes us to dec or code, keeps at code
	};
	unsigned char x;
	input_t x2;
	char buf[MAX];
	int bufpos = 0;
	state_t cs = s_init, ns; // current state, next state



    int i = 0;
	while(i < sizeof(&input)){
		x = input[i];
		x2 = LUT[x]; // we look up the input type
		ns = NS[x2][cs]; // we look up next state
        //printf("cs is %d\n", cs);
        //printf("ns is %d\n\n", ns);
		if(ns != s_init)
			buf[bufpos++] = x;
		else if(ns != cs){
			buf[bufpos++] = 0;
			if(cs == s_text)
				printf("text \"%s\"\n", buf);
			else if(cs == s_int)
				printf("int \"%d\"\n", atoi(buf));
			else if(cs == s_dec){
                if(comma == 46)
				    printf("dec \"%g\"\n", atof(buf));
                else
                    printf("dec \"%s\"\n", buf);
            }
			if(cs == s_code)
				printf("code \"%s\"\n", buf);
			bufpos = 0;
		}
       

		cs = ns; // set current state to next state
        i++;
	}
}



int main(void){
	

    char input[MAX];
    char separator[2];
    char comma[2];
   
    printf("Insert separator (pressing space works too) : ");
    fgets(separator, 2, stdin);
    
    getchar();
    
    printf("Insert comma symbol : ");
    
    fgets(comma, 2, stdin);
    getchar();
    
    int ascii_comma = comma[0];

    

    fgets(input, MAX, stdin);


    //Teen massiivi korrektsete indeksitega, peale igat tühikut uus element kuni end of line ehk enter, millal token = NULL
    char *token = strtok(input, separator);
    char *correct[MAX];
    int i, len = 0;
    
    while (token != NULL){
        correct[i++] = token;
        token = strtok(NULL, separator);
        len++; //Saan elementide arvu
    }

    printf("len is %d\n", len);
    for(i = 0; i < len; i++){
        printf("%s\n", correct[i]);
    }
    for(i = 0; i < len; i++)
        extract(correct[i], len, ascii_comma);

    return 0;
}