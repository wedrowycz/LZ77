#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "naglowek.h"
/*Parametry okna, najczesciej proponowane w materialach na ktorych sie opieralem*/
#define DICT 32000  //Slownik
#define BUFF 256    //Bufor    tez dlatego zeby sie zmiescil w unsigned char a nie integer 1 bajt zamiast 2
#define WINDOW (DICT+BUFF) //Cale okno czyli slownik+bufor


int main(int argc, char *argv[])
{
    int parametry[3]={-1,-1,-1};
//parametry[0]:1-kodowanie,2-dekodowanie; parametry[1] - nazwa pliku wejsciowego; parametry[2] - nazwa pliku wyjsciowego
    if(podzielPolecenia(argc,argv,parametry)) //Jezeli sprawdzanie parametrow dalo pozytywny wynik
    {
        if(parametry[0]==1)             //KOMPRESJA
        {
            kompresja(argv[parametry[1]],argv[parametry[2]],DICT,BUFF,WINDOW);
        }
        else if(parametry[0]==2)        //DEKOMPRESJA
        {
            dekompresja(argv[parametry[1]],argv[parametry[2]],DICT);
        }
    }
    return 0;
}
