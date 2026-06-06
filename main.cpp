#include "LibraryApp.h"
#include "LibraryException.h"
#include <iostream>

using namespace std;

int main() {
	//system("chcp 1251");

	cout << "Информационная система библиотеки\n"
		<< "Курсовой проект по дисциплине ООП\n"
		<< "Воронежский государственный технический университет\n\n";
	try {
		LibraryApp app;
		app.run();
	}
	catch (const LibraryException& e) {
		cerr << "Ошибка: " << e.what() << "\n";
		return 1;
	}
	return 0;
}
