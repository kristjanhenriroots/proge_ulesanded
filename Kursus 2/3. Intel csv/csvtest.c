#include <stdio.h>
#include <stdlib.h>

#define MAX 1000
//#define LUTSIZE 256;

typedef enum { s_init = 0, s_text = 1, s_int = 2, s_dec = 3, s_code = 4 , s_currency = 5 } inputState_t; // uus klass valuuta jaoks
// text a-zA-Z
// int 0-9
// dec 0-9.0-9
// code a-zA-Z0-9.

typedef enum { i_other = 0, i_letter = 1, i_number = 2, i_decimal = 3 , i_currency = 5} input_t;

int main(void){
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
    
    /*
    char usd = '$';
    printf("%d\n", usd); // 36 
    */
    LUT[36] = 5; // laisk viis LUTi muutmiseks dollari juures
    LUT[32] = 4;
    LUT[40] = 4;
    LUT[41] = 4;

	//next state based on NS[input][current state]
	char NS[6][6] = {
		{ 0, 0, 0, 0, 0, 0 }, // i_other always takes us to i_init
		{ 1, 1, 4, 4, 4, 4 }, // i_letter takes us to text or code, keeps at text, code
		{ 2, 4, 2, 3, 4, 5 }, // i_num takes us to int or code, keeps at int, dec, code
		{ 3, 4, 3, 4, 4, 5 },  // i_dec takes us to dec or code, keeps at code
        { 4, 4, 4, 4, 4, 4 }, 
        { 5, 4, 4, 4, 4, 4 } // pole kena lahendus aga töötab, i_currency takes us to currency, keeps at currency or code
	};
	unsigned char x;
	input_t x2;
	char buf[MAX];
	int bufpos = 0;
	inputState_t cs = s_init, ns; // current state, next state
    
	while(1){
		x = getchar();
		x2 = LUT[x]; // we look up the input type
		ns = NS[x2][cs]; // we look up next state
        printf("%c input type is: %d\n", x, x2);
        printf("NS[%d][%d] = %d\n", x2, cs, ns);
        printf("cs = %d\n", cs);
        printf("ns = %d\n", ns);

		if(ns != s_init)
			buf[bufpos++] = x;
		else if(ns != cs){
			buf[bufpos++] = 0;
			if(cs == s_text)
				printf("text \"%s\"\n", buf);
			else if(cs == s_int)
				printf("int \"%d\"\n", atoi(buf));
			else if(cs == s_dec)
				printf("dec \"%g\"\n", atof(buf));
            else if(cs == s_currency)
				printf("currency \"%s\"\n", buf);
			if(cs == s_code)
				printf("code \"%s\"\n", buf);
			bufpos = 0;
		}
		cs = ns; // set current state to next state
	}	
}