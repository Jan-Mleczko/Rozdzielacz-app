#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "userio.c"

#define DBG(X) /*fprintf (stderr, "*** DEBUG: " #X "=%d\n", (X));*/

#define SZUFLADY 12
#define RZECZY 99
#define TAK 1
#define NIE 0
#define SYGNATURA 1

struct Szuflada {
	char nazwa[15], pelna[16];
	char obcieta;
	unsigned short int zawart;
	};

struct Rzecz {
	char nazwa[16];
	unsigned short int szufl;
	};

struct Szuflada szufy[SZUFLADY];
struct Rzecz rzeczy[RZECZY];
unsigned int ilszuflad, ilrzeczy;
char zajszuf[SZUFLADY], zajrzecz[RZECZY];
char npliku[] = "ROZDZIAL.DAT";

zajmij (char *tablica) {
	char *poczatek;
	
	poczatek = tablica;
	while (*tablica)
		++tablica;
	assert (*tablica == 0);
	*tablica = TAK;
	return tablica - poczatek;
	}

void czysc () {
#ifdef _WIN32
	system ("CLS");
#endif
	}

void naglow () {
	czysc ();
	putln (".:  ROZDZIELACZ - Jan Mleczko, 2025.  :.");
	}

void wyswietl () {
	unsigned int i, j, k;
	unsigned int czworka, wysok, wys1, indszuf;
	unsigned int rz[4], *rw, r1, n;
	char szukac;
	struct Szuflada *istnszuf[SZUFLADY], **wskszuf;
	
	assert (ilszuflad <= SZUFLADY);
	
	if (ilszuflad < 1) {
		puts ("\n"
		" X   X\n"
		"  X X\n"
		"   X    Nie ma zadnych szuflad.\n"
		"  X X\n"
		" X   X\n");
		return;
		}
	
	wskszuf = istnszuf;
	i = 0;
	do {
		if (zajszuf[i])
			*wskszuf++ = szufy + i;
		} while (++i < 12);
	assert (wskszuf - istnszuf == ilszuflad);
	
	i = 0;
	do {	assert (i < ilszuflad);
		
		if ((czworka = i + 4) > ilszuflad)
			czworka = ilszuflad;
		assert (czworka <= ilszuflad);
		assert (czworka > i);

		wysok = 0;
		j = i;
		do {	assert (j >= i);
			assert (j < czworka);
			if ((wys1 = istnszuf[j]->zawart) > wysok)
				wysok = wys1;
			} while (++j < czworka);
		
		j = i;
		do {	assert (j >= i);
			assert (j < czworka);
			if (j > i)
				putchar ('|');
			printf ("%.15s #%-2d", istnszuf[j]->nazwa,
					istnszuf[j] - szufy + 1);
			} while (++j < czworka);
		newln ();
		
		rw = rz + 4;
		do	*--rw = 0;
			while (rw > rz);
		assert (*rz == 0);
		assert (rz[1] == 0);
		assert (rz[2] == 0);
		assert (rz[3] == 0);
		
		j = wysok;
		while (j--) {
			k = i;
			rw = rz;
			do {	assert (k >= i);
				assert (k < czworka);
				assert (rw < rz + 4);
				if (k > i)
					putchar ('|');
				r1 = *rw;
				indszuf = istnszuf[k] - szufy;
				assert (indszuf < SZUFLADY);
				szukac = TAK;
				while (r1 < RZECZY && szukac) {
					if (zajrzecz[r1]
					&& rzeczy[r1].szufl
					== istnszuf[k] - szufy) {
						n = printf ("%s #%d",
						rzeczy[r1].nazwa, r1 + 1);
						assert (n <= 19);
						n = 19 - n;
						assert (n >= 0);
						while (n--)
							putchar (' ');
						szukac = NIE;
						}
					++r1;
					}
				*rw++ = r1;
				assert (!szukac || r1 >= RZECZY);
				if (szukac)
					putstr ("         :         ");
				} while (++k < czworka);
			newln ();
			}
		n = (czworka - i) * 20 - 1;
		assert (n < 80);
		do	putchar ('-');
			while (--n);
		newln ();
		} while ((i = czworka) < ilszuflad);
	}

czytsz (char *pytanie) {
	int sz;
	
	assert (ilszuflad >= 1);
	while ((sz = mgetn (pytanie)) < 1 || sz > SZUFLADY
		|| !zajszuf[--sz])
	puts ("\tNieprawidlowy numer szuflady!");
	
	assert (sz >= 0);
	assert (sz < SZUFLADY);
	assert (zajszuf[sz]);
	return sz;
	}

