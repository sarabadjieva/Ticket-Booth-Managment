#include "Manager.h"

Manager::Manager(const char* f, const char* hf)
{
	if (f == nullptr)
	{
		strcpy_s(fileName, FILENAME_MAX, EVENTS_FILE);
	}
	else
	{
		strcpy_s(fileName, FILENAME_MAX, f);
	}

	if (hf == nullptr)
	{
		strcpy_s(hallsFileName, FILENAME_MAX, HALLS_FILE);
	}
	else
	{
		strcpy_s(hallsFileName, FILENAME_MAX, hf);
	}

	strcpy_s(tempName, FILENAME_MAX, TEMPORARY_FILE);

	//we can read them in the constructor, because we will not operate with them after this
	readHalls();
}

Manager::~Manager()
{
	if (!halls.empty())
	{
		halls.clear();
	}
	if (!events.empty())
	{
		events.clear();
	}
}

void Manager::readHalls()
{
	if (!halls.empty())
	{
		halls.clear();
	}

	hallsFile.open(hallsFileName);

	//check if empty
	hallsFile.seekp(0, std::ios::end);
	int size = (int)hallsFile.tellg();
	if (size == 0)
	{
		std::cout << "File is empty.\n";
		hallsFile.close();
		return;
	}

	hallsFile.clear();
	hallsFile.seekg(0, std::ios::beg);
	Hall* h = new Hall;
	while (hallsFile.getline(h->name, MAX_HALL_NAME_LENGTH, DELIMITER))
	{
		size_t row, seats;
		while (hallsFile >> row >> seats)
		{
			h->rows.insert(std::pair<size_t, size_t>(row, seats));		
			hallsFile.get();
			if (!(hallsFile.peek() >= (int)'0' && hallsFile.peek() <= (int)'9')) break;
		}
		halls.push_back(h);
		h = new Hall;
	}

	hallsFile.close();
}

void Manager::readEvents()
{
	if (!events.empty())
	{
		events.clear();
	}

	temp.open(tempName);

	//check if empty
	temp.seekp(0, std::ios::end);
	int size = (int)temp.tellg();
	if (size == 0)
	{
		std::cout << "File is empty.\n";
		temp.close();
		return;
	}

	temp.clear();
	temp.seekg(0, std::ios::beg);
	Event* e = new Event;
	while (temp)
	{
		while (temp >> *e) 
		{
			//check if the halls have a show from this event on any date 
			//if yes -> we add the date of the show to the dates, on which the hall is busy
			for (auto x : e->getShows())
			{
				for (auto& y : halls)
				{
					if (strcmp(x.second, y->name) == 0)
					{
						y->busy.push_back(x.first);
					}
				}
			}

			events.push_back(e);
			e = new Event;
		}
	}
	temp.close();

	for (auto x : events)
	{
		std::cout << *x;
	}
}

void Manager::writeEvents()
{
	temp.open(tempName, std::ios::out | std::ios::trunc);

	for (auto x : events)
	{
		temp << *x;
	}

	temp.close();
}

void Manager::callManager()
{

	std::cout << "Choose what you want to do:\nopen, close, save, saveas, help, exit.\n";

	char choice[20];
	std::cin >> choice;
	std::cin.clear();
	std::cin.ignore();

	if (strcmp(choice, "open") == 0)
	{
		open();
		workwithfile();
	}
	else if (strcmp(choice, "close") == 0)
	{
		close();
	}
	else if (strcmp(choice, "save") == 0)
	{
		save();
		callManager();
	}
	else if (strcmp(choice, "saveas") == 0)
	{
		saveas();
		callManager();
	}
	else if (strcmp(choice, "help") == 0)
	{
		help();
		callManager();
	}
	else if (strcmp(choice, "exit") == 0)
	{
		exitFunction();
	}

	else callManager();
}


