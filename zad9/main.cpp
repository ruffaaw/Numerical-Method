#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;

const double p = 1.0, q = 0.0, r = -4.0; // współczynniki równania
const double alfa = 0.0, beta = 1.0, gamma = -1.0; // współczynniki z warunków brzegowych
const double phi = 0.0, psi = 1.0, theta = 0.0; //psi jest tutaj dowolne
const double start = 0.0, end_ = 1.0; //granice przedziałów double

int S(double x){ return -x; }

double analityczne(double x){ //równanie analityczne
    return ( exp( 2.0-2.0*x ) - 4*exp( 4.0 - x * 2.0 )+ 4*exp( x * 2.0 ) - exp( 2.0 + 2.0*x ) - x + x*exp(4.0)) / ( 4.0 - 4*exp(4.0) );
}

void thomas(double *l, double *d, double *u, double *b, double *x, int N){ //algorytm
//Thomasa dla zadanych wektorów
    double *tempB = new double[N], *tempD = new double[N];  //tworzymy tymczasowe zmienne

    tempD[0] = d[0]; //przepisanie pierwszego elementu
    tempB[0] = b[0];

    for (int i = 1; i < N; i++){ // przekształcenia algorytmu Thomasa
        tempD[i] = d[i] - l[i - 1] * (u[i - 1] /tempD[i - 1]);
    }

    for (int i = 1; i < N; i++){
        tempB[i] = b[i] - l[i - 1] * tempB[i - 1] / tempD[i - 1];
    }

    x[N - 1] = tempB[N - 1] / tempD[N - 1];

    for (int i = N - 2; i >= 0; i--){
        x[i] = (tempB[i] - u[i] * x[i + 1]) / tempD[i];
    }
}

int maxDifference(double *blad, int N){ //wyznaczamy największy błąd
    double max = fabs(blad[0]); //zakładamy max jako pierwszy element, później sprawdzamy ew. zmiany
    int indexMax;
    for (int i = 0; i < N; i++)
        if (fabs( blad[i] ) > max){
            max = fabs(blad[i]); indexMax=i;
        }
    return indexMax;
}

double numerow( double h, int N )
{
    double *l, *d, *u, *b, *x, *blad;
    double x1 = start, x2 = start;

    fstream fileNumerow, fileAnalitycznie;
    fileNumerow.open( "resultsNumerow.txt", ios_base::app );
    fileAnalitycznie.open( "resultsAnalitycznie.txt", ios_base::app);
    fileAnalitycznie << scientific, fileNumerow << scientific;

    l = new double[N], d = new double[N], u = new double[N], b = new double[N], x = new double[N], blad = new double[N];

    u[0] = alfa / h; // uzupełniamy wartości pierwszych wyrazów
    d[0] = beta - alfa / h;
    b[0] = -gamma;

    for(int i = 1; i < N - 1; i++) // obliczamy wyrazy środkowe
    {
        l[i - 1] = p / (h * h) + r / 12.0;
        d[i] = (-2.0 * p) / (h * h) + r * (10.0 / 12.0); u[i] = p / (h * h) + r / 12.0;
        b[i] = -S(x1 + h * i);

    }

    l[N - 2] = -phi / h; // końcowe wyrazy
    d[N - 1] = -phi / h + psi;
    b[N - 1] = -theta;

    thomas( l, d, u, b, x, N ); //wykonujemy algorytm Thomasa

    for (int i = 0; i < N; i++){ // liczymy tutaj ile wynosi błąd, czyli różnica między Numerowem a rozw. analitycznym
        blad[i] = fabs( x[i] - analityczne(x2) );
        x2 += h;
    }
    int maxIndexDifference = maxDifference(blad, N); //znajdujemy największy błąd

    if(N==1002){ //dla przykładowego N zapisujemy dane do pliku
        for (int i = 0; i < N; i++){ fileNumerow<<x1<<"\t"<<x[i]<<"\t"<<endl;
            fileAnalitycznie<<x1<<"\t"<<analityczne(x1)<<endl;
            x1 += h;
        }
    }

    fileAnalitycznie.close();
    fileNumerow.close();
    return blad[maxIndexDifference]; // tutaj zwracany jest największy błąd
}

double konwencjonalnaTrzypunktowa(double h, int N) //funkcja do realizacji dyskretyzacji konwencjonalnej trzypunktowej
{
    double *l, *d, *u, *b, *x, *blad;
    double x1 = start;
    double x2 = start;

    fstream fileKonwencjonalnie;
    fileKonwencjonalnie.open( "resultsKonwencjonalnie.txt", ios_base::app);
    fileKonwencjonalnie << scientific;
    l = new double[N], d = new double[N], u = new double[N], b = new double[N], x = new double[N], blad = new double[N];

    u[0] = alfa / h;
    d[0] = beta - alfa / h;
    b[0] = -gamma;

    for ( int i = 1; i < N - 1; i++ )
    {
        l[i - 1] = p / ( h * h ) - q / ( 2.0 * h );
        d[i] = ( -2.0 * p ) / ( h *h ) + r;
        u[i] = p / ( h * h ) + q / ( 2.0 * h );
        b[i] = (x1+i*h); // s
    }

    l[N - 2] = -phi / h;
    d[N - 1] = -phi / h + psi;
    b[N - 1] = -theta;

    thomas( l, d, u, b, x, N );

    for ( int i = 0; i < N; i++ ){ // liczymy ile wynosi błąd
        blad[i] = fabs( x[i] - analityczne(x2) );
        x2 += h;
    }

    int maxIndexDifference = maxDifference(blad, N);
    if(N==1002){
        for ( int i = 0; i < N; i++ ){
            fileKonwencjonalnie << x1 << "\t" << x[i] << endl;  //zapis do pliku
            x1 += h;
        }
    }

    return blad[maxIndexDifference]; // tutaj zwracany jest największy błąd
}

int main()
{
    double h, diffNumerow, diffKonwencjonalnie;
    int N; // ilość iteracji

    fstream fileKonw, fileNumerow;
    fileKonw.open( "resultsDiffKonwencjonalnie.txt", fstream::out );
    fileNumerow.open( "resultsDiffNumerow.txt", fstream::out );
    fileKonw << scientific; //ustawienie precyzji
    fileNumerow << scientific;

    for ( N = 2 ; N < 100000; N += 100){
        h = ( end_ - start ) / ( N-1 ); // obliczamy krok h

        diffKonwencjonalnie = ( konwencjonalnaTrzypunktowa( h, N ) ); //obliczamy blad z konwencjonalnej trzypunktowej
        diffNumerow =( numerow( h, N ) ); // obliczamy blad z Numerowa

        fileKonw<<log10(h)<<"\t"<<log10(diffKonwencjonalnie)<<endl; // log10 z h od bledu z Konwencjonalnej
        fileNumerow<<log10(h)<<"\t"<<log10(diffNumerow)<<endl;
    }
    fileNumerow.close();
    fileKonw.close();
    return 0;
}