czytrz (char *pytanie) {
	int rz;
	
	assert (ilrzeczy >= 1);
	while ((rz = mgetn (pytanie)) < 1 || rz > RZECZY || !zajrzecz[--rz])
		puts ("\tNieprawidlowy numer elementu!");
		
	assert (rz < RZECZY);
	assert (zajrzecz[rz]);
	return rz;
	}

void nazwszuf (struct Szuflada *cel) {
	unsigned int dlnzw;
	char nzw[17], *wsk;
	unsigned char zn;

	mgetl ("Nazwa dla szuflady", nzw, 16);
	if (!*nzw)
		strcpy (nzw, "<BEZ NAZWY>");
	strncpy (cel->pelna, nzw, 16);
	if (cel->pelna[15]) {
		cel->obcieta = TAK;
		cel->pelna[15] = 0;
		}
	else	cel->obcieta = NIE;

	wsk = nzw;
	while (zn = *wsk) {
		if (zn >= 'a' && zn <= 'z')
			*wsk = zn & '_';
		else switch (zn) {  /* Polskie znaki CP852. */
		case 0245:
			*wsk = 0244; break;
		case 0206:
			*wsk = 0217; break;
		case 0251:
			*wsk = 0250; break;
		case 0210:
			*wsk = 0235; break;
		case 0344:
			*wsk = 0343; break;
		case 0242:
			*wsk = 0340; break;
		case 0230:
			*wsk = 0227; break;
		case 0276:
			*wsk = 0275; break;
		case 0253:
			*wsk = 0215; break;
			}
		++wsk;
		}
	assert (wsk > nzw);
	if ((dlnzw = wsk - nzw) > 15) {
		strcpy (nzw + 12, "...");
		dlnzw = 15;
		}
		
	assert (dlnzw <= 15);
	memset (cel->nazwa, '_', 15);
	memcpy (cel->nazwa + (15 - dlnzw >> 1), nzw, dlnzw);
	}

char *dodszuf () {
	struct Szuflada *cel;
	
	if (ilszuflad >= SZUFLADY)
		return "Zbyt wiele szuflad!";
	cel = szufy + zajmij (zajszuf);
	
	nazwszuf (cel);
	cel->zawart = 0;
	++ilszuflad;
	return NULL;
	}

void nazwrzecz (struct Rzecz *cel) {
	char nzw[17];

	mgetl ("Nazwa dla elementu", nzw, 16);

	if (strlen (nzw) > 15)
		strcpy (nzw + 12, "...");
	assert (strlen (nzw) <= 15);
	strcpy (cel->nazwa, nzw);
	}

char *dodaj () {
	unsigned int sz;
	struct Rzecz *cel;
	
	if (ilszuflad < 1)
		return "Trzeba najpierw utworzyc jakas szuflade!";
	if (ilrzeczy >= RZECZY)
		return "Zbyt wiele elementow!";
	
	cel = rzeczy + zajmij (zajrzecz);
	nazwrzecz (cel);
	cel->szufl = sz = czytsz ("Szuflada #");
	++szufy[sz].zawart;
	++ilrzeczy;
	
	return NULL;
	}

char *przenies () {
	unsigned int rz, ds;
	struct Rzecz *rr;
	
	if (ilrzeczy < 1)
		return "Nie ma zadnych elementow!";
	assert (ilszuflad >= 1);
	
	rz = czytrz ("Przenoszony element #");
	rr = rzeczy + rz;
	
	ds = czytsz ("Docelowa szuflada #");
	if (rr->szufl == ds)
		return "Ta sama szuflada!";
	
	assert (szufy[rr->szufl].zawart > 0);
	--szufy[rr->szufl].zawart;
	++szufy[rr->szufl = ds].zawart;
	return NULL;
	}

upewnij () {
	do {	putstr ("Czy na pewno usunac? [T/N] ");
		switch (getchar ()) {
		case 'T':
		case 't':
			if (getchar () != '\n')
				goto dokoncz;
			return TAK;
		case 'N':
		case 'n':
			if (getchar () != '\n')
				goto dokoncz;
			return NIE;
			break;
		default:
		dokoncz:
			while (getchar () != '\n');
		case '\n':
			puts ("\tNierozpoznana odpowiedz!");
			break;
			}
		} while (TAK);
	}

char *usun () {
	unsigned int rz;
	
	if (ilrzeczy < 1)
		return "Nie ma zadnych elementow!";

	rz = czytrz ("Usuwany element #");
	assert (rz < RZECZY);
	assert (zajrzecz[rz]);
	assert (szufy[rzeczy[rz].szufl].zawart > 0);
	if (upewnij ()) {
		--szufy[rzeczy[rz].szufl].zawart;
		zajrzecz[rz] = NIE;
		--ilrzeczy;
		}
	return NULL;
	}

