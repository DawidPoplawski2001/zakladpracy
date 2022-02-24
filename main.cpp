#include <map>
#include <list>
#include <iostream>

#include "Main.h"

void firstSection()
{
	// Print some texts, I use 'print' ('wcout') to properly show Polish characters.
	std::wcout << L"WYBIERZ OPCJĘ:" << '\n';
	std::wcout << L"1 => LISTA WSZYSTKICH PRACOWNIKÓW" << '\n';
	std::wcout << L"2 => WYLICZ PENSJĘ PRACOWNIKA" << '\n';
	std::wcout << L"3 => ZAKOŃCZ PROGRAM" << '\n';
	std::wcout << L"WYBIERZ 1, 2 LUB 3: ";

	// Create variable which will be store number.
	// I use 'cin' but 'getline' is also correct.
	short choice; std::cin >> choice;

	// If user type 3, exit program.
	switch (choice)
	{
	case 1:
		printUsersList();
		break;

	case 2:
		calculateSalary();
		break;

	case 3:
	default:
		_Exit(0);
	}
}

std::list<std::tuple<int, std::wstring, int, int, int, std::wstring, double, double>> users =
{
	{ 1, L"Jan Nowak", 04, 03, 2002, L"pracownik fizyczny", 18.5, 0.0 },
	{ 2, L"Agnieszka Kowalska", 15, 12, 1973, L"urzędnik", 0.0, 2800 },
	{ 3, L"Robert Lewandowski", 23, 05, 1980, L"pracownik fizyczny", 29.0, 0.0 },
	{ 4, L"Zofia Plucińska", 02, 11, 1998, L"urzędnik", 0.0, 4.750 },
	{ 5, L"Grzegorz Braun", 29, 01, 1960, L"pracownik fizyczny", 48.0, 0.0 },
};

void printUsersList()
{
	std::cout << '\n';

	for (const auto& [value, user, birthDay, birthMonth, birthYear, position, grossHourlyWage, fixedGrossSalary] : users)
	{
		std::wcout << value
			<< " | "
			<< user
			<< " | "
			<< birthDay
			<< '.'
			<< birthMonth
			<< '.'
			<< birthYear
			<< " | "
			<< position
			// << " | "
			// << grossHourlyWage
			// << " | "
			// << fixedGrossSalary
			<< '\n';
	}

	std::cout << '\n';
	firstSection();
}

void calculateSalary()
{
	std::wcout << '\n';
	std::wcout << L"PROSZĘ PODAĆ ID PRACOWNIKA DLA KTÓREGO ZOSTANIE WYLICZONE WYNAGRODZENIE: ";
	short id; std::cin >> id;

	// if (std::find(users.begin(), users.end(), id) != users.end())

	for (const auto& [value, user, birthDay, birthMonth, birthYear, position, grossHourlySalary, fixedGrossSalary] : users)
	{
		if (value != id)
			continue;

		// Calculate currently year.
		tm newtime = { };
		const time_t now = time(nullptr);
		localtime_s(&newtime, &now);
		const int year = newtime.tm_year;

		// Calculate age.
		const int age = 1900 + year - birthYear;

		std::wcout << '\n';
		std::wcout << L"WYLICZANIE WYNAGRODZENIA PRACOWNIKA" << '\n';
		std::wcout << L"-----------------------------------" << '\n';
		std::wcout << L"DANE PRACOWNIKA:" << '\n';
		std::wcout << L"IMIE I NAZWISKO: " << user << '\n';
		std::wcout << L"WIEK: " << age << '\n';
		std::wcout << L"STANOWISKO: " << position << '\n';

		if (grossHourlySalary == 0.0)
			std::wcout << L"PENSJA STAŁA: " << fixedGrossSalary << L" zł" << '\n';
		else
			std::wcout << L"STAWKA GODZINOWA: " << grossHourlySalary << L" zł" << '\n';

		/*
		 * SEKCJA OD OBLICZANIA (HARD CODED).
		 */

	daysAgain:
		std::wcout << '\n' << L"PROSZĘ PODAĆ ILOŚĆ PRZEPRACOWANYCH DNI PRZEZ PRACOWNIKA (MAX. 20) ";
		short days; std::cin >> days;

		// JEŚLI LICZBA DNI JEST WIĘKSZA NIŻ 20, WYŚWIETL COŚ I WRÓĆ DO 'daysAgain'.
		if (days > 20)
		{
			std::wcout << '\n' << L"Ilość dni jest nieprawidłowa!" << '\n';
			goto daysAgain;
		}

		short salary = 0;

		// JEŚLI PRZEPRACOWANA ILOŚC DNI JEST RÓWNA 20, MOŻNA DORZUCIĆ PREMIĘ.
		if (days == 20)
		{
			std::wcout << L"PROSZĘ PODAĆ KWOTĘ PREMII DLA PRACOWNIKA: ";
			std::cin >> salary;
		}

		// JEŚLI OSOBA MA MNIEJ NIŻ 26 LAT, PODATEK JEST ZEROWY.
		int tax = 0;

		if (age > 26)
			tax = 18;

		// OBLICZANIE ILE GODZIN MA JAKAŚ TAM ILOŚC DNI.
		const int hours = 24 * days;

		// TERAZ OBLICZANIE NAGRODY.
		int reward = 0;

		// JEŚLI STAWKA GODZINOWA JEST WIĘKSZA NIŻ 0,
		// NAGRODA SKŁADA SIĘ Z PREMII (O ILE PRZEPRACOWANA ILOŚC DNI JEST RÓWNA 20).
		// PÓŹNIEJ DOLICZANA JEST GODZINOWA STAWKA RAZY ILOŚĆ GODZIN DZIELONE NA 3.
		if (grossHourlySalary > 0)
			reward = salary + grossHourlySalary * hours / 3;

		// JEŚLI PENSJA STAŁA JEST WIĘKSZA NIŻ 0,
		// NAGRODA SKŁADA SIĘ Z PREMII (O ILE PRZEPRACOWANA ILOŚC DNI JEST RÓWNA 20).
		// PÓŹNIEJ DOLICZANA JEST PENSJA STAŁA.
		if (fixedGrossSalary > 0)
		{
			if (position == L"urzędnik")
			{
				if (days < 20)
					reward = fixedGrossSalary * 80 / 100;
				else
					reward = fixedGrossSalary;
			}
			else if (position == L"pracownik fizyczny")
				reward = days * grossHourlySalary * 8;
		}

		system("cls");
		std::wcout << L"WYNAGRODZENIE PRACOWNIKA BRUTTO WYNOSI: " << reward << '\n';

		// OBLICZANIE POPRAWIONEGO PODATKU (ODJĘTEGO OD CENY).
		int fixedTax = 0;

		// JEŚLI NA OSOBĘ NIE ZOSTAŁ NAŁOŻONY PODATEK, TO GIT.
		if (tax == 0)
			std::wcout << L"BRAK PODATKU" << '\n';
		else
		{
			// JEŚLI ZOSTAŁ, NAGRODĘ POMNÓŻ RAZY PODATEK I PODZIEL PRZEZ 100.
			fixedTax = reward * tax / 100;
			std::wcout << L"POTRĄCONY PODATEK (18%): " << fixedTax << L" zł" << '\n';
		}

		std::wcout << L"DO WYPŁATY: " << reward - fixedTax;
		break;
	}
}

int main()
{
	// First, fix Polish characters in console.
	setlocale(LC_ALL, "pl_PL");

	firstSection();
}
