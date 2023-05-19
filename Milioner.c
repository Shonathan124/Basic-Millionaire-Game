/* Pri otvaranju datoteka dodat flag 'b'
   Za "Pitanja.txt" -> "rb"
   Za "Rezultati.txt" -> "ab"
   To je zbog razlika u windows i linux compiler-ima
   Neki compiler cita iz datoteke kraj reda kao \r\n dok neki \n
   Da ne bi drugacije citalo datoteke u zavisnosti od razlicitih compiler-a
   dodat je flag 'b' dodatno koji postavlja jedan isti standard za sve compiler-e
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// Dodata biblioteka kako bi nasli random broj gde je potreban
#include <time.h>

#define MAX_SLOVA_PITANJE 100
#define MAX_SLOVA_OPCIJA 100
#define UKUPAN_BROJ_PITANJA 15
#define MAX_IME 50

// Struktura u kojoj cuvamo pitanje, opcije i tacan odgovor
typedef struct{
	char pitanje[MAX_SLOVA_PITANJE];
	char opcija_A[MAX_SLOVA_OPCIJA], opcija_B[MAX_SLOVA_OPCIJA], opcija_C[MAX_SLOVA_OPCIJA], opcija_D[MAX_SLOVA_OPCIJA];
	char tacan_odgovor;
} Pitanja;

// Struktura u kojoj cuvamo duzine pitanja i opcija
typedef struct{
	int pitanje;
	int opcija_A, opcija_B, opcija_C, opcija_D;
} Duz_Pitanja;

/* Deklaracija funkcija
   Ispod main f-je se nalazi objasnjenje svake
*/
void error_file();
void error_dinamicka_alokacija();
void ucitaj_pitanja();
void modifikuj_pitanja();
void pocetni_page();
void pogresan_karakter_page();
void info_page();
void pomoc_page1();
void pomoc_page2();
void pomoc_page3();
void nickname_page();
void tezina_page();
void print_pitanja();
void pomoc1();
void pomoc2();
void end_game();
void lose_page();

int main(){

	// Poziv pocetnog ekrana
	pocetni_page();
	char ime[MAX_IME];
	// Stranica unos imena
	nickname_page(ime);
	int duzina_imena=strlen(ime);

	char tezina;
	int broj_pitanja;
	// Biranje tezine
	tezina_page(&tezina,ime,duzina_imena,&broj_pitanja);

	// Otvaranje datoteke "Pitanja.txt"
	FILE* file_pitanja = fopen("Pitanja.txt", "rb");
	if(file_pitanja==NULL){
		error_file("Pitanja.txt");
	}

	// Alociranje memorije za pitanja
	Pitanja* pitanja = malloc(broj_pitanja*sizeof(Pitanja));
	if(pitanja==NULL){
		error_dinamicka_alokacija("pitanja");
	}

	// Alociranje memorije za duzine pitanje
	Duz_Pitanja* duz_pitanja = malloc(broj_pitanja*sizeof(Duz_Pitanja));
	if(duz_pitanja==NULL){
		error_dinamicka_alokacija("duz_pitanja");
	}

	// Ucitavanje pitanja
	ucitaj_pitanja(pitanja,file_pitanja,broj_pitanja);
	// Zatvaranje datoteke "Pitanja.tt"
	fclose(file_pitanja);
	// Modifikovanje pitanja
	modifikuj_pitanja(pitanja,broj_pitanja,duz_pitanja);

	/* z predstavlja broj trenutnog pitanja
	   h1 indikator za pomoc1, h2 indikator za pomoc2
	*/
	int z=1, h1=0, h2=0;
	// Printanje pitanja i dalje ta f-ja poziva ostale
	print_pitanja(ime,duzina_imena,z,pitanja,duz_pitanja,broj_pitanja,h1,h2);

	// Oslobadjanje memorija za pitanja i duzine pitanja
	free(pitanja);
	free(duz_pitanja);

	exit(EXIT_SUCCESS);
}

// Provera da li je file dobro ucitan (Pitanja.txt i Rezultati.txt)
void error_file(const char *s){
	fprintf(stderr, "File %s nije dobro ucitan!!!\n", s);
	exit(EXIT_FAILURE);
}

// Provera da li je dobro promenljiva dobro dinamicki alocirana
void error_dinamicka_alokacija(const char *s){
	fprintf(stderr, "Promenljiva %s nije dobro dinamicki alocirana!!!\n", s);
	exit(EXIT_FAILURE);
}