char *us_sz () {
	unsigned int sz;
	
	if (ilszuflad < 1)
		return "Nie ma zadnych szuflad!";
	sz = czytsz ("Usuwana szuflada #");
	assert (sz < SZUFLADY);
	assert (zajszuf[sz]);
	if (szufy[sz].zawart > 0)
		return "W tej szufladzie sa elementy!";
	assert (szufy[sz].zawart == 0);
	if (upewnij ()) {
		zajszuf[sz] = NIE;
		--ilszuflad;
		}
	return NULL;
	}

void zeruj () {
	memset (zajszuf, NIE, SZUFLADY);
	memset (zajrzecz, NIE, RZECZY);
	ilszuflad = ilrzeczy = 0;
	}

void uswszyst () {
	puts ("Usuniete zostana WSZYSTKIE elementy i szuflady.");
	if (upewnij ())
		zeruj ();
	}

char *zapisz () {
	static char k[50];
	FILE *plik;
	unsigned int i;
	long int rozmiar;
	
	if (!ilszuflad) {
		remove (npliku);
		return "Brak danych.";
		}

	if ((plik = fopen (npliku, "wb")) != NULL) {
		if (fputc (SYGNATURA, plik) == EOF)
			goto cblad;
		if (fwrite (zajszuf, 1, SZUFLADY, plik) != SZUFLADY)
			goto cblad;
		if (fwrite (zajrzecz, 1, RZECZY, plik) != RZECZY)
			goto cblad;
		if (fputc (0, plik) == EOF)
			goto cblad;
		i = 0;
		do {	assert (i < SZUFLADY);
			if (zajszuf[i] && fwrite (szufy + i,
					sizeof (struct Szuflada), 1, plik
					) != 1)
				goto cblad;
			} while (++i < SZUFLADY);
		i = 0;
		do {	assert (i < RZECZY);
			if (zajrzecz[i] && fwrite (rzeczy + i,
					sizeof (struct Rzecz), 1, plik
					) != 1)
				goto cblad;		
			} while (++i < RZECZY);
		rozmiar = ftell (plik);
		fclose (plik);
		}
	else	goto blad;
	
	if (rozmiar == -1L)
		return "Dane zostaly zapisane.";
	sprintf (k, "Dane zostaly zapisane. Rozmiar: %lu B.", rozmiar);
	return k;

cblad:	fclose (plik);
blad:	return "Zapis nie powiodl sie!";
	}

char *laduj () {
	FILE *plik;
	unsigned int i;
	
	if ((plik = fopen (npliku, "rb")) != NULL) {
		ilrzeczy = ilszuflad = 0;
		if (fgetc (plik) != SYGNATURA)
			goto blad;
		if (fread (zajszuf, 1, SZUFLADY, plik) != SZUFLADY)
			goto blad;
		if (fread (zajrzecz, 1, RZECZY, plik) != RZECZY)
			goto blad;
		if (fgetc (plik) == EOF)
			goto blad;
		i = 0;
		do {	assert (i < SZUFLADY);
			if (zajszuf[i]) {
				if (fread (szufy + i, sizeof (struct Szuflada),
						1, plik) != 1)
					goto blad;
				++ilszuflad;
				}
			} while (++i < SZUFLADY);
		i = 0;
		do {	assert (i < RZECZY);
			if (zajrzecz[i]) {
				if (fread (rzeczy + i, sizeof (struct Rzecz),
						1, plik) != 1)
					goto blad;
				++ilrzeczy;
				}
			} while (++i < RZECZY);
		fclose (plik);
		}
	else	return NULL;
	
	return "Zaladowano dane z pliku.";
blad:
	fclose (plik);
	zeruj ();
	return "Uszkodzone dane w pliku!";
	}

char opcja () {
	char odp, dalsze;
	
	odp = dalsze = getchar ();
	while (dalsze != '\n')
		dalsze = getchar ();
	return odp;
	}

void lstszuf () {
	unsigned int i;
	
	assert (ilszuflad > 0);
	puts ("Lista szuflad:");
	i = 0;
	do {	assert (i < SZUFLADY);
		if (zajszuf[i])
			printf ("- #%d %s\n", i + 1, szufy[i].pelna);
		} while (++i < SZUFLADY);
	}

void lstrzeczy () {
	unsigned int i;
	
	assert (ilrzeczy > 0);
	puts ("Lista elementow:");
	i = 0;
	do {	assert (i < RZECZY);
		if (zajrzecz[i])
			printf ("- #%d %s\n", i + 1, rzeczy[i].nazwa);
		} while (++i < RZECZY);
	}

