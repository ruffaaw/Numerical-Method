/**
 * Dana jest macierz
     |1     -20     30      -4 |
     |2     -40     -6      50 |
 A = |9     -180    11      -12|
     |-16   15      -140    13 |

     |35  |
     |104 |
 b = |-366|
     |-354|

Napisz program w języku „C/C++”, realizujący dekompozycję LU macierzy A, przy zastosowaniu
eliminacji Gaussa z częściowym wyborem elementu podstawowego, a następnie rozwiązujący układ
równań Ax = b.
Uwaga: należy zaimplementować wariant dekompozycji omawiany na wykładzie.
Program należy zrealizować w postaci dwóch odrębnych procedur: jednej, która operuje wyłącznie
na macierzy A, i drugiej, która operuje wyłącznie na wektorze b, korzystając z wyników działania
procedury pierwszej.
 */
#include<iostream>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#define N 4

using namespace std;

double **stworzMacierz(int n);
void usunMacierz(double **matrix, int n);
void uzupelnijMacierz(double **matrix, int n);
void pokazMacierz(double **matrix, int n, int *index);
int  wybor_elementu_podstawowego(double **matrix, int n, int j, int *index);
void gauss(double **matrix, int *index);
void macierzL(double**l, double *b, int *index, int n);
void macierzU(double**u, double *b, int *index, int n);
void pokazWektor(double *b, int n);

int main()
{
    double** matrix = stworzMacierz(N);
    double b[4] = { 35.0, 104.0, -366.0, -354.0 };
    int index[4] = { 0, 1, 2, 3 }; //Wektor indeksów.

    uzupelnijMacierz(matrix, N);
    cout << "Dana w zadaniu macierz A: " << endl;
    pokazMacierz(matrix, N, index);

    cout << endl << endl << "Dany w zadaniu wektor b: " << endl;
    pokazWektor(b, N);

    gauss(matrix, index);
    cout << endl << endl << "Macierz po dekompozycji LU, przy zastosowaniu eliminacji Gaussa z czesciowym wyborem elementu podstawowego: " << endl;
    pokazMacierz(matrix, N, index);
    cout << "(Macierz L oraz U zapisana zostala w tej samej, jednej macierzy)" << endl;

    cout << endl << endl <<"Roziwiazanie ukladu rownan Ax = b:" << endl;
    macierzL(matrix, b, index, N-1);
    cout << endl << "Wektor y (Ly = b):" << endl;;

    pokazWektor(b, N);
    cout << endl << endl;
    macierzU(matrix, b, index, N-1);

    cout << "Wektor x (Ux = y):" << endl;
    pokazWektor(b, N);

    usunMacierz(matrix, N);

    return 0;
}

double **stworzMacierz(int n)
{
    double **matrix;
    matrix = new double *[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new double[n];
    return matrix;
}

void uzupelnijMacierz(double **matrix, int n)
{
    double tmp[4][4] =
            {{ 1.0, -20.0, 30.0, -4.0 },
             { 2.0, -40.0,-6.0, 50.0 },
             { 9.0, -180.0, 11.0, -12.0 },
             { -16.0, 15.0, -140.0, 13.0 }};

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

void pokazWektor(double *b, int n)
{
    for (int i = 0; i < n;i++)
        cout << b[i] << endl;
}

void pokazMacierz(double **matrix, int n, int *index)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << setw(8) << matrix[index[i]][j] << "\t";
        cout << endl;
    }
}

//Częściowy wybór elementu podstawowego (w przypadku, gdy dana wartość na przekątnej macierzy jest równa 0):
int wybor_elementu_podstawowego(double **matrix, int n, int j, int *index)
{
    int row;
    for (int i = n; i < j; i++)
    {
        if (fabs(matrix[index[i]][n]) < fabs(matrix[index[i + 1]][n]))
        {
            row = index[i + 1];
        }
        else
        {
            row = index[i];
        }
    }
    return row;
}

void gauss(double **matrix, int *index)
{
    int row;
    double v;
    for (int k = 0; k < 3; k++)
    {
        if (matrix[index[k]][index[k]] == 0.0)
        {
            row = wybor_elementu_podstawowego(matrix, index[k], 3, index);
            //Zapisanie w tablicy i podmienienie indeksu:
            index[row] = index[k];
            index[k] = row;
        }
        for (int i = k + 1; i < 4; i++)
        {
            //element v ponizej diagonali ktory zerujemy
            v = matrix[index[i]][k];
            for (int j = k + 1; j < 4; j++)
                //Wyliczanie wartości macierzy U z eliminacji Gaussa:
                matrix[index[i]][j] = matrix[index[i]][j] - matrix[index[k]][j] * (v / matrix[index[k]][k]);
            //Zapisanie macierzy L w tej samej macierzy co U:
            matrix[index[i]][k] = v / (matrix[index[k]][k]); //L macierz wspóczynnikłw przez które mnożymy kolejne wiersze (A[i][k]/A[k][k]).
        }
    }
}

//Wyznaczenie y (Ly = b):
void macierzL(double **l, double *b, int *index, int n)
{
    double sum = 0;
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j < i; j++)
            sum = sum + l[index[i]][j] * b[index[j]];
        b[index[i]] = (b[index[i]] - sum) / 1.0; //Macierz L posiada jedynki na głownej przekątnej
        sum = 0;
    }
}

//Wyznaczenie x (Ux = y):
void macierzU(double **u, double *b, int *index, int n)
{
    double sum = 0;
    for (int i = n; i >= 0; i--)
    {
        for (int j = i + 1; j <= n; j++)
            sum = sum + u[index[i]][j] * b[index[j]];
        b[index[i]] = (b[index[i]] - sum) / (u[index[i]][i]);
        sum = 0;
    }
}
