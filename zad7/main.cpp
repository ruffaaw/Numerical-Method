/**
 * Napisz program w języku „C/C++”, rozwiązujący układ czterech równań liniowych metodami
iteracyjnymi: (a) Jacobiego, (b) Gaussa-Seidela, (c) SOR z parametrem omega = 1/2, a następnie zastosuj
ten program do rozwiązania układu równań liniowych Ax = b, gdzie
     |100   -1      2       -3 |
     |1     200     -4      5  |
 A = |-2    4       300     -6 |
     |3     -5      6       400|

     |116  |
     |-226 |
 b = |912  |
     |-1174|

                                       |2|
 Przyjmij przybliżenie początkowe x0 = |2|.
                                       |2|
                                       |2|
 Zastosuj trzy niezależne kryteria zakończenia iteracji. Zadbaj o to, aby wyprowadzać na konsolę
wyniki pośrednie obliczeń dla każdej iteracji, tak aby możliwe było obserwowanie zbieżności
kolejnych przybliżeń pierwiastków i porównanie liczby iteracji niezbędnych do uzyskania (za
pomocą różnych metod) rozwiązania o zadanej dokładności bezwzględnej. W szczególności oblicz
jak zmienia się estymator błędu rozwiązania oraz residuum układu w trakcie kolejnych iteracji.
 */
#include<iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#define N 4

using namespace std;

const int ITERACJE = 50; //Maks. liczba iteracji
const double BLAD = 1e-7; //Tolerancja bledu
const double REZIDUUM = 1e-7; //Tolerancja reziduum
const double omegat = 0.5;
double **nowaMacierz(int n);
void usunMacierz(double **matrix, int n);
void wypelnijMarcierz(double **matrix, int n);
void wypiszMacierz(double **matrix, int n);
double *reziduum(double **matrix, double *x, double *b, int n);
double norma_maksimum(double *vector, int n);
double *nowyWektor(int n);
void wypelnijWektor(double *x, double *xtmp, int n);
void wypiszWektor(double *vector, int n);
void m_jacobiego(double **matrix, double *x, double *b, int n);
void m_gauus_seidel(double **matrix, double *x, double *b, int n);
void m_sor(double **matrix, double *x, double *b,double omega, int n);
double *roznica(double *a, double *b);

int main()
{
    double**matrix = nowaMacierz(N);
    double bt[N] = { 116.0, -226.0, 912.0, -1174.0 }, xt[N] = { 2.0, 2.0, 2.0, 2.0 };
    double *b = nowyWektor(N);
    double *x = nowyWektor(N);
    wypelnijMarcierz(matrix, N);
    wypelnijWektor(x, xt, N);
    wypelnijWektor(b, bt, N);
    cout << "Dane w zadaniu:\n";
    cout << "\nMacierz A:\n";
    wypiszMacierz(matrix, N);
    cout << "Wektor x:";
    wypiszWektor(x, N);
    cout << "\nWektor b:";
    wypiszWektor(b, N);

    cout << endl << "1. Metoda Jacobiego:" << endl << endl;
    cout << left << setw(20) << "|Iteracja|" << setw(20) << "|x1|" << setw(20) << "|x2|" << setw(20) << "|x3|" << setw(20) << "|x4|" << setw(20) << "|Estymator bledu|" << setw(20 ) << "|Reziduum|" << endl;
    m_jacobiego(matrix, x, b, N);

    wypelnijMarcierz(matrix, N);
    wypelnijWektor(x, xt, N);
    wypelnijWektor(b, bt, N);

    cout << endl << "2. Metoda Gaussa-Seidela:" << endl << endl;
    cout << left << setw(20) << "|Iteracja|" << setw(20) << "|x1|" << setw(20) << "|x2|" << setw(20) << "|x3|" << setw(20) << "|x4|" << setw(20) << "|Estymator bledu|" << setw(20) << "|Reziduum|" << endl;    m_gauus_seidel(matrix, x, b, N);

    wypelnijMarcierz(matrix, N);
    wypelnijWektor(x, xt, N);
    wypelnijWektor(b, bt, N);

    cout << endl << "3. Metoda SOR (Successive Over-Relaxation):" << endl << endl;
    cout << left << setw(20) << "|Iteracja|" << setw(20) << "|x1|" << setw(20) << "|x2|" << setw(20) << "|x3|" << setw(20) << "|x4|" << setw(20) << "|Estymator bledu|" << setw(20) << "|Reziduum|" << endl;    m_sor(matrix, x, b,omegat, N);

    return 0;
}

//Obliczanie reziduum R = Ax - b:
double *reziduum(double **matrix, double *x, double *b, int n)
{
    double sum = 0;
    double *r = new double[n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            sum += matrix[i][j] * x[j];
        }
        r[i] = sum - b[i];
        sum = 0;
    }
    return r;
}

double norma_maksimum(double *vector, int n)
{
    double maksimum = fabs(vector[0]);
    for (int i = 1; i < n; i++) {
        if (fabs(vector[i]) > maksimum)
            maksimum = fabs(vector[i]);
    }
    return maksimum;
}

double *roznica(double *a, double *b)
{
    double *wynik = new double[N];
    for (int i = 0; i < N; i++) {
        wynik[i] = a[i] - b[i];
    }
    return wynik;
}