/* Ucitavamo n pitanja u zavisnosti od toga koju tezinu je igrac izabrao
   fgetc(file_pitanja); ucitava prazne redove koji se nalaze izmedju pitanja
   u datoteci kako bi lepo ucitala ostala pitanja
*/
void ucitaj_pitanja(Pitanja pitanja[],FILE* file_pitanja,int n){
	int i;
	for(i=0;i<n;i++){
		fgets(pitanja[i].pitanje, MAX_SLOVA_PITANJE, file_pitanja);
		fgets(pitanja[i].opcija_A, MAX_SLOVA_OPCIJA, file_pitanja);
		fgets(pitanja[i].opcija_B, MAX_SLOVA_OPCIJA, file_pitanja);
		fgets(pitanja[i].opcija_C, MAX_SLOVA_OPCIJA, file_pitanja);
		fgets(pitanja[i].opcija_D, MAX_SLOVA_OPCIJA, file_pitanja);
		pitanja[i].tacan_odgovor = fgetc(file_pitanja);
		fgetc(file_pitanja);
		fgetc(file_pitanja);
		fgetc(file_pitanja);
		fgetc(file_pitanja);
	}
}

/* Prvi deo:
   Posto smo pitanja ucitali sa fgets() u njemu se nalazi takodje i \r\n
   sto nam ne odgovara da bi printovali posle pitanja kako treba, zato
   '\0' pomeramo 2 mesta levo, npr: "p i t a n j e \r \n \0" -> "p i t a n j e \0"
                                     0 1 2 3 4 5 6  7  8  9  ->  0 1 2 3 4 5 6  7
   Drugi deo:
   U u promenljivu duz_pitana[] cuvamo duzine svakog pitanja i opcije
   kako bi znali kasnije zbog printa
*/
void modifikuj_pitanja(Pitanja pitanja[],int n,Duz_Pitanja duz_pitanja[]){
	int i;
	for(i=0;i<n;i++){
		// Prvi deo
		int duzina_pitanje = strlen(pitanja[i].pitanje);
		int duzina_opcija_A = strlen(pitanja[i].opcija_A);
		int duzina_opcija_B = strlen(pitanja[i].opcija_B);
		int duzina_opcija_C = strlen(pitanja[i].opcija_C);
		int duzina_opcija_D = strlen(pitanja[i].opcija_D);
		pitanja[i].pitanje[duzina_pitanje-2]='\0';
		pitanja[i].opcija_A[duzina_opcija_A-2]='\0';
		pitanja[i].opcija_B[duzina_opcija_B-2]='\0';
		pitanja[i].opcija_C[duzina_opcija_C-2]='\0';
		pitanja[i].opcija_D[duzina_opcija_D-2]='\0';
		// Drugi deo
		duz_pitanja[i].pitanje = strlen(pitanja[i].pitanje);
		duz_pitanja[i].opcija_A = strlen(pitanja[i].opcija_A);
		duz_pitanja[i].opcija_B = strlen(pitanja[i].opcija_B);
		duz_pitanja[i].opcija_C = strlen(pitanja[i].opcija_C);
		duz_pitanja[i].opcija_D = strlen(pitanja[i].opcija_D);
	}
}

// Pocetni prozor koji cita karakter i dalje zove fukcije
void pocetni_page(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                         DOBRODOSLI U KVIZ                                          |    15    1,000,000 Din  |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                                                                                                    |    11       64,000 Din  |\n");
	printf("|                                                                                                    |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                                                                                    |    8         8,000 Din  |\n");
	printf("|             Pritisnite ENTER za Start!                          Pritisnite P za Pomoc!             |    7         4,000 Din  |\n");
	printf("|                                                                                                    |    6         2,000 Din  |\n");
	printf("|                                                                                                    |    5         1,000 Din  |\n");
	printf("|                                                                                                    |    4           500 Din  |\n");
	printf("|                                        Pritisnite I za Info!                                       |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|                                                                                                    |    1           100 Din  |\n");
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	char c = toupper(getchar());
	if(c=='I'){
		info_page();
	}
	else if(c=='P'){
		pomoc_page1();
	}
	else if(c!='\n'){
		pogresan_karakter_page();
	}
}

// Prozor koji se otvara ukoliko je unet pogresan karakter i cita novi
void pogresan_karakter_page(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                         DOBRODOSLI U KVIZ                                          |    15    1,000,000 Din  |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                                     UNELI STE POGRESAN KARAKTER                                    |    11       64,000 Din  |\n");
	printf("|                                          POKUSAJTE PONOVO                                          |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                                                                                    |    8         8,000 Din  |\n");
	printf("|             Pritisnite ENTER za Start!                          Pritisnite P za Pomoc!             |    7         4,000 Din  |\n");
	printf("|                                                                                                    |    6         2,000 Din  |\n");
	printf("|                                                                                                    |    5         1,000 Din  |\n");
	printf("|                                                                                                    |    4           500 Din  |\n");
	printf("|                                        Pritisnite I za Info!                                       |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|                                                                                                    |    1           100 Din  |\n");
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='I'){
		info_page();
	}
	else if(c=='P'){
		pomoc_page1();
	}
	else if(c!='\n'){
		pogresan_karakter_page();
	}
}

