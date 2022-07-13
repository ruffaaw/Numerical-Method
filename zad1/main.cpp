/**
 * Napisz program w języku „C/C++”, umożliwiający „doświadczalne” wyznaczenie liczby bitów
mantysy oraz tzw. epsylona maszynowego, dla zmiennych typu float i double, tj. najmniejszej liczby
 takiej, że fl( + 1) > 1. Aby znaleźć odpowiedź na pytanie jak napisać taki program, zacznij od
wyjaśnienia kwestii jaki jest związek  z precyzją arytmetyki.
 */
#include<iostream>
void machineEpsilonDouble(double epsilon_double){
    int x = 0;
    double y = (0.5 * epsilon_double + 1.0);
    while(y > 1.0){
        epsilon_double *= 0.5;
        y = (0.5 * epsilon_double + 1.0);
        x++;
    }
    std::cout << "y = " << epsilon_double << std::endl << "ilosc bitow mantysy = " << x << std::endl;

}
void machineEpsilonFloat(float epsilon_float){

    int x = 0;
    float y = (0.5f * epsilon_float + 1.0f);
    while(y > 1.0f){
        epsilon_float *= 0.5f;
        y = (0.5f * epsilon_float + 1.0f);
        x++;
    }
    std::cout << "y = " << epsilon_float << std::endl << "ilosc bitow mantysy = " << x << std::endl;

}

int main(){
    machineEpsilonDouble(1.0);
    machineEpsilonFloat(1.0f);
    return 0;
}