void Manager::open()
{
	file.open(fileName);
	if (!file)
	{
		std::cerr << "File couldn’t be opened!\n";
		callManager();
	}

	hallsFile.open(hallsFileName);
	if (!hallsFile)
	{
		std::cerr << "Couldn’t open the file for reading halls!\n";
		callManager();
	}

	std::ofstream temp(tempName);
	if (!temp.is_open())
	{
		std::cerr << "There was a problem, try again!\n";
		callManager();
	}

	file.seekg(0);
	temp << file.rdbuf();

	file.close();
	hallsFile.close();
	temp.close();

	readEvents();
}

void Manager::close()
{
	if (file.is_open())
	{
		file.close();
	}

	if (hallsFile.is_open())
	{
		hallsFile.close();
	}

	if (temp.is_open())
	{
		temp.close();
	}

	remove(tempName);
	system("pause");
	system("cls");

	open();
}

void Manager::save()
{
	std::ofstream file(fileName, std::ios::trunc);
	temp.open(tempName);
	file << temp.rdbuf();

	file.close();
	temp.close();

	if (hallsFile.is_open())
	{
		hallsFile.close();
	}

	remove(tempName);
	system("pause");
	system("cls");
}

void Manager::saveas()
{
	std::cout << "How do you want to be the name of the new file?:\n";
	char newName[FILENAME_MAX];
	std::cin >> newName;
	std::cin.clear();
	std::cin.ignore();

	std::ofstream newFile(newName, std::ios::out | std::ios::trunc);

	if (!newFile)
	{
		std::cout << "New file can't be opened, try again!\n\n";
		saveas();
	}

	temp.open(tempName);
	newFile << temp.rdbuf();
	newFile.close();
	temp.close();

	if (file.is_open())
	{
		file.close();
	}

	if (hallsFile.is_open())
	{
		hallsFile.close();
	}

	remove(tempName);
}

void Manager::help()
{
	std::cout << "The following commands are supported:\n"
		<< "-------------------------------------\n"
		<< "open <file>\topens <file>\n"
		<< "close\t\tcloses currently opened file\n"
		<< "save\t\tsaves the currently open file\n"
		<< "saveas <file>\tsaves the currently open file in <file>\n"
		<< "help\t\tprints this information\n"
		<< "exit\t\texists the program\n"
		<< "exit\t\texists the program\n"
		<< "----------------------------------\n";
	system("pause");
	system("cls");
}

void Manager::exitFunction()
{
	if (file.is_open())
	{
		std::cout << "You haven't closed the opened files, so i will!\n";
		file.close();
	}

	if (hallsFile.is_open())
	{
		std::cout << "You haven't closed the opened files, so i will!\n";
		hallsFile.close();
	}

	if (temp.is_open())
	{
		std::cout << "You haven't closed the opened files, so i will!\n";
		temp.close();
	}


	remove(tempName);
	exit(0);
}

