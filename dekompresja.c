#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naglowek.h"

//Glowna funkcja pobiera jako parametry nazwy plikow wej i wyj oraz wielkosc slownika
void dekompresja(char nazwaPlikOdczyt[],char nazwaPlikZapis[], int slownik)
{
    //Deklaracje zmiennych
    FILE *wskPlikOdczyt,*wskPlikZapis;  //Wskazniki do plikow
	errno_t openstatus;
    struct elementListy *poczatekListy; //Wskaznik na strukture (poczatek listy)
    struct dopasowanie rekord;          //Struktura dopasowanie
    /* Otwieramy pliki */
	openstatus = fopen_s(&wskPlikOdczyt, nazwaPlikOdczyt, "rb");
    if(wskPlikOdczyt==NULL)   //Sprawdzanie czy prawidlowo otwarto plik do odczytu
    {
        printf("Nie mozna otworzyc pliku do odczytu !!!\n");
        wyswietlPomoc();
        exit(0);
    }
	openstatus = fopen_s(&wskPlikZapis ,nazwaPlikZapis, "w");
    if(wskPlikZapis==NULL)   //Sprawdzanie czy prawidlowo otwarto plik do zapisu
    {
        printf("Nie mozna otworzyc pliku do zapisu !!!\n");
        fclose(wskPlikOdczyt);
        wyswietlPomoc();
        exit(0);
    }
    /* Pierwszy rekord z pliku .dat */
    fread(&rekord,sizeof(rekord),1,wskPlikOdczyt);
    /* Zapis pierwszego znaku do pliku .txt */
    putc(rekord.znak,wskPlikZapis);
    /* Budujemy liste */
    poczatekListy=budujListe(slownik);
    /* Pierwsze wypelnienie listy */
    wypelnijListeDekomp(poczatekListy,rekord.znak,slownik);
/************************************************************************************************/
//Glowna petla, odczytuje dane z pliku .dat az do napotkania konca pliku
    while(fread(&rekord,sizeof(rekord),1,wskPlikOdczyt))
    {
        dekoduj(poczatekListy,rekord,wskPlikZapis,slownik);
        poczatekListy=przesunWskaznikDekomp(poczatekListy,rekord.dlugosc+1,slownik);
    }
/************************************************************************************************/
    /* Zamykamy pliki */
    fclose(wskPlikOdczyt);
    fclose(wskPlikZapis);
    /* Zwalniamy liste */
    kasujListe(poczatekListy,slownik);
    printf("Program zostal prawidlowo wykonany !!!\n");
}
//Funkcja dekoduje przekazany jej w parametrach rekord z pliku .dat
//Funkcja zapisuje w pliku wyjsciowym .txt zdekodowany fragment tekstu
void dekoduj(struct elementListy *poczatekListy, struct dopasowanie rekord, FILE *wskPlik, int slownik)
{
    struct elementListy *temp;
    int iLicznik;
    temp=przesunWskaznikDekomp(poczatekListy,rekord.start,slownik);
    //Zapisuje fragment slownika do pliku wyjsciowego .txt
    for(iLicznik=0;iLicznik<rekord.dlugosc;iLicznik++)
    {
        poczatekListy->znak=temp->znak;
        putc(poczatekListy->znak,wskPlik);
        poczatekListy=poczatekListy->nastepny;
        temp=temp->nastepny;
    }
    //Zapisuje ostatni znak z zakodowanej czesci do pliku wyjsciowego .txt
    if(rekord.znak!=0)
    {
        poczatekListy->znak=rekord.znak;
        putc(poczatekListy->znak,wskPlik);
        poczatekListy=poczatekListy->nastepny;
    }
}
//Funkcja wypelnia liste, na poczatku dzialania algorytmu cala lista wypelniona jest jednym znakiem
void  wypelnijListeDekomp(struct elementListy *poczatekListy, char znak,int slownik)
{
    int iLicznik;
    struct elementListy *temp;
    temp=poczatekListy;
    for(iLicznik=0;iLicznik<slownik;iLicznik++)
    {
        temp->znak=znak;
        temp=temp->nastepny;
    }
}
//Funkcja przesuwa wskaznik, ktory pokazuje na poczatek listy (lista cykliczna, dwukierunkowa)
//Zeby szybciej znalezc fragment do przepisania przesuwa sie w dwoch kierunkach w zaleznosci od tego
//w ktorym miejscu znajduje sie poczatek poszukiwanego tekstu, tym rozni sie od podobnej funkcji dla kompresji
struct elementListy *przesunWskaznikDekomp(struct elementListy *adresElement, int ileElementow, int slownik)
{
    struct elementListy *nowyAdres;
    int iLicznik;
    nowyAdres=adresElement;
    if(ileElementow>=slownik/2)                 //jezeli poszukiwany tekst znajduje sie za polowa to szukaj od konca
    {
        for(iLicznik=0;iLicznik<slownik-ileElementow;iLicznik++)
        {
            nowyAdres=nowyAdres->poprzedni;
        }
    }
    else                                        //w przeciwnym wypadku szukaj normalnie
    {
        for(iLicznik=0;iLicznik<ileElementow;iLicznik++)
        {
            nowyAdres=nowyAdres->nastepny;
        }
    }
    return nowyAdres;
}

