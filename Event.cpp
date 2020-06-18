#include "Event.h"

Event::Event(const char* n) : shows(), ticketsSize(0), ticketsCapacity(100)
{
	setName(n);
	tickets = new Ticket[ticketsCapacity];
}

Event::Event(const Event& e)
{
	if (!shows.empty()) shows.clear();
	if (tickets != nullptr) delete[] tickets;

	setName(e.getEventName());
	shows = e.shows;
	if (e.tickets != nullptr && e.ticketsSize > 0)
	{
		tickets = new Ticket[e.ticketsSize];
		for (unsigned i = 0; i < e.ticketsSize; i++)
		{
			tickets[i] = e.tickets[i];
		}
	}
	ticketsSize = e.ticketsSize;
	ticketsCapacity = e.ticketsCapacity;
}

Event& Event::operator=(const Event& e)
{
	if (this != &e)
	{
		if (!shows.empty()) shows.clear();
		if (tickets != nullptr) delete[] tickets;

		setName(e.getEventName());
		shows = e.shows;
		if (e.tickets != nullptr && e.ticketsSize > 0)
		{
			tickets = new Ticket[e.ticketsSize];
			for (unsigned i = 0; i < e.ticketsSize; i++)
			{
				tickets[i] = e.tickets[i];
			}
		}
		ticketsSize = e.ticketsSize;
		ticketsCapacity = e.ticketsCapacity;
	}

	return *this;
}

Event::~Event()
{
	if (eventName != nullptr)
	{
		delete[] eventName;
		eventName = nullptr;
	}

	shows.erase(shows.begin(), shows.end());
	shows.clear();

	if (tickets != nullptr)
	{
		delete[] tickets;
		tickets = nullptr;
	}
}

void Event::setName(const char* nv)
{
	if (eventName != nullptr)
	{
		delete[] eventName;
		eventName = nullptr;
	}

	if (nv != nullptr )
	{
		int length = strlen(nv) + 1;
		if (length)
		{
			this->eventName = new char[length];
			strcpy_s(this->eventName, length, nv);
		}
	}
}

bool Event::addShow(const Date& date, const char* hall)
{
	if (shows.count(date) == 1)
	{
		return false;
	}

	int length = strlen(hall) + 1;
	char* temp = new char[length];
	strcpy_s(temp, length, hall);
	shows.insert(std::pair<Date, char*>(date, temp));
	return true;
}

bool Event::addTicket(const Ticket& t)
{
	if (!validCode(t.getCode())) return false;

	if (strcmp(t.getEvent(), eventName) != 0)
	{
		return false;
	}

	bool found = false;
	for (auto& x : shows)
	{
		if (t.getDate() == x.first && strcmp(t.getHall(), x.second) == 0)
		{
			found = true;
		}
	}
	if (!found)
	{
		std::cerr << "No such show on this date!\n";
		return false;
	}

	for (size_t i = 0; i < ticketsSize; ++i)
	{
		if (t == tickets[i])
		{
			return false;
		}
	}

	if (ticketsSize >= ticketsCapacity)
	{
		ticketsCapacity+=10;

		Ticket* temp = new  Ticket[ticketsCapacity];

		for (size_t i = 0; i < ticketsSize; ++i)
		{
			temp[i] = tickets[i];
		}

		delete[] tickets;
		tickets = temp;
	}

	tickets[ticketsSize] = t;
	ticketsSize++;
	return true;
}

bool Event::bookTicket( Ticket& t)
{
	if (addTicket(t))
	{
		t.setBookedTo(true);
		return true;
	}
	else return false;
	
}

bool Event::buyTicket(Ticket& t)
{
	if (addTicket(t))
	{
		t.setBoughtTo(true);
		return true;
	}
	else return false;
}

void Event::unbookTicket(const Ticket& t)
{
	for (size_t i = 0; i < ticketsSize; i++)
	{
		if (t == tickets[i] && tickets[i].getBookedValue())
		{
			for (size_t j = i; j < ticketsSize - 1; j++)
			{
				tickets[j] = *new Ticket(tickets[j + 1]);
			}		
			tickets[ticketsSize - 1] = nullptr;
			ticketsSize--;
			std::cout << "Ticket successfully unbooked!\n";
			return;
		}
	}

	std::cerr << "Can't unbook this ticket!\n";
}