void Manager::workwithfile()
{
	std::cout << "\nPlease, enter function with parameters:";
	char function[1000];
	std::cin.getline(function, 1000);
	std::stringstream ss(function);

	char word[MAX_NOTE_LENGTH];
	int wordCount = 0;
	while (ss >> word)
	{
		wordCount++;
	}
	if (wordCount == 0)
	{
		std::cerr << "Please write something!\n";
		workwithfile();
	}

	ss.clear();
	ss.seekg(0, std::ios::beg);
	char functionName[20];
	ss.getline(functionName, 20, DELIMITER);

	if (strcmp(functionName, "addevent") == 0)
	{
		if (wordCount != 4)
		{
			std::cout << "Wrong parameters!\n";
			workwithfile();
		}

		Date d;
		char eName[MAX_EVENT_NAME_LENGTH];
		char hall[MAX_HALL_NAME_LENGTH];

		ss >> d;
		ss.get();
		ss.getline(hall, MAX_EVENT_NAME_LENGTH, DELIMITER);
		ss.getline(eName, MAX_HALL_NAME_LENGTH);

		addevent(d,hall, eName);
		writeEvents();
		readEvents();
	}
	else if (strcmp(functionName, "freeseats") == 0)
	{
		if (wordCount != 3)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		Date d;
		char eName[MAX_EVENT_NAME_LENGTH];

		ss >> d;
		ss.get();
		ss.getline(eName, MAX_EVENT_NAME_LENGTH, DELIMITER);

		freeseats(d, eName);
	}
	else if (strcmp(functionName, "book") == 0)
	{
		if (wordCount < 6)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned short row = 0;
		unsigned short seat = 0;
		Date d;
		char eName[MAX_EVENT_NAME_LENGTH];
		char note[10000];

		ss >> row;
		ss.get();
		ss >> seat;
		ss.get();
		ss >> d;
		ss.get();
		ss.getline(eName, MAX_EVENT_NAME_LENGTH, DELIMITER);
		ss.getline(note, MAX_NOTE_LENGTH);

		book(row, seat ,d, eName, note);
		writeEvents();
		readEvents();
	}
	else if (strcmp(functionName, "unbook") == 0)
	{
		if (wordCount != 5)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned short row = 0;
		unsigned short seat = 0;
		Date d;
		char eName[MAX_EVENT_NAME_LENGTH];

		ss >> row;
		ss.get();
		ss >> seat;
		ss.get();
		ss >> d;
		ss.get();
		ss.getline(eName, MAX_EVENT_NAME_LENGTH);

		unbook(row, seat, d, eName);
		writeEvents();
		readEvents();
	}
	else if (strcmp(functionName, "buy") == 0)
	{
		if (wordCount != 5)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}

		unsigned short row = 0;
		unsigned short seat = 0;
		Date d;
		char eName[MAX_EVENT_NAME_LENGTH];

		ss >> row;
		ss.get();
		ss >> seat;
		ss.get();
		ss >> d;
		ss.get();
		ss.getline(eName, MAX_EVENT_NAME_LENGTH);

		buy(row, seat, d, eName);
		writeEvents();
		readEvents();
	}
	else if (strcmp(functionName, "bookings") == 0)
	{
		if (wordCount == 3)
		{
			Date d;
			char hall[MAX_HALL_NAME_LENGTH];
			ss >> d;
			ss.get();
			ss.getline(hall, MAX_HALL_NAME_LENGTH);
			bookings(d, hall);
		}
		else if (wordCount == 2)
		{
			char parameter[MAX_HALL_NAME_LENGTH];
			ss.getline(parameter, MAX_HALL_NAME_LENGTH);
			bookings(parameter);
		}
		else
		{
			std::cout << "Wrong parameters!\n";
			workwithfile();
		}

	}
	else if (strcmp(functionName, "check") == 0)
	{
		if (wordCount != 2)
		{
			std::cerr << "Wrong parameters!\n";
			workwithfile();
		}
	
		char code[MAX_TICKET_CODE_LENGTH];

		ss.getline(code, MAX_TICKET_CODE_LENGTH);

		check(code);
	}
	else if (strcmp(functionName, "report") == 0)
	{
	if (wordCount == 3)
	{
		Date d1, d2;
		ss >> d1;
		ss.get();
		ss >> d2;
		ss.get();
		report(d1, d2);
		}
		
	else
	{
		Date d1, d2;
		char hall[MAX_HALL_NAME_LENGTH];

		ss >> d1;
		ss.get();
		ss >> d2;
		ss.get();
		ss.getline(hall, MAX_HALL_NAME_LENGTH);

		report(d1, d2, hall);
	}

	}
	else if (strcmp(functionName, "mostwatched") == 0)
	{
		mostwatched();
	}
	else
	{
		std::cout << "Wrong input!\n\n";
	}

	std::cout << "\nDo you want to work with the same file more(y/n)\n";
	char answer;
	std::cin >> answer;
	std::cin.clear();
	std::cin.ignore();
	if (answer == 'y')
	{
		workwithfile();
	}
	else callManager();
}

