#include "lecture.h"
#include <sstream>

Lecture::Lecture() {};
Lecture::Lecture(const char* newLectureID) {
	setLectureID(newLectureID);
}
Lecture::Lecture(const Lecture& s) {
	setLectureID(s.LectureID);
	setSubject(s.Subject);
	setLevel(s.Level);
	setPrice(s.Price);
	setExtension(s.Extension);
	setDueDate(s.DueDate);
	setNameOfTeacher(s.NameOfTeacher);
	setTextBook(s.TextBook);
}

Lecture& Lecture::operator = (const Lecture& s) {
	setLectureID(s.LectureID);
	setSubject(s.Subject);
	setLevel(s.Level);
	setPrice(s.Price);
	setExtension(s.Extension);
	setDueDate(s.DueDate);
	setNameOfTeacher(s.NameOfTeacher);
	setTextBook(s.TextBook);
	return *this;
}

bool Lecture::operator == (const Lecture& s) {
	if (strcmp(LectureID, s.LectureID) == 0) 
		return true;
	else
		return false;
}

bool Lecture::operator!=(const Lecture &s) {
	if (strcmp(LectureID, s.LectureID) != 0) 
		return true;
	else
		return false;
}

bool Lecture::Pack(IOBuffer &Buffer) const {
	int numBytes;
	string sLectureID(LectureID, LEN_LECTURE_ID);
	string sLevel(Level, LEN_LEVEL);
	string sPrice = to_string(Price);
	string sExtension(Extension, LEN_EXTENSION);
	string sDueDate = to_string(DueDate);

	Buffer.Clear();
	numBytes = Buffer.Pack(sLectureID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Subject.c_str());	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sLevel.c_str());		if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sPrice.c_str());		if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sExtension.c_str());	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sDueDate.c_str());	if (numBytes == -1) return false;
	numBytes = Buffer.Pack(NameOfTeacher.c_str());if (numBytes == -1) return false;
	numBytes = Buffer.Pack(TextBook.c_str());	if (numBytes == -1) return false;
	return true;
}

bool Lecture::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];
	string sPrice;
	string sDueDate;

	numBytes = Buffer.Unpack(LectureID, LEN_LECTURE_ID);	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);	if (numBytes == -1) return false; Subject= buf;	
	numBytes = Buffer.Unpack(Level, LEN_LEVEL);	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);	if (numBytes == -1) return false; sPrice = buf; Price = stoi(sPrice);
	numBytes = Buffer.Unpack(Extension, LEN_EXTENSION);	if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf);	if (numBytes == -1) return false; sDueDate = buf; DueDate = stoi(sDueDate);
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false; NameOfTeacher = buf;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false; TextBook = buf;
	return true;
}

istream & operator >> (istream & is, Lecture &s) {

	string st;

	is.exceptions(istream::failbit | istream::badbit);

	try {
		getline(is, st);
	}
	catch (istream::failure e) {
		return is;
	}

	istringstream iss(st);
	string token;

	getline(iss, token, '|');
	s.setLectureID(token.data());
	getline(iss, token, '|');
	s.setSubject(token.data());
	getline(iss, token, '|');
	s.setLevel(token.data());
	getline(iss, token, '|');
	s.setPrice(stoi(token.data()));
	getline(iss, token, '|');
	s.setExtension(token.data());
	getline(iss, token, '|');
	s.setDueDate(stoi(token.data()));
	getline(iss, token, '|');
	s.setNameOfTeacher(token.data());
	getline(iss, token, '|');
	s.setTextBook(token.data());
	return is;

}

ostream & operator << (ostream &os, Lecture &s) {
	string sLectureID(s.LectureID, LEN_LECTURE_ID);
	string sLevel(s.Level, LEN_LEVEL);
	string sPrice = to_string(s.Price);
	string sExtension(s.Extension, LEN_EXTENSION);
	string sDueDate = to_string(s.DueDate);

	os << "LectureID: " << s.LectureID << endl;
	os << "Subject: " << s.Subject << endl;
	os << "Level: " << s.Level << endl;
	os << "Price: " << s.Price << endl;
	os << "Extension: " << s.Extension << endl;
	os << "DueDate: " << s.DueDate << endl;
	os << "NameOfTeacher: " << s.NameOfTeacher << endl;
	os << "TextBook: " << s.TextBook << endl;
	return os;
}

// Project2 added.
char* Lecture::Key(void) {
	key.assign(LectureID, 12);
	return (char*)(key.c_str());
}