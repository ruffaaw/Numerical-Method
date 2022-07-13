/**
 * Napisz program w języku „C/C++”, obliczający przybliżone wartości pierwszych pochodnych
funkcji f(x) = sin(x) w punktach końcowych i środkowym przedziału [0, /2] zmiennej x. Zastosuj
wszystkie omawiane na wykładzie i na ćwiczeniach przybliżenia różnicowe dwupunktowe i
trzypunktowe (jednostronne bądź centralne, w zależności od położenia punktu w przedziale) na sieci
jednorodnej o kroku h. Wykonaj (na jednym rysunku) wykresy przedstawiające zależności błędów
bezwzględnych przybliżeń różnicowych od kroku sieci, posługując się skalą logarytmiczną (tzn.
wykresy zależności log10|błędu| od log10 h ). Na podstawie wykresów wyznacz doświadczalnie rzędy
dokładności przybliżeń różnicowych. Sprawdź, czy tak wyznaczone rzędy dokładności pokrywają
się z rzędami teoretycznymi i wyjaśnij ewentualne rozbieżności. Ponadto zidentyfikuj wartości
kroku sieci poniżej których pojawia się wpływ błędów maszynowych. Obliczenia powtórz dla
dwóch typów zmiennych rzeczywistych (float, i double) i porównaj wyniki.
Uwaga: najwygodniej jest zastosować wzorzec funkcji (function template) z typem zmiennych jako
parametrem wzorca.
 */
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <typeinfo>

using namespace std;

int krokow = 10;
int metod = 7;

template <class type>
type roznica_progresywna(type x, type h){
    return (sin(x + h) - cos(x)) / h;
}

template <class type>
type roznica_wsteczna(type x, type h){
    return (sin(x) - cos(x - h)) / h;
}

template <class type>
type roznica_centralna(type x, type h){
    return (sin(x + h) - sin(x - h)) / (2.0 * h);
}

template <class type>
type roznica_progresywna_3p(type x, type h){
    return (-3.0 * (sin(x)) + 4.0 * cos(x + h) -  (cos(x + 2.0*h))) / (2.0 * h);
}

template <class type>
type roznica_wsteczna_3p(type x, type h){
    return (sin(x - 2.0*h) - 4.0 * sin(x - h) + 3.0 * (sin(x))) / (2.0 * h);
}

template <class type>
type pochodna(type x){
    return cos(x);
}

template <class type>
type licz_roznica (type *tablicaKrokow, type **tablicaWynikow){
    type const poczatek = 0.0;
    type const srodek = M_PI / 4;
    type const koniec = M_PI / 2;
    type krok = 0.1;

    for(int i = 0; i < 10; i++){
        tablicaKrokow[i] = krok;
        tablicaWynikow[0][i] = fabs(pochodna(srodek) - roznica_progresywna(srodek, krok));
        tablicaWynikow[1][i] = fabs(pochodna(poczatek) - roznica_progresywna(poczatek, krok));
        tablicaWynikow[2][i] = fabs(pochodna(srodek) - roznica_wsteczna(srodek, krok));
        tablicaWynikow[3][i] = fabs(pochodna(koniec) - roznica_wsteczna(koniec, krok));
        tablicaWynikow[4][i] = fabs(pochodna(srodek) - roznica_centralna(srodek, krok));
        tablicaWynikow[5][i] = fabs(pochodna(poczatek) - roznica_progresywna_3p(poczatek,  krok));
        tablicaWynikow[6][i] = fabs(pochodna(koniec) - roznica_wsteczna_3p(koniec, krok));
        krok /= 10.0;
    }
}

template <class type>
type zapis(type * tablicaKrokow,type ** tablicaWynikow){
    fstream plik;
    string nazwa = "Typ_";

    nazwa += typeid(type).name();
    nazwa += ".txt";

    plik.open(nazwa.c_str(), fstream::out);

    for(int i=0;i<10;i++)
        plik << tablicaKrokow[i] << endl;
    plik << endl;

    for(int i=0;i<7;i++){
        for(int j=0;j<10;j++)
            plik << tablicaWynikow[i][j] << endl;
        plik << endl;
    }
    plik.close();
}

/* PROGRAM GLOWNY */
int main(int argc, char *argv[]){

    float *krokiFloat;
    double *krokiDouble;

    krokiFloat = new float [krokow];
    krokiDouble = new double [krokow];

    float **wynikiFloat;
    double **wynikiDouble;

    wynikiFloat = new float *[metod];
    wynikiDouble = new double *[metod];

    for(int i=0;i<metod;i++){
        wynikiFloat[i] = new float [krokow];
        wynikiDouble[i] = new double [krokow];
    }

    licz_roznica(krokiFloat, wynikiFloat);
    licz_roznica(krokiDouble, wynikiDouble);

    zapis(krokiFloat, wynikiFloat);
    zapis(krokiDouble, wynikiDouble);

    return 0;
}
