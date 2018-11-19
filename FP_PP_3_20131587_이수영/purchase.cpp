#include "purchase.h"
#include <sstream>

Purchase::Purchase() {}
Purchase::Purchase(const char* newPurchaseID) {
	setPurchaseID(newPurchaseID);
}
Purchase::Purchase(const Purchase &s) {
	setPurchaseID(s.PurchaseID);
	setLectureID(s.LectureID);
	setMemberID(s.MemberID);
	setMileage(s.Mileage);
}

Purchase& Purchase::operator= (const Purchase &s) {
	setPurchaseID(s.PurchaseID);
	setLectureID(s.LectureID);
	setMemberID(s.MemberID);
	setMileage(s.Mileage);
	return *this;
}

bool Purchase::operator==(const Purchase &s) {
	if (strcmp(s.PurchaseID, PurchaseID) == 0) 
		return true;
	else
		return false;
}

bool Purchase::operator!=(const Purchase &s) {
	if (strcmp(s.PurchaseID, PurchaseID) != 0)
		return true;
	else
		return false;
}

bool Purchase::Pack(IOBuffer &Buffer) const {
	int numBytes;
	string sPurchaseID(PurchaseID, LEN_PURCHASE_ID);
	string sLectureID(LectureID, LEN_LECTURE_ID);
	string sMileage(Mileage, LEN_MILEAGE);

	Buffer.Clear();

	numBytes = Buffer.Pack(sPurchaseID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sLectureID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(MemberID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sMileage.c_str()); if (numBytes == -1) return false;
	return true;
}

bool Purchase::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(PurchaseID, LEN_PURCHASE_ID); if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(LectureID, LEN_LECTURE_ID); if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false; MemberID = buf;
	numBytes = Buffer.Unpack(Mileage, LEN_MILEAGE); if (numBytes == -1) return false;
	return true;
}

istream & operator >> (istream &is, Purchase &s) {
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
	s.setPurchaseID(token.data());
	getline(iss, token, '|');
	s.setLectureID(token.data());
	getline(iss, token, '|');
	s.setMemberID(token.data());
	getline(iss, token, '|');
	s.setMileage(token.data());

	return is;
}

ostream & operator << (ostream &os, Purchase &s) {
	string sPurchaseID(s.PurchaseID, LEN_PURCHASE_ID);
	string sLectureID(s.LectureID, LEN_LECTURE_ID);
	string sMileage(s.Mileage, LEN_MILEAGE);

	os << "PurchaseID: " << sPurchaseID << endl;
	os << "LectureID: " << sLectureID << endl;
	os << "MemberID: " << s.MemberID << endl;
	os << "Mileage: " << s.Mileage << endl;
	return os;
}