char *zna_sz () {
	if (ilszuflad < 1)
		return "Brak szuflad!";
	
	naglow ();
	puts ("Zmiana nazwy szuflady.\n");
	lstszuf ();
	newln ();
	
	nazwszuf (szufy + czytsz ("Szuflada # do zmiany nazwy"));
	return NULL;
	}

char *zna_rz () {
	if (ilrzeczy < 1)
		return "Brak elementow!";
	
	naglow ();
	puts ("Zmiana nazwy elementu.\n");
	lstrzeczy ();
	newln ();
	
	nazwrzecz (rzeczy + czytrz ("Element # do zmiany nazwy"));
	return NULL;
	}

char *uszawart () {
	unsigned int sz, i, pozost;
	
	if (ilszuflad < 1)
		return "Brak szuflad!";

	naglow ();
	puts ("Usuwanie szuflady z zawartoscia.\n");
	lstszuf ();
	newln ();
	sz = czytsz ("Szuflada # do usuniecia z zawartoscia");
	assert (sz < SZUFLADY);
	assert (zajszuf[sz]);
	
	printf ("Ilosc elementow w szufladzie: %u.\n",
			pozost = szufy[sz].zawart);
	assert (pozost <= ilrzeczy);
	if (upewnij ()) {
		zajszuf[sz] = NIE;
		--ilszuflad;
		i = 0;
		while (pozost) {
			assert (i < RZECZY);
			if (zajrzecz[i] && rzeczy[i].szufl == sz) {
				zajrzecz[i] = NIE;
				assert (pozost >= 1);
				--pozost;
				assert (ilrzeczy >= 1);
				--ilrzeczy;
				}
			++i;
			}
		}
	return NULL;
	}

char *pcala () {
	unsigned int zs, ds, i, pozost;
	
	if (ilszuflad < 2)
		return "Nie ma dwoch szuflad!";
	if (ilrzeczy < 1)
		return "Brak elementow!";
	
	naglow ();
	puts ("Przenoszenie calej zawartosci szuflady.\n");
	lstszuf ();
	newln ();
	zs = czytsz ("Zrodlowa szuflada #");
	ds = czytsz ("Docelowa szuflada #");
	if (zs == ds)
		return "Ta sama szuflada!";
	
	szufy[ds].zawart += pozost = szufy[zs].zawart;
	szufy[zs].zawart = 0;
	i = 0;
	while (pozost) {
		assert (i < RZECZY);
		if (zajrzecz[i] && rzeczy[i].szufl == zs) {
			rzeczy[i].szufl = ds;
			assert (pozost > 0);
			--pozost;
			}
		++i;
		}
	return NULL;
	}

FILE *eksport;
char ekspnazwa[20];

void utweksport (char *rozsz) {
	int n;

	n = 1;
	do {	assert (n >= 1 && n <= 999);
		sprintf (ekspnazwa, "Rozdz%03u.%s", n, rozsz);
		if ((eksport = fopen (ekspnazwa, "r")) != NULL) {
			fclose (eksport);
			eksport = NULL;
			}
		else	eksport = fopen (ekspnazwa, "w");
		} while (eksport == NULL && ++n < 1000);
	}

char podg1, podg2;

void podglad () {
#ifdef _WIN32
	char komenda[50];
	
	sprintf (komenda, "start explorer /select,%s", ekspnazwa);
	if (!system (komenda))
		podg1 = TAK;
	sprintf (komenda, "start %s", ekspnazwa);
	if (!system (komenda))
		podg2 = TAK;
#endif
	}

void wpodglad () {
	printf ("Zapisano plik pod nazwa \"%s\".\n", ekspnazwa);
	podg1 = podg2 = NIE;
	podglad ();
	if (podg1)
		puts ("Otwarto katalog z zapisanym plikiem.");
	else	puts ("Otwarcie katalogu z zapisanym plikiem nieudane!");
	
	if (podg2)
		puts ("Otwarto utworzony plik.");
	else	puts ("Otwarcie utworzonego pliku nieudane!");
	}

void eksptekst () {
	unsigned int i, j, pozost;
	char pierwsza;
	
	pierwsza = TAK;
	i = 0;
	while (i < SZUFLADY) {
		if (zajszuf[i]) {
			if (pierwsza)
				pierwsza = NIE;
			else	fputs ("\n\n", eksport);
			fprintf (eksport, "%s:", szufy[i].pelna);
			if (pozost = szufy[i].zawart) {
				j = 0;
				do {	assert (pozost > 0);
					assert (j < RZECZY);
					if (zajrzecz[j]
					&& rzeczy[j].szufl == i) {
						fprintf (eksport, "\n  - %s",
						rzeczy[j].nazwa);
						--pozost;
						}
					++j;
					} while (pozost);
				}
			else	fputs ("\n  (pusta szuflada)", eksport);
			}
		++i;
		}
	}

