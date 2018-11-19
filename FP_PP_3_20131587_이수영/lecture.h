#ifndef LECTURE_H
#define LECTURE_H

#include <iostream>
#include <string>
#include "delim.h"

#define LEN_LECTURE_ID 13
#define LEN_LEVEL 2
#define LEN_EXTENSION 2
#define STDMAXBUF 256

using namespace std;

class Lecture {
private: // fields.
	char LectureID[LEN_LECTURE_ID];
	string Subject;
	char Level[LEN_LEVEL];
	int Price;
	char Extension[LEN_EXTENSION];
	int DueDate;
	string NameOfTeacher;
	string TextBook;
	// Project2 added.
	string key;

public: // operations.
	Lecture();
	Lecture(const char * newlectureID);
	Lecture(const Lecture &s);
	
	Lecture & operator = (const Lecture &s);
	bool operator == (const Lecture &s);
	bool operator != (const Lecture &s);

	friend istream & operator >> (istream &is, Lecture &s);
	friend ostream & operator << (ostream &os, Lecture &s);

	void setLectureID(const char* newLectureID) { memcpy(LectureID, newLectureID, LEN_LECTURE_ID); }
	void setSubject(const string  newSubject) { Subject = newSubject; }
	void setLevel(const char* newLevel) { memcpy(Level, newLevel, LEN_LEVEL); }
	void setPrice(const int newPrice) { Price = newPrice; }
	void setExtension(const char* newExtension) { memcpy(Extension, newExtension, LEN_EXTENSION); }
	void setDueDate(const int newDueDate) { DueDate = newDueDate; }
	void setNameOfTeacher(const string newNameOfTeacher) { NameOfTeacher = newNameOfTeacher; }
	void setTextBook(const string newTextBook) { TextBook = newTextBook; }

	char* getLectureID(void) {
		return LectureID;
	}
	string getSubject(void) {
		return Subject;
	}
	char* getLevel(void) {
		return Level;
	}
	int getPrice(void) {
		return Price;
	}
	char* getExtension(void) {
		return Extension;
	}
	int getDueDate(void) {
		return DueDate;
	}
	string getNameOfTeacher(void) {
		return NameOfTeacher;
	}
	string getTextBook(void) {
		return TextBook;
	}
	int getLength(void) {
		return strlen(LectureID) + Subject.length() + strlen(Level) + sizeof(Price) + strlen(Extension) + sizeof(DueDate) + NameOfTeacher.length() + TextBook.length();
	}

	bool Pack(IOBuffer &Buffer) const;
	bool Unpack(IOBuffer &);

	// Project2 added.
	char * Key();
};

istream & operator >> (istream &is, Lecture &s);
ostream & operator << (ostream &os, Lecture &s);
#endif