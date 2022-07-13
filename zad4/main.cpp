/**
 * Napisz program w języku „C/C++”, realizujący uogólnioną metodę Newtona rozwiązywania układu
trzech algebraicznych równań nieliniowych, i zastosuj ten program do przykładu z zadania 1.
Przyjmij takie przybliżenie początkowe, aby uzyskać zbieżność metody. Zastosuj trzy niezależne
kryteria zakończenia iteracji. Zadbaj o to, aby wyprowadzać na konsolę wyniki pośrednie obliczeń
dla każdej iteracji, tak aby możliwe było obserwowanie zbieżności kolejnych przybliżeń
pierwiastków. W szczególności oblicz jak zmienia się estymator błędu rozwiązania oraz residuum
układu w trakcie iteracji.
 */
#include <iostream>
#include <math.h>
using namespace std;

const double tol = 1e-16;
const int maxIt = 100;

 //wybranie max z 3 liczbami, zwracamy najwieksza wartosc z trzech podanych
double max(double a, double b, double c) {
    double maximum = a;

    if (b > maximum)
        maximum = b;

    if(c > maximum)
        maximum = c;

    return maximum;
}

double funkcja1(double x, double y, double z) {
    return x * x + y * y + z * z - 2.0;
}

double funkcja2(double x, double y, double z) {
    return x * x + y * y - 1.0;
}

double funkcja3(double x, double y, double z) {
    return x * x - y;
}

// Obliczenie wektora abc, wzory obliczone wczesniej na kartce
double obliczA(double x, double y, double z) {
    return (x * x - y * y - 1.0 + 2.0 * x * x * y) / (2.0 * x * (1.0 + 2.0 * y));
}

double obliczB(double x, double y, double z) {
    return (y * y + y - 1.0) / (1.0 + 2.0 * y);
}

double obliczC(double x, double y, double z) {
    return (z * z + 2.0 * z * z * y - 2.0 * y - 1.0) / (2.0 * z * (1.0 + 2.0 * y));
}


 //sprawdzenie czy dane wartosci naleza do dziedziny funkcji, true jesli naleza do dziedziny lub false jesli nie naleza
bool sprawdzZmienne(double x, double y, double z){
    if (x == 0.0) {
        std::cout << "Nieprawidlowy x\n";
        return false;
    } else if (z == 0.0) {
        std::cout << "Nieprawidlowy y\n";
        return false;
    } else if (y == -1.0 / 2.0 || 4.0 * y * x + 2.0 * x == 0.0) {
        std::cout << "Nieprawidlowy z\n";
        return false;
    } else
        return true;
}

 //wyznaczony jakobian, zwracony wskaznik do jakobianu 3x3
double **Jakobian(double x, double y, double z) {
    // Storzenie pomocniczej macierzy 3x3
    auto **temp = new double *[3];
    for (int i = 0; i < 3; ++i) {
        temp[i] = new double[3];
    }

    temp[0][0] = 2.0 * x;
    temp[0][1] = 2.0 * y;
    temp[0][2] = 2.0 * z;
    temp[1][0] = 2.0 * x;
    temp[1][1] = 2.0 * y;
    temp[1][2] = 0.0;
    temp[2][0] = 2.0 * x;
    temp[2][1] = -1.0;
    temp[2][2] = 0.0;

    return temp;
}

void newton(double x, double y, double z){
    double **macierz = NULL;
    double estymator = 0.0, residuum = 0.0, wartosciFunkji[3], wektorABC[3];
    int i = 1;
    bool kont = true;

    wartosciFunkji[0] = funkcja1(x,y,z);
    wartosciFunkji[1] = funkcja2(x,y,z);
    wartosciFunkji[2] = funkcja3(x,y,z);

    while(kont){
        for(int j = 0; j < 3; ++j)
            wektorABC[j] = 0;

        if(!sprawdzZmienne(x,y,z))
            break;

        macierz = Jakobian(x,y,z);

        wektorABC[0] = obliczA(x,y,z);
        wektorABC[1] = obliczB(x,y,z);
        wektorABC[2] = obliczC(x,y,z);

        x = x - wektorABC[0];
        y = y - wektorABC[1];
        z = z - wektorABC[2];

        estymator = max(fabs(wektorABC[0]), fabs(wektorABC[1]), fabs(wektorABC[2]));

        residuum = max(fabs(wartosciFunkji[0]), fabs(wartosciFunkji[1]), fabs(wartosciFunkji[2]));

        std::cout << "Iteracja "<< i << "\nx: " << x << "\ny: " << y << "\nz: " << z << "\nestymator: " << estymator << "\nreziduum: " << residuum << "\n\n";

        if(fabs(residuum) <= tol && estymator <= tol || i >= maxIt)
            kont = false;

        wartosciFunkji[0] = funkcja1(x,y,z);
        wartosciFunkji[1] = funkcja2(x,y,z);
        wartosciFunkji[2] = funkcja3(x,y,z);

        i++;
    }

    if(macierz != NULL){
        for (int j = 0; j<3; ++j)
            delete [] macierz[j];

        delete macierz;
        macierz = NULL;
    }
}

int main() {
    newton(5.0,3.0,5.0);

    return 0;
}