void powrot () {
	mwait ("Aby wrocic, nacisnij [ENTER]");
	}

void dotekstu () {
	naglow ();
	puts ("Zapis danych jako plik tekstowy.\n");
	
	utweksport ("TXT");
	if (eksport == NULL)
		puts ("Nie udalo sie utworzyc pliku tekstowego!");
	else {
		eksptekst ();
		fclose (eksport);
		wpodglad ();
		}
	powrot ();
	}

struct StylHT {
	char	*pocz1,
		*pocz2,
		*pocz3,
		*szuft,
		*szufp,
		*szufk,
		*rzeczp,
		*rzeczk,
		*pusta,
		*koniec;
	} *styl;
#define GEN "Rozdzielacz 1.0, J.M."

void htpisz (unsigned char *tekst) {
	unsigned int znak;
	
	while (znak = *tekst++) {
		switch (znak) {
		case '<':
			fputs ("&lt;", eksport);
			continue;
		case '>':
			fputs ("&gt;", eksport);
			continue;
		case '&':
			fputs ("&amp;", eksport);
			continue;
		case '\"':
			fputs ("&quot;", eksport);
			continue;
		
		/* Polskie znaki z CP852 na Unicode. */
		case 0245: znak = 0x105; break;
		case 0206: znak = 0x107; break;
		case 0251: znak = 0x119; break;
		case 0210: znak = 0x142; break;
		case 0344: znak = 0x144; break;
		case 0242: znak = 0xF3; break;
		case 0230: znak = 0x15B; break;
		case 0276: znak = 0x17C; break;
		case 0253: znak = 0x17A; break;
		
		case 0244: znak = 0x104; break;
		case 0217: znak = 0x106; break;
		case 0250: znak = 0x118; break;
		case 0235: znak = 0x141; break;
		case 0343: znak = 0x143; break;
		case 0340: znak = 0xD3; break;
		case 0227: znak = 0x15A; break;
		case 0275: znak = 0x17B; break;
		case 0215: znak = 0x179; break;
			}
		if (znak >= 32 && znak <= 126)
			fputc (znak, eksport);
		else	fprintf (eksport, "&#%u;", znak);
		}
	}

struct StylHT klasyczny = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n  </HEAD>\n  <BODY>\n"
	"    <H1>",
	/*--------------------*/
	"</H1>\n",
	/*--------------------*/
	"    <H2>",
	/*--------------------*/
	"</H2>\n    <UL>\n",
	/*--------------------*/
	"    </UL>\n",
	/*--------------------*/
	"      <LI>", "</LI>\n",
	/*--------------------*/
	"      <LI><EM>pusta szuflada</EM></LI>\n",
	/*--------------------*/
	"  </BODY>\n</HTML>"};

struct StylHT xklasyczny = {
	"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
	" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
	"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n  <head>\n"
	"    <meta name=\"generator\" "
	"content=\"" GEN "\" />\n"
	"    <title>",
	/*--------------------*/
	"</title>\n  </head>\n  <body>\n"
	"    <h1>",
	/*--------------------*/
	"</h1>\n",
	/*--------------------*/
	"    <h2>",
	/*--------------------*/
	"</h2>\n    <ul>\n",
	/*--------------------*/
	"    </ul>\n",
	/*--------------------*/
	"      <li>", "</li>\n",
	/*--------------------*/
	"      <li><em>pusta szuflada</em></li>\n",
	/*--------------------*/
	"  </body>\n</html>"};

struct StylHT klas5 = {
	"<!DOCTYPE html>\n<html>\n  <head>\n"
	"    <meta charset=\"utf-8\">\n"
	"    <meta name=\"generator\" "
	"content=\"" GEN "\">\n"
	"    <title>",
	/*--------------------*/
	"</title>\n  </head>\n  <body>\n"
	"    <h1>",
	/*--------------------*/
	"</h1>\n",
	/*--------------------*/
	"    <h2>",
	/*--------------------*/
	"</h2>\n    <ul>\n",
	/*--------------------*/
	"    </ul>\n",
	/*--------------------*/
	"      <li>", "</li>\n",
	/*--------------------*/
	"      <li><em>pusta szuflada</em></li>\n",
	/*--------------------*/
	"  </body>\n</html>"};

struct StylHT upieksz = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n  </HEAD>\n  <BODY>\n"
	"    <H1 ALIGN=\"center\"><I>",
	/*--------------------*/
	"</I></H1>\n    <HR>\n\n",
	/*--------------------*/
	"    <H2>",
	/*--------------------*/
	"</H2>\n    <UL TYPE=\"square\">\n",
	/*--------------------*/
	"    </UL>\n    <HR>\n",
	/*--------------------*/
	"      <LI>", "</LI>\n",
	/*--------------------*/
	"      <LI><EM>pusta szuflada</EM></LI>\n",
	/*--------------------*/
	"  </BODY>\n</HTML>"};

