/**
 * Zaimplementuj w języku „C/C++” algorytm obliczający przybliżone wartości funkcji
f(x) = [1 - exp(-x)]/x dla x nalezacego do [10^(-30),10^9], korzystając z funkcji standardowej exp(). W oparciu o
zbiór dokładnych wartości tej funkcji, udostępniony przez prowadzącego zajęcia, zbadaj jak
zmieniają się błędy względne przybliżenia funkcji w tym algorytmie, w zależności od x. W tym celu
wykonaj rysunek przedstawiający zależność logarytmu dziesiętnego z bezwzględnej wartości błędu
względnego od logarytmu dziesiętnego z x. Z wykresu odczytaj zakres zmiennej x, w którym błąd
względny pozostaje na poziomie błędu reprezentacji, oraz zakres zmiennej x, w którym błąd
względny jest większy. Wyjaśnij przyczynę obserwowanych zmian błędów. Na tej podstawie
zaproponuj alternatywny sposób obliczania wartości funkcji f(x) w sytuacjach gdy obserwowany
błąd jest duży. Dokonaj stosownej modyfikacji programu, tak aby uzyskać błąd względny na
poziomie błędu reprezentacji (czyli tzw. dokładność maszynową) dla dowolnego x nalezacego do [10^-30, 10^9].
W obliczeniach zastosuj zmienne podwójnej precyzji. Do wykonania rysunku w tym ćwiczeniu (a
także w niektórych dalszych ćwiczeniach) najlepiej użyć programu GNUPLOT (dostępnego za
darmo z Internetu).
 */
#include <iostream>
#include <math.h>
#include <fstream>

// podstawowa rozszerzona
#define podstawowa

// funkcja z zadania
double funkcjaFX(double x) {
  return (1 - exp(-x)) / x;
}

// funkcje 1 - exp(-x) rozwijamy w szereg Taylora
// zapobiega odejmowaniu podobnych licz co by powodowalo utrate cyfr znaczacych
double funkcjaUlepszona(double x) {
  double wynik = 1;
  double znak = -1;
  double krok = 1;

  // ograniczenie i wynika z tego ze obliczamy za jego pomoca silnie
  for (int i = 2; i < 20; ++i){
    krok = krok * (x / i);
    wynik += znak * krok;
    znak = -znak;
  }

  return wynik;
}

int main() {
  std::ifstream plikDane;
  std::ofstream daneWynikowe, danePoprawione;
  double wartoscPlik = 0, wartoscObliczona = 0, wartoscLog10 = 0, argument = 0, blad = 0, bladLog = 0;
  int pom = 0;

  // Plik zawieraja wartosci pobrane ze strony
  daneWynikowe.open("daneWynikowe.txt");
  danePoprawione.open("danePoprawione.txt");
  plikDane.open("dane.txt");

  std::cout << "|      Argument Funkcji      |     Blad Wzgledny     |    log10(blad)    |"
               "   ~Blad Wzgledny    |   ~log10(blad)    |\n";

  while (!plikDane.eof()) {
	// Pobranie wartosci z plikow
    plikDane >> wartoscLog10;
    plikDane >> argument;
    plikDane >> wartoscPlik;

#ifdef podstawowa
	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego
	wartoscObliczona = funkcjaFX(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	daneWynikowe << wartoscLog10 << " " << bladLog << "\n";

    std::cout << "|          ";
    std::cout << argument << "         |         ";
    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |         ";

	// Obliczenie wartosci funkcji oraz oblicznie bledu wzglednego z szeregu taylora
	// Korzystamy z rozwiniecia w szereg aby uniknac odejmowanie dwoch podbnych liczb
	wartoscObliczona = funkcjaUlepszona(argument);
	blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);

	// Obliczenie zaleznosci logarytmu dziesietnego z bezgledna wartoscia bledu wzglednego
	bladLog = log10(blad);

	// Zapisanie wyniku do pliku
	danePoprawione << wartoscLog10 << " " << bladLog << "\n";

    std::cout << blad << "         |         ";
    std::cout << bladLog << "         |\n";
#endif

#ifdef rozszerzona
	if(wartoscLog10 < -0.5){
	  wartoscObliczona = funkcjaUlepszona(argument);
	  blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);
	  bladLog = log10(blad);
	  danePoprawione << wartoscLog10 << " " << bladLog << "\n";
	} else {
	  wartoscObliczona = funkcjaFX(argument);
	  blad = abs((wartoscObliczona - wartoscPlik) / wartoscPlik);
	  bladLog = log10(blad);
	  danePoprawione << wartoscLog10 << " " << bladLog << "\n";
	}

#endif
  }

  daneWynikowe.close();
  danePoprawione.close();
  plikDane.close();

  return 0;
}