#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(void){
	float x1, x2, x3, y1, y2, y3, vx1, vx2, vx3, vy1, vy2, vy3, p1, p2, p3, P;

	printf("Kolmnurga ümbermõõdu arvutamine\n");
	printf("Sisesta esimese tipu koordinaadid x1, y1: ");
	scanf("%g %g", &x1, &y1);
	printf("Sisesta teise tipu koordinaadid x2, y2: ");
	scanf("%g %g", &x2, &y2);
	printf("Sisesta komanda tipu koordinaadid x3, y3: ");
	scanf("%g %g", &x3, &y3);
	
	// Kontrollin kas tipud kattuvad
	if(((x1 == x2) && (y1 == y2)) || ((x1 == x3) && (y1 == y3)) || ((x2 == x3) && (y2 == y3))){
		printf("Tipud kattuvad! Tegemist ei ole kolmnurgaga.\n");
		return 0;
	}

	// Vektori kooridinaatide leidmine lõpp miinus algus
	vx1 = x2 - x1;
	vx2 = x3 - x1;
	vx3 = x3 - x2;
	
	vy1 = y2 - y1;
	vy2 = y3 - y1;
	vy3 = y3 - y2;
	
	// Vektori pikkuste leidmine p on pikkus
	p1 = sqrt(abs( (vx1 * vx1) + (vy1 * vy1) ));
	p2 = sqrt(abs( (vx2 * vx2) + (vy2 * vy2) ));
	p3 = sqrt(abs( (vx3 * vx3) + (vy3 * vy3) ));

	//Kontrollin kas moodustub kolmnurk, mistahes kahe külje summa peab olema suurem kui kolmas külg
	if( (p1 + p2 > p3) && (p1 + p3 > p2) && (p2 + p3 > p1)){

	}
	else{
		printf("Kolmnurka ei moodustu!\n");
		return 0;
	}

	//Ümbermõõdu leidmine
	P = p1 + p2 + p3;
	printf("Kolmnurga ümbermõõt on %8.4f\n", P);
	return 0;
}