struct StylHT nowoczesny = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n    <STYLE TYPE=\"text/css\">\n"
	"      body {font-family: sans-serif}\n"
	"      div.opak {padding: 0px 20%}\n"
	"      h2 {color: gray; border-bottom: 1px solid gray;}\n"
	"      li em {font-style: normal; color: red}\n"
	"    </STYLE>\n  </HEAD>\n  <BODY>\n"
	"    <DIV CLASS=\"opak\">\n      <H1>",
	/*--------------------*/
	"</H1>\n",
	/*--------------------*/
	"      <H2>",
	/*--------------------*/
	"</H2>\n      <UL>\n",
	/*--------------------*/
	"      </UL>\n",
	/*--------------------*/
	"        <LI>", "</LI>\n",
	/*--------------------*/
	"        <LI><EM>pusta szuflada</EM></LI>\n",
	/*--------------------*/
	"    </DIV>\n  </BODY>\n</HTML>"};

struct StylHT ogrod = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n    <STYLE TYPE=\"text/css\">\n"
	"      body {background-color: green}\n"
	"      h1 {color: white; text-align: center}\n"
	"      div.szuflada {margin: 25px 10%; "
	"border: 5px ridge white;\n        background-color: #FFD; "
	"padding: 25px}\n"
	"      ul, li {list-style-type: circle}\n"
	"      h2 {color: green; font-style: italic}\n"
	"      li em {font-style: normal; color: silver}\n"
	"    </STYLE>\n  </HEAD>\n  <BODY>\n    <H1>",
	/*--------------------*/
	"</H1>\n",
	/*--------------------*/
	"    <DIV CLASS=\"szuflada\">\n      <H2>",
	/*--------------------*/
	"</H2>\n      <UL>\n",
	/*--------------------*/
	"      </UL>\n    </DIV>\n",
	/*--------------------*/
	"        <LI>", "</LI>\n",
	/*--------------------*/
	"        <LI><EM>pusta szuflada</EM></LI>\n",
	/*--------------------*/
	"  </BODY>\n</HTML>"};

struct StylHT energia = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n    <STYLE TYPE=\"text/css\">\n"
	"      body {background-color: black}\n"
"      h1, h2 {font-family: \"Impact\", sans-serif; font-weight: normal;}\n"
	"      h1 {color: white}\n"
	"      h2 {color: red; font-style: italic; background-color: silver;\n"
	"        margin-bottom: 0px}\n"
	"      div {background-color: gray; margin-top: 0px}\n"
	"      div div {color: white; font-family: sans-serif}\n"
	"    </STYLE>\n  </HEAD>\n  <BODY>\n"
	"    <H1>",
	/*--------------------*/
	"</H1>\n",
	/*--------------------*/
	"    <H2>",
	/*--------------------*/
	"</H2>\n    <DIV>\n",
	/*--------------------*/
	"    </DIV>\n",
	/*--------------------*/
	"      <DIV>", "</DIV>\n",
	/*--------------------*/
	"      ---\n",
	/*--------------------*/
	"  </BODY>\n</HTML>"};

struct StylHT panele = {
	"<HTML>\n  <HEAD>\n"
	"    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n"
	"    <TITLE>",
	/*--------------------*/
	"</TITLE>\n    <STYLE TYPE=\"text/css\">\n"
	"      body {background-color: #7878FF}\n"
	"      h1 {color: white}\n"
	"      div.szuflada {display: inline-block; width: 250px;\n"
	"        background-color: white; padding: 15px; color: black;\n"
	"        margin: 5px; vertical-align: top}\n"
	"      ul, li {list-style-type: disc}\n"
	"      h2 {background-color: navy; color: white; font-size: 100%;\n"
	"        text-align: center; margin: 0px; padding: 4px}\n"
	"    </STYLE>\n  </HEAD>\n  <BODY>\n    <H1>",
	/*--------------------*/
	"</H1>\n",
	/*--------------------*/
	"    <DIV CLASS=\"szuflada\">\n      <H2>",
	/*--------------------*/
	"</H2>\n      <UL>\n",
	/*--------------------*/
	"      </UL>\n    </DIV>\n",
	/*--------------------*/
	"        <LI>", "</LI>\n",
	/*--------------------*/
	"        <LI><U>Ta szuflada jest pusta.</U></LI>\n",
	/*--------------------*/
	"  </BODY>\n</HTML>"};

char zmieniac;

