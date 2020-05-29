#include "Ticket.h"

bool validCode(const char* code) 
{

	if (code == nullptr)
	{
		return false;
	}

	//in the code there must be 4 $(the delimiters between the different parts), so we will check first this
	//the delimiter for dates is '-' and the one for event names in the ticket code is '.'
	int count = 0;
	int length = strlen(code) + 1;
	for (int i = 0; i < length; i++)
	{
		if (code[i] == DELIMITER4)
		{
			count++;
		}
	}
	if (count != 4) return false;

	std::stringstream ss(code);
	char* eventN = new char[MAX_EVENT_NAME_LENGTH];
	Date d;
	char* hall = new char[MAX_HALL_NAME_LENGTH];
	unsigned short row = 0;
	unsigned short seat = 0;

	ss.getline(eventN, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	ss >> d;
	ss.get();
	ss.getline(hall, MAX_HALL_NAME_LENGTH, DELIMITER4);
	ss >> row;
	ss.get();
	ss >> seat;

	if (strlen(eventN) == 0 || !d.isValidDate() || strlen(hall) == 0 || row == 0 || seat == 0 || row > MAX_ROWS || seat > MAX_SEATS)
	{
		delete[] eventN;
		delete[] hall;
		return false;
	}
	else
	{
		delete[] eventN;
		delete[] hall;
		return true;
	}
}

Ticket::Ticket(const char* c) : note(nullptr), booked(false), bought(false)
{
	if (c == nullptr) this->code = nullptr;
	setCode(c);
}

Ticket::Ticket(const Ticket& t) :  booked(t.booked), bought(t.bought)
{
	setCode(t.getCode());
	setNote(t.getNote());
}

Ticket& Ticket::operator=(const Ticket& t)
{
	if (this != &t)
	{
		setCode(t.getCode());
		setBookedTo(t.getBookedValue());
		setBoughtTo(t.getBoughtValue());
		setNote(t.getNote());
	}

	return *this;
}

Ticket::~Ticket()
{
	delete[] this->code;
	this->code = nullptr;
}

void Ticket::setCode(const char* nv)
{
	if (code != nullptr)
	{
		delete[] this->code;
		this->code = nullptr;
	}

	if (validCode(nv))
	{
		int length = strlen(nv) + 1;
		if (length)
		{
			this->code = new char[length];
			strcpy_s(this->code, length, nv);
		}
	}
}

void Ticket::setNote(const char* nv)
{
	if (note != nullptr)
	{
		delete[] note;
		note = nullptr;
	}

	if (nv != nullptr)
	{
		int length = strlen(nv) + 1;
		if (length < MAX_NOTE_LENGTH && nv != nullptr)
		{
			note = new char[length];
			strcpy_s(note, length, nv);
		}
	}

}

void Ticket::setBookedTo(bool nv)
{
	this->booked = nv;
}

void Ticket::setBoughtTo(bool nv)
{
	if (nv)
	{
		this->booked = false;
		this->bought = nv;
	}
}

void Ticket::setCodeviaParam(const Date& d, const char* eName, const char* h, const unsigned short r, const unsigned short s)
{
	std::stringstream ss;
	ss << eName << DELIMITER4
		<< d << DELIMITER4
		<< h << DELIMITER4
		<< r << DELIMITER4
		<< s;
	setCode(ss.str().c_str()); 
}

const char* Ticket::getEvent() const
{
	std::stringstream ss(getCode());
	char* eventN = new char[MAX_EVENT_NAME_LENGTH];
	ss.getline(eventN, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	return eventN;

}

const Date& Ticket::getDate() const
{
	std::stringstream ss(getCode());
	char* temp = new char[MAX_EVENT_NAME_LENGTH];
	for (int wordpos = 0; wordpos < 1; wordpos++)
	{
		ss.getline(temp, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	}
	delete[] temp;

	Date* d = new Date;
	ss >> *d;
	return *d;

}

const char* Ticket::getHall() const
{
	std::stringstream ss(getCode());
	char* temp = new char[MAX_EVENT_NAME_LENGTH];
	for (int wordpos = 0; wordpos < 2; wordpos++)
	{
		ss.getline(temp, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	}
	delete[] temp;

	char* hall = new char[MAX_HALL_NAME_LENGTH];
	ss.getline(hall, MAX_HALL_NAME_LENGTH, DELIMITER4);

	return hall;

}

const unsigned short Ticket::getRow() const
{
	std::stringstream ss(getCode());
	char* temp = new char[MAX_EVENT_NAME_LENGTH];
	for (int wordpos = 0; wordpos < 3; wordpos++)
	{
		ss.getline(temp, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	}
	delete[] temp;

	unsigned short row = 0;
	ss >> row;
	return row;
}

const unsigned short Ticket::getSeat() const
{
	std::stringstream ss(getCode());
	char* temp = new char[MAX_EVENT_NAME_LENGTH];
	for (int wordpos = 0; wordpos < 4; wordpos++)
	{
		ss.getline(temp, MAX_EVENT_NAME_LENGTH, DELIMITER4);
	}
	delete[] temp;

	unsigned short seat = 0;
	ss >> seat;
	return seat;
}

void Ticket::print_ticket() const
{
	if (booked)
	{
		print_booked_ticket();
	}
	else if (bought)
	{
		print_bought_ticket();
	}
	else
	{
		print_regular_ticket();
	}
}



void Ticket::print_booked_ticket() const
{
	int length = strlen(code);
	//we remove 13 from length, because we need to write BOOKED TICKET in the middle of the lane
	//it is sage to remove 13, because the length of the code is at least 15 (date + delimiters), if it is correct 
	int middle = (length - 13) / 2;

	//one tab is 7 spaces and you write the new char at the 8th pos
	int lengthAfterEvent = length - strlen(getEvent()) - 7;
	int lengthAfterDate = length - (DATE_LENGTH - 1) - 7;
	int lengthAfterHall = length - strlen(getHall()) - 7;
	int lengthAfterRow = length - strlen(std::to_string(getRow()).c_str()) - 7;
	int lengthAfterSeat = length - strlen(std::to_string(getSeat()).c_str()) - 7;

	std::cout << '\t' << DELIMITER << std::string(length, DELIMITER_) << DELIMITER << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n';

	if (length % 2 == 0)
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "BOOKED TICKET" << std::string(middle + 1, DELIMITER) << DELIMITER5 << '\n';
	}
	else
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "BOOKED TICKET" << std::string(middle, DELIMITER) << DELIMITER5 << '\n';
	}

	std::cout << '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << getCode() << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Event:\t" << getEvent() << std::string(lengthAfterEvent, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Date:\t" << getDate() << std::string(lengthAfterDate, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Hall:\t" << getHall() << std::string(lengthAfterHall, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Row:\t" << getRow() << std::string(lengthAfterRow, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Seat:\t" << getSeat() << std::string(lengthAfterSeat, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n';

}

void Ticket::print_bought_ticket() const
{
	int length = strlen(code);
	//we remove 13 from length, because we need to write BOOKED TICKET in the middle of the lane
	//it is sage to remove 13, because the length of the code is at least 15 (date + delimiters), if it is correct 
	int middle = (length - 13) / 2;

	//one tab is 7 spaces and you write the new char at the 8th pos
	int lengthAfterEvent = length - strlen(getEvent()) - 7;
	int lengthAfterDate = length - (DATE_LENGTH - 1) - 7;
	int lengthAfterHall = length - strlen(getHall()) - 7;
	int lengthAfterRow = length - strlen(std::to_string(getRow()).c_str()) - 7;
	int lengthAfterSeat = length - strlen(std::to_string(getSeat()).c_str()) - 7;

	std::cout << '\t' << DELIMITER << std::string(length, DELIMITER_) << DELIMITER << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n';

	if (length % 2 == 0)
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "BOUGHT TICKET" << std::string(middle + 1, DELIMITER) << DELIMITER5 << '\n';
	}
	else
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "BOUGHT TICKET" << std::string(middle, DELIMITER) << DELIMITER5 << '\n';
	}

	std::cout << '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << getCode() << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Event:\t" << getEvent() << std::string(lengthAfterEvent, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Date:\t" << getDate() << std::string(lengthAfterDate, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Hall:\t" << getHall() << std::string(lengthAfterHall, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Row:\t" << getRow() << std::string(lengthAfterRow, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Seat:\t" << getSeat() << std::string(lengthAfterSeat, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n';

}

void Ticket::print_regular_ticket() const
{
	int length = strlen(code);
	//we remove 6 from length, because we need to write TICKET in the middle of the lane
	int middle = (length - 6) / 2;

	//one tab is 7 spaces and you write the new char at the 8th pos
	int lengthAfterEvent = length - strlen(getEvent()) - 7;
	int lengthAfterDate = length - (DATE_LENGTH - 1) - 7;
	int lengthAfterHall = length - strlen(getHall()) - 7;
	int lengthAfterRow = length - strlen(std::to_string(getRow()).c_str()) - 7;
	int lengthAfterSeat = length - strlen(std::to_string(getSeat()).c_str()) - 7;

	std::cout << '\t' << DELIMITER << std::string(length, DELIMITER_) << DELIMITER << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n';

	if (length % 2 == 0)
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "TICKET" << std::string(middle, DELIMITER) << DELIMITER5 << '\n';
	}
	else
	{
		std::cout << '\t' << DELIMITER5 << std::string(middle, DELIMITER) << "TICKET" << std::string(middle + 1, DELIMITER) << DELIMITER5 << '\n';
	}

	std::cout << '\t' << DELIMITER5 << std::string(length, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << getCode() << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Event:\t" << getEvent() << std::string(lengthAfterEvent, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Date:\t" << getDate() << std::string(lengthAfterDate, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Hall:\t" << getHall() << std::string(lengthAfterHall, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Row:\t" << getRow() << std::string(lengthAfterRow, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << "Seat:\t" << getSeat() << std::string(lengthAfterSeat, DELIMITER) << DELIMITER5 << '\n'
		<< '\t' << DELIMITER5 << std::string(length, DELIMITER_) << DELIMITER5 << '\n';

}

bool Ticket::operator==(const Ticket& t) const
{
	return (strcmp(getCode(), t.getCode()) == 0);
}

std::ostream& operator<<(std::ostream& os, Ticket const& t)
{
	os << t.getCode();

	if (t.note != nullptr)
	{
		return os << DELIMITER << "Note:" << DELIMITER << t.getNote();
	}
	else
	{
		return os;
	}
}

std::istream& operator>>(std::istream& is, Ticket& t)
{
	
	char line[MAX_TICKET_CODE_LENGTH + MAX_NOTE_LENGTH + 2];
	char code[MAX_TICKET_CODE_LENGTH];
	char note[MAX_NOTE_LENGTH];
	char temp[10];

	is.getline(line, MAX_TICKET_CODE_LENGTH + MAX_NOTE_LENGTH);
	std::stringstream ss(line);
	
	ss.getline(code, MAX_NOTE_LENGTH, DELIMITER);
	if (strchr(line, DELIMITER))
	{
		ss.getline(temp, 10, DELIMITER);
		ss.getline(note, MAX_NOTE_LENGTH);
		t.setCode(code);
		t.setNote(note);
		t.setBookedTo(true);
	}
	else
	{
		t.setCode(code);
		t.setBoughtTo(true);
	}

	return is;
}


