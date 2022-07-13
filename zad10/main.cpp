/**
 * Napisz program w języku „C/C++”, rozwiązujący równanie różniczkowe z zadania 1, za pomocą
metod:
(a) bezpośredniej Eulera
(b) pośredniej Eulera
(c) metody trapezów.
Dla metod (b) i (c) wykonaj oddzielne rysunki przedstawiające po dwa wykresy: wykres
przykładowego rozwiązania numerycznego oraz (dla porównania) wykres rozwiązania
analitycznego: y(t) = 1  exp{10[t + arctg(t)]}. Oba wykresy winny przedstawiać zależność y od
zmiennej niezależnej t. Rozwiązania analityczne zaznacz linią ciągłą, a numeryczne punktami. W
przypadku metody (a) wykonaj dwa takie rysunki: jeden uzyskany w warunkach numerycznej
stabilności metody, a drugi w warunkach numerycznej niestabilności. Wyjaśnij różnice pomiędzy
uzyskanymi wykresami.
Pokaż, że rząd dokładności uzyskanych stabilnych rozwiązań numerycznych jest zgodny z
przewidywaniami teoretycznymi. W tym celu wykonaj (na jednym rysunku) wykresy
przedstawiające zależności maksymalnych błędów bezwzględnych rozwiązań uzyskanych trzema
metodami, od kroku sieci czasowej t, posługując się skalą logarytmiczną (tzn. wykresy zależności
log10|błędu| od log10 t ). Na podstawie wykresów wyznacz doświadczalnie rzędy dokładności
rozwiązań uzyskanych za pomocą różnych metod i porównaj je z rzędami teoretycznymi. O ile to
możliwe, zidentyfikuj też wartości kroku sieci poniżej których pojawia się wpływ błędów
maszynowych.
 */
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double funkcjaBezposrednia(double dt, double tk, double yk) {return yk + dt * (- ((10.*tk*tk + 20.)/(tk*tk + 1)) * (yk - 1.));}
double funkcjaAnalityczna(double tk) {return 1 - exp(-10.0 * (tk + atan(tk)));}
double funkcjaPosrednia(double dt,double tk, double y_prev){
    double y, temp;
    temp = ( 10.0 * ( tk + dt ) * ( tk + dt ) + 20.0 ) / ( ( tk + dt ) * ( tk + dt
    ) + 1.0 );
    y = ( y_prev + dt * temp ) / ( 1 + dt * temp );
    return y;
}

double funkcjaPosredniaBlad(double dt){
    double y, diff, tk = 0, maxDiff = 0.0, y_poprzednie = 0.0;

    while(tk < 1.0){
        y = funkcjaPosrednia(dt,tk, y_poprzednie);
        diff = fabs(funkcjaAnalityczna(tk) - y_poprzednie); y_poprzednie = y;
        if ( diff > maxDiff ) maxDiff = diff;
        tk += dt;
    }
    return maxDiff;
}

double funkcjaBezposredniaBlad(double dt){
    double y, maxDiff = 0.0, diff, tk = 0, y_poprzednie = 0.0;
    while(tk < 1.0){
        y = funkcjaBezposrednia(dt,tk,y_poprzednie);
        diff = fabs(funkcjaAnalityczna(tk) - y_poprzednie); y_poprzednie = y;
        if ( diff > maxDiff )
            maxDiff = diff;
        tk += dt;
    }
    return maxDiff;
}


double funkcjaTrapezow(double dt, double tk, double y_poprzedni){
    double y, tempf, tempf1, i;
    while(i < tk) {
        tempf = ((10.0 * i * i + 20.0) / (i * i + 1.0)); //część f(tk,yk)
        tempf1 = (10.0 * (i + dt) * (i + dt) + 20.0) / ((i + dt) * (i + dt) + 1.0);
        y = ((-dt / 2.0) * (tempf * (y_poprzedni - 1.0) - tempf1) + y_poprzedni) / (1.0
                                                                                    + (dt / 2.0) * tempf1);
        i += dt;
    }
    return y;
}