//Metoda Jacobiego:
void m_jacobiego(double **matrix, double *x, double *b, int n)
{
    int l_iteracji = 1;
    double m = 0;
    double *Xn = new double[n];
    while (1)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                    //Wartosc (L+U)x:
                    m += matrix[i][j] * x[j];
            }
            //Aktualne przyblizenie:
            Xn[i] = (1.0 / matrix[i][i]) * (b[i] - m);
            m = 0;
        }
        l_iteracji++;

        cout << left << setw(20) << l_iteracji << setw(20) << Xn[0] << setw(20) << Xn[1] << setw(20) << Xn[2] << setw(20) << Xn[3] << setw(20) << norma_maksimum(roznica(Xn, x), N) << setw(20) << (norma_maksimum(reziduum(matrix, Xn, b, N), N)) << endl;

        //Kryteria zakończenia iteracji:
        //Norma wart. bezwgl. f. wektora resuidalnego dla kolejnych przyblizen || roznica miedzy kolejnymi przyblizeniami || skonczona liczba iteracji
        if (((norma_maksimum(reziduum(matrix, Xn, b, N), N)) < REZIDUUM) && norma_maksimum(roznica(Xn, x), N) < BLAD  &&  l_iteracji > ITERACJE)
            break;

        for (int i = 0; i < n; i++)
            x[i] = Xn[i];
    }
    cout << endl << "Rozwiazanie:";
    wypiszWektor(Xn, N);
}

//Metoda Gaussa Seidela:
void m_gauus_seidel(double **matrix, double *x, double *b, int n)
{
    int l_iteracji = 1;
    double *Xp = new double[n];
    double m = 0;
    while (1)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i != j)
                    m += matrix[i][j] * x[j];
            }
            Xp[i] = x[i];
            x[i] = (1.0 / matrix[i][i]) * (b[i] - m);
            m = 0;
        }
        l_iteracji++;
        cout << left << setw(20) << l_iteracji << setw(20) << x[0] << setw(20) << x[1] << setw(20) << x[2] << setw(20) << x[3] << setw(20) << norma_maksimum(roznica(x, Xp), N) << setw(20) << (norma_maksimum(reziduum(matrix, x, b, N), N)) << endl;

        //Kryteria zakonczenia iteracji:
        if (((norma_maksimum(reziduum(matrix, x, b, N), N)) < REZIDUUM) &&  norma_maksimum(roznica(x, Xp), N) < BLAD  &&  l_iteracji > ITERACJE)
            break;

    }
    cout << endl << "Rozwiazanie:";
    wypiszWektor(x, N);
}

//Metoda sukcesywnej relaksacji (SOR):
void m_sor(double **matrix, double *x, double *b, double omega,int n)
{
    int l_iteracji = 1;
    double *Xn = new double[n];
    double m = 0, k = 0;

    while (1)
    {
        for (int i = 0; i < n; i++)
        {
            k += (1.0 - omega) * x[i];
            for (int j = 0; j <= i - 1; j++)
            {
                m += matrix[i][j] * x[j];
            }
            for (int j = i + 1; j < n; j++)
            {
                m += matrix[i][j] * x[j];
            }
            Xn[i] = k + (omega* (1.0 / matrix[i][i]))*(b[i] - m);
            m = 0;
            k = 0;
        }
        l_iteracji++;
        cout << left << setw(20) << l_iteracji << setw(20) << Xn[0] << setw(20) << Xn[1] << setw(20) << Xn[2] << setw(20) << Xn[3] << setw(20) << norma_maksimum(roznica(Xn, x), N) << setw(20) << (norma_maksimum(reziduum(matrix, Xn, b, N), N)) << endl;

        //Kryteria zakonczenia iteracji:
        if (((norma_maksimum(reziduum(matrix, Xn, b, N), N)) < REZIDUUM) && norma_maksimum(roznica(Xn, x), N) < BLAD  &&  l_iteracji > ITERACJE)
            break;
        for (int i = 0; i < n; i++)
            x[i] = Xn[i];

    }
    cout << endl << "Rozwiazanie:";
    wypiszWektor(Xn, N);
}

double **nowaMacierz(int n)
{
    double **matrix;
    matrix = new double *[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new double[n];
    return matrix;
}

void wypelnijMarcierz(double **matrix, int n)
{
    double tmp[4][4] = { { 100.0, -1.0, 2.0, -3.0 },
                         { 1.0, 200.0, -4.0, 5.0 },
                         { -2.0, 4.0, 300.0, -6.0 },
                         { 3.0, -5.0, 6.0, 400.0 } };
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = tmp[i][j];
}

void usunMacierz(double **matrix, int n)
{
    for (int i = n - 1; i >= 0; i--)
        delete[]matrix[i];
    delete[]matrix;
}

void wypiszMacierz(double **matrix, int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << matrix[i][j] << "\t";
        cout << endl;
    }
    cout << endl;
}

double *nowyWektor(int n)
{
    double *vector = new double[n];
    return vector;
}
void wypelnijWektor(double *v, double *vtmp, int n)
{
    for (int i = 0; i < n; i++)
        v[i] = vtmp[i];
}
void wypiszWektor(double *vector, int n)
{
    cout << endl;
    for (int i = 0; i < n; i++)
        cout << vector[i] << "\t";
    cout << endl;
}
