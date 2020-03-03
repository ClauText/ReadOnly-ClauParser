
//#include <vld.h> // for memory leak checking

#include <iostream>

#include "readonly_clau_parser.h"

#include <ctime>


int main(void)
{
	std::string fileName;

	wiz::Node global;

	std::cin >> fileName;

	char* buffer = nullptr;
	std::vector<wiz::MemoryPool> pool;
	int a = clock();
	if (wiz::LoadData::LoadDataFromFile(fileName, &global, &buffer, &pool, 0, 0)) {
		
	}
	int b = clock();

		//wiz::LoadData::SaveWizDB(global, buffer, "output.eu4");

		if (buffer) {
			delete[] buffer;
		}
		if (pool.empty() == false) {
			for (auto& x : pool) {
				x.Clear();
			}
		}

	std::cout << b - a << "ms" << "\n";


	return 0;
}
