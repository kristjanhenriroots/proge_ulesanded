#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// kokku pandud tunni näide ja scanner 3 valuuta klassi lisandiga
#define MAX 1000
#define FIELDCOUNT 100

typedef struct{
	FILE *f;
	int isEOL;
	//int error;
	char current[FIELDCOUNT][MAX]; // muudan massivi kahemõõtmeliseks
    int typekeep[FIELDCOUNT]; // hoiab väljade tüüpe
    int dec_char; // komakoht LUT tabelis
    char separator; // eraldaja
} field_t;

typedef enum { s_init, s_nonquot, s_quot, s_quot2 } state_t;

typedef enum { s_initializer = 0, s_text = 1, s_int = 2, s_dec = 3, s_code = 4 , s_currency = 5 } inputState_t; // uus klass valuuta jaoks
// text a-zA-Z
// int 0-9
// dec 0-9.0-9
// code a-zA-Z0-9.

typedef enum { i_other = 0, i_letter = 1, i_number = 2, i_decimal = 3 , i_currency = 5} input_t;

void *extract(field_t *x){
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
    LUT[32] = 1; // () ja space tuvastati muidu initializerina
    LUT[40] = 1;
    LUT[41] = 1;
	if(x->dec_char != 46){
        LUT[x->dec_char] = 3; // koma sümboli muutmine
        LUT[46] = 0;
    }
	//next state based on NS[input][current state]
	char NS[6][6] = {
		{ 0, 0, 0, 0, 0, 0 }, // i_other always takes us to i_init
		{ 1, 1, 4, 4, 4, 4 }, // i_letter takes us to text or code, keeps at text, code
		{ 2, 4, 2, 3, 4, 5 }, // i_num takes us to int or code, keeps at int, dec, code
		{ 3, 4, 3, 4, 4, 5 },  // i_dec takes us to dec or code, keeps at code
        { 4, 4, 4, 4, 4, 4 },  // pole otseselt vaja
        { 5, 4, 4, 4, 4, 4 } // pole kena lahendus aga töötab, i_currency takes us to currency, keeps at  code
	};
	input_t x2;
	inputState_t cs = s_initializer, ns; // current state, next state
	
    int bufpos = 0;
	state_t state;
	state = s_init;
	unsigned char c;
	int reading = 1, idx = 0; // idx peab veergudel järge
	
    while(reading && idx <= 100){
		c = fgetc(x->f); // read one letter
		switch(state){
			case s_init: if(c == x->separator || c == '\n' || feof(x->f)){
				if(c == x->separator){
					state = s_init;
                    idx++;
					bufpos = 0;
					cs = 0;
					break;
				}
					else
						break;	
				}
				if(c == '\"'){
					state = s_quot;
					break;
				}
				x2 = LUT[c];
       			ns = NS[x2][cs]; // we look up next state
				x->current[idx][bufpos++] = c;
				x->typekeep[idx] = ns;
				cs = ns;
				state = s_nonquot;
				break;
			case s_nonquot: if(c == x->separator || c == '\n' || feof(x->f)){
					if(c == x->separator){
						state = s_init;
						idx++;
						bufpos = 0;
						cs = 0;
						break;
					}
					else{
						reading = 0; // exit while loop
						break;
					}
					x->current[idx][bufpos++] = c;
					x->typekeep[idx] = ns;
					break;
				}
				x2 = LUT[c];
        		ns = NS[x2][cs]; // we look up next state
				x->current[idx][bufpos++] = c;
				x->typekeep[idx] = ns;
				cs = ns;
				break;
			case s_quot: if( feof(x->f) ){ //error, unfinished quoted text by end of file
					reading = 0; // exit while loop
					break;
				}				
				if(c == '\"'){
					state = s_quot2;
					break;
				}
				x2 = LUT[c];
       			ns = NS[x2][cs]; // we look up next state
				x->current[idx][bufpos++] = c;
				x->typekeep[idx] = ns;
				cs = ns;
				break;
			case s_quot2: if(c == x->separator || c == '\n' || feof(x->f)){
					if(c == x->separator){
						state = s_init;
						idx += 1;
						bufpos = 0;
						cs = 0;
						break;
					}
					else{
						reading = 0; // exit while loop
						break;
					}
				}
				if(c == '\"'){
					x2 = LUT[c];
       				ns = NS[x2][cs]; // we look up next state
					x->current[idx][bufpos++] = c;
					x->typekeep[idx] = ns;
					state = s_quot;
					cs = ns;
					break;
				}
		}		
	}
	x->current[idx][bufpos] = '\0'; //end text
	x->isEOL = (c == '\n' || feof(x->f));
    for(int i = 0; i < idx; i++)
		printf("|%s (%d)\t", x->current[i], x->typekeep[i]);
    return 0;
}

int main(int argc, char **argv){
	int filename = 1; // maybe user specified file on commandline
	FILE *f;
	if(argc > 1){ // argument present, let's try it
		f = fopen(argv[1], "r");
		if(f == NULL)
			filename = 0; // nope
	} else
		filename = 0;
	char buf[MAX];
	if(filename == 0){
		printf("Please enter filename: ");
		scanf("%s", buf);
		f = fopen(buf, "r");
		if(f == NULL)
			return 0; // user supplied one didn't work either, quit
	}
	field_t fld;
	fld.f = f;
	char dec;
	printf("Sisestage eraldaja: ");
	scanf("%c", &fld.separator);
	getchar();
	printf("Sisestage komakoht: ");
	scanf("%c", &dec);
	fld.dec_char = dec;
	printf("0 - tühi väli, 1 - tekst, 2 - int, 3 - dec, 4 - kood, 5 - valuuta\n");

	while(!feof(f)){
        extract(&fld);
		if(fld.isEOL)
			printf("\n");
		memset(fld.current, 0, sizeof fld.current); // teen kirjutatu tühjaks, muidu tekkis probleeme eelneva ülekirjutamisega
		memset(fld.typekeep, 0, sizeof fld.typekeep); // tühjad väljad tuvastas muidu koodina
	}	
	fclose(f);
	return 0;
}

