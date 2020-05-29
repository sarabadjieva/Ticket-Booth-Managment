#include "Manager.h"

int main()
{
	char filename[FILENAME_MAX];
	char hallsfile[FILENAME_MAX];

	std::cout << "Do you want to work with the default text file? (y/n)\n";
	char choice;
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();
	if (choice == 'n')
	{
		std::cout << "What is the name of the file?\n";
		std::cin >> filename;
		std::cin.clear();
		std::cin.ignore();
	}

	std::cout << "Do you want to read the events from the default file? (y/n)\n";
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();
	if (choice == 'n')
	{
		std::cout << "What is the name of the file?\n";
		std::cin >> hallsfile;
		std::cin.clear();
		std::cin.ignore();
	}
	
	
	if (strlen(filename) > 0)
	{
		if (strlen(hallsfile) > 0)
		{
			Manager m(filename, hallsfile); 
			m.callManager();
		}
		else
		{
			Manager m(filename);
			m.callManager();
		}
	} 
	
	if (strlen(hallsfile) > 0)
	{
		Manager m(nullptr, hallsfile);
	}
	else
	{
		Manager m;
		m.callManager();
	}
	
	return 0;
}