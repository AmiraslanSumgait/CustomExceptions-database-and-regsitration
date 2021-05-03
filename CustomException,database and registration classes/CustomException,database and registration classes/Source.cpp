#include <iostream>
#include <string>
#include <time.h>

using namespace std;
class Exception {
protected:
	string text;
	int  line;
	string source;
	string exceptionTime;
	Exception(const string& text, int line
		, const string& source, const string& Time)
	{
		SetTime(Time);
		SetText(text);
		SetSource(source);
		SetLine(line);

	}
	string& GetText() {
		return text;
	}
	int& GetLine() {
		return line;
	}
	string GetSource() {
		return source;
	}
	string GetTime() {
		return exceptionTime;
	}
	void SetText(const string& text) {
		this->text = text;
	}
	void SetLine(const int& line) {
		this->line = line;
	}
	void SetSource(const string& source) {
		this->source = source;
	}
	void SetTime(const string& Time) {
		time_t currently = time(0);
		char dt[30]{};
		ctime_s(dt, sizeof dt, &currently);
		string time = dt;
		this->exceptionTime = dt;
	}
public:
	void PrintMsg()const {
		cout << "------ Exception Info ------" << endl;
		cout << "Text:" << text << endl;
		cout << "Line:" << line << endl;
		cout << "Source:" << source << endl;
		cout << "Time:" << exceptionTime << endl;
	}
};
class DatabaseException :public Exception {
public:
	DatabaseException(const string& text, const int& line
		, const string& source, const string& Time)
		:Exception(text, line, source, Time)
	{

	}
};
class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException(const string& text, const int& line
		, const string& source, const string& Time)
		:Exception(text, line, source, Time)
	{

	}
};

class User {
	int id;
	string username;
	string password;
	string name;
	string surname;

public:
	User() :id(), username(), password(), name(), surname()
	{

	}
	User(const int& id, const string& username, const string& password, const string& name, const string surname)
	{
		SetId(id);
		SetUsername(username);
		SetPassword(password);
		SetName(name);
		SetSurname(surname);
	}
	string& GetUsername() {
		return username;
	}
	string& GetPassword() {
		return password;
	}
	string& GetName() {
		return name;
	}
	string& GetSurname() {
		return surname;
	}
	int& GetId() {
		return id;
	}
	void SetId(int id) {
		this->id = id;
	}
	void SetUsername(const string& username) {
		if (username == "") {
			throw InvalidArgumentException("Username is empty", __LINE__, "Source.cpp", __TIMESTAMP__);
		}
		if (username.length() < 6) throw InvalidArgumentException("Charaters size less than 6", __LINE__, "Source.cpp", __TIMESTAMP__);
		if (username[0] >= 97 && username[0] <= 122) throw InvalidArgumentException("First character must be capital letter", __LINE__, "Source.cpp", __TIMESTAMP__);
		this->username = username;
	}
	void SetPassword(const string& password) {
		if (password == "") {
			throw InvalidArgumentException("Password is empty", __LINE__, "Source.cpp", __TIMESTAMP__);
		}
		if (password.length() < 6)  throw InvalidArgumentException("Paswword size less than 6", __LINE__, "Source.cpp", __TIMESTAMP__);
		this->password = password;
	}
	void SetName(const string& name) {
		if (name == "") {
			throw InvalidArgumentException("Name is empty", __LINE__, "Source.cpp", __TIMESTAMP__);
		}
		if (name.length() < 4) throw InvalidArgumentException("Name size less than 4", __LINE__, "Source.cpp", __TIMESTAMP__);
		this->name = name;
	}
	void SetSurname(const string& surname) {
		if (surname == "") {
			throw InvalidArgumentException("Surname is empty", __LINE__, "Source.cpp", __TIMESTAMP__);
		}
		if (surname.length() < 4) throw InvalidArgumentException("Surname size less than 4", __LINE__, "Source.cpp", __TIMESTAMP__);
		this->surname = surname;

	}
	void show() const {
		cout << "ID:" << id << endl;
		cout << "Username:" << username << endl;
		cout << "Password:" << password << endl;
		cout << "Name:" << name << endl;
		cout << "Surname:" << surname << endl;
	};
};

class Database {
public:
	User** users = NULL;
	int user_count = 0;

