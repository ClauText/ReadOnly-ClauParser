
#include <iostream>

#include "readonly_clau_parser.h"

#include <ctime>


int main(void)
{
	std::string fileName;

	wiz::Node global;

	std::cin >> fileName;

	char* buffer = nullptr;
	int a = clock();
	if (wiz::LoadData::LoadDataFromFile(fileName, &global, &buffer, 0, 0)) {
		
	}
	int b = clock();

	//	wiz::LoadData::SaveWizDB(global, buffer, "output.eu4");

		if (buffer) {
			delete[] buffer;
		}

	std::cout << b - a << "ms" << "\n";


	return 0;
}
