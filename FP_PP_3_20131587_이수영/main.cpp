#include <iostream>
#include <fstream>
#include <string>
#include "deltext.h"
#include "iobuffer.h"
#include "member.h"
#include "lecture.h"
#include "purchase.h"
#include "buffile.h"
#include "recfile.h"
// Project2 added.
#include "indfile.h"
#include "buffile.h"
#include "tindbuffer.h"
#include "textind.h"
#include <fstream>
#include <vector>
// Project3 added.
#include "simpind.h"
#include "simpind.cpp"
#include "btree.h"
#include "btree.cpp"
#include "btnode.h"
#include "btnode.cpp"

using namespace std;

void showMenu(void);
void showMember(void);
void showLecture(void);
void showPurchase(void);
void MemberTest(int print_flag);
void LectureTest(int print_flag);
void PurchaseTest(int print_flag);
void LecturePurchaseSystem(void);
void showSystemMenu(void);
int SearchingByID(int flag, string filename, string wantID);
void InsertingByID(string log_id, int level);
void DeletingByID(string log_id, int level);
void ModifyingByID(string log_id, int level);
void DeleteMember(string ID);
void DeleteLecture(string ID);
void DeletePurchase(string ID);

// Project2 added.
void MakeIndexFile(void);
int SearchingByPW(int flag, string filename, string wantID, string wantPW);
int proveAssociation(string loginID, string wantID);

// Project3 added.
void DeleteKey(void);
void make_purchaseIdxFile(void);

Member* memberArr = new Member[12000];
Lecture* lectureArr = new Lecture[12000];
Purchase* purchaseArr = new Purchase[12000];
int index;
// Project2 added.
int mindex;
int lindex;
int pindex;
// Project3 added.
const char * keys = "CSDTAMPIBWNGURKEHOLJYQZFXV";
const int BTreeSize = 4;
BTree <char> bt(BTreeSize);
char world_key;

int main(void) {
	string input;
	int input_int;
	while (1) {
		showMenu();
		std::cin >> input;
		std::cout << "*************************************" << endl;
		input_int = atoi(input.c_str());
		switch (input_int) {
		case 1: showMember(); break;
		case 2: showLecture(); break;
		case 3: showPurchase(); break;
		case 4: MemberTest(1); break;
		case 5: LectureTest(1); break;
		case 6: PurchaseTest(1); break;
		case 7: LecturePurchaseSystem(); break;
		case 8: return 0;
		default: break;
		}
	}
	return 0;
}

void showMenu(void) {
	std::cout << "*************************************" << endl;
	std::cout << "1. showMember            ***********" << endl;
	std::cout << "2. showLecture           ***********" << endl;
	std::cout << "3. showPurchase          ***********" << endl;
	std::cout << "4. MemberTest            ***********" << endl;
	std::cout << "5. LectureTest           ***********" << endl;
	std::cout << "6. PurchaseTest          ***********" << endl;
	std::cout << "7. LecturePurchaseSystem ***********" << endl;
	std::cout << "8. Exit                  ***********" << endl;
	std::cout << "*************************************" << endl;
	std::cout << "What do you want to do? : ";
}


void showMember(void) {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail())
	{
		std::cout << "listOfMember.txt File Open Error!" << endl << endl;
		return;
	}

	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i<10; i++)
	{
		Member M;
		ifs >> M;
		std::cout << M << endl;
	}
	ifs.close();
	return;
}


void showLecture(void) {
	ifstream ifs("listOfLecture.txt");
	if (ifs.fail()) {
		std::cout << "Opening listOfLetcure.txt File Fail!" << endl;
		return;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i<10; i++){
		Lecture M;
		ifs >> M;
		std::cout << M << endl;
	}
	ifs.close();
	return;
}

void showPurchase() {
	ifstream ifs("listOfPurchase.txt");
	if (ifs.fail()) {
		std::cout << "Opening listOfPurchase.txt File Fail!" << endl;
		return;
	}
	int temp;
	ifs >> temp;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	for (int i = 0; i<10; i++){
		Purchase M;
		ifs >> M;
		std::cout << M << endl;
	}
	ifs.close();
	return;
}

