#include <iostream>
#include <string>
#include "delim.h"

using namespace std;

#define LEN_PURCHASE_ID 17
#define LEN_LECTURE_ID 13
#define LEN_MILEAGE 11
#define STDMAXBUF 256

class Purchase{
private:
	char PurchaseID[17];
	char LectureID[13];
	string MemberID;
	char Mileage[11];

public:
	Purchase();
	Purchase(const char* newPurchase_ID);
	Purchase(const Purchase &);

	Purchase & operator = (const Purchase &s);
	bool operator == (const Purchase &s);
	bool operator != (const Purchase &s);

	friend istream & operator >> (istream &is, Purchase &s);
	friend ostream & operator << (ostream &os, Purchase &s);

	void setPurchaseID(const char* newPurchaseID) { memcpy(PurchaseID, newPurchaseID, LEN_PURCHASE_ID); };
	void setLectureID(const char* newLectureID) { memcpy(LectureID, newLectureID, LEN_LECTURE_ID); };
	void setMemberID(const string newMemberID) { MemberID = newMemberID; };
	void setMileage(const char* newMileage) { memcpy(Mileage, newMileage, LEN_MILEAGE); };

	char* getPurchaseID(void) {
		return PurchaseID;
	};
	char* getLectureID(void) {
		return LectureID;
	};
	string getMemberID(void) {
		return MemberID;
	};
	char* getMileage(void) {
		return Mileage;
	};
	int getLength(void) {
		return strlen(PurchaseID) + strlen(LectureID) + MemberID.size() + strlen(Mileage);
	}

	bool Pack(IOBuffer &Buffer) const;
	bool Unpack(IOBuffer &);
};

istream & operator >> (istream &is, Purchase &s);
ostream & operator << (ostream &os, Purchase &s);