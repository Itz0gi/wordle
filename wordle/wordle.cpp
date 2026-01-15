#include <iostream>
#include <fstream>
using namespace std;

const int MAX_LENGTH = 17;
const int MIN_LENGTH = 3;

int strLen(const char str[]){
    int i = 0;
    while(str[i] != '\0') i++;
    return i;
}

void strCopy(char dest[], const char src[], int maxLength) {
    int i = 0;
    while (src[i] != '\0' && i < maxLength - 1) {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool isEqual(const char str1[], const char str2[]) {
    int i = 0;
    if(strLen(str1) != strLen(str2)) return false;
    while (str1[i] != '\0' && str2[i] != '\0'){
        if(str1[i] != str2[i]) return false;
        i++;
    }
    return true;
}

void removeLine() {
    cin.clear();
    cin.ignore(100000, '\n');
}

bool accountExistsInFile(const char username[], const char password[], bool checkPassword) {
    ifstream fin("users.txt");
    if (!fin) return false;

    char user[MAX_LENGTH], pass[MAX_LENGTH];

    while (fin >> user >> pass) {
        if (isEqual(user, username)) {
            if (!checkPassword || isEqual(pass, password)) {
                fin.close();
                return true;
            }
        }
    }

    fin.close();
    return false;
}

bool checkAccountExistanceReg(const char username[]){ //register
    return accountExistsInFile(username, "", false);
}

bool checkCredentialsLog(const char username[], const char password[]) { //login
    return accountExistsInFile(username, password, true);
}

bool isValidCredential(const char str[]) {  //login
    int len = 0;
    int i = 0;

    while (str[i] != '\0') {
        if (str[i] == ' ' || str[i] == '\t') {
            return false;
        }
        len++;
        i++;
    }

    if (len < MIN_LENGTH) {
        return false;
    }

    return true;
}

void readUsername(char username[]) {    //register
    cout << "Enter new username (at least 3 characters, no spaces): " << endl;
    
    while(true){
        
        cin.getline(username, MAX_LENGTH);
        if(cin.fail()){
            removeLine();
            cout << "Username is too long, please try again: " << endl;
            continue;
        }

        if(!isValidCredential(username)) {
            cout << "Username is too short or contains spaces. Try again (at least 3 characters, no spaces): " << endl;
            continue;
        }

        if(checkAccountExistanceReg(username)) {
            cout << "An account with this username already exists. Try again: " << endl;
            continue;
        }

        return;
    }
}

void readPassword(char password[]){   //register
    cout << "Set your password: " << endl;

    while(true){
        
        cin.getline(password, MAX_LENGTH);
        if(cin.fail()) {
            removeLine();
            cout << "Password is too long, please try again: " << endl;
            continue;
        }

        if(!isValidCredential(password)) {
            cout << "Password is too short or contains spaces. Try again (at least 3 characters, no spaces): " << endl;
            continue;
        }

        return;
    }
}

void addUserToFile(const char username[], const char password[]){     //register
    ofstream fout("users.txt", ios::app);
    fout << username << " " << password << endl;
    fout.close();
}

void readCredential(const char prompt[], char input[]) {    //login
    while (true) {
        cout << prompt << endl;
        cin.getline(input, MAX_LENGTH);

        if (cin.fail()) {
            removeLine();
            cout << "Input is too long. Try again." << endl;
            continue;
        }

        if (!isValidCredential(input)) {
            cout << "Input is too short or contains spaces. Try again." << endl;
            continue;
        }

        return;
    }
}

void readLoginUsername(char username[]) {   //login
    readCredential("Please enter your username:", username);
}

void readLoginPassword(char password[]) {
    readCredential("Please enter your password:", password);
}

void readLogin(char username[], char password[]) {    //login
    readLoginUsername(username);
    readLoginPassword(password);
}

bool processLogin(char currentUser[]){      //login
    while (true) {
        char username[MAX_LENGTH];
        char password[MAX_LENGTH];

        readLogin(username, password);

        if (checkCredentialsLog(username, password)) {
            strCopy(currentUser, username, MAX_LENGTH);
            cout << "Login successful! Let's play!" << endl;
            return true;
        }

        cout << "Invalid username or password. Try again." << endl;
    }
}

void login(){
    char currentUser[MAX_LENGTH];
    processLogin(currentUser);
}

void registerNewUser(){
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    readUsername(username);
    readPassword(password);
    addUserToFile(username, password);
    
    cout << "Your account has been registered successfully" << endl;
}

bool checkChoice(int choice){ //both
    if(choice == 1){ 
        login();
        return true;
    }
    
    if(choice == 2){
        registerNewUser();
        return true;
    }

    return false;
}

void menu(){
    int choice;

    cout << "___________________________________________" << endl;
    cout << "                   WORDLE                  " << endl;
    cout << "___________________________________________" << endl;
    cout << "| Press 1 to login      |" << endl;
    cout << "| Press 2 to register   |" << endl;
    cout << "Please enter your choice: " << endl;

    
    while (true) {
        cin >> choice;
        if (!cin) {
            cin.clear();
            cin.ignore(100000, '\n');
            cout << "Invalid input (not a number). Please try again: " << endl;
            continue;
        }

        cin.ignore(100000, '\n');

        if (!checkChoice(choice)) {
            cout << "Invalid input. Please enter 1 or 2: " << endl;;
            continue;
        }
        break;
    }   
}

int main(){
    menu();
    return 0;
}