// Info stranica
void info_page(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                                                                                                    |    11       64,000 Din  |\n");
	printf("|                                                                                                    |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                                                                                    |    8         8,000 Din  |\n");
	printf("|                                                                                                    |    7         4,000 Din  |\n");
	printf("|                                    Autor: Nenad Kubat(RT-31/20)                                    |    6         2,000 Din  |\n");
	printf("|                                            01.01.2022.                                             |    5         1,000 Din  |\n");
	printf("|                                             COPYRIGHT                                              |    4           500 Din  |\n");
	printf("|                                                                                                    |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|                                                                                                    |    1           100 Din  |\n");
	printf("|                                       Pritisnite N za nazad!                                       |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='N'){
		getchar();
		pocetni_page();
	}
	else{
		pogresan_karakter_page();
	}
}

// Prva pomoc stranica - Opis tezina
void pomoc_page1(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                      Igrac ima opciju da izabere izmedju 3 razlicite tezine:                       |    12      125,000 Din  |\n");
	printf("|                                       Lako, Srednje i Tesko                                        |    11       64,000 Din  |\n");
	printf("|                                                                                                    |    10       32,000 Din  |\n");
	printf("|                Lako - Igrac odgovora na 5 pitanja i moze osvojiti najvise 1,000 Din                |    9        16,000 Din  |\n");
	printf("|             Srednje - Igrac odogovara na 10 pitanja i moze osvojiti najvise 32,000 Din             |    8         8,000 Din  |\n");
	printf("|          Tesko - Igrac odogovara na svih 15 pitanja i moze osvojiti najvise 1,000,000 Din          |    7         4,000 Din  |\n");
	printf("|                                                                                                    |    6         2,000 Din  |\n");
	printf("|          Koju god opciju da izabere igrac ce za svako pitanje imati 4 ponudjena odgovora:          |    5         1,000 Din  |\n");
	printf("|                                         1 tacan i 3 netacna                                        |    4           500 Din  |\n");
	printf("|                           (Korisnik unosi karakter 'A', 'B', 'C' ili 'D')                          |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|                                                                                 Page 1/3           |    1           100 Din  |\n");
	printf("|                                       Pritisnite N za nazad!               (2 za sledeci Page)     |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='N'){
		getchar();
		pocetni_page();
	}
	else if(c=='2'){
		pomoc_page2();
	}
	else{
		pogresan_karakter_page();
	}
}

// Druga pomoc stranica - Opis pomoci koje igrac ima na raspolaganju
void pomoc_page2(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                   Za vreme odgovaranja igrac moze da izabere jednu od dve pomoci                   |    12      125,000 Din  |\n");
	printf("|                          Svaku pomoc moze iskoristiti jedanput u toku igre                         |    11       64,000 Din  |\n");
	printf("|                                                                                                    |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                   Pomoc \"pola-pola\"                        \"Pomoc publike\"                         |    8         8,000 Din  |\n");
	printf("|           Kompjuter odstranjuje dva netacna       Publika prisutna u \"studiju\" daje                |    7         4,000 Din  |\n");
	printf("|             odgovora, ostaje jedan tacan i           misljenje o tacnom odgovoru                   |    6         2,000 Din  |\n");
	printf("|                 jedan netacan odgovor                    pritiskom na taster                       |    5         1,000 Din  |\n");
	printf("|                     (Karakter 'H')                          (Karakter 'K')                         |    4           500 Din  |\n");
	printf("|                                                                                                    |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|    	    Page 2/3                                                              Page 2/3           |    1           100 Din  |\n");
	printf("|     (1 za prethodni Page)             Pritisnite N za nazad!               (2 za sledeci Page)     |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='N'){
		getchar();
		pocetni_page();
	}
	else if(c=='1'){
		pomoc_page1();
	}
	else if(c=='2'){
		pomoc_page3();
	}
	else{
		pogresan_karakter_page();
	}
}

// Treca pomoc stranica - Dodatne informacije
void pomoc_page3(){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                            Igrac moze tokom pitanja da odustane i zavrsi                           |    11       64,000 Din  |\n");
	printf("|                               zavrsi igru sa trenutno osvojenom sumom                              |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                                                                                    |    8         8,000 Din  |\n");
	printf("|                              Ako igrac pogresi pitanje igra se prekida                             |    7         4,000 Din  |\n");
	printf("|                                 i gubi se sav novac do tada osvojen                                |    6         2,000 Din  |\n");
	printf("|                                                                                                    |    5         1,000 Din  |\n");
	printf("|                                                                                                    |    4           500 Din  |\n");
	printf("|                                                                                                    |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|    	    Page 3/3                                                                                 |    1           100 Din  |\n");
	printf("|     (1 za prethodni Page)             Pritisnite N za nazad!                                       |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='N'){
		getchar();
		pocetni_page();
	}
	else if(c=='1'){
		pomoc_page2();
	}
	else{
		pogresan_karakter_page();
	}
}