void Event::report(const Date& d1, const Date& d2, const char* hall)
{
	//do some validation
	/*if (d1 > Date() || d2 > Date())
	{
		std::cerr << "Enter dates before the current!\n";
		return;
	}*/
	if (d1 > d2)
	{
		std::cerr << "The first date should be before the second one!\n";
		return;
	}

	int count = 0;
	std::vector<Date> interval = getDatesBetween(d1, d2);
	for (size_t i = 0; i < ticketsSize; i++)
	{
		if (tickets[i].getBoughtValue())
		{
			if (hall == nullptr)
			{
				for (auto const& x : interval)
				{
					if (tickets[i].getDate() == x)
					{
						count++;
					}
				}
			}
			else if (strlen(hall) == 0 || strcmp(tickets[i].getHall(), hall) == 0)
			{
				for (auto const& x : interval)
				{
					if (tickets[i].getDate() == x)
					{
						count++;
					}
				}
			}
		}
	}

	 std::cout << eventName << "\t\t" << count << '\n';
}

std::vector<std::pair<size_t, size_t> > Event::getFreeSeats(const Date& d, std::map<size_t, size_t> places)
{
	std::vector<std::pair <size_t, size_t> > freeS;

	//push all the seats in a vector (even if they are not free)
	for (auto x : places)
	{
		for (size_t i =1; i<= x.second; i++)
		freeS.push_back(std::pair<size_t, size_t>(x.first, i));
	}

	//delete the booked/bought tickets from the vector
	for (size_t i = 0; i <ticketsSize; i++)
	{
		if (tickets[i].getDate() == d)
		{
			size_t r = tickets[i].getRow();
			size_t s = tickets[i].getSeat();

			auto begin = freeS.begin();
			auto end = freeS.end();
			for (auto it = begin; it != end; ++it)
			{
				if ((*it).first == r && (*it).second == s)
				{
					freeS.erase(it);
					break;
				}
			}
		}
	}

	return freeS;
}

std::ostream& operator<<(std::ostream& os, const Event& e)
{
	os <<"EVENT:"<<DELIMITER<< e.getEventName() << '\n';
	os << "----------" <<'\n';
	for (auto& x : e.shows)
	{
		os <<"DATE:" <<DELIMITER<< x.first << DELIMITER<<"HALL:" <<DELIMITER<< x.second<< '\n';
		for (size_t i = 0; i < e.ticketsSize ; i++)
		{
			if (e.tickets[i].getDate() == x.first)
			{
				os << e.tickets[i] << '\n';
			}
		}
		os << "----------" << '\n';
	}
	return os << "----------" << '\n' << '\n';
}

std::istream& operator>>(std::istream& is, Event& e)
{
	char temp[15];
	char event[MAX_EVENT_NAME_LENGTH];
	Date d;
	char* hall = new char[MAX_HALL_NAME_LENGTH];
	Ticket t;

	e.ticketsSize = 0;

	is.getline(temp, 15, DELIMITER);
	is.getline(event, MAX_EVENT_NAME_LENGTH);
	is.getline(temp, 15);

	if (strlen(event) <= 0) return is;

	e.setName(event);
	
	while ( is.peek() != (int)DELIMITER2)//while not new event
	{
			is.getline(temp, 15, DELIMITER); // Date:
			is >> d;
			is.get();
			is.getline(temp, 15, DELIMITER); //Hall:
			is.getline(hall, MAX_HALL_NAME_LENGTH);
			e.addShow(d, hall);
			
			while (is.peek() != (int)DELIMITER2)
			{
				is >> t;
				if (t.getBookedValue())
				{
					e.bookTicket(t);
				}
				else e.buyTicket(t);
			}

			hall = new char[MAX_HALL_NAME_LENGTH];
			is.getline(temp, 15);
	}

	delete[] hall;
	
	is.getline(temp, 15);
	is.get();

	return is;

}
