#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <conio.h>
#include "io.h"

using namespace std;

const string USERS_FILENAME = "users.dat";
const string DATA_FILENAME = "data.dat";
// I'm lazy, so can you please replace it with input urself? I've already wrote ~650 lines of that
// fucking boilerplate code
//
//
// I hate C++
const int BASE_SCHOLARSHIP = 1000;

struct Pupil
{
    string name;
    int group;
    bool isFree, isActive;
    bool isOkLogic, isOkBelorussian, isOkHistory, isOkEngineering, isOkPolitics;
    int math, physics, cs, philosophy;
};

struct User
{
    string login;
    string password;
    int accessLevel;
    User(string login, string password, int accessLevel)
    {
        this->login = login;
        this->password = password;
        this->accessLevel = accessLevel;
    }
};

// Convert access level to string for gut UX
string accessLeveltoStr(int level)
{
    switch(level)
    {
    case 0:
        return "User";
    case 1:
        return "Admin";
    default:
        return "???";
    }
}

// Calculate average grade
double calculateAvegareGrade(Pupil &pupil){
    return (pupil.cs + pupil.math + pupil.philosophy + pupil.physics) / 4;
}
// This function calculates scholarship based on user info
int calculateScholarship(Pupil &pupil){
    double average = calculateAvegareGrade(pupil);
    if(average >= 9 && pupil.isActive) {
        return BASE_SCHOLARSHIP * 0.5;
    } else if (average >= 9){
        return BASE_SCHOLARSHIP * 0.25;
    } else if (average > 5){
        return BASE_SCHOLARSHIP;
    } else {
        return 0;
    }
}

// AverageGrade comparator
bool sortByAverageGrade(Pupil a, Pupil b){
    return calculateAvegareGrade(a) > calculateAvegareGrade(b);
}

// Scholarship comparator
bool sortByScholarship(Pupil a, Pupil b){
    return calculateScholarship(a) > calculateScholarship(b);
}

// Name comparator
bool sortByName(Pupil a, Pupil b){
    int compareResult = a.name.compare(b.name);
    if(compareResult == 0 || compareResult == 1)
        return false;
    return true;
}

// Check if login is unique
bool isLoginUnique(vector<User> &users, User &user)
{
    for(int i = 0; i < users.size(); i++)
    {
        if(users[i].login == user.login)
            return false;
    }
    return true;
}

// Convert number to true or false
int boolToNumber(bool boolean)
{
    return boolean ? 1 : 0;
}

// Convert bool to true or false
string boolToStr(bool boolean)
{
    return boolean ? "True" : "False";
}