void ekspHTML (char *tytul) {
	unsigned int i, j, pozost;
	char zmiana;

	assert (!zmieniac || styl == &energia);
	
	fputs (styl->pocz1, eksport);
	htpisz (tytul);
	fputs (styl->pocz2, eksport);
	htpisz (tytul);
	fputs (styl->pocz3, eksport);

	i = 0;
	while (i < SZUFLADY) {
		if (zajszuf[i]) {
			fputs (styl->szuft, eksport);
			htpisz (szufy[i].pelna);
			fputs (styl->szufp, eksport);
			if (pozost = szufy[i].zawart) {
				j = 0;
				zmiana = NIE;
				do {
					assert (pozost > 0);
					assert (j < RZECZY);
					if (zajrzecz[j]
					&& rzeczy[j].szufl == i) {
						fputs (zmieniac && zmiana
						? "      <DIV STYLE=\""
							"text-align:right\">"
						: styl->rzeczp, eksport);
						htpisz (rzeczy[j].nazwa);
						fputs (styl->rzeczk, eksport);
						--pozost;
						zmiana = !zmiana;
						}
					++j;
					} while (pozost);
				}
			else	fputs (styl->pusta, eksport);
			fputs (styl->szufk, eksport);
			}
		++i;
		}
	fputs (styl->koniec, eksport);
	}

void eksptabela (char *tytul) {
	/* Specjalny podprogram tworzenia dokumentu HTML w stylu Tabela,
	   poniewa¿ kolejnoœæ danych w pliku docelowym jest wtedy inna. */
	unsigned int i, j, pozost;
	unsigned int istnszuf[SZUFLADY];
	
	fputs ("<HTML>\n  <HEAD>\n    <META NAME=\"generator\" "
	"CONTENT=\"" GEN "\">\n    <TITLE>", eksport);
	htpisz (tytul);
	fputs ("</TITLE>\n  </HEAD>\n  <BODY>\n    <H1>", eksport);
	htpisz (tytul);
	fputs ("</H1>\n    <TABLE BORDER=\"3\">\n      <TR>\n", eksport);

	i = j = 0;
	while (i < SZUFLADY) {
		if (zajszuf[i]) {
			assert (j < SZUFLADY);
			istnszuf[j++] = i;
			fputs ("        <TH SCOPE=\"col\">", eksport);
			htpisz (szufy[i].pelna);
			fputs ("</TH>\n", eksport);
			}
		++i;
		}
	fputs ("      </TR><TR>\n", eksport);

	assert (j == ilszuflad);
	i = 0;
	while (i < ilszuflad) {
		assert (i < SZUFLADY);
		fputs ("        <TD VALIGN=\"top\">", eksport);
		if (pozost = szufy[i].zawart) {
			fputs ("<UL>\n", eksport);
			j = 0;
			do {	assert (pozost > 0);
				assert (j < RZECZY);
				if (zajrzecz[j] && rzeczy[j].szufl == i) {
					fputs ("          <LI>", eksport);
					htpisz (rzeczy[j].nazwa);
					fputs ("</LI>\n", eksport);
					--pozost;
					}
				++j;
				} while (pozost);
			fputs ("        </UL>", eksport);
			}
		else	fputs ("pusta szuflada", eksport);
		fputs ("</TD>\n", eksport);
		++i;
		}
	
	fputs ("      </TR>\n    </TABLE>\n  </BODY>\n</HTML>", eksport);
	}

char *doHTML () {
	char tyt[101], tabela;

	if (ilszuflad < 1)
		return "Nie ma czego zapisywac!";

	naglow ();	
	putstr ("Zapis danych w formacie HTML.\n\n\
Wybierz styl wygladu.\n\
  1) Klasyczny\n\
  2) Klasyczny XHTML\n\
  3) Klasyczny HTML5\n\
  4) Tabela\n\
  5) Upiekszony klasyczny\n\
  6) Nowoczesny\n\
  7) Zielony ogrod\n\
  8) Energiczny szaro-czerwony\n\
  9) Panele\n?");
  	zmieniac = tabela = NIE;
	switch (opcja ()) {
	case '1':
		styl = &klasyczny;
		break;
	case '2':
		styl = &xklasyczny;
		break;
	case '3':
		styl = &klas5;
		break;
	case '4':
		tabela = TAK;
		break;
	case '5':
		styl = &upieksz;
		break;
	case '6':
		styl = &nowoczesny;
		break;
	case '7':
		styl = &ogrod;
		break;
	case '8':
		styl = &energia;
		zmieniac = TAK;
		break;
	case '9':
		styl = &panele;
		break;
	default:
		puts ("\tNierozpoznana odpowiedz!");
		styl = NULL;
		break;
		}
	if (styl != NULL || tabela) {
		mgetl ("Tytul strony", tyt, 100);
		if (!*tyt)
			strcpy (tyt, "Bez tytulu");
	
		utweksport ("htm");
		if (eksport == NULL)
			puts ("Utworzenie pliku nie powiodlo sie!");
		else {
			if (tabela)
				eksptabela (tyt);
			else	ekspHTML (tyt);
			fclose (eksport);
			wpodglad ();
			}
		}
	powrot ();
	return NULL;
	}