/* Stranica koju igrac vidi kada startuje igru - Unos imena
   Sve dok igrac ne unese korektno ime stranica poziva samu sebe rekurzivno
*/
void nickname_page(char ime[]){
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    |   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                                                                                                    |    11       64,000 Din  |\n");
	printf("|                                         Unesite vase ime:                                          |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                 Ime mora da sadrzi 3-20 karaktera!                                 |    8         8,000 Din  |\n");
	printf("|                                                                                                    |    7         4,000 Din  |\n");
	printf("|                                                                                                    |    6         2,000 Din  |\n");
	printf("|                                                                                                    |    5         1,000 Din  |\n");
	printf("|                                   Ovaj prozor ce se otvarati sve                                   |    4           500 Din  |\n");
	printf("|                                     dok nije uneto korektno ime                                    |    3           300 Din  |\n");
	printf("|                                                                                                    |    2           200 Din  |\n");
	printf("|                                                                                                    |    1           100 Din  |\n");
	printf("|                                       Pritisnite N za nazad!                                       |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	fgets(ime, MAX_IME, stdin);
	int duzina_imena=strlen(ime);
	ime[duzina_imena-1]='\0';
	if(strcmp(ime,"n")==0 || strcmp(ime,"N")==0){
		pocetni_page();
	}
	if((duzina_imena-1)<3 || (duzina_imena-1)>20){
		nickname_page(ime);
	}
}

// Stranica za odabir tezine
void tezina_page(char *tezina,char ime[],int n,int *broj_pitanja){
	int i;
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|     Igrac: %s", ime);
	for(i=0;i<(88-n);i++){
		printf(" ");
	}
	printf("|   50-50       Publika   |\n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  |    15    1,000,000 Din  |\n");
	printf("|                                                                                                    |    14      500,000 Din  |\n");
	printf("|                                                                                                    |    13      250,000 Din  |\n");
	printf("|                                                                                                    |    12      125,000 Din  |\n");
	printf("|                                                                                                    |    11       64,000 Din  |\n");
	printf("|                    Unesite neki od sledecih karaktera da izaberete tezinu igre:                    |    10       32,000 Din  |\n");
	printf("|                                                                                                    |    9        16,000 Din  |\n");
	printf("|                                            'L' za Lako                                             |    8         8,000 Din  |\n");
	printf("|                                           'S' za Srednje                                           |    7         4,000 Din  |\n");
	printf("|                                            'T' za Tesko                                            |    6         2,000 Din  |\n");
	printf("|                                                                                                    |    5         1,000 Din  |\n");
	printf("|                                                                                                    |    4           500 Din  |\n");
	printf("|                                                                                                    |    3           300 Din  |\n");
	printf("|                                 Ovaj prozor ce se otvarati sve dok                                 |    2           200 Din  |\n");
	printf("|                                     nije unet korektan karakter                                    |    1           100 Din  |\n");
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	*tezina = toupper(getchar());
	if(*tezina=='L'){
		*broj_pitanja=5;
	}
	else if(*tezina=='S'){
		*broj_pitanja=10;
	}
	else if(*tezina=='T'){
		*broj_pitanja=15;
	}
	else{
		tezina_page(tezina,ime,n,broj_pitanja);
	}
}

/* Stranica koja ispisuje pitanja
   z oznacava broj trenutnog pitanja na kome se nalazi igrac pocevsi od 1
   Ako igrac da tacan odgovor funkcija se poziva rekurzivno za z+1 (sledece pitanje)
   ili ako je poslednje pitanje onda se poziva end_game()
   Ako igrac odustane trenutni novac koji je zaradio mu ostaje i tu se opet poziva end_game()
   Ukoliko igrac pogresi poziva se funkcija lose_page() i tu je kraj igre
   Igrac ima 2 pomoci: Pomoc 50-50 i Pomoc publike
   Indikator h1 oznacava da li je iskoriscena Pomoc 50-50, a h2 Pomoc publike
   Kad se jedanput iskoristi pomoc igrac vise nema pravo da ih koristi
   Ako je unet los karakter funkcija se ponovo poziva
*/
void print_pitanja(char ime[],int n,int z,Pitanja pitanja[],Duz_Pitanja duz_pitanja[],int broj_pitanja,int h1,int h2){
	int i;
	char indikator[2][10] = {"  ", "->"};
	int n1=0, n2=0, n3=0, n4=0;
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |    (H)          (K)     |\n");
	printf("|     Igrac: %s", ime);
	for(i=0;i<(88-n);i++){
		printf(" ");
	}
	if(h1==0){
		printf("|   50-50      ");
	}
	else{
		printf("|  /50-50/     ");
	}
	if(h2==0){
		printf(" Publika   |\n");
	}
	else{
		printf("/Publika/  |\n");
	}
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  | %s 15    1,000,000 Din  |\n", z==15 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 14      500,000 Din  |\n", z==14 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 13      250,000 Din  |\n", z==13 ? indikator[1] : indikator[0]);
	printf("|");
	for(i=0;i<45;i++){
		printf(" ");
	}
	printf("Pitanje %d", z);
	if(z<10){
		for(i=0;i<46;i++){
			printf(" ");
		}
	}
	else{
		for(i=0;i<45;i++){
			printf(" ");
		}
	}
	printf("| %s 12      125,000 Din  |\n", z==12 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 11       64,000 Din  |\n", z==11 ? indikator[1] : indikator[0]);
	printf("|");
	int x = duz_pitanja[z-1].pitanje;
	int a=x/2, b=x-a;
	for(i=0;i<(50-b);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].pitanje);
	for(i=0;i<(50-a);i++){
		printf(" ");
	}
	printf("| %s 10       32,000 Din  |\n", z==10 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 9        16,000 Din  |\n", z==9 ? indikator[1] : indikator[0]);
	printf("|");
	int x_a = duz_pitanja[z-1].opcija_A;
	int a_a=x_a/2, b_a=x_a-a_a;
	for(i=0;i<(25-b_a);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].opcija_A);
	for(i=0;i<(25-a_a);i++){
		printf(" ");
	}
	int x_b = duz_pitanja[z-1].opcija_B;
	int a_b=x_b/2, b_b=x_b-a_b;
	for(i=0;i<(25-b_b);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].opcija_B);
	for(i=0;i<(25-a_b);i++){
		printf(" ");
	}
	printf("| %s 8         8,000 Din  |\n", z==8 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 7         4,000 Din  |\n", z==7 ? indikator[1] : indikator[0]);
	printf("|");
	int x_c = duz_pitanja[z-1].opcija_C;
	int a_c=x_c/2, b_c=x_c-a_c;
	for(i=0;i<(25-b_c);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].opcija_C);
	for(i=0;i<(25-a_c);i++){
		printf(" ");
	}
	int x_d = duz_pitanja[z-1].opcija_D;
	int a_d=x_d/2, b_d=x_d-a_d;
	for(i=0;i<(25-b_d);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].opcija_D);
	for(i=0;i<(25-a_d);i++){
		printf(" ");
	}
	printf("| %s 6         2,000 Din  |\n", z==6 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 5         1,000 Din  |\n", z==5 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 4           500 Din  |\n", z==4 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 3           300 Din  |\n", z==3 ? indikator[1] : indikator[0]);
	printf("|                                 Ovaj prozor ce se otvarati sve dok                                 | %s 2           200 Din  |\n", z==2 ? indikator[1] : indikator[0]);
	printf("|                                     nije unet korektan karakter                                    | %s 1           100 Din  |\n", z==1 ? indikator[1] : indikator[0]);
	printf("|   Pritisnite 'Q' za odustajanje                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c=='A' || c=='B' || c=='C' || c=='D'){
		if(c==pitanja[z-1].tacan_odgovor){
			if(z==broj_pitanja){
				end_game(z,ime,n,h1,h2);
			}
			else{
				print_pitanja(ime,n,z+1,pitanja,duz_pitanja,broj_pitanja,h1,h2);
			}
		}
		else{
			lose_page(z,h1,h2);
		}
	}
	else if(c=='H' && h1==0){
		h1++;
		pomoc1(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2,n1,n2,n3,n4);
	}
	else if(c=='K' && h2==0){
		h2++;
		pomoc2(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2);
	}
	else if(c=='Q'){
		end_game(z-1,ime,n,h1,h2);
	}
	else{
		print_pitanja(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2);
	}
}