double funkcjaTrapezowBlad(double dt){
    double diff, tk = 0, y, maxDiff = 0.0, y_poprzedni = 0.0;

    while(tk < 1.0){
        y = funkcjaTrapezow(dt, tk , y_poprzedni);
        diff= fabs(funkcjaAnalityczna(tk)-y_poprzedni); y_poprzedni = y;
        if ( diff > maxDiff )
            maxDiff = diff;
        tk += dt;
    }
    return maxDiff;
}

int main()
{
    fstream diffBezposrednia, diffPosrednia, diffTrapezow, resultsBezposredniaSTB, resultsPosrednia, resultsTrapezow,
            resultsAnalitycznie, resultsBezposredniaNST; //tworzenie plików

    diffBezposrednia.open( "diffBezposrednia.txt", fstream::out ); // otwieramy do zapisu
    diffPosrednia.open( "diffPosrednia.txt", fstream::out );
    diffTrapezow.open( "diffTrapezow.txt", fstream::out );

    resultsBezposredniaSTB.open( "resultsBezposredniaSTB.txt", fstream::out );
    resultsPosrednia.open( "resultsPosrednia.txt", fstream::out );
    resultsTrapezow.open( "resultsTrapezow.txt", fstream::out );
    resultsAnalitycznie.open( "resultsAnalitycznie.txt", fstream::out );
    resultsBezposredniaNST.open( "resultsBezposredniaNST.txt", fstream::out );

    resultsBezposredniaSTB<< scientific; //ustawienie precyzji
    resultsPosrednia<< scientific;
    resultsTrapezow<< scientific;
    resultsAnalitycznie<< scientific;
    resultsBezposredniaNST<< scientific;

    diffBezposrednia<< scientific;
    diffPosrednia<< scientific;
    diffTrapezow<< scientific;

    double dt = 0.005, y_nastepne, yn = 0, tk = 0, blad, y_poprzednie;

//analityczna
    while(tk < 5.0){
        y_nastepne = funkcjaAnalityczna(tk);
        resultsAnalitycznie << tk << " " << y_nastepne << endl;
        tk += dt;
    }

    tk = 0;

    //bezpośrednia - stabilna
    while(tk < 1.0){
        y_nastepne = funkcjaBezposrednia(dt, tk, yn);
        yn = y_nastepne;
        resultsBezposredniaSTB << tk << " " << y_nastepne << endl; tk += dt;
    }

    dt = 0.2, yn = 0, tk = 0;

//bezpośrednia - niestabilna
    while(tk < 5.0){
        y_nastepne = funkcjaBezposrednia(dt, tk, yn);
        resultsBezposredniaNST << tk << " " << y_nastepne << endl;
        yn = y_nastepne;
        tk += dt;
    }

    dt = 0.005, tk = 0;
    y_poprzednie = 0.0;
//pośrednia
    while(tk < 1.0){
        y_nastepne = funkcjaPosrednia(dt, tk, y_poprzednie);
        y_poprzednie = y_nastepne;
        resultsPosrednia << tk << " " << y_nastepne << endl;
        tk += dt;
    }

    tk = 0;

//trapezów

    y_poprzednie = 0.0;
    while(tk < 1.0){
        y_nastepne = funkcjaTrapezow(dt, tk, y_poprzednie);
        y_poprzednie = y_nastepne;
        resultsTrapezow << tk << " " << y_nastepne << endl;
        tk += dt;
    }

// liczymy błędy maksymalne
    dt=0.1;
    while(dt > 1e-14){
        blad = log10(funkcjaBezposredniaBlad(dt));
        diffBezposrednia << log10(dt) << " " << blad << endl;
        blad = log10(funkcjaPosredniaBlad(dt));
        diffPosrednia << log10(dt) << " " << blad << endl;
        blad = log10(funkcjaTrapezowBlad(dt));
        diffTrapezow << log10(dt) << " " << blad << endl;
        dt = dt/1.12;
    }

    resultsBezposredniaSTB.close(); //zamykamy pliki
    resultsPosrednia.close();
    resultsAnalitycznie.close();
    resultsBezposredniaNST.close();
    resultsTrapezow.close();

    diffBezposrednia.close();
    diffPosrednia.close();
    diffTrapezow.close();
}