void Manager::addevent(const Date& date, const char* hall, const char* name)
{
	//hall : check if it does exist and if it is free on this date

	if (!date.isValidDate() || date < Date())
	{
		std::cerr << "Enter a valid future date!\n";
		return;
	}

	bool found = false;
	for (auto& x : halls)
	{
		if (strcmp(hall, x->name) == 0)
		{
			found = true;
			for (auto x : x->busy)
			{
				if (date == x)
				{
					std::cerr << "There is already a show in this hall on this date!\n";
					return;
				}
			}
			break;
		}
	}
	if (!found)
	{
		std::cout <<"The hall doesn't exist!\n" ;
		return;
	}
	
	//add show for an existing event
	for (auto& x : events)
	{
		if (strcmp(x->getEventName(), name) == 0)
		{
			if (x->addShow(date, hall))
			{
				std::cout << "Show added.\n";
			}
			else
			{
				std::cout << "Couldn't add show.\n";
			}
			return;
		}
	}

	//add show for a new event
	Event* e = new Event(name);
	e->addShow(date, hall);
	events.push_back(e);
	std::cout << "Event added.\n";

}

void Manager::freeseats(const Date& d, const char* eName)
{
	if (!d.isValidDate())
	{
		std::cerr << "Enter a valid date!\n";
		return;
	}

	std::vector<std::pair <size_t, size_t> > freeS = getFreeSeatsManager(d, eName); //getFreeSeats checks the name and the date

	if (freeS.empty())
	{
		std::cerr << "No available seats!\n";
		return;
	}

	//first way to print the seats
	//
	size_t row = 0;
	for (auto x : freeS)
	{
		if (x.first == row)
		{
			//here we assume that we have max 99 seats in a row
			//if there are more we should add an if statement everytime we reach 10^x
			if (x.second < 10)
			{
				std::cout << DELIMITER << DELIMITER5 << DELIMITER << x.second << DELIMITER5;
			}
			else
			{
				std::cout << DELIMITER << DELIMITER5 << x.second << DELIMITER5;
			}
		}
		else
		{
			std::cout << '\n' << 'R' << x.first;
			if (x.second < 10)
			{
				std::cout << DELIMITER << DELIMITER5 << DELIMITER << x.second << DELIMITER5;
			}
			else
			{
				std::cout << DELIMITER << DELIMITER5 << x.second << DELIMITER5;
			}
			row++;
		}
	}

	//second way to print the seats 
	//
	/*	size_t row = 0;
		size_t countSeatsInRow = 0;
	
		std::cout << "\t\t\t\t|ROW SEAT|\n";
		for (auto x : freeS)
		{
			if (row < x.first)
			{
				std::cout << '\n';
				row = x.first;
				if (countSeatsInRow % 10 != 0)
				{
					std::cout << '\n';
				}

				countSeatsInRow = 0;
			}

			if (x.first < 10)
			{
				if (x.second < 10)
				{
					std::cout << DELIMITER5 << DELIMITER << x.first << DELIMITER << x.second << DELIMITER << DELIMITER5 << '\t';
				}
				else
				{
					std::cout << DELIMITER5 << DELIMITER << x.first << DELIMITER << x.second << DELIMITER5 << '\t';
				}
			}
			else if (x.second < 10)
			{
				std::cout << DELIMITER5 << x.first << DELIMITER << x.second << DELIMITER << DELIMITER5 << '\t';
			}
			else
			{
				std::cout << DELIMITER5 << x.first << DELIMITER << x.second << DELIMITER5 << '\t';
			}

			countSeatsInRow++;
		}*/
}