unsigned int zakres, faza;

unsigned int podzlos () {
	return rand () % zakres;
	}

unsigned int podzrown () {
	if (faza >= zakres)
		faza = 0;
	return faza++;
	}

char *autorozdz (unsigned int (*podzial) (), char *opis) {
	unsigned int sz, los, cel;
	unsigned int i, pozost;
	
	if (ilszuflad < 3)
		return "Potrzebne 3 szuflady!";
	if (ilrzeczy < 1)
		return "Brak elementow do rozdzielenia!";
	
	naglow ();
	puts (opis);
	
	lstszuf ();
	sz = czytsz ("Szuflada # do rozdzielenia");
	zakres = ilszuflad - 1;
	assert (zakres < ilszuflad);
	assert (zakres > 1);
	
	if ((pozost = szufy[sz].zawart) < 1)
		return "Ta szuflada jest pusta!";
	i = 0;
	do {	assert (pozost > 0);
		assert (i < RZECZY);
		if (zajrzecz[i] && rzeczy[i].szufl == sz) {
			los = podzial ();
			assert (los < ilszuflad);
			cel = 0;
		petla:
			while (!zajszuf[cel] || cel == sz)
				++cel;
			if (los) {
				--los;
				++cel;
				goto petla;
				}
			assert (cel < SZUFLADY);
			assert (zajszuf[cel]);
			rzeczy[i].szufl = cel;
			++szufy[cel].zawart;
			--pozost;
			}
		++i;
		} while (pozost);
	szufy[sz].zawart = 0;
	
	return NULL;
	}

char dzialac;

char *inopc () {
	char ponow;
	
	do {	naglow ();
		putstr ("\n\
  Inne opcje:\n\
     w) Wroc\n\
     k) Zakoncz\n\
\n\
     o) Odswiez dane z dysku\n\
     z) Zmien nazwe szuflady\n\
     x) Zmien nazwe elementu\n\
     u) Usun szuflade z zawartoscia\n\
     c) Przenies cala zawartosc szuflady\n\
     l) Losowo rozdziel szuflade\n\
     r) Mozliwie rowno rozdziel szuflade\n\
\n\
     t) Zapisz jako plik tekstowy\n\
     h) Zapisz w formacie HTML\n?");
     
		ponow = NIE;
		switch (opcja () | 32) {
		case 'w':
			break;
		case 'k':
			dzialac = NIE;
			break;
		case 'o':
			return laduj ();
		case 'z':
			return zna_sz ();
		case 'x':
			return zna_rz ();
		case 'u':
			return uszawart ();
		case 'c':
			return pcala ();
		case 'l':
			return autorozdz (podzlos,
			"Losowe rozdzielanie szuflady.\n");
			break;
		case 'r':
			faza = 0;
			return autorozdz (podzrown,
			"Mozliwie rowne rozdzielanie szuflady.\n");
			break;
		case 't':
			dotekstu ();
			break;
		case 'h':
			return doHTML ();
			break;
		default:
			ponow = TAK;
			}
		} while (ponow);
	return NULL;
	}

main () {
	char odp, *komunikat;
	
	komunikat = laduj ();
	dzialac = TAK;
	
	do {	naglow ();
		wyswietl ();
		puts ("\
  n) Nowy element     u) Usun element     p) Przenies       z) Zapisz\n\
  m) Nowa szuflada    s) Usun szuflade    y) Usun wszystko  k) Zakoncz\n\
  i) Inne opcje");
  		if (komunikat != NULL) {
  			printf ("===== %s =====\n", komunikat);
  			komunikat = NULL;
  			}
  			
		putchar ('?');
  		switch (odp = opcja () | 32) {
  		case 'n':
  			komunikat = dodaj ();
  			break;
		case 'm':
			komunikat = dodszuf ();
			break;
		case 'u':
			komunikat = usun ();
			break;
		case 's':
			komunikat = us_sz ();
			break;
		case 'p':
			komunikat = przenies ();
			break;
		case 'y':
			uswszyst ();
			break;
		case 'z':
			komunikat = zapisz ();
			break;
		case 'k':
			dzialac = NIE;
			break;
		case 'i':
			komunikat = inopc ();
			break;
		default:
			komunikat = "Dziwna opcja!";
			break;
  			}
		} while (dzialac);
	
	return 0;	
	}