	void addUser(User& user) {

		auto newUsers = new User * [user_count + 1];
		for (size_t i = 0; i < user_count; i++)
		{
			newUsers[i] = users[i];


		}
		newUsers[user_count] = &user;
		delete[]users;
		users = newUsers;
		newUsers = nullptr;
		user_count++;


	};
	void showALlUsers() {
		for (size_t i = 0; i < user_count; i++)
		{
			users[i]->show();
			cout << endl;
		}
	}
	User& getUserByUsername(string username) {
		for (size_t i = 0; i < user_count; i++)
		{
			if (username == users[i]->GetUsername()) {
				return*(users[i]);
			}
		}
		throw InvalidArgumentException("This user not found", __LINE__, "Source.cpp", __TIMESTAMP__);

	};
	void updateUser(User& olduser, User& newuser) {
		for (size_t i = 0; i < user_count; i++)
		{
			if (olduser.GetId() != users[i]->GetId()) throw InvalidArgumentException("This user is not found", __LINE__, "Source.cpp", __TIMESTAMP__);
		}
		olduser.GetUsername() = newuser.GetUsername();
		olduser.GetPassword() = newuser.GetPassword();
		olduser.GetName() = newuser.GetName();
		olduser.GetSurname() = newuser.GetSurname();
	};
	size_t HashCode(const string& str) {
		int h = 0;
		for (size_t i = 0; i < str.size(); ++i)
			h = h * 31 + static_cast<int>(str[i]);
		return h;
	}
	void hashUserDataById(const int id) {
		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->GetId() == id) {
				int hasvalueName = HashCode(users[i]->GetName());
				int hasvalueSurname = HashCode(users[i]->GetSurname());
				int hasvaluePassword = HashCode(users[i]->GetPassword());
				int hashvalueUsername = HashCode(users[i]->GetUsername());
				users[i]->GetName() = to_string(hasvalueName);
				users[i]->GetSurname() = to_string(hasvalueSurname);
				users[i]->GetPassword() = to_string(hasvaluePassword);
				users[i]->GetUsername() = to_string(hashvalueUsername);
			}
		}
	}
	void deleteUserbyId(const int id) {
		int index;
		for (size_t i = 0; i < user_count; i++)
		{
			if (users[i]->GetId() == id) {
				index = i;
				break;
			}
		}
		auto newUsers = new User * [user_count - 1]{};
		for (size_t i = 0, k = 0; i < user_count; i++)
		{
			if (i != index) {
				newUsers[k] = users[i];
				k++;
			}
		}
		user_count--;
		delete[]users;
		users = newUsers;
		newUsers = nullptr;

	}
};

class Registration {
	Database _database;
public:
	Registration(const Database& database) {
		_database = database;
	}
	Database& GetDataBase() {
		return _database;
	}
	void signIn(string username, string password) {
		for (size_t i = 0; i < _database.user_count; i++)
		{
			if (username == _database.users[i]->GetUsername()) {
				if (_database.users[i]->GetPassword() == password) {
					cout << "*******You sucessfully registered********" << endl;
					break;
				}
				else throw DatabaseException("This password incorrect", __LINE__, "Source.cpp", __TIMESTAMP__);
			}
			else throw DatabaseException("This  username incorrect", __LINE__, "Source.cpp", __TIMESTAMP__);




		}
	};
	void signUp(int id, string username, string password, string name, string surname) {
		for (size_t i = 0; i < _database.user_count; i++)
		{
			if (username == _database.users[i]->GetUsername()) throw DatabaseException("This username already exist in database", __LINE__, "Source.cpp", __TIMESTAMP__);

		}
		if (username.length() < 6) throw InvalidArgumentException("Charaters size less than 6", __LINE__, "Source.cpp", __TIMESTAMP__);
		if (username[0] >= 97 && username[0] <= 122) throw InvalidArgumentException("First character must be capital letter", __LINE__, "Source.cpp", __TIMESTAMP__);
		if (password.length() < 6)  throw InvalidArgumentException("Paswword size less than 6", __LINE__, "Source.cpp", __TIMESTAMP__);
		if (name.length() < 4) throw InvalidArgumentException("Name size less than 4", __LINE__, "Source.cpp", __TIMESTAMP__);
		if (surname.length() < 4) throw InvalidArgumentException("Surname size less than 4", __LINE__, "Source.cpp", __TIMESTAMP__);
		User* newUser = new User;
		newUser->SetId(id);
		newUser->SetUsername(username);
		newUser->SetPassword(password);
		newUser->SetName(name);
		newUser->SetSurname(surname);

		GetDataBase().addUser(*newUser);
	};
};
class System {
public:
	static void Control() {
		Database db;
		Registration twitter(db);

		try
		{
			User user1(1, "Amiraslan", "123456", "Emiraslan", "Eliyev");
			User user2(2, "Rasimaqa", "karamela", "Rasimaqe", "Agaliyev");
			User user3(3, "Nabi123", "6565685", "Nebi", "Nebili");

			twitter.GetDataBase().addUser(user1);
			twitter.GetDataBase().addUser(user2);
			twitter.GetDataBase().addUser(user3);
			twitter.signUp(4, "KamalFame", "bskamal7", "Kamal", "Eliyev");
			twitter.signIn("Amiraslana", "123456");
			//twitter.GetDataBase().updateUser(user3, user2);
			//twitter.GetDataBase().hashUserDataById(2);
			twitter.GetDataBase().deleteUserbyId(2);
			twitter.GetDataBase().showALlUsers();
		}
		catch (const Exception& ex)
		{
			cout << "Exception has occured!!!" << endl;
			ex.PrintMsg();
		}

	}

};

void main() {
	System::Control();
}