void Manager::book(unsigned short row, unsigned short seat, const Date& d, const char* name, const char* note)
{
	if (!d.isValidDate() || d < Date())
	{
		std::cerr << "Enter a valid future date!\n";
		return;
	}

	//check if the seat is free
	std::vector<std::pair <size_t, size_t> > freeS = getFreeSeatsManager(d, name); //getFreeSeats checks the name and the date
	if (freeS.empty()) return;
	bool free = false;
	for (auto x : freeS)
	{
		if (x.first == row && x.second == seat)
		{
			free = true;
			break;
		}
	}
	if (!free)
	{
		std::cout << "Check our free seats first!\n";
		return;
	}

	for (auto x : events)
	{
		if (strcmp(x->getEventName(), name) == 0)
		{
			//find the hall in which the show takes place on date d
			char hall[MAX_HALL_NAME_LENGTH];
			int length = strlen(x->getShows().at(d)) + 1;
			strcpy_s(hall, length, x->getShows().at(d));

			//make the ticket
			Ticket t;
			t.setCodeviaParam(d, name, hall, row, seat);
			t.setNote(note);
			if (x->bookTicket(t))
			{
				std::cout << "Ticket successfully booked!\n";
			}
			else
			{
				std::cout << "Couldn't book ticket!\n";
			}
			return;
		}
	}
}

void Manager::unbook(unsigned short row, unsigned short seat, const Date& d, const char* name)
{
	if (!d.isValidDate() || d<Date())
	{
		std::cerr << "Enter a valid future date!\n";
		return;
	}

	for (auto x : events)
	{
		if (strcmp(x->getEventName(), name) == 0)
		{
			Ticket* t = x->getTickets();
			for (size_t i = 0; i < x->getSize(); i++)
			{
				if (t[i].getBookedValue() && t[i].getDate() == d
					&& t[i].getRow() == row && t[i].getSeat() == seat)
				{
					//the function prints message  if the ticket is unbooked / can't be unbooked
					x->unbookTicket(t[i]);
					return;
				}
					
			}
		}
	}
}

void Manager::buy(unsigned short row, unsigned short seat, const Date& d, const char* name)
{
	if (!d.isValidDate() || d < Date())
	{
		std::cerr << "Enter a valid future date!\n";
		return;
	}

	//check if the seat is free
	std::vector<std::pair <size_t, size_t> > freeS = getFreeSeatsManager(d, name); //getFreeSeats checks the name and the date
	if (freeS.empty()) return;
	bool free = false;
	for (auto x : freeS)
	{
		if (x.first == row && x.second == seat)
		{
			free = true; 
			break;
		}
	}
	if (!free)
	{
		std::cout << "Check our free seats first!\n";
		return;
	}

	for (auto x : events)
	{
		if (strcmp(x->getEventName(), name) == 0)
		{
			//find the hall in which the show takes place on date d
			char hall[MAX_HALL_NAME_LENGTH];
			int length = strlen(x->getShows().at(d)) + 1;
			strcpy_s(hall, length, x->getShows().at(d));

			//make the ticket
			Ticket t;
			t.setCodeviaParam(d, name, hall, row, seat);
			if (x->buyTicket(t))
			{
				std::cout << "Ticket successfully bought!\n";
			}
			else
			{
				std::cout << "Couldn't buy ticket!\n";
			}
		}
	}
}

void Manager::bookings(const Date& date, const char* eName)
{
	for (auto& x : events)
	{
		if (strcmp(eName, x->getEventName()) == 0)
		{
			Ticket* t = x->getTickets();
			for (size_t i = 0; i < x->getSize(); i++)
			{
				if (t[i].getDate() == date)
				{
					t[i].print_ticket();
				}
			}
		}
	}
}

void Manager::bookings(const char* parameter)
{
	if (strlen(parameter) == 0)
	{
		std::cerr << "Please write at least date or event after bookings!\n";
		return;
	}

	for (auto& x : events)
	{
		if (strcmp(x->getEventName(), parameter) == 0)
		{
			std::cout << "Write the function with a date to print the booked tickets!\nThis event is available at: \n";
			std::map<Date, char*> shows = x->getShows();
			for (auto y : shows)
			{
				std::cout << y.first << " in hall " << y.second << '\n';
			}
			return;
		}
	}

	std::stringstream ss(parameter);
	Date d;
	if (ss >> d)
	{
		std::cout << "Write for which event you want to see the bookings!\nEvents we have on this day are: \n";
		for (auto& x : events)
		{
			std::map<Date, char*> shows = x->getShows();
			for (auto y : shows)
			{
				if (y.first == d)
				{
					std::cout << x->getEventName() << '\n';
					break;
				}
			}
		}
	}
	else
	{
		std::cout << "No such event or date!\n";
	}

}

