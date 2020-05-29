#pragma once
#include "Date.h"

const unsigned short MAX_ROWS = 10;
const unsigned short MAX_SEATS = 10;
const unsigned short MAX_EVENT_NAME_LENGTH = 1000;
const unsigned short MAX_HALL_NAME_LENGTH = 1000;
const int MAX_TICKET_CODE_LENGTH = MAX_EVENT_NAME_LENGTH + DATE_LENGTH + MAX_HALL_NAME_LENGTH + 2 * 5;

const unsigned short MAX_NOTE_LENGTH = 500;

bool validCode(const char*);

class Ticket
{
private:
	char* code;
	char* note;
	bool booked; 
	bool bought;

	void print_booked_ticket() const;
	void print_bought_ticket() const;
	void print_regular_ticket() const;

public:

	Ticket(const char* c = nullptr);
	Ticket(const Ticket&);
	Ticket& operator=(const Ticket&);
	~Ticket();

	void setCode(const char*);
	void setBookedTo(bool);
	void setBoughtTo(bool);
	void setNote(const char*);
	void setCodeviaParam(const Date&, const char* eName = UNKNOWN, const char* h = UNKNOWN, const unsigned short r = 0, const unsigned short s = 0);

	const char* getCode() const { return this->code; };
	bool getBookedValue() const { return this->booked; };
	bool getBoughtValue() const { return this->bought; };
	const char* getNote() const { return this->note; };

	//the information the ticket holds
	const char* getEvent() const;
	const Date& getDate() const;
	const char* getHall() const;
	const unsigned short getRow() const;
	const unsigned short getSeat() const;


	void print_ticket() const;

	bool operator==(const Ticket&) const;
	friend std::ostream& operator<<(std::ostream&, const Ticket&);
	friend std::istream& operator>>(std::istream&, Ticket&);
};

