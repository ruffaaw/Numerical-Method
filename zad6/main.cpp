/**
 * Napisz program w języku „C/C++”, realizujący algorytm Thomasa dla macierzy trój-diagonalnej o
dowolnych rozmiarach N  N, a następnie zastosuj ten program do rozwiązania układu równań
Ax = b, w którym
     |10     1/2                                 |
     |1/3    20      1/4                         |
 A = |       1/5     30      1/6                 |
     |               1/7     30      1/8         |
     |                       1/9     20      1/10|
     |                               1/11    10  |

     |31     |
     |165/4  |
 b = |917/30 |
     |851/28 |
     |3637/90|
     |332/11 |
Program należy zrealizować w postaci dwóch odrębnych procedur: jednej, która operuje wyłącznie
na macierzy A, i drugiej, która operuje wyłącznie na wektorze b, korzystając z wyników działania
procedury pierwszej.
Uwaga: ponieważ macierz trój-diagonalna jest macierzą rzadką, więc w programie NIE NALEŻY
używać tablic kwadratowych do reprezentacji macierzy A.

 */
#include <iostream>
#define N 6

using namespace std;

//Procedura operujaca na macierzy A:
void algThomasA(double *u, double *d, double *l, double*n, int size)
{
    n[0] = d[0];
    for (int i = 1; i < size + 1; i++)
        n[i] = d[i] - l[i - 1] * u[i - 1] / n[i - 1];
}

//Procedura operujaca na wektorze b:
void algThomasB(double *u, double *d, double *l, double *b, double *x, double*n, double*r, int size)
{
    r[0] = b[0];
    for (int i = 1; i < size + 1; i++)
        r[i] = b[i] - l[i - 1] * r[i - 1] / n[i - 1];

    //Obliczenie wyniku:
    x[size] = r[size] / n[size];
    for (int i = size - 1; i >= 0; i--)
        x[i] = (r[i] - u[i] * x[i + 1]) / n[i];
}

void uzupelnijWektor(double*u, double*d, double*l, double*b)
{
    u[0] = 1.0 / 2.0; u[1] = 1.0 / 4.0; u[2] = 1.0 / 6.0; u[3] = 1.0 / 8.0; u[4] = 1.0 / 10.0;
    d[0] = 10.0; d[1] = 20.0; d[2] = 30.0; d[3] = 30.0; d[4] = 20.0; d[5] = 10.0;
    l[0] = 1.0 / 3.0; l[1] = 1.0 / 5.0; l[2] = 1.0 / 7.0; l[3] = 1.0 / 9.0; l[4] = 1.0 / 11.0;
    b[0] = 31.0; b[1] = 165.0 / 4.0; b[2] = 917.0 / 30.0; b[3] = 851.0 / 28.0; b[4] = 3637.0 / 90.0; b[5] = 332.0 / 11.0;
}

void pokazWektor(double *vector, int size)
{
    cout << endl << endl;
    for (int i = 0; i < size; i++)
        cout << vector[i] << endl;
    cout << endl;
}

void usunWektor(double *vector)
{
    delete[]vector;
}
int main()
{
    double *u, *d, *l, *b, *x, *n, *r;

    u = new double[N - 1];  //Wartosci nad przekątną
    d = new double[N];      //Przekątna
    l = new double[N - 1];  //Wartosci pod przekątną
    b = new double[N];      //Dany wektor b
    x = new double[N];      //Rozwiazanie
    n = new double[N];      //eta
    r = new double[N];

    uzupelnijWektor(u, d, l, b);
    algThomasA(u, d, l, n, N-1);
    algThomasB(u, d, l, b, x, n, r, N - 1);

    cout << "DANE:" << endl;
    cout << endl << "Wektor l (Wartosci nad przekatna macierzy A):";
    pokazWektor(l, N-1);
    cout << endl << "Wektor d (Wartosci na przekatnej macierzy A):";
    pokazWektor(d, N);
    cout << endl << "Wektor u (Wartosci pod przekatna macierzy A):";
    pokazWektor(u, N-1);
    cout << endl << "Wektor b:";
    pokazWektor(b, N);

    cout << endl << endl << "Realizacja Algorytmu Thomasa:" << endl;
    cout << endl << "Wektor n (eta) - wyznaczony przez procedure operujaca na macierzy A:";
    pokazWektor(n, N);
    cout << endl << "Wektor r (przeksztalcony wektor b) - wyznaczony przez procedure operujaca na wektorze b:";
    pokazWektor(r, N);
    cout << endl << "Wektor x (rozwiazanie):";
    pokazWektor(x, N);

    usunWektor(l);
    usunWektor(d);
    usunWektor(u);
    usunWektor(b);
    usunWektor(x);

    return 0;
}

