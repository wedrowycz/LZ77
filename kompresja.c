#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naglowek.h"

//Glowna funkcja do kompresji pobiera w parametrach nazwy plikow wej i wyj oraz parametry okna
void kompresja(char nazwaPlikOdczyt[],char nazwaPlikZapis[], int slownik, int bufor,int okno)
{
        /* Deklaracje zmiennych zwiazanych z plikami */
    FILE *wskPlikOdczyt,*wskPlikZapis;
	errno_t openstatus;
    /*Inne zmienne */
    struct elementListy *poczatekListy, *dictWsk, *buffWsk;
    struct dopasowanie rekord;
    /* Otwieranie plikow, jeden do odczytu, drugi do zapisu */
	openstatus = fopen_s(&wskPlikOdczyt,nazwaPlikOdczyt, "r");
    if(wskPlikOdczyt==NULL)   //Sprawdzanie czy prawidlowo otwarto plik do odczytu
    {
        printf("Nie mozna otworzyc pliku do odczytu !!!\n");
        wyswietlPomoc();
        exit(0);
    }
	openstatus = fopen_s(&wskPlikZapis ,nazwaPlikZapis, "wb");
    if(wskPlikZapis==NULL)   //Sprawdzanie czy prawidlowo otwarto plik do zapisu
    {
        printf("Nie mozna otworzyc pliku do zapisu !!!\n");
        fclose(wskPlikOdczyt);
        wyswietlPomoc();
        exit(0);
    }
    /* Tworzenie listy */
    poczatekListy=budujListe(okno);
    /* Przesuwanie wskaznikow na poczatek slownika i poczatek bufora wejsciowego */
    dictWsk=poczatekListy;
    buffWsk=przesunWskaznik(dictWsk,slownik);
    /* Wypelnianie listy po raz pierwszy */
    wypelnijListe(dictWsk,wskPlikOdczyt,slownik,bufor);
    /*Tworzenie pierwszego zapisu w pliku wyjsciowym: (0,0,pierwszy znak z pliku wejsciowego) */
    rekord.start=0;
    rekord.dlugosc=0;
    rekord.znak=poczatekListy->znak;
    /* Zapisywanie pierwszego rekordu do pliku wyjsciowego */
    fwrite(&rekord,sizeof(rekord),1,wskPlikZapis);
/******************************************************************************************************/
    while(rekord.znak)
    {
    /* Szukamy najlepszego dopasowania */
        rekord=szukaj(dictWsk,buffWsk,slownik,bufor);
    /* Zapisujemy to co znalezlismy do pliku wyjsciowego */
        fwrite(&rekord,sizeof(rekord),1,wskPlikZapis);
    /* Uzupelniamy listy o dlugosc najlepszego dopasowania + 1 */
        uzupelnijListe(dictWsk,rekord.dlugosc+1,wskPlikOdczyt);
    /* Przesuwamy oczywiscie tez nasze wskazniki o tyle samo miejsc */
        dictWsk=przesunWskaznik(dictWsk,rekord.dlugosc+1);
        buffWsk=przesunWskaznik(buffWsk,rekord.dlugosc+1);
    }
/******************************************************************************************************/
    /* Zwalniamy pamiec zajmowana przez liste */
    kasujListe(poczatekListy,okno);
    /* Zamykamy pliki */
    fclose(wskPlikOdczyt);
    fclose(wskPlikZapis);
    printf("Program zostal prawidlowo wykonany !!!\n");
}
//Funkcja wyszukuje najlepsze dopasowanie w slowniku metoda brute-force search
struct dopasowanie szukaj(struct elementListy *dictWsk, struct elementListy *buffWsk, int slownik, int bufor)
{
    struct elementListy *dictTempWsk,*dictTemp1Wsk, *buffTempWsk;
    int iSlownika,iBufora;
    int dlugosc;                                    //Ilosc dopasowanych znakow (dlugosc podciau)
    struct dopasowanie rekord;                      //Zawiera informacje o dopasowanym ciagu, funkcja go zwroci
    rekord.dlugosc=0;                               //Inicjalizujemy zerem dla porzadku*/
    rekord.start=0;                                 //Inicjalizujemy zerem dla porzadku*/
    rekord.znak=buffWsk->znak;                      //Inicjalizujemy pierwszym elementem bufora jakby nic nie znalazlo
    dictTemp1Wsk=dictWsk;
    for(iSlownika=0;iSlownika<slownik;iSlownika++)  //Dla kazdego znaku ze slownika szuka dopasowania w buforze
    {
        dictTempWsk=dictTemp1Wsk;
        buffTempWsk=buffWsk;
        dlugosc=0;
        if(dictTempWsk->znak==buffTempWsk->znak)            //Zaczyna od poczatku
        {
            dlugosc=1;                                      //Pierwszy znak pasuje, szuka dalej
            for(iBufora=1;iBufora<bufor;iBufora++)          //Zaczyna od drugiego bo pierwszy juz pasuje
            {
                dictTempWsk=dictTempWsk->nastepny;
                buffTempWsk=buffTempWsk->nastepny;
                if(dictTempWsk->znak==buffTempWsk->znak)    //Kolejny znak pasuje
                {
                    ++dlugosc;                              //Inkrementujemy dlugosc
                }
                else
                {
                    break;                                  //Wychodzimy poziom wyzej jak nie pasuje
                }
            }
        }
        if(dlugosc>rekord.dlugosc)                 //Jezeli znalazlo lepsze dopasowanie to przepisuje struktury
        {
            rekord.dlugosc=dlugosc;
            rekord.start=iSlownika;
            rekord.znak=przesunWskaznik(buffWsk,dlugosc)->znak;
            if(dlugosc==bufor)           //To na wypadek gdyby dopasowanie bylo pelne czyli nie mozna juz znalezc lepszego
            {
                break;
            }
        }
        dictTemp1Wsk=dictTemp1Wsk->nastepny;
    }
    return rekord;                                 //Zwraca strukture zawierajaca opis najlepszego dopasowania
}
//Funkcja wypelnia liste pierwszym znakiem z pliku .txt ktory mamy skompresowac
void  wypelnijListe(struct elementListy *poczatekListy, FILE *wskPlik, int slownik, int bufor)
{
    int iLicznik;
    unsigned char cZnak;
    struct elementListy *temp;
    temp=poczatekListy;
    cZnak=getc(wskPlik);
    while((cZnak==239) ||(cZnak==187)||(cZnak==191))
    {
        cZnak=getc(wskPlik);
    }
    for(iLicznik=0;iLicznik<slownik;iLicznik++)
    {
        temp->znak=cZnak;
        temp=temp->nastepny;
    }
    for(iLicznik=0;iLicznik<bufor;iLicznik++)
    {
        temp->znak=getc(wskPlik);
        temp=temp->nastepny;
    }
}
//Funkcja uzupelnia okno o kolejne znaki z pliku
void uzupelnijListe(struct elementListy *poczatekListy, int ileElementow, FILE *wskPlik)
{
    int iLicznik;
    struct elementListy *temp;
    unsigned char cZnak;
    temp=poczatekListy;
    for(iLicznik=0;iLicznik<ileElementow;iLicznik++)
    {
         if((cZnak=getc(wskPlik))==(unsigned char)EOF)    /*wazne bo jak bedzie bez rzutowania to unfinite loop*/
        {
            temp->znak=0;
        }
        else
        {
            temp->znak=cZnak;
        }
        temp=temp->nastepny;
    }
}
//Przesuwanie wskaznika w liscie w jednym kierunku tym razem
struct elementListy *przesunWskaznik(struct elementListy *adresElementu, int ileElementow)
{
    struct elementListy *nowyAdres;
    int iLicznik ;
    nowyAdres=adresElementu;
    for(iLicznik=0;iLicznik<ileElementow;iLicznik++)
    {
        nowyAdres=nowyAdres->nastepny;
    }
    return nowyAdres;
}
