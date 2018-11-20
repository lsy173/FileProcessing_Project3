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
	// Project3 added.
	setKey(s.key);
}

Purchase& Purchase::operator= (const Purchase &s) {
	setPurchaseID(s.PurchaseID);
	setLectureID(s.LectureID);
	setMemberID(s.MemberID);
	setMileage(s.Mileage);
	// Project3 added.
	setKey(s.key);
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
	// Project3 added.
	char temp[2];
	temp[0] = key;
	temp[1] = '\0';
	string KEY(temp, 1);


	Buffer.Clear();

	numBytes = Buffer.Pack(sPurchaseID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sLectureID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(MemberID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sMileage.c_str()); if (numBytes == -1) return false;
	// Project3 added.
	numBytes = Buffer.Pack(KEY.c_str()); if (numBytes == -1) return false;
	return true;
}

bool Purchase::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];
	char temp[1000];

	numBytes = Buffer.Unpack(PurchaseID, LEN_PURCHASE_ID); if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(LectureID, LEN_LECTURE_ID); if (numBytes == -1) return false;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false; MemberID = buf;
	numBytes = Buffer.Unpack(Mileage, LEN_MILEAGE); if (numBytes == -1) return false;
	// Project3 added.
	numBytes = Buffer.Unpack(temp, 2);
	key = temp[0]; if (numBytes == -1) return false;
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
	// Project3 added.
	char temp[2];
	temp[0] = s.key;
	temp[1] = '\0';
	string KEY(temp, 2);


	os << "PurchaseID: " << sPurchaseID << endl;
	os << "LectureID: " << sLectureID << endl;
	os << "MemberID: " << s.MemberID << endl;
	os << "Mileage: " << s.Mileage << endl;
	// Project3 added.
	os << "Key : " << KEY << endl;
	return os;
}

char Purchase::Key() {
	return key;
}