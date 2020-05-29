#pragma once
#include "Event.h"

const char EVENTS_FILE[] = "events.txt";
const char HALLS_FILE[] = "halls.txt";
const char TEMPORARY_FILE[] = "temp.txt";

struct Hall
{
	std::map<size_t, size_t> rows; //row , number of seats on row
	std::vector<Date> busy;
	char name[MAX_HALL_NAME_LENGTH];
};

//copied from cplusplus.com
template <class T> 
struct greater  
{
	T x, y;
	bool operator() (const T& x, const T& y) const { return x > y; }
};

class Manager
{
private:
	std::fstream file;
	std::fstream temp;
	std::fstream hallsFile;
	char fileName[FILENAME_MAX];
	char tempName[FILENAME_MAX];
	char hallsFileName[FILENAME_MAX];
	std::vector<Event*> events;
	std::vector<Hall*> halls;

public:
	Manager(const char* f = nullptr, const char* hf = nullptr);
	~Manager();

	void readHalls();

	void readEvents();
	void writeEvents();

	void callManager();

	void open();
	void close();
	void save();
	void saveas();
	void help();
	void exitFunction(); //had to rename it, because when calling exit(), the program called this function

	void workwithfile();
	void addevent(const Date&, const char* hall, const char* name);
	void freeseats(const Date&, const char*);
	void book(unsigned short row, unsigned short seat, const Date&, const char* eName, const char* note);
	void unbook(unsigned short, unsigned short, const Date&, const char*);
	void buy(unsigned short row, unsigned short seat, const Date&, const char* eName);
	void bookings(const Date&, const char* hall);
	void bookings(const char*);
	void check(const char* code);
	void report(const Date&, const Date&, const char* hall = nullptr);
	void mostwatched();

	std::vector<std::pair<size_t, size_t> > getFreeSeatsManager(const Date&, const char*);
};