// WHY THE FUCK IT ISN'T BUILT IN?
// C++ SUCKS AGAIN
string toLowerCase(string str)
{
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Print in CL info about user
void printUser(User &user)
{
    cout << user.login << " | access level: " << accessLeveltoStr(user.accessLevel) << endl;
}

// Print in CL info about pupil
void printPupil(Pupil &pupil)
{
    cout << pupil.name << endl
    << "\t Group: " << pupil.group << endl
    << "\t Studies for free: " << boolToStr(pupil.isFree) << endl
    << "\t Participates at public activities: " << boolToStr(pupil.isActive) << endl
    << "\t Passed politics test: " << boolToStr(pupil.isOkPolitics) << endl
    << "\t Passed belorussian test: " << boolToStr(pupil.isOkBelorussian) << endl
    << "\t Passed engineering test: " << boolToStr(pupil.isOkEngineering) << endl
    << "\t Passed history test: " << boolToStr(pupil.isOkHistory) << endl
    << "\t Passed logic test: " << boolToStr(pupil.isOkLogic) << endl
    << "\t Computer Science grade: " << pupil.cs << endl
    << "\t Physics grade: " << pupil.physics << endl
    << "\t Philosophy grade: " << pupil.philosophy << endl
    << "\t Math grade: " << pupil.math << endl
    << "\t Average grade: " << calculateAvegareGrade(pupil) << endl
    << "\t Scholarship: " << calculateScholarship(pupil) << endl;
}

// Read users from file
vector<User> readUsers()
{
    ifstream file(USERS_FILENAME.c_str());
    string login, password;
    vector<User> users;
    int access_level;
    while(file >> login >> password >> access_level)
    {
        users.push_back(User(login, password, access_level));
    }
    return users;
}

// Print users. this time from file coz we aren't filtering 'em
void printUsers()
{
    vector<User> users = readUsers();
    for(int i = 0; i < users.size(); i++)
    {
        printUser(users[i]);
        cout << endl;
    }
}

// Read pupils from file
// Remark: first number in file is amount of pupils in file
// Remark: have to getline every time when I reach end of line
// P.S. C++ sucks
vector<Pupil> readPupils()
{
    ifstream file(DATA_FILENAME.c_str());
    vector<Pupil> pupils;
    int pupilCount;
    file >> pupilCount;
    string stupid_cpp;
    getline(file, stupid_cpp);
    for(int i = 0; i < pupilCount; i++)
    {
        Pupil pupil;
        getline(file, pupil.name);
        file >> pupil.group;
        pupil.isActive = readBooleanFile(file);
        pupil.isFree = readBooleanFile(file);
        pupil.isOkBelorussian = readBooleanFile(file);
        pupil.isOkEngineering = readBooleanFile(file);
        pupil.isOkHistory = readBooleanFile(file);
        pupil.isOkLogic = readBooleanFile(file);
        pupil.isOkPolitics = readBooleanFile(file);
        file >> pupil.math >> pupil.philosophy >> pupil.physics >> pupil.cs;
        getline(file, stupid_cpp);
        pupils.push_back(pupil);
    }
    file.close();
    return pupils;
}

// Show all pupils in array
void printPupils(vector<Pupil> pupils)
{
    for(int i = 0; i < pupils.size(); i++)
    {
        printPupil(pupils[i]);
    }
}


// Saving pupils to file
// Remark: first number in file is amount of pupils in file
void savePupils(vector<Pupil> pupils)
{
    ofstream file(DATA_FILENAME.c_str());
    file << pupils.size() << endl;
    for(int i = 0; i < pupils.size(); i++)
    {
        file << pupils[i].name << endl
             << pupils[i].group << " " << boolToNumber(pupils[i].isActive) << " "
             << boolToNumber(pupils[i].isFree) << " " << boolToNumber(pupils[i].isOkBelorussian) << " "
             << boolToNumber(pupils[i].isOkEngineering)<< " "  << boolToNumber(pupils[i].isOkHistory) << " "
             << boolToNumber(pupils[i].isOkLogic)<< " "  << boolToNumber(pupils[i].isOkPolitics) << " "
             << pupils[i].math << " "  << pupils[i].philosophy << " "  << pupils[i].physics << " "
             << pupils[i].cs << endl;
    }
    file.close();
}

// Entering process of user
User enterUser()
{
    string login = readString("Enter login: ");
    login = toLowerCase(login);
    string password = readString("Enter password: ");
    int accessLevel = readNumber("Enter access level (0 - User, 1 - Admin): ");
    return User(
               login,
               password,
               accessLevel
           );
}

// Entering process of pupil
Pupil enterPupil()
{
    Pupil pupil;
    pupil.name = readString("Enter pupil's name: ");
    pupil.group = readNumber("Enter pupil's group: ");
    pupil.isActive = readBoolean("Does pupil participate in public activities(0 - false, 1 - true) : ");
    pupil.isFree = readBoolean("Does pupil study for state's budget(0 - false, 1 - true) : ");
    pupil.isOkBelorussian = readBoolean("Did pupil pass belorussian test(0 - false, 1 - true) : ");
    pupil.isOkEngineering = readBoolean("Did pupil pass engineering test(0 - false, 1 - true) : ");
    pupil.isOkHistory = readBoolean("Did pupil pass history test(0 - false, 1 - true) : ");
    pupil.isOkLogic = readBoolean("Did pupil pass logic test(0 - false, 1 - true) : ");
    pupil.isOkPolitics = readBoolean("Did pupil pass politics test(0 - false, 1 - true) : ");
    pupil.cs = readNumber("Pupil's computer science grade: ");
    pupil.math = readNumber("Pupil's math grade: ");
    pupil.philosophy = readNumber("Pupil's philosophy grade: ");
    pupil.physics = readNumber("Pupil's physics grade: ");
    return pupil;
}

// Save users to file
void saveUsers(vector<User> users)
{
    ofstream file(USERS_FILENAME.c_str());
    for(int i = 0; i < users.size(); i++)
    {
        file << users[i].login << " " << users[i].password << " "
             << users[i].accessLevel << endl;
    }
}

// Login in system
int login()
{
    vector<User> users = readUsers();
    int authorizationLevel = -1;
    while(authorizationLevel == -1)
    {
        string login = readString("Enter login: ");
        login = toLowerCase(login);
        string password = readString("Enter password: ");
        for(int i = 0; i < users.size(); i++)
        {
            if(users[i].login == login && users[i].password == password)
            {
                return users[i].accessLevel;
            }
        }
        cout << "Authorization error. Try again" << endl;
    }
    return authorizationLevel;
}

// Delete user by index
void deleteUser()
{
    vector<User> users = readUsers();
    for(int i = 0; i < users.size(); i++)
    {
        cout << i+1 << ". ";
        printUser(users[i]);
    }
    while(1)
    {
        int option =  readNumber("Enter user's number(-1 to exit): ");
        if(option == -1)
        {
            return;
        }
        else if(option <= users.size() && option > 0)
        {
            users.erase(users.begin() + option - 1);
            saveUsers(users);
            return;
        }
    }
}

// Delete pupil by index
void deletePupil()
{
    vector<Pupil> pupils = readPupils();
    for(int i = 0; i < pupils.size(); i++)
    {
        cout << i+1 << ". ";
        cout << pupils[i].name << " from " << pupils[i].group << endl;
    }
    while(1)
    {
        int option = readNumber("Enter pupil's number(-1 to exit): ");
        if(option == -1)
        {
            return;
        }
        else if(option <= pupils.size() && option > 0)
        {
            pupils.erase(pupils.begin() + option - 1);
            savePupils(pupils);
            return;
        }
    }
}

// Edit pupil entirely, coz. FUCK THIS BOILERPLATE CODE BULLSHIT
void editPupil()
{
    vector<Pupil> pupils = readPupils();
    for(int i = 0; i < pupils.size(); i++)
    {
        cout << i+1 << ". ";
        cout << pupils[i].name << " from " << pupils[i].group << endl;
    }
    while(1)
    {
        int option =  readNumber("Enter user's number(-1 to exit): ");
        if(option == -1)
        {
            return;
        }
        else if(option <= pupils.size() && option > 0)
        {
            pupils[option-1] = enterPupil();
            _getch();
            return;
        }
    }
}

// Edit user by some param
void editUser()
{
    vector<User> users = readUsers();
    for(int i = 0; i < users.size(); i++)
    {
        cout << i+1 << ". ";
        printUser(users[i]);
    }
    while(1)
    {
        int option =  readNumber("Enter user's number(-1 to exit): ");
        if(option == -1)
        {
            return;
        }
        else if(option <= users.size() && option > 0)
        {
            cout << "What are you going to edit?" << endl;
            cout << "1. Password" << endl;
            cout << "2. Access level" << endl;
            // TODO: Add idiot check
            int editOption =  readNumber("Enter menu's number: ");
            switch(editOption)
            {
            case 1:
            {
                string password = readString("Enter new password: ");
                users[option-1].password = password;
                break;
            }

            case 2:
            {
                int accessLevel = readNumber("Enter new access level: ");
                users[option-1].accessLevel = accessLevel;
                break;
            }
            }

            saveUsers(users);
            return;
            break;
        }
    }
}

// Create new user
void createNewUser()
{
    vector<User> users = readUsers();
    while(1)
    {
        User user = enterUser();
        if(isLoginUnique(users, user))
        {
            users.push_back(user);
            break;
        }
        cout << "This user already exists. Try again." << endl;
    }
    saveUsers(users);
}

// Create new pupil(aka. student)
void createNewPupil()
{
    vector<Pupil> pupils = readPupils();
    Pupil pupil = enterPupil();
    pupils.push_back(pupil);

    savePupils(pupils);
}

// Delete file with data
void deleteDataFile()
{
    remove(DATA_FILENAME.c_str());
}

// Sort by something using algorithm library with custom comparators
void sortPupils(){
    vector<Pupil> pupils = readPupils();
    while(1){
        cout << "1. Sort by name" << endl;
        cout << "2. Sort by average grade" << endl;
        cout << "3. Sort by scholarship" << endl;
        int type = readNumber("Select sort type: ");
        if(type > 3 || type < 1)
            continue;
        switch(type){
        case 1:
            sort(pupils.begin(), pupils.end(), sortByName);
            break;
        case 2:
            sort(pupils.begin(), pupils.end(), sortByAverageGrade);
            break;
        case 3:
            sort(pupils.begin(), pupils.end(), sortByScholarship);
            break;
        }
        break;
    }
    printPupils(pupils);
}

// Search by param, fetch data from file and remove everything wrong :)
void searchPupils(){
    vector<Pupil> pupils = readPupils();
    while(1){
        cout << "1. Search by name" << endl;
        cout << "2. Search by group" << endl;
        cout << "3. Search by activity status" << endl;
        int type = readNumber("Select search type: ");
        if(type > 3 || type < 1)
            continue;
        switch(type){
        case 1:{
            string name = readString("Enter name: ");
            name = toLowerCase(name);
            for(int i = 0; i < pupils.size(); i++) {
                string pupilName = toLowerCase(pupils[i].name);
                if(pupilName != name)
                    pupils.erase(pupils.begin() + i);
            }
            break;
        }
        case 2:{
            int group = readNumber("Enter group: ");
            for(int i = 0; i < pupils.size(); i++) {
                if(pupils[i].group != group)
                    pupils.erase(pupils.begin() + i);
            }
            break;
        }
        case 3:{
            bool isActive = readBoolean("Enter active status(1 - active, 0 - not active): ");
            for(int i = 0; i < pupils.size(); i++) {
                if(pupils[i].isActive != isActive)
                    pupils.erase(pupils.begin() + i);
            }
            break;
        }
        }
        break;
    }
    printPupils(pupils);
}

// Print admin's menu
void adminMenu()
{
    cout << "User management" << endl;
    cout << "1. View all users" << endl;
    cout << "2. Create new user" << endl;
    cout << "3. Edit user" << endl;
    cout << "4. Delete user" << endl;
    cout << endl;
    cout << "Pupil management" << endl;
    cout << "5. Delete data file" << endl;
    cout << "6. View all pupils" << endl;
    cout << "7. Add new pupil" << endl;
    cout << "8. Delete pupil" << endl;
    cout << "9. Edit pupil" << endl;
    cout << "10. Print sorted pupils" << endl;
    cout << "11. Search pupils" << endl;

}
// Print user's menu
void userMenu()
{
    cout << "1. View all pupils" << endl;
    cout << "2. Print sorted pupils" << endl;
    cout << "3. Search pupils" << endl;

}
// Clean console, Windows only, coz. sometimes C++ sucks
void cls(){
    system("cls");
}

int main()
{
    int accessLevel = login();
    cls();
    switch(accessLevel)
    {
    case 0:

        while(1)
        {
            userMenu();
            int option = readNumber("Enter number: ");
            switch(option)
            {
            case 1:
                printPupils(readPupils());
                break;
            case 2:
                sortPupils();
                break;
            case 3:
                searchPupils();
                break;
            }
            cout << "Press any button to continue..." << endl;
            _getch();
            cls();
        }
        break;
    case 1:
        while(1)
        {
            adminMenu();
            int option = readNumber("Enter number: ");
            switch(option)
            {
            case 1:
                printUsers();
                break;
            case 2:
                createNewUser();
                break;
            case 3:
                editUser();
                break;
            case 4:
                deleteUser();
                break;
            case 5:
                deleteDataFile();
                break;
            case 6:
                printPupils(readPupils());
                break;
            case 7:
                createNewPupil();
                break;
            case 8:
                deletePupil();
                break;
            case 9:
                editPupil();
                break;
            case 10:
                sortPupils();
                break;
            case 11:
                searchPupils();
                break;
            }
            cout << "Press any button to continue..." << endl;
            _getch();
            cls();
        }
        break;
    }
    return 0;
}