/* Prva pomoc: Pomoc 50-50
   Ostaje jedan tacan odgovor i jedan random izabran netacan odgovor
   n1, n2, n3 i n4 sluze da pamte koje odgovore treba printovati
   Jedan ima vrednost 1 i je to tacan odgovor
   Jedan ima vrednost 2 i je to random netacan odgovor
   Ostala 2 imaju vrednost 0 i njih ne treba printovati
*/
void pomoc1(char ime[],int n,int z,Pitanja pitanja[],Duz_Pitanja duz_pitanja[],int broj_pitanja,int h1,int h2,int n1,int n2,int n3,int n4){
	int i;
	char indikator[2][10] = {"  ", "->"};
	int niz[3];
	if(n1==0 && n2==0 && n3==0 && n4==0){
		if(pitanja[z-1].tacan_odgovor=='A'){
			n1=1;
		}
		else if(pitanja[z-1].tacan_odgovor=='B'){
			n2=1;
		}
		else if(pitanja[z-1].tacan_odgovor=='C'){
			n3=1;
		}
		else{
			n4=1;
		}
	}
	int j=0;
	if(n1==0){
		niz[j++]=1;
	}
	if(n2==0){
		niz[j++]=2;
	}
	if(n3==0){
		niz[j++]=3;
	}
	if(n4==0){
		niz[j++]=4;
	}
	srand(time(NULL));
	int random_br=rand()%3;
	int br = niz[random_br];
	int br_nula=0;
	if(n1==0){br_nula++;} if(n2==0){br_nula++;} if(n3==0){br_nula++;} if(n4==0){br_nula++;}
	if(br_nula==3){
		if(br==1){
			n1=2;
		}
		else if(br==2){
			n2=2;
		}
		else if(br==3){
			n3=2;
		}
		else{
			n4=2;
		}
	}
	char druga_opcija;
	if(n1==2){
		druga_opcija='A';
	}
	else if(n2==2){
		druga_opcija='B';
	}
	else if(n3==2){
		druga_opcija='C';
	}
	else{
		druga_opcija='D';
	}
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |    (H)          (K)     |\n");
	printf("|     Igrac: %s", ime);
	for(i=0;i<(88-n);i++){
		printf(" ");
	}
	if(h1==0){
		printf("|   50-50      ");
	}
	else{
		printf("|  /50-50/     ");
	}
	if(h2==0){
		printf(" Publika   |\n");
	}
	else{
		printf("/Publika/  |\n");
	}
	printf("|                                                                                                    |                         |\n");
	printf("|                                  Zelite li da postanete milioner?                                  | %s 15    1,000,000 Din  |\n", z==15 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 14      500,000 Din  |\n", z==14 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 13      250,000 Din  |\n", z==13 ? indikator[1] : indikator[0]);
	printf("|");
	for(i=0;i<45;i++){
		printf(" ");
	}
	printf("Pitanje %d", z);
	if(z<10){
		for(i=0;i<46;i++){
			printf(" ");
		}
	}
	else{
		for(i=0;i<45;i++){
			printf(" ");
		}
	}
	printf("| %s 12      125,000 Din  |\n", z==12 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 11       64,000 Din  |\n", z==11 ? indikator[1] : indikator[0]);
	printf("|");
	int x = duz_pitanja[z-1].pitanje;
	int a=x/2, b=x-a;
	for(i=0;i<(50-b);i++){
		printf(" ");
	}
	printf("%s", pitanja[z-1].pitanje);
	for(i=0;i<(50-a);i++){
		printf(" ");
	}
	printf("| %s 10       32,000 Din  |\n", z==10 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 9        16,000 Din  |\n", z==9 ? indikator[1] : indikator[0]);
	printf("|");
	int x_a = duz_pitanja[z-1].opcija_A;
	int a_a=x_a/2, b_a=x_a-a_a;
	for(i=0;i<(25-b_a);i++){
		printf(" ");
	}
	if(n1==1 || n1==2){
		printf("%s", pitanja[z-1].opcija_A);
	}
	else{
		for(i=0;i<duz_pitanja[z-1].opcija_A;i++){
			printf(" ");
		}
	}
	for(i=0;i<(25-a_a);i++){
		printf(" ");
	}
	int x_b = duz_pitanja[z-1].opcija_B;
	int a_b=x_b/2, b_b=x_b-a_b;
	for(i=0;i<(25-b_b);i++){
		printf(" ");
	}
	if(n2==1 || n2==2){
		printf("%s", pitanja[z-1].opcija_B);
	}
	else{
		for(i=0;i<duz_pitanja[z-1].opcija_B;i++){
			printf(" ");
		}
	}
	for(i=0;i<(25-a_b);i++){
		printf(" ");
	}
	printf("| %s 8         8,000 Din  |\n", z==8 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 7         4,000 Din  |\n", z==7 ? indikator[1] : indikator[0]);
	printf("|");
	int x_c = duz_pitanja[z-1].opcija_C;
	int a_c=x_c/2, b_c=x_c-a_c;
	for(i=0;i<(25-b_c);i++){
		printf(" ");
	}
	if(n3==1 || n3==2){
		printf("%s", pitanja[z-1].opcija_C);
	}
	else{
		for(i=0;i<duz_pitanja[z-1].opcija_C;i++){
			printf(" ");
		}
	}
	for(i=0;i<(25-a_c);i++){
		printf(" ");
	}
	int x_d = duz_pitanja[z-1].opcija_D;
	int a_d=x_d/2, b_d=x_d-a_d;
	for(i=0;i<(25-b_d);i++){
		printf(" ");
	}
	if(n4==1 || n4==2){
		printf("%s", pitanja[z-1].opcija_D);
	}
	else{
		for(i=0;i<duz_pitanja[z-1].opcija_D;i++){
			printf(" ");
		}
	}
	for(i=0;i<(25-a_d);i++){
		printf(" ");
	}
	printf("| %s 6         2,000 Din  |\n", z==6 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 5         1,000 Din  |\n", z==5 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 4           500 Din  |\n", z==4 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 3           300 Din  |\n", z==3 ? indikator[1] : indikator[0]);
	printf("|                                 Ovaj prozor ce se otvarati sve dok                                 | %s 2           200 Din  |\n", z==2 ? indikator[1] : indikator[0]);
	printf("|                                     nije unet korektan karakter                                    | %s 1           100 Din  |\n", z==1 ? indikator[1] : indikator[0]);
	printf("|   Pritisnite 'Q' za odustajanje                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char c = toupper(getchar());
	if(c==pitanja[z-1].tacan_odgovor || c==druga_opcija){
		if(c==pitanja[z-1].tacan_odgovor){
			if(z==broj_pitanja){
				end_game(z,ime,n,h1,h2);
			}
			else{
				print_pitanja(ime,n,z+1,pitanja,duz_pitanja,broj_pitanja,h1,h2);
			}
		}
		else{
			lose_page(z,h1,h2);
		}
	}
	else if(c=='Q'){
		end_game(z-1,ime,n,h1,h2);
	}
	else{
		pomoc1(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2,n1,n2,n3,n4);
	}
}

/* Druga pomoc: Pomoc publike
   Publika glasa za tacan odgovor
   Odgovori su prikazani zvezdicama (*)
   Broj zvezdica tacnog odgovora random izabran od 51-80
*/
void pomoc2(char ime[],int n,int z,Pitanja pitanja[],Duz_Pitanja duz_pitanja[],int broj_pitanja,int h1,int h2){
	int i;
	char indikator[2][10] = {"  ", "->"};
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |    (H)          (K)     |\n");
	printf("|     Igrac: %s", ime);
	for(i=0;i<(88-n);i++){
		printf(" ");
	}
	if(h1==0){
		printf("|   50-50      ");
	}
	else{
		printf("|  /50-50/     ");
	}
	if(h2==0){
		printf(" Publika   |\n");
	}
	else{
		printf("/Publika/  |\n");
	}
	srand(time(NULL));
	int tacan = (rand()%30)+51;
	int random_A, random_B, random_C, random_D;
	if(pitanja[z-1].tacan_odgovor == 'A'){
		random_A=tacan;
	}
	else if(pitanja[z-1].tacan_odgovor == 'B'){
		random_B=tacan;
	}
	else if(pitanja[z-1].tacan_odgovor == 'C'){
		random_C=tacan;
	}
	else{
		random_D=tacan;
	}
	if(random_A==tacan){
		random_C = (100-random_A)/2;
		random_B = (100-random_A-random_C)/3;
		random_D = 100-(random_A+random_B+random_C);
	}
	else if(random_B==tacan){
		random_D = (100-random_B)/2;
		random_A = (100-random_B-random_D)/3;
		random_C = 100-(random_A+random_B+random_D);
	}
	else if(random_C==tacan){
		random_A = (100-random_C)/2;
		random_B = (100-random_A-random_C)/3;
		random_D = 100-(random_A+random_B+random_C);
	}
	else{
		random_B = (100-random_D)/2;
		random_A = (100-random_B-random_D)/3;
		random_C = 100-(random_A+random_B+random_D);
	}
	printf("|                                  Zelite li da postanete milioner?                                  |                         |\n");
	printf("|                                                                                                    | %s 15    1,000,000 Din  |\n", z==15 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 14      500,000 Din  |\n", z==14 ? indikator[1] : indikator[0]);
	printf("|         A: ");
	for(i=0;i<random_A;i++){
		printf("*");
	}
	for(i=0;i<(88-random_A);i++){
		printf(" ");
	}
	printf("| %s 13      250,000 Din  |\n", z==13 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 12      125,000 Din  |\n", z==12 ? indikator[1] : indikator[0]);
	printf("|         B: ");
	for(i=0;i<random_B;i++){
		printf("*");
	}
	for(i=0;i<(88-random_B);i++){
		printf(" ");
	}
	printf("| %s 11       64,000 Din  |\n", z==11 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 10       32,000 Din  |\n", z==10 ? indikator[1] : indikator[0]);
	printf("|         C: ");
	for(i=0;i<random_C;i++){
		printf("*");
	}
	for(i=0;i<(88-random_C);i++){
		printf(" ");
	}
	printf("| %s 9        16,000 Din  |\n", z==9 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 8         8,000 Din  |\n", z==8 ? indikator[1] : indikator[0]);
	printf("|         D: ");
	for(i=0;i<random_D;i++){
		printf("*");
	}
	for(i=0;i<(88-random_D);i++){
		printf(" ");
	}
	printf("| %s 7         4,000 Din  |\n", z==7 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 6         2,000 Din  |\n", z==6 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 5         1,000 Din  |\n", z==5 ? indikator[1] : indikator[0]);
	printf("|                                    Pritisnite 'N' da se vratite                                    | %s 4           500 Din  |\n", z==4 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 3           300 Din  |\n", z==3 ? indikator[1] : indikator[0]);
	printf("|                                 Ovaj prozor ce se otvarati sve dok                                 | %s 2           200 Din  |\n", z==2 ? indikator[1] : indikator[0]);
	printf("|                                     nije unet korektan karakter                                    | %s 1           100 Din  |\n", z==1 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	char t = toupper(getchar());
	if(t=='N'){
		print_pitanja(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2);
	}
	else{
		pomoc2(ime,n,z,pitanja,duz_pitanja,broj_pitanja,h1,h2);
	}
}

/* Ako je igrac odgovorio na sva pitanja tacno ili odustao u nekom trenutku
   poziva se ova funkcija i rezultat se printa u "Rezultati.txt"
*/
void end_game(int z,char ime[],int duzina_imena,int h1,int h2){
	int i;
	char indikator[2][10] = {"  ", "->"};
	char pitanje[2][15] = {"PITANJE", "PITANJA"};
	char nagrade[16][10] = {"0", "100", "200", "300", "500", "1,000", "2,000", "4,000", "8,000", "16,000", "32,000", "64,000", "125,000", "250,000", "500,000", "1,000,000"};
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    ");
	if(h1==0){
		printf("|   50-50      ");
	}
	else{
		printf("|  /50-50/     ");
	}
	if(h2==0){
		printf(" Publika   |\n");
	}
	else{
		printf("/Publika/  |\n");
	}
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    | %s 15    1,000,000 Din  |\n", z==15 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 14      500,000 Din  |\n", z==14 ? indikator[1] : indikator[0]);
	printf("|");
	int x = duzina_imena+11;
	int a=x/2, b=x-a;
	for(i=0;i<(50-b);i++){
		printf(" ");
	}
	printf("CESTITAMO %s!", ime);
	for(i=0;i<(50-a);i++){
		printf(" ");
	}
	printf("| %s 13      250,000 Din  |\n", z==13 ? indikator[1] : indikator[0]);
	printf("|                                USPESNO STE ODGOVORILI NA %d %s!", z, z==1 ? pitanje[0] : pitanje[1]);
	if(z<10){
		for(i=0;i<32;i++){
			printf(" ");
		}
	}
	else{
		for(i=0;i<31;i++){
			printf(" ");
		}
	}
	printf("| %s 12      125,000 Din  |\n", z==12 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 11       64,000 Din  |\n", z==11 ? indikator[1] : indikator[0]);
	printf("|");
	int y = strlen(nagrade[z])+18;
	int c=y/2, d=y-c;
	for(i=0;i<(50-d);i++){
		printf(" ");
	}
	printf("OSVOJILI STE %s DIN!", nagrade[z]);
	for(i=0;i<(50-c);i++){
		printf(" ");
	}
	printf("| %s 10       32,000 Din  |\n", z==10 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 9        16,000 Din  |\n", z==9 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 8         8,000 Din  |\n", z==8 ? indikator[1] : indikator[0]);
	printf("|                      VAS REZULTAT CE BITI ISPISAN U DATOTECI \"Rezultati.txt\"                       | %s 7         4,000 Din  |\n", z==7 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 6         2,000 Din  |\n", z==6 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 5         1,000 Din  |\n", z==5 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 4           500 Din  |\n", z==4 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 3           300 Din  |\n", z==3 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 2           200 Din  |\n", z==2 ? indikator[1] : indikator[0]);
	printf("|                                  PRITISNITE BILO STA DA IZADJETE                                   | %s 1           100 Din  |\n", z==1 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	// Otvaranje datoteke "Rezultati.txt"
	FILE* file_rezultati = fopen("Rezultati.txt", "ab");
	if(file_rezultati==NULL){
		error_file("Rezultati.txt");
	}
	fprintf(file_rezultati, "Igrac: %s\nUspesno odgovorenih pitanja: %d\nOsvojen novac: %s Din\n\n", ime, z, nagrade[z]);
	// Zatvaranje datoteke "Rezultati.txt"
	fclose(file_rezultati);
	getchar();
}

// Ukoliko je igrac dao netacan odgovor poziva se ova funkcija i tu je kraj igre
void lose_page(int z,int h1,int h2){
	int i;
	char indikator[2][10] = {"  ", "->"};
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    ");
	if(h1==0){
		printf("|   50-50      ");
	}
	else{
		printf("|  /50-50/     ");
	}
	if(h2==0){
		printf(" Publika   |\n");
	}
	else{
		printf("/Publika/  |\n");
	}
	printf("|                                                                                                    |                         |\n");
	printf("|                                                                                                    | %s 15    1,000,000 Din  |\n", z==15 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 14      500,000 Din  |\n", z==14 ? indikator[1] : indikator[0]);
	printf("|                                          IZGUBILI STE!!!                                           | %s 13      250,000 Din  |\n", z==13 ? indikator[1] : indikator[0]);
	printf("|                            DALI STE NETACAN ODGOVOR NA PITANJE BROJ");
	printf(" %d", z);
	if(z<10){
		for(i=0;i<30;i++){
			printf(" ");
		}
	}
	else{
		for(i=0;i<29;i++){
			printf(" ");
		}
	}
	printf("| %s 12      125,000 Din  |\n", z==12 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 11       64,000 Din  |\n", z==11 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 10       32,000 Din  |\n", z==10 ? indikator[1] : indikator[0]);
	printf("|                                        VISE SRECE DRUGI PUT                                        | %s 9        16,000 Din  |\n", z==9 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 8         8,000 Din  |\n", z==8 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 7         4,000 Din  |\n", z==7 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 6         2,000 Din  |\n", z==6 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 5         1,000 Din  |\n", z==5 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 4           500 Din  |\n", z==4 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 3           300 Din  |\n", z==3 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    | %s 2           200 Din  |\n", z==2 ? indikator[1] : indikator[0]);
	printf("|                                  PRITISNITE BILO STA DA IZADJETE                                   | %s 1           100 Din  |\n", z==1 ? indikator[1] : indikator[0]);
	printf("|                                                                                                    |                         |\n");
	printf(" ------------------------------------------------------------------------------------------------------------------------------ \n");
	getchar();
	getchar();
}