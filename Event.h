#pragma once
#include "Ticket.h"

class Event
{
private:
	char* eventName;
	std::map<Date, char*> shows;
	Ticket* tickets;
	size_t ticketsSize;
	size_t ticketsCapacity;

	bool addTicket(const Ticket&);

public:
	Event(const char* n = nullptr);
	Event(const Event&);
	Event& operator=(const Event&);
	~Event();

	void setName(const char*);
	bool addShow(const Date&, const char*);	

	const char* getEventName() const { return this->eventName; };
	std::map<Date, char*> getShows() const { return this->shows; };
	Ticket* getTickets() const { return this->tickets; };
	size_t getSize() const { return this->ticketsSize; };

	bool bookTicket(Ticket&);
	bool buyTicket(Ticket&);
	void unbookTicket(const Ticket&);
	void report(const Date&, const Date&,  const char* hall = nullptr);
	std::vector<std::pair<size_t, size_t> > getFreeSeats(const Date&, std::map<size_t, size_t> places);

	friend std::ostream& operator<<(std::ostream&, const Event&);
	friend std::istream& operator>>(std::istream&, Event&);
};