void Manager::check(const char* code)
{
	if (validCode(code))
	{
		Ticket t(code);
		for (auto& x : events)
		{
			if (strcmp(x->getEventName(), t.getEvent()) == 0)
			{
				if (x->getShows().count(t.getDate()) == 0)
				{
					std::cout << "This code isn't valid!\n";
					return;
				}
				else
				{
					if (strcmp(x->getShows().at(t.getDate()), t.getHall()) == 0)
					{
						for (size_t i = 0; i < x->getSize(); i++)
						{
							if (t == x->getTickets()[i])
							{
								x->getTickets()[i].print_ticket();
								return;
							}
						}
						t.print_ticket();
						return;
					}
				}

			}
		}
	}

	std::cout << "This code isn't valid!\n";
}

void Manager::report(const Date& d1, const Date& d2, const char* hall)
{
	//do some validation
	/*if (d1 > Date() || d2 > Date() || d1 > d2)
	{
		std::cerr << "Enter dates before the current. The first of them should be before the second one!\n";
		return;
	}*/

	if (hall == nullptr)
	{
		for (auto& x : events)
		{
			x->report(d1,d2);
		}
		return;
	}

	//check the hall
	bool found = false;
	for (auto& x : halls)
	{
		if (strcmp(hall, x->name) == 0)
		{
			found = true;
			break;
		}
	}
	if (!found)
	{
		std::cout << "This hall doesn't exist!";
		return;
	}

	std::cout << "Event:\t\tNumber of bought tickets:\n"
		<< std::string(40, DELIMITER_) << '\n';
	for (auto& x : events)
	{
		x->report(d1, d2, hall);
	}
}

void Manager::mostwatched()
{
	std::multimap<size_t, char*, greater<size_t> > temp;
	size_t boughtTicketsCount;
	size_t size = 0;
	for (auto& x : events)
	{
		boughtTicketsCount = 0;
		size = x->getSize();
		char* name = new char[strlen(x->getEventName()) + 1];
		strcpy_s(name, strlen(x->getEventName()) + 1, x->getEventName());

		for (size_t i = 0; i < size; i++)
		{
			if (x->getTickets()[i].getBoughtValue()) boughtTicketsCount++;
		}
		
		temp.insert(std::pair<size_t, char*> (boughtTicketsCount,name));
	}

	for (std::multimap<size_t, char*>::iterator it = temp.begin(); it != temp.end(); it++)
	{
		std::cout <<'\n' << (*it).first << '\t' << (*it).second;
	}
}

std::vector<std::pair<size_t, size_t> > Manager::getFreeSeatsManager(const Date& d, const char* eName)
{
	//hall: we can only add events with existing hall => here we must see the rows and the seats

	std::vector<std::pair <size_t, size_t> > empty;

	if (!d.isValidDate())
	{
		std::cerr << "Enter a valid date!";
		return empty;
	}

	//check if the event exists
	for (auto& x : events)
	{
		if (strcmp(x->getEventName(), eName) == 0)
		{
			//does the event have a show on date d
			if (x->getShows().count(d) == 0)
			{
				std::cerr << "No show on this date\n";
				return empty;
			}

			//if it has a show -> in which hall
			char hall[MAX_HALL_NAME_LENGTH];
			strcpy_s(hall, MAX_HALL_NAME_LENGTH, x->getShows().at(d));
			std::map<size_t, size_t> places;

			//extract the map containing the rows and the seats
			for (auto& x : halls)
			{
				if (strcmp(hall, x->name) == 0)
				{
					places = x->rows;
					break;
				}
			}
			// return a vector with the free, valid seats 
			return x->getFreeSeats(d, places);
		}
	}

	std::cout << "There isn't such event!\n";
	return empty;
}
