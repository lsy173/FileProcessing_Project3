#include "member.h"
#include <sstream>

Member::Member() {};
Member::Member(const string newID) {
	setID(newID);
};
Member::Member(const Member &s) {
	setID(s.ID);
	setPassword(s.Password);
	setName(s.Name);
	setPhoneNumber(s.PhoneNumber);
	setAddress(s.Address);
	setMileage(s.Mileage);
	// Project2 added.
	setLevel();
}
Member& Member::operator=(const Member &s) {
	setID(s.ID);
	setPassword(s.Password);
	setName(s.Name);
	setPhoneNumber(s.PhoneNumber);
	setAddress(s.Address);
	setMileage(s.Mileage);
	// Project2 added.
	setLevel();
	return *this;
}

bool Member::operator==(const Member &s) {
	return (getID() == s.ID);
}

bool Member::operator!=(const Member &s) {
	return (getID() != s.ID);
}

bool Member::Pack(IOBuffer &Buffer) const {
	int numBytes;
	string sMileage(Mileage, LEN_MIL);
	// Project 2 added.
	string sLevel(Level, LEN_LEVEL);

	Buffer.Clear();
	numBytes = Buffer.Pack(ID.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Password.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Name.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(PhoneNumber.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(Address.c_str()); if (numBytes == -1) return false;
	numBytes = Buffer.Pack(sMileage.c_str()); if (numBytes == -1) return false;
	// Project2 added.
	numBytes = Buffer.Pack(sLevel.c_str()); if (numBytes == -1) return false;
	return true;
}

bool Member::Unpack(IOBuffer &Buffer) {
	int numBytes;
	char buf[STDMAXBUF];

	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false;
	ID = buf;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false;
	Password = buf;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false;
	Name = buf;
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false;
	PhoneNumber = buf; 
	numBytes = Buffer.Unpack(buf); if (numBytes == -1) return false;
	Address = buf;
	numBytes = Buffer.Unpack(Mileage, LEN_MIL); if (numBytes == -1) return false;
	// Project2 added.
	numBytes = Buffer.Unpack(Level, 2); if (numBytes == -1) return false;

	return true;
}

istream & operator >> (istream & is, Member &s) {
	string st;

	is.exceptions(istream::failbit | istream::badbit);

	try {
		getline(is, st);
	}
	catch(istream::failure e){
		return is;
	}

	istringstream iss(st);
	string token;

	getline(iss, token, '|');
	s.setID(token.data());
	getline(iss, token, '|');
	s.setPassword(token.data());
	getline(iss, token, '|');
	s.setName(token.data());
	getline(iss, token, '|');
	s.setPhoneNumber(token.data());
	getline(iss, token, '|');
	s.setAddress(token.data());
	getline(iss, token, '|');
	s.setMileage(token.data());
	// Project2 added.
	s.setLevel();

	return is;

}

ostream & operator << (ostream &os, Member &s) {
	string Mileage(s.Mileage, LEN_MIL);

	os << "ID : " << s.ID << endl;
	os << "Password : " << s.Password << endl;
	os << "Name : " << s.Name << endl;
	os << "Phone_Number : " << s.PhoneNumber << endl;
	os << "Address : " << s.Address << endl;
	os << "Mileage : " << Mileage << endl;
	// Project2 added.
	os << "Level : " << s.Level << endl;

	return os;
}

// Project 2 added.
char* Member::Key(void) {
	key.assign(ID.c_str(), strlen(ID.c_str()));
	return (char*)(key.c_str());
}