void MemberTest(int print_flag) {
	ifstream ifs("listOfMember.txt");
	if (ifs.fail()) {
		std::cout << "Opening listOfMember.txt File Fail!" << endl;
		return;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	mindex = n;
	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Member> MemberFile(buffer);

	// Write test
	MemberFile.Create("fileOfMember.dat", ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Member s;
		ifs >> s;
		int recaddr;
		if ((recaddr = MemberFile.Write(s)) == -1)
			if (print_flag)
				std::cout << "Write Error!" << endl;
			else
				if (print_flag)
					std::cout << "Write at " << recaddr << endl;
	}
	MemberFile.Close();

	// Read test
	MemberFile.Open("fileOfMember.dat", ios::in);
	for (int i = 0; i < 10; i++) {
		Member s;
		MemberFile.Read(s);
		if (print_flag)
			std::cout << s << endl;
	}
	MemberFile.Close();
	ifs.close();
	return;
}

void LectureTest(int print_flag) {
	ifstream ifs("listOfLecture.txt");
	if (ifs.fail()){
		std::cout << "Opening listOfLecture.txt File Fail!" << endl;
		return;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	lindex = n;
	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Lecture> LectureFile(buffer);

	// Write test
	LectureFile.Create("fileOfLecture.dat", ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Lecture s;
		ifs >> s;
		int recaddr;
		if ((recaddr = LectureFile.Write(s)) == -1)
			if (print_flag)
				std::cout << "Write Error!" << endl;
			else
				if (print_flag)
					std::cout << "Write at " << recaddr << endl;
	}
	LectureFile.Close();

	// Read test
	LectureFile.Open("fileOfLecture.dat", ios::in);
	for (int i = 0; i < 10; i++) {
		Lecture s;
		LectureFile.Read(s);
		if (print_flag)
			std::cout << s << endl;
	}
	LectureFile.Close();
	ifs.close();
	return;
}

void PurchaseTest(int print_flag) {
	ifstream ifs("listOfPurchase.txt");
	if (ifs.fail()){
		std::cout << "Opening listOfPurchase.txt File Fail!" << endl;
		return;
	}

	int n;
	ifs >> n;
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	pindex = n;
	DelimFieldBuffer buffer('|', 10000);
	RecordFile <Purchase> PurchaseFile(buffer);

	// Write test
	PurchaseFile.Create("fileOfPurchase.dat", ios::out | ios::trunc);
	for (int i = 0; i < n; i++) {
		Purchase s;
		ifs >> s;
		int recaddr;
		if ((recaddr = PurchaseFile.Write(s)) == -1)
			if (print_flag)
				std::cout << "Write Error!" << endl;
			else
				if (print_flag)
					std::cout << "Write at " << recaddr << endl;
	}
	PurchaseFile.Close();

	// Read test
	PurchaseFile.Open("fileOfPurchase.dat", ios::in);
	for (int i = 0; i < 10; i++) {
		Purchase s;
		PurchaseFile.Read(s);
		if (print_flag)
			std::cout << s << endl;
	}
	PurchaseFile.Close();
	ifs.close();
	return;
}

void LecturePurchaseSystem() {
	string input;
	int input_int;
	string filename;
	string want_ID;
	static int test = 0;
	// Project2 added.
	string filename_index;
	static string login_id = "";
	static string login_pw = "";
	static int access_level;

	if (!test) {
		MemberTest(0);
		LectureTest(0);
		PurchaseTest(0);
		MakeIndexFile();
		test = 1;
	}

	// Project2 added.

	while (login_id == "") {
		std::cout << "Login ID: ";
		std::cin >> login_id;
		std::cout << "Password : ";
		std::cin >> login_pw;
		if (!SearchingByID(0, "fileOfMember.dat", login_id)) {
			std::cout << "There is no such ID in Member!" << endl;
			login_id = "";
			login_pw = "";
			continue;
		}
		if (!(access_level = SearchingByPW(0, "fileOfMember.dat", login_id, login_pw))) {
			std::cout << "You entered wrong password!" << endl;
			login_id = "";
			login_pw = "";
		}
	}


	//
	while (1) {
		showSystemMenu();
		std::cin >> input;
		std::cout << "*************************************" << endl;
		input_int = atoi(input.c_str());
		switch (input_int) {
		case 1:
			while (1) {
				std::cout << "***********************************" << endl;
				std::cout << "1. Member Information Search   ****" << endl;
				std::cout << "2. Lecture Information Search  ****" << endl;
				std::cout << "3. Purchase Information Search ****" << endl;
				std::cout << "**********************************" << endl;
				std::cin >> filename_index;
				if (atoi(filename_index.c_str()) == 1){
					filename = "fileOfMember.dat";
					break;
				}
				else if (atoi(filename_index.c_str()) == 2) {
					filename = "fileOfLecture.dat";
					break;
				}
				else if (atoi(filename_index.c_str()) == 3) {
					filename = "fileOfPurchase.dat";
					break;
				}
			}
			std::cout << "Finding ID: ";
			std::cin >> want_ID;
		
			if (filename.compare("fileOfMember.dat") == 0) {
				if (login_id.compare("admin") == 0) {}
				else if (want_ID.compare(login_id) != 0) {
					std::cout << "Permission denied!" << endl;
					break;
				}
			}
			else if (filename.compare("fileOfPurchase.dat") == 0) {
				if(login_id.compare("admin") == 0){ }
				else if (want_ID.compare(login_id) != 0) {
					std::cout << "Permission denied!" << endl;
					break;
				}
			}

			SearchingByID(1, filename, want_ID); break;
		case 2: InsertingByID(login_id, access_level); break;
		case 3: DeletingByID(login_id, access_level); break;
		case 4: ModifyingByID(login_id, access_level); break;
		case 5: // logout. 
			login_id = "";
			login_pw = "";
			std::cout << "Log out." << endl;
			return;
		default: return;
		}
	}
}


void showSystemMenu() {
	std::cout << endl;
	std::cout << "*************************************" << endl;
	std::cout << " 1. Search            ***************" << endl;
	std::cout << " 2. Insert            ***************" << endl;
	std::cout << " 3. Delete            ***************" << endl;
	std::cout << " 4. Modify            ***************" << endl;
	std::cout << " Exit                 ***************" << endl;
	std::cout << "*************************************" << endl;
	std::cout << "What do you want? : ";

	return;
}


int SearchingByID(int flag, string filename, string want_ID) {
	Member m;
	Purchase p;
	Lecture l;
	Purchase* p_arr = new Purchase[30];
	int find_ID = 0;
	ifstream ifs(filename);
	int find_Addr = 0;
	int* addr_arr = (int*)malloc(sizeof(int));
	int id_cnt = 0;

	if (ifs.fail()) {
		std::cout << filename << " open error!" << endl;
		return 0;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	if (filename.compare("fileOfMember.dat") == 0) {
		DelimFieldBuffer buffer('|', 1200);
		TextIndexedFile<Member> MemIdxedFile(buffer, 30, 1200);
		MemIdxedFile.Open("fileOfMember");
		char wantID[20];
		
		strcpy(wantID, want_ID.c_str());
		if (MemIdxedFile.Read(wantID, m) != -1)
			find_ID = 1;
		ifs.close();
		buffer.Clear();
	}
	else if (filename.compare("fileOfLecture.dat") == 0) {
		DelimFieldBuffer buffer('|', 1200);
		TextIndexedFile<Lecture> LecIdxedFile(buffer, 30, 1200);
		LecIdxedFile.Open("fileOfLecture");
		char wantID[20];
		strcpy(wantID, want_ID.c_str());
		if (LecIdxedFile.Read(wantID, l) == -1)
			find_ID = 2;
		ifs.close();
		buffer.Clear();
		

	}
	else if (filename.compare("fileOfPurchase.dat") == 0) {
		string want_key_str;
		int want_key_int;
		int find_check;
		char want_key_char = 0;
		id_cnt = 0;

KS:		std::cout << "Finding ID's key (1<= key <= 127) ";
		std::cin >> want_key_str;
		want_key_int = atoi(want_key_str.c_str());
		if (want_key_int < 1 || want_key_int > 127) {
			std::cout << "You have wrong key." << endl;
			goto KS;
		}
		want_key_char = (char)want_key_int;
		world_key = want_key_char;
		find_check = bt.Search(want_key_char);
		if (find_check != -1) {
			Purchase p_temp;
			DelimFieldBuffer buffer('|', 1200);
			RecordFile <Purchase> PurchaseFile(buffer);
			PurchaseFile.Open("fileOfPurchase.dat", ios::in);
			PurchaseFile.Read(p_temp, find_check);
			PurchaseFile.Close();
			p = p_temp;
			find_ID = 3;
		}
	}
	else {
		if( flag)
			std::cout << "There is no such file!" << endl;
		ifs.close();
		return 0;
	}

	if (find_ID) {
		if (flag) {
			std::cout << endl << "Searching Complete" << endl << endl;
			std::cout << "From " << filename << endl;
			switch (find_ID) {
			case 1:	std::cout << m; break;
			case 2: std::cout << l; break;
			case 3: std::cout << p; break;
				break;
			}
			
		}
		return 1;
	}
	else {
		if (flag)
			std::cout << "This id doesn't exist in DB." << endl;
		return 0;
	}
	return 0;
}


void InsertingByID(string log_id, int level) {
	string want_ID;
	string filename;
	// Project 2 added.
	string filename_index;
	int find;
	
	std::cout << "*******************************" << endl;
	std::cout << "1. Member Information Insert.  " << endl;
	std::cout << "2. Lecture Information Insert. " << endl;
	std::cout << "3. Purchase Information Insert." << endl;
	std::cout << "4. Exit this.                  " << endl;
	std::cout << "*******************************" << endl;
	std::cout << "Enter the number: " << endl;
	std::cin >> filename_index;
	switch (atoi(filename_index.c_str())) {
		case 1: filename = "fileOfMember.dat"; break;
		case 2: filename = "fileOfLecture.dat"; break;
		case 3: filename = "fileOfPurchase.dat"; break;
		case 4: return;
	}
	
	ifstream ifs(filename);
	if (ifs.fail()) {
		std::cout << "File Open Error!" << endl;
		return;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');
	
	if (level == 9) {
		if (filename.compare("fileOfMember.dat") == 0 || filename.compare("fileOfLecture.dat")==0) {
			std::cout << "You have no permits" << endl;
			return;
		}
		if (filename.compare("fileOfMember.dat") != 0 && filename.compare("fileOfPurchase.dat") != 0 && filename.compare("fileOfLecture.dat") != 0) {
			std::cout << "Invalid filename." << endl;
			return;
		}
	}
	
	std::cout << "want ID? : " << endl;
	std::cin >> want_ID;


	find = SearchingByID(0, filename, want_ID);
	if (find) {
		std::cout << "**********************" << endl;
		std::cout << "this id already exist." << endl;
		std::cout << "**********************" << endl;
	}
	else {
		if (filename.compare("fileOfMember.dat") == 0) {
			
			Member n;
			string temp;
			//char* update_temp = new char(100);
			char update_temp[100];

			n.setID(want_ID);

			std::cout << "New Password : ";
			std::cin >> temp;
			n.setPassword(temp);

			std::cout << "New Name : ";
			std::cin >> temp;
			n.setName(temp);

			std::cout << "New Phone Number : ";
			std::cin >> temp;
			n.setPhoneNumber(temp);

			std::cout << "New Address : ";
			std::cin >> temp;
			n.setAddress(temp);

			std::cout << "New Mileage : ";
			std::cin >> update_temp;
			if (strlen(update_temp) != 10) {
				std::cout << "Wrong Mileage length!" << endl;
				return;
			}
			n.setMileage(update_temp);
			n.setLevel();

			DelimFieldBuffer buffer('|', 1200);
			TextIndexedFile<Member> MemIdxedFile(buffer, 30, 1200);
			MemIdxedFile.Open("fileOfMember");
			MemIdxedFile.Append(n);
			MemIdxedFile.Close();
			std::cout << endl << "Inserting Complete" << endl << endl;
			mindex += 1;
		}
		else if (filename.compare("fileOfLecture.dat") == 0) {
			
			Lecture n;
			string temp;
			char* update_temp = new char(100);
			int temp_i;

			if (strlen(want_ID.c_str()) != 12) {
				std::cout << "Wrong length" << endl;
				return;
			}
			n.setLectureID(want_ID.c_str());

			std::cout << "New Subject : ";
			std::cin >> update_temp;
			n.setSubject(update_temp);

			std::cout << "New Level : ";
			std::cin >> update_temp;
			if (strlen(update_temp) != 1) {
				std::cout << "Wrong Level length!" << endl;
				return;
			}
			n.setLevel(temp.c_str());

			std::cout << "New Price : ";
			std::cin >> temp_i;
			n.setPrice(temp_i);

			std::cout << "New Extension : ";
			std::cin >> update_temp;
			if (strlen(update_temp) != 1) {
				std::cout << "Wrong Extension length!" << endl;
				return;
			}
			n.setExtension(update_temp);

			std::cout << "New DueDate : ";
			std::cin >> temp_i;
			n.setDueDate(temp_i);

			std::cout << "New NameOfTeacher : ";
			std::cin >> update_temp;
			n.setNameOfTeacher(update_temp);

			std::cout << "New TextBook : ";
			std::cin >> update_temp;
			n.setTextBook(update_temp);

			DelimFieldBuffer buffer('|', 1200);
			TextIndexedFile<Lecture> LecIdxedFile(buffer, 12, 1200);
			LecIdxedFile.Open("fileOfLecture");
			LecIdxedFile.Append(n);
			LecIdxedFile.Close();
			std::cout << endl << "Insert Complete!" << endl;
			lindex += 1;
			
			
		}
		else if (filename.compare("fileOfPurchase.dat") == 0) {
			
			Purchase p;
			char* update_temp = new char(100);
			string temp;

			std::cout << "New Purchase ID : ";
			std::cin >> want_ID;

			if (strlen(want_ID.c_str()) != 16) {
				std::cout << "Wrong PID length" << endl;
				return;
			}
			p.setPurchaseID(want_ID.c_str());

			std::cout << "New Lecture ID : ";
			std::cin >> temp;
			if (strlen(temp.c_str()) != 12) {
				std::cout << "Wrong LID length" << endl;
				return;
			}
			int find_flag = SearchingByID(0, "fileOfLecture.dat", temp);
			if (!find_flag) {
				std::cout << "This Lecture ID doesn't exist" << endl;
				return;
			}
			p.setPurchaseID(temp.c_str());

			std::cout << "New Member ID : ";
			std::cin >> temp;
			if (level == 9) {
				if (log_id.compare(temp) != 0) {
					std::cout << "You have no permits" << endl;
					return;
				}
			}
			find_flag = SearchingByID(0, "fileOfMember.dat", temp);
			if (!find_flag) {
				std::cout << "This Member ID doesn't exist" << endl;
				return;
			}
			p.setMemberID(temp);

			std::cout << "New Mileage : ";
			std::cin >> update_temp;
			if (strlen(update_temp) != 10) {
				std::cout << "Wrong length" << endl;
				return;
			}
			p.setMileage(update_temp);

			if (world_key < 1 || world_key > 127) {
				std::cout << "Exceed key's llmit" << endl;
				return;
			}
			p.setKey(world_key);
			std::cout << "New Key : " << world_key << endl;
			int recaddr;
			DelimFieldBuffer buffer('|', 1000);
			RecordFile<Purchase> PurchaseFile(buffer);
			PurchaseFile.Open("fileOfPurchase.dat", ios::in);
			index = 0;
			while (1) {
				Purchase p_temp;
				if (PurchaseFile.Read(p_temp) == -1)
					break;
				purchaseArr[index] = p_temp;
				index += 1;
			}
			PurchaseFile.Close();
			PurchaseFile.Create("fileOfPurchase.dat", ios::out | ios::trunc);
			
			for (int i = 0; i < index; i++) {
				recaddr = PurchaseFile.Write(purchaseArr[i]);
			}
			recaddr = PurchaseFile.Write(p);
			PurchaseFile.Close();
			bt.Insert(p.Key(), recaddr);
			std::cout << endl << "Inserting Complete" << endl;

		}
		std::cout << endl << "Inserting Complete" << endl << endl;
	}
	ifs.close();
}

void DeletingByID(string log_id, int level) {
	string want_ID;
	string filename;
	string filename_index;
	std::cout << "Enter ID : ";
	std::cin >> want_ID;
	int find;

	std::cout << "************************" << endl;
	std::cout << "1. Member Delete.       " << endl;
	std::cout << "2. Lecture Delete.      " << endl;
	std::cout << "3. Purchase Delete.     " << endl;
	std::cout << "************************" << endl;
	std::cin >> filename_index;
	switch (atoi(filename_index.c_str())) {
	case 1: filename = "fileOfMember.dat"; break;
	case 2: filename = "fileOfLecture.dat"; break;
	case 3: filename = "fileOfPurchase.dat"; break;
	default: return;
	}

	if (filename.compare("fileOfMember.dat") == 0 || filename.compare("fileOfLecture.dat") == 0) {
		std::cout << "want ID? : ";
		std::cin >> want_ID;
	}


	if (level == 9) {
		if (want_ID.compare(log_id)) {
			if (!proveAssociation(log_id, want_ID)) {
				std::cout << "You have no permits" << endl;
				return;
			}
		}
	}
	/*
	find = SearchingByID(0, "fileOfMember.dat", want_ID);
	if (!find) {
		cout << "This id doesn't exist in db." << endl;
		return;
	}

	DeleteMember(want_ID);
	DeleteLecture(want_ID);
	DeletePurchase(want_ID);
	*/
	if (filename.compare("fileOfMember.dat") == 0) {
		find = SearchingByID(0, "fileOfMember.dat", want_ID);
		if (!find) {
			std::cout << "this ID doesn't exist in DB." << endl;
			return;
		}
		DeleteMember(want_ID);
		DeletePurchase(want_ID);
	}
	else if (filename.compare("fileOfLecture.dat") == 0) {
		find = SearchingByID(0, "fileOfLecture.dat", want_ID);
		if (!find) {
			std::cout << "this ID doesn't exist in DB." << endl;
			return;
		}
		DeleteLecture(want_ID);
		DeletePurchase(want_ID);
	}
	else {
		DeleteKey();
	}
}

void DeleteMember(string want_ID) {
	index = 0;
	ifstream ifs("fileOfMember.dat");
	if (ifs.fail()) {
		std::cout << "fileOfMember.dat open error!" << endl;
		return;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	Member m;
	int ifs_last;
	int find_ID = 0;
	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Member> MemberFile(buffer);
	MemberFile.Open("fileOfMember.dat", ios::in);
	ifs.clear();
	ifs.seekg(0, ifs.end);
	ifs_last = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	while (ifs.tellg() < ifs_last) {
		MemberFile.Read(m);
		if (!m.getID().compare(want_ID)) {
			find_ID = 1;
			ifs.seekg(m.getLength(), ifs.cur);
		}
		else {
			memberArr[index] = m;
			index += 1;
		}
		ifs.seekg(m.getLength(), ifs.cur);
	}
	MemberFile.Close();

	if (find_ID) {
		MemberFile.Create("fileOfMember.dat", ios::out | ios::trunc);
		int recaddr;
		for (int i = 0; i < index; i++) {
			if ((recaddr = MemberFile.Write(memberArr[i])) == -1)
				std::cout << "Write Error!" << endl;
		}
		MemberFile.Close();
		// Project 2 added.
		TextIndex MemberIndex(1200);

		MemberFile.Open("fileOfMember.dat", ios::in);
		while (1) {		// 학생 데이터를 읽어서 인덱스를 생성
			Member m;
			int recaddr = MemberFile.Read(m);
			if (recaddr == -1) break;
			MemberIndex.Insert(m.Key(), recaddr);
			//cout << recaddr << '\n' << m;
		}
		MemberFile.Close();

		TextIndexBuffer idxbuf(30, 1200);
		BufferFile MemIdxFile(idxbuf);
		MemIdxFile.Create("fileOfMember.ind", ios::out | ios::trunc);
		MemIdxFile.Rewind(); //헤더 다음의 첫번째 레코드 위치로 이동
							 //Todo...
		idxbuf.Pack(MemberIndex);
		int result = MemIdxFile.Write();
		MemIdxFile.Close();

		std::cout << "Deleting Complete from fileOfMember.dat" << endl;
	}
	ifs.close();
}

void DeleteLecture(string want_ID) {
	index = 0;
	ifstream ifs("fileOfLecture.dat");
	if (ifs.fail()) {
		std::cout << "fileOfLecture.dat open error!" << endl;
		return;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	int ifs_last;
	int find_ID = 0;
	Lecture l;
	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Lecture> LectureFile(buffer);
	LectureFile.Open("fileOfLecture.dat", ios::in);

	ifs.clear();
	ifs.seekg(0, ifs.end);
	ifs_last = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	while (ifs.tellg() < ifs_last) {
		LectureFile.Read(l);
		if (!memcmp(l.getLectureID(), want_ID.c_str(), strlen(l.getLectureID()))) {
			find_ID = 1;
			ifs.seekg(l.getLength(), ifs.cur);
		}
		else {
			lectureArr[index] = l;
			index += 1;
		}
		ifs.seekg(l.getLength(), ifs.cur);
	}
	LectureFile.Close();


	if (find_ID) {
		lindex -= 1;
		LectureFile.Create("fileOfLecture.dat", ios::out | ios::trunc);
		int recaddr;
		for (int i = 0; i < index; i++) {
			if ((recaddr = LectureFile.Write(lectureArr[i])) == -1)
				std::cout << "Write Error!" << endl;
		}
		LectureFile.Close();
		// Project 2 added.

		TextIndex LectureIndex(1200);

		LectureFile.Open("fileOfLecture.dat", ios::in);
		while (1) {
			int recaddr = LectureFile.Read(l);
			if (recaddr == -1) break;
			LectureIndex.Insert(l.Key(), recaddr);
		}
		LectureFile.Close();

		TextIndexBuffer lecidxbuffer(10, 1200);
		BufferFile LecIdxFile(lecidxbuffer);
		LecIdxFile.Create("fileOfLecture.ind", ios::out || ios::trunc);
		LecIdxFile.Rewind();

		lecidxbuffer.Pack(LectureIndex);
		int result = LecIdxFile.Write();
		LecIdxFile.Close();

		std::cout << "Deleting Complete from fileOfLecture.dat" << endl;
	}
	ifs.close();
}

void DeletePurchase(string want_ID) {
	index = 0;
	ifstream ifs("fileOfPurchase.dat");
	if (ifs.fail()) {
		std::cout << "fileOfPurchase.dat open error!" << endl;
		return;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	int ifs_last;
	int find_ID = 0;
	// Project3 added.
	char delete_key[200];
	int delete_key_index = 0;
	//
	Purchase p;
	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Purchase> PurchaseFile(buffer);
	PurchaseFile.Open("fileOfPurchase.dat", ios::in);

	ifs.clear();
	ifs.seekg(0, ifs.end);
	ifs_last = ifs.tellg();
	ifs.seekg(0, ifs.beg);
	while (ifs.tellg() < ifs_last) {
		if (PurchaseFile.Read(p) == -1)
			break;
		if (!strcmp(p.getPurchaseID(), want_ID.c_str())) {
			find_ID = 1;
			ifs.seekg(p.getLength(), ifs.cur);
			delete_key[delete_key_index] = p.Key();
			delete_key_index += 1;
			pindex -= 1;
		}
		else if (!strcmp(p.getLectureID(), want_ID.c_str())) {	// When Lecture is deleted, then related Purchase will be deleted too.
			find_ID = 1;
			ifs.seekg(p.getLength(), ifs.cur);
			delete_key[delete_key_index] = p.Key();
			delete_key_index += 1;
			pindex -= 1;
		}
		else if (!p.getMemberID().compare(want_ID)) {	// When Member is deleted, then related Purchase will be deleted too.
			find_ID = 1;
			ifs.seekg(p.getLength(), ifs.cur);
			delete_key[delete_key_index] = p.Key();
			delete_key_index += 1;
			pindex -= 1;
		}
		else {
			purchaseArr[index] = p;
			index += 1;
		}
		ifs.seekg(p.getLength(), ifs.cur);
	}
	PurchaseFile.Close();


	if (find_ID) {
		PurchaseFile.Create("fileOfPurchase.dat", ios::out | ios::trunc);
		int recaddr;
		for (int i = 0; i < index; i++) {
			recaddr = PurchaseFile.Write(purchaseArr[i]);

			BTreeNode<char> *BTNode = bt.FindLeaf(purchaseArr[i].Key());
			int find_key = true;
			for (int j = 0; j < BTNode->numKeys(); j++) {
				if (BTNode->Keys[j] == purchaseArr[i].Key()) {
					BTNode->RecAddrs[j] = recaddr;
					find_key = false;
					break;
				}
			}
			if (find_key)
				BTNode->RecAddrs[BTNode->numKeys()] = recaddr;
			bt.Store(BTNode);
		}
		for (int i = 0; i < delete_key_index; i++)
			bt.Remove(delete_key[i]);
		PurchaseFile.Close();

		std::cout << "Deleting Complete from fileOfPurchase.dat" << endl;
	}
	ifs.close();
}

void ModifyingByID(string login_id, int level) {
	string want_ID;
	string filename;
	// Project 2 added.
	string filename_index = "";
	int find;

	// Project 2 added.
	
	std::cout << "*******************************" << endl;
	std::cout << "1. Modify Member Information.  " << endl;
	std::cout << "2. Modify Lecture Information. " << endl;
	std::cout << "3. Modify Purchase Information." << endl;
	std::cout << "4. Exit this.                  " << endl;
	std::cout << "*******************************" << endl;
	std::cin >> filename_index;
	switch (atoi(filename_index.c_str())) {
		case 1: filename = "fileOfMember.dat"; break;
		case 2: filename = "fileOfLecture.dat"; break;
		case 3: filename = "fileOfPurchase.dat"; break;
		case 4: return;
	}
	

	std::cout << "Enter ID : ";
	std::cin >> want_ID;
	
	if (level == 9) {
		if (filename.compare("fileOfLecture.dat") == 0) {
			std::cout << "You have no permits!" << endl;
			return;
		}
		else if (filename.compare("fileOfPurchase.dat") == 0) {
			if (want_ID.compare(login_id) != 0) {
				std::cout << "You have no permits!" << endl;
				return;
			}
		}
		else if (filename.compare("fileOfMember.dat") == 0) {
			if (want_ID.compare(login_id) != 0) {
				std::cout << "You have no permits!" << endl;
				return;
			}
		}
	}

	if (filename.compare("fileOfMember.dat") != 0 && filename.compare("fileOfPurchase.dat") != 0 && filename.compare("fileOfLecture.dat") != 0) {
		cout << "Invalid file name!" << endl;
		return;
	}
	// Project 2 checks whether the User is administrator or not.
	//std::cout << "Enter file name : ";
	//std::cin >> filename;
	find = SearchingByID(0, filename, want_ID);
	if (find == 1) {
		if (filename.compare("fileOfMember.dat") == 0) {
			index = 0;
			ifstream ifs("fileOfMember.dat");
			if (ifs.fail()) {
				std::cout << "fileOfMember.dat open error!" << endl;
				return;
			}
			ifs.ignore(numeric_limits<streamsize>::max(), '\n');

		Member m;
		DelimFieldBuffer buffer('|', 1000);
		RecordFile <Member> MemberFile(buffer);
		int ifs_last;
		int find_ID = 0;
		MemberFile.Open("fileOfMember.dat", ios::in);

		ifs.clear();
		ifs.seekg(0, ifs.end);
		ifs_last = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		while (index < mindex/*ifs.tellg() < ifs_last*/) {
			if (MemberFile.Read(m) == -1) {
				std::cout << "Member File Read Fail!" << endl;
				break;
			}
			if (find_ID == 0 && m.getID().compare(want_ID) == 0/*!m.getID().compare(want_ID)*/) {
				find_ID = 1;
				string temp;
				char update_temp[100];

				std::cout << "New Password : ";
				std::cin >> temp;
				m.setPassword(temp);

				std::cout << "New Name : ";
				std::cin >> temp;
				m.setName(temp);

				std::cout << "New Phone Number : ";
				std::cin >> temp;
				m.setPhoneNumber(temp);

				std::cout << "New Address : ";
				std::cin >> temp;
				m.setAddress(temp);

				std::cout << "New Mileage : ";
				std::cin >> update_temp;
				if (strlen(update_temp) != 10) {
					std::cout << "Mileage is Wrong length!" << endl;
					return;
				}
				m.setMileage(update_temp);

				//ifs.seekg(m.getLength(), ifs.cur);
				std::cout << endl << endl;;
				std::cout << "update success!!!" << endl;
				std::cout << endl;
				std::cout << m;
			}
			memberArr[index] = m;
			index += 1;
			ifs.seekg(m.getLength(), ifs.cur);
		}

		ifs.close();
		MemberFile.Close();

		if (find_ID) {
			MemberFile.Create("fileOfMember.dat", ios::out | ios::trunc);
			int recaddr;
			for (int i = 0; i < index; i++) {
				if ((recaddr = MemberFile.Write(memberArr[i])) == -1)
					std::cout << "Write Error!" << endl;
			}
			MemberFile.Close();

			TextIndex MemberIndex(1200);
			MemberFile.Open("fileOfMember.dat", ios::in);
			while (1) {
				Member m;
				int recaddr = MemberFile.Read(m);
				if (recaddr == -1) break;
				MemberIndex.Insert(m.Key(), recaddr);
			}
			MemberFile.Close();

			TextIndexBuffer IdxBuffer2(25, 1000);
			BufferFile MemIdxFile(IdxBuffer2);

			MemIdxFile.Create("fileOfMember.ind", ios::out || ios::trunc);
			MemIdxFile.Rewind();
			IdxBuffer2.Pack(MemberIndex);
			MemIdxFile.Write();
			MemIdxFile.Close();
		}
		std::cout << endl << "Modifying Complete!" << endl;
			//ifs.close();
	}
	else if (filename.compare("fileOfLecture.dat") == 0) {
		index = 0;
		ifstream ifs("fileOfLecture.dat");
		if (ifs.fail()) {
			std::cout << "fileOfLecture.dat open error!" << endl;
			return;
		}
		ifs.ignore(numeric_limits<streamsize>::max(), '\n');

		Lecture l;
		int ifs_last;
		int find_ID = 0;
		DelimFieldBuffer buffer('|', 1000);
		RecordFile <Lecture> LectureFile(buffer);
		LectureFile.Open("fileOfLecture.dat", ios::in);

		ifs.clear();
		ifs.seekg(0, ifs.end);
		ifs_last = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		while (index < lindex/*ifs.tellg() < ifs_last*/) {
			if (LectureFile.Read(l) == -1) {
				std::cout << "Lecture File Read fail!" << endl;
				break;
			}
			if (!memcmp(l.getLectureID(), want_ID.c_str(), strlen(l.getLectureID()))) {
				find_ID = 1;
				string temp;
				char update_temp[100];
				int temp_i;
				std::cout << "New Subject : ";
				std::cin >> temp;
				l.setSubject(temp);

				std::cout << "New Level : ";
				std::cin >> update_temp;
				if (strlen(update_temp) != 1) {
					std::cout << "Level Wrong length!" << endl;
					return;
				}
				l.setLevel(update_temp);

				std::cout << "New Price : ";
				std::cin >> temp_i;
				l.setPrice(temp_i);

				std::cout << "New Extension : ";
				std::cin >> update_temp;
				if (strlen(update_temp) != 1) {
					std::cout << "Extension Wrong length!" << endl;
					return;
				}
				l.setExtension(update_temp);

				std::cout << "New Due Date : ";
				std::cin >> temp_i;
				l.setDueDate(temp_i);

				std::cout << "New NameOfTeacher : ";
				std::cin >> temp;
				l.setNameOfTeacher(temp);

				std::cout << "New TextBook : ";
				std::cin >> temp;
				l.setTextBook(temp);

				ifs.seekg(l.getLength(), ifs.cur);

				std::cout << endl << endl;;
				std::cout << "update success!!!" << endl;
				std::cout << endl;
				std::cout << l;
			}
			lectureArr[index] = l;
			index += 1;
			ifs.seekg(l.getLength(), ifs.cur);
		}
		LectureFile.Close();


		if (find_ID) {
			LectureFile.Create("fileOfLecture.dat", ios::out | ios::trunc);
			int recaddr;
			for (int i = 0; i < index; i++) {
				if ((recaddr = LectureFile.Write(lectureArr[i])) == -1)
					std::cout << "Write Error!" << endl;
			}
			LectureFile.Close();

			TextIndex LectureIndex(1200);

			LectureFile.Open("fileOfLecture.dat", ios::in);
			while (1) {
				Lecture l;
				int recaddr = LectureFile.Read(l);
				if (recaddr == -1) break;
				LectureIndex.Insert(l.Key(), recaddr);
			}
			LectureFile.Close();

			TextIndexBuffer lecidxbuffer(10, 1200);
			BufferFile LecIdxFile(lecidxbuffer);
			LecIdxFile.Create("fileOfLecture.ind", ios::out | ios::trunc);
			LecIdxFile.Rewind();

			lecidxbuffer.Pack(LectureIndex);
			int result = LecIdxFile.Write();
			LecIdxFile.Close();
		}
		ifs.close();
		std::cout << endl << "Modifying Complete!" << endl;
	}
	else if (filename.compare("fileOfPurchase.dat") == 0) {
		int recaddr;
		index = 0;
		
		Purchase p;
		int find_ID = 0;
		DelimFieldBuffer buffer('|', 10000);
		RecordFile <Purchase> PurchaseFile(buffer);
		PurchaseFile.Open("fileOfPurchase.dat", ios::in);

		
		while (1) {
			if (PurchaseFile.Read(p) == -1) {
				std::cout << "Purchae File Read Fail!" << endl;
				break;
			}
			if (p.Key() == world_key) {
				char update_temp[100];
				std::cout << "New Mileage : ";
				std::cin >> update_temp;
				if (strlen(update_temp) != 10) {
					std::cout << "Mileage Wrong length!" << endl;
					return;
				}
				p.setMileage(update_temp);

				
				std::cout << endl << endl;;
				std::cout << "update success!!!" << endl;
				std::cout << endl;
				std::cout << p;
			}
			purchaseArr[index] = p;
			index += 1;
			
		}

		PurchaseFile.Close();
		int modify_address = 0;
		PurchaseFile.Create("fileOfPurchase.dat", ios::out | ios::trunc);
		for (int i = 0; i < index; i++) {
			recaddr = PurchaseFile.Write(purchaseArr[i]);
			if (world_key == purchaseArr[i].Key())
				modify_address = recaddr;

			BTreeNode<char> *BTNode = bt.FindLeaf(purchaseArr[i].Key());
			bool find_key = true;
			for (int j = 0; j < BTNode->numKeys(); j++) {
				if (BTNode->Keys[j] == purchaseArr[i].Key()) {
					BTNode->RecAddrs[j] = recaddr;
					find_key = false;
					break;
				}
			}
			if (find_key)
				BTNode->RecAddrs[BTNode->numKeys()] = recaddr;
			bt.Store(BTNode);
		}
		bt.Remove(world_key);
		bt.Insert(world_key, modify_address);
		PurchaseFile.Close();
		
		std::cout << endl << "Modifying Complete" << endl << endl;
	}
	
	}
	else
		std::cout << "This id doesn't exist." << endl;
}


// Project2 added.

int SearchingByPW(int flag, string filename, string wantID, string wantPW) {
	Member m;
	ifstream ifs(filename);
	int findPW = 0;
	int ifs_last;

	if (ifs.fail() && flag) {
		std::cout << filename << " open error!" << endl;
			return 0;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	if (filename.compare("fileOfMember.dat") == 0) {
		DelimFieldBuffer buffer('|', 1200);
		RecordFile <Member> MemberFile(buffer);
		MemberFile.Open("fileOfMember.dat", ios::in);
		ifs.clear();
		ifs.seekg(0, ifs.end);
		ifs_last = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		while (ifs.tellg() < ifs_last) {
			MemberFile.Read(m);
			ifs.seekg(m.getLength(), ifs.cur);
			if (!m.getID().compare(wantID)) {
				if (!m.getPassword().compare(wantPW)) {
					if (strcmp(m.getLevel(), "9") == 0)
						findPW = 9;
					else
						findPW = 1;
					break;
				}
				else {
					findPW = 0;
					break;
				}
			}
		}
		MemberFile.Close();
		ifs.close();
	}
	return findPW;
}

void MakeIndexFile(void) {
	DelimFieldBuffer buffer('|', 1200);
	RecordFile <Member> MemberFile(buffer);
	TextIndex MemberIndex(1200);

	MemberFile.Open("fileOfMember.dat", ios::in);
	while (1) {
		Member m;
		int recaddr = MemberFile.Read(m);
		if (recaddr == -1) break;
		MemberIndex.Insert(m.Key(), recaddr);

	}
	MemberFile.Close();

	TextIndexBuffer idxbuf(30, 1200);
	BufferFile MemIdxFile(idxbuf);
	MemIdxFile.Create("fileOfMember.ind", ios::out | ios::trunc);
	MemIdxFile.Rewind();

	idxbuf.Pack(MemberIndex);
	int result = MemIdxFile.Write();
	MemIdxFile.Close();

	DelimFieldBuffer buffer1('|', 1200);
	RecordFile <Lecture> LectureFile(buffer1);
	TextIndex LectureIndex(1200);

	LectureFile.Open("fileOfLecture.dat", ios::in);
	while (1) {
		Lecture l;
		int recaddr = LectureFile.Read(l);
		if (recaddr == -1) break;
		LectureIndex.Insert(l.Key(), recaddr);
	}
	LectureFile.Close();

	TextIndexBuffer lecidbuffer(30, 1200);
	BufferFile LecIdxFile(lecidbuffer);
	LecIdxFile.Create("fileOfLecture.ind", ios::out | ios::trunc);
	LecIdxFile.Rewind();

	lecidbuffer.Pack(LectureIndex);
	result = LecIdxFile.Write();
	LecIdxFile.Close();

	// Project3 added.
	result = 0;
	int i;
	DelimFieldBuffer buffer2('|', 1200);
	RecordFile <Purchase> PurchaseFile(buffer2);
	result = bt.Create("fileOfPurchase.ind", ios::in | ios::out | ios::trunc);
	if (!result) {
		std::cout << "Please delete fileOfPurchase.ind" << endl;
		return;
	}
	PurchaseFile.Open("fileOfPurchase.dat", ios::in);
	char key = 1;
	while (1) {
		Purchase p;
		int recaddr = PurchaseFile.Read(p);
		if (recaddr == -1) break;
		p.setKey(key);
		bt.Insert(p.Key(), recaddr);
		key += 1;
	}
	PurchaseFile.Close();
}

int proveAssociation(string loginID, string wantID) {
	int ifs_last;
	int findID = 0;
	Purchase p;
	ifstream ifs("fileOfPurchase.dat");

	if (ifs.fail()) {
		std::cout << "fileOfPurchase.dat" << " open error!" << endl;
		return 0;
	}
	ifs.ignore(numeric_limits<streamsize>::max(), '\n');

	DelimFieldBuffer buffer('|', 1000);
	RecordFile <Purchase> PurchaseFile(buffer);
	PurchaseFile.Open("fileOfPurchase.dat", ios::in);
	ifs.clear();
	ifs.seekg(0, ifs.end);
	ifs_last = ifs.tellg();
	ifs.seekg(0, ifs.beg);

	while (ifs.tellg() < ifs_last) {
		PurchaseFile.Read(p);
		ifs.seekg(p.getLength(), ifs.cur);
		if(loginID.compare(p.getMemberID()) == 0 ){
			if (!strcmp(p.getPurchaseID(), wantID.c_str())) {
				findID = 1;
				break;
			}
			else if (!strcmp(p.getLectureID(), wantID.c_str())) {
				findID = 1;
				break;
			}
			else if (!p.getMemberID().compare(wantID)) {
				findID = 1;
				break;
			}

		}
	}
	PurchaseFile.Close();
	ifs.close();

	return findID;
}

// Project3 added.
void DeleteKey(void) {
	string temp;

	std::cout << "Want key: ";
	std::cin >> temp;
	int temp_int = atoi(temp.c_str());
	char temp_char = (char)temp_int;
	if (bt.Search(temp_char) == -1) {
		std::cout << "this key doesn't exist in DB." << endl;
		return;
	}
	else {
		int recaddr;
		DelimFieldBuffer buffer('|', 1000);
		RecordFile<Purchase> PurchaseFile(buffer);
		PurchaseFile.Open("fileOfPurchase.dat", ios::in);
		index = 0;
		while (1) {
			Purchase p;
			if (PurchaseFile.Read(p) == -1)
				break;
			if (p.Key() == temp_char) {
				std::cout << p;
				continue;
			}
			purchaseArr[index] = p;
			index += 1;
		}
		PurchaseFile.Close();
		PurchaseFile.Create("fileOfPurchase.dat", ios::out | ios::trunc);
		for (int i = 0; i < index; i++) {
			recaddr = PurchaseFile.Write(purchaseArr[i]);
			BTreeNode<char> * BTNode = bt.FindLeaf(purchaseArr[i].Key());
			int find_key = true;
			for (int j = 0; j < BTNode->numKeys(); j++) {
				if (BTNode->Keys[j] == purchaseArr[i].Key()) {
					BTNode->RecAddrs[j] = recaddr;
					find_key = false;
					break;
				}
			}
			if (find_key)
				BTNode->RecAddrs[BTNode->numKeys()] = recaddr;
			bt.Store(BTNode);
		}
		bt.Remove(temp_char);
		PurchaseFile.Close();
		std::cout << "Deleting Complete from fileOfPurchase.dat" << endl;
	}
}









