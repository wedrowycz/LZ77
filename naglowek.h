#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef NAGLOWEK_H_INCLUDED
#define NAGLOWEK_H_INCLUDED
/*Definicje struktur*/
struct elementListy
{
    unsigned char znak;
    struct elementListy *nastepny;
    struct elementListy *poprzedni;
};
struct dopasowanie
{
    short int start;
    unsigned char dlugosc;
    unsigned char znak;
};
/*Deklaracje funkcji*/
/******************************************************************************************************/
struct elementListy *budujListe(int ileElementow);
void kasujListe(struct elementListy *poczatekListy, int ileElementow);
/******************************************************************************************************/
void kompresja(char nazwaPlikOdczyt[],char nazwaPlikZapis[], int slownik, int bufor,int okno);
void wypelnijListe(struct elementListy *poczatekListy, FILE *wskPlik, int slownik, int bufor);
void uzupelnijListe(struct elementListy *poczatekListy, int ileElementow, FILE *wskPlik);
struct elementListy *przesunWskaznik(struct elementListy *adresElementu, int ileElementow);
struct dopasowanie szukaj(struct elementListy *dictWsk, struct elementListy *buffWsk, int slownik, int bufor);
/******************************************************************************************************/
void dekompresja(char nazwaPlikOdczyt[],char nazwaPlikZapis[], int slownik);
void dekoduj(struct elementListy *poczatekListy,struct dopasowanie rekord, FILE *wskPlik, int slownik);
void wypelnijListeDekomp(struct elementListy *poczatekListy, char znak, int slownik);
struct elementListy *przesunWskaznikDekomp(struct elementListy *adresElement, int ileElementow, int slownik);
/******************************************************************************************************/
void wyswietlPomoc(void);
int ktorePolecenie(char argument[]);
void dopasujPolecenia(char *argv[],int parametry[]);
int podzielPolecenia(int argc, char *argv[], int parametry[]);
int sprawdzajPolecenia(int parametry[],char *argv[]);

#endif // NAGLOWEK_H_INCLUDED
