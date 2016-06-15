#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "naglowek.h"
//Wspolne funkcje dla kompresji i dekompresji plus dodatkowe luzne
//Tworzy liste cykliczna dwukierunkowa, zwraca wskaznik na pierwszy element listy
struct elementListy *budujListe(int ileElementow)
{
/*Funkcja zwraca wskaznik na poczatek dwukierunkowej listy cyklicznej*/
/*Lista tworzona jest poprzez dodawanie na koncu kolejnego elementu*/
	struct elementListy *poczatekWsk;
	struct elementListy *wsk = NULL;
	struct elementListy *temp;
    int iLicznik;
/*Tworzenie pierwszego elementu*/
    poczatekWsk=malloc(sizeof(struct elementListy));
    temp=poczatekWsk;
    /*Tworzenie pozostalych elementow*/
    for(iLicznik=1;iLicznik<ileElementow;iLicznik++)
    {
        wsk=malloc(sizeof(struct elementListy));
        wsk->poprzedni=temp;
        temp->nastepny=wsk;
        temp=wsk;
    }
/*Laczenie elementu ostatniego z pierwszym*/
	if (wsk != NULL)
	{
		wsk->nastepny = poczatekWsk;
		poczatekWsk->poprzedni = wsk;
	}
/*Zwracanie wskaznika na pierwszy element listy*/
    return poczatekWsk;
}
//Funkcja zwalnia pamiec zarezerwowana dla listy
void kasujListe(struct elementListy *poczatekListy, int ileElementow)
{
/*Funkcja zwalnia pamiec zajmowana przez liste cykliczna*/
    struct elementListy *temp1, *temp2;
    int iLicznik;
    temp1=poczatekListy;
    for(iLicznik=0;iLicznik<ileElementow;iLicznik++)
    {
        temp2=temp1->nastepny;
        free(temp1);
        temp1=temp2;
    }
}
//Funkcja wyswietla pomoc
void wyswietlPomoc(void)
{
    printf("Parametry wywolania programu:\n");
    printf("-c : kompresja\n");
    printf("-d : dekompresja\n");
    printf("-f : nazwa pliku wejsciowego\n");
    printf("-o : nazwa pliku wyjsciowego\n");
    printf("-h : wyswietla pomoc\n");
    printf("Parametry moga byc podane w dowolnej kolejnosci ale\n");
    printf("po parametrach wywolania -f i -o nalezy podac nazwy plikow\n"); /*Mysle ze to sensowne zalozenie*/
    printf("Przyklad: LZ77 -c -f plik.txt -o plik.dat\n");
}
/******************************************************************************************************/
//Funkcje do sprawdzania poprawnosci wprowadzonych parametrow
/******************************************************************************************************/
//Funkcja wyszukuje przelaczniki i zwraca ktory to przelacznik
int ktorePolecenie(char argument[])
{
    int iLicznik,polecenie=-1;
    char *tablicaPrzelacznikow[]={"-c","-d","-f","-o"};
    for(iLicznik=0;iLicznik<4;iLicznik++)
    {
        if(!strcmp(tablicaPrzelacznikow[iLicznik],argument))
        {
            polecenie=iLicznik;
            break;
        }
    }
    return polecenie;
}
//Funkcja w zaleznosci od tego ktory to przelacznik uzupelnia tablice
void dopasujPolecenia(char *argv[],int parametry[])
{
    int iLicznik;
    int polecenie;
    for(iLicznik=1;iLicznik<6;iLicznik++)       /*Przeszukuje wektor argumentow*/
    {
        polecenie=ktorePolecenie(argv[iLicznik]);
        if(polecenie==0&&parametry[0]==-1)            //to na wypadek gdyby przelaczniki sie powtorzyly
        {
            parametry[0]=1;
        }
        else if(polecenie==1&&parametry[0]==-1)
        {
            parametry[0]=2;
        }
        else if(polecenie==2&&parametry[1]==-1)     //nazwa pliku wejsciowego powinna byc po tym parametrze
        {                                           //ale czy tak jest to dopiero w programie sie okaze czy to prawidlowa nazwa
            parametry[1]=iLicznik+1;
            iLicznik++;
        }
        else if(polecenie==3&&parametry[2]==-1)     //to samo co wyzej
        {
            parametry[2]=iLicznik+1;
            iLicznik++;
        }
        else
        {
            break;  //Jezeli cos pojdzie nie tak to wektor tab bedzie nieprawidlowy
        }
    }
}
//Funkcja sprawdza liczbe podanych parametrow w zaleznosci od tego ile jest parametrow podejmuje rozne dzialania
int podzielPolecenia(int argc, char *argv[], int parametry[])
{
    int prawidloweParametry=0;
    if(argc==2)
    {
        if(!strcmp(argv[1],"-h"))                   //Program wywolany z jednym parametrem tzn. poza nazwa
        {
            wyswietlPomoc();
        }
        else
        {
            printf("Podales jeden parametr i do tego nieprawidlowy\n"); //To chyba nie jest potrzebne
            wyswietlPomoc();
        }
    }
    else if(argc==6)                                //Jest 6 parametrow czyli prawidlowa ilosc to probujemy je dopasowac
    {
        dopasujPolecenia(argv,parametry);
        prawidloweParametry=sprawdzajPolecenia(parametry,argv);
        if (!prawidloweParametry)
        {
            wyswietlPomoc();
        }
    }
    else
    {
        printf("Nieprawidlowa liczba parametrow !!!\n");
        wyswietlPomoc();
    }
    return prawidloweParametry;
}
int sprawdzajPolecenia(int parametry[],char *argv[])   //finalne sprawdzanie przed uruchomieniem
{
    int prawidloweParametry=1;
    int iLicznik;
    for(iLicznik=0;iLicznik<3;iLicznik++)
    {
        if(parametry[iLicznik]==-1)
        {
            prawidloweParametry=0;
            printf("Nieprawidlowe parametry !!!\n");
        }
    }
    if(prawidloweParametry)
    {
        if(parametry[0]==1)   //czyli kompresja
        {
            if(!((strstr(argv[parametry[1]],".txt"))&&(strstr(argv[parametry[2]],".dat"))))//sprawdzamy czy nazwy plikow sa prawidlowe
            {
                prawidloweParametry=0;
                printf("Nieprawidlowa nazwa plikow !!!\n");
            }
        }
        else if(parametry[0]==2) //czyli dekompresja
        {
            if(!((strstr(argv[parametry[1]],".dat"))&&(strstr(argv[parametry[2]],".txt"))))//sprawdzamy czy nazwy plikow sa prawidlowe
            {
                prawidloweParametry=0;
                printf("Nieprawidlowa nazwa plikow !!!\n");
            }
        }
    }
    return prawidloweParametry;  //1-OK 0-nie OK
}
