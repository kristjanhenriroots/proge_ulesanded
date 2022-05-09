#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1000
#define LUTSIZE 256
#define MAXLINES 100

typedef enum { s_init, s_nonquot, s_quot, s_quot2 } state_t;
typedef enum {t_init, t_integer, t_float, t_text, t_currency, t_code} type_t;
typedef enum { i_other = 0, i_letter = 1, i_number = 2, i_decimal = 3, i_currency = 4} input_t;

typedef struct{
	FILE *f;
	int isEOL;
	int cellpos;
	char separator;
	char decimalChar;
	char current[MAXLINES][MAX];
	type_t type[MAXLINES];
} field_t;

char* returntype(type_t s) {
	if(s == t_text)
		return "T";
	else if(s == t_integer)
		return "I";
	else if(s == t_float)
		return "F";
	else if(s == t_code)
		return "C";
	else if (s == t_currency)
		return "M";
	else
		return "";
}

void FillLUT(char* LUT) {
	int i;
	for(i = 0; i < LUTSIZE; i++)
		LUT[i] = i_other;
	for(i = 'a'; i <= 'z'; i++)
		LUT[i] = i_letter;
	for(i = 'A'; i <= 'Z'; i++)
		LUT[i] = i_letter;
	for(i = '0'; i <= '9'; i++)
		LUT[i] = i_number;
	LUT['.'] = i_decimal;
	LUT['$'] = i_currency;
	LUT[','] = i_decimal;
}

type_t switchtype(type_t t, unsigned char x, char* LUT) {	
	input_t x2;
	x2 = LUT[x];
	if(x2 == i_letter){ // letter
		if(t == t_init)
			t = t_text;
	else if(t == t_init || t == t_float || t == t_currency)
			t = t_code;		
	}
	else if(x2 == i_number){ //number
		if(t == t_init)
			t = t_integer;
		else if(t == t_text)
			t = t_code;				
	}
	else if(x2 == i_decimal){ //decimal separator
		if(t == t_init || t == t_integer)
			t = t_float;
		else if(t == t_text || t == t_float) //second . not in decimal
			t = t_code;		
	}
	else if(x2 == i_currency) {
		if(t == t_init)
			t = t_currency;
		else
			t = t_code;
	}	
	return t;
	
}
void extract(field_t *x, char* LUTP){
	int bufpos = 0;
	x->cellpos = 0;
	state_t state;
	state = s_init;
	x->type[x->cellpos] = t_init;
	unsigned char c;
	int reading = 1;
	while(reading){
		if (x->cellpos >= 100) {
			reading = 0;
			break;
		}
		c = fgetc(x->f); // read one letter
		switch(state){
			case s_init: if(c == x->separator || c == '\n' || feof(x->f)){
				if(c == x->separator) {
					state = s_init;
					x->cellpos++;
					x->type[x->cellpos] = t_init;
					bufpos = 0;
					break;
				}else{
					//reading = 0; // exit while loop
					break;
					}
				}
				if(c == '\"'){
					state = s_quot;
					break;
				}
				x->current[x->cellpos][bufpos++] = c;
				x->type[x->cellpos] = switchtype(x->type[x->cellpos], c, LUTP);
				state = s_nonquot;
				break;
			case s_nonquot: if(c == x->separator || c == '\n' || feof(x->f)){
				if(c == x->separator) {
					state = s_init;
					x->cellpos++;
					x->type[x->cellpos] = t_init;
					bufpos = 0;
					break;
				}else{
					reading = 0; // exit while loop
					break;
					}
				}
				x->current[x->cellpos][bufpos++] = c;
				x->type[x->cellpos] = switchtype(x->type[x->cellpos], c, LUTP);
				break;
			case s_quot: if( feof(x->f) ){ //error, unfinished quoted text by end of file
					reading = 0; // exit while loop
					break;
				}				
				if( c == '\"'){
					state = s_quot2;
					break;
				}
				x->current[x->cellpos][bufpos++] = c;
				x->type[x->cellpos] = switchtype(x->type[x->cellpos], c, LUTP);
				break;
			case s_quot2: if(c == x->separator || c == '\n' || feof(x->f)){
				if(c == x->separator) {
					state = s_init;
					x->cellpos++;
					x->type[x->cellpos] = t_init;
					bufpos = 0;
					break;
				}else{
					reading = 0; // exit while loop
					break;
					}
				}
				if( c == '\"'){
					x->current[x->cellpos][bufpos++] = c;
					x->type[x->cellpos] = switchtype(x->type[x->cellpos], c, LUTP);
					state = s_quot;
					break;
				}
		}
		
	}
	x->current[x->cellpos][bufpos] = '\0'; //end text
	x->isEOL = (c == '\n' || feof(x->f));
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
	printf("Please enter character for separator");
	scanf("%s", &fld.separator);
	printf("Please enter decimal point character");
	scanf("%s", &fld.decimalChar);
	if (fld.separator == fld.decimalChar) 
		return 0;
	char LUT[LUTSIZE];
	char *LUTP = LUT;
	FillLUT(LUTP);
	printf("T = text, C = code, I = integer, F = float, M = currency\n");
	while(!feof(f)){
		extract(&fld, LUTP);
		for(int i = 0; i < fld.cellpos; i++)
			printf("|%s(%s)\t", fld.current[i],returntype(fld.type[i]));
		if(fld.isEOL)
			printf("\n");
		memset(fld.current, 0, sizeof fld.current);
		memset(fld.type, 0, sizeof fld.type);
	}	
	fclose(f);
	return 0;
}
