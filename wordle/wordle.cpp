#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_LENGTH = 17;
const int MIN_LENGTH = 3;
const int WORD_LENGTH = 6;
const int ATTEMPTS = 6;
const int MAX_PLAYERS = 100;

struct Player {
    char username[MAX_LENGTH];
    int games;
    int wins;
};

Player player[MAX_PLAYERS];
int playerCount = 0;

int strLen(const char str[]) {
    int i = 0;
    while (str[i] != '\0')
        i++;

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
    if (strLen(str1) != strLen(str2)) 
        return false;

    while (str1[i] != '\0' && str2[i] != '\0') {
        if(str1[i] != str2[i]) 
            return false;

        i++;
    }
    return true;
}

double winRate(const Player& player) {
    if (player.games == 0) return 0;

    return (double) player.wins / (double) player.games;
}

void swapPlayers(Player& a, Player& b) {
    Player tmp = a;
    a = b;
    b = tmp;
}

void removeLine() {
    if (cin.bad()) return;
    cin.clear();
    cin.ignore(100000, '\n');
}

void sortByGamesPlayed() {
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = 0; j < playerCount - 1 - i; j++) {
            if (player[j].games < player[j + 1].games) {
                swapPlayers(player[j], player[j + 1]);
            }
        }
    }
}

void sortByWinRate() {
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = 0; j < playerCount - 1 - i; j++) {
            double a = winRate(player[j]);
            double b = winRate(player[j + 1]);

            if (a < b) {
                swapPlayers(player[j], player[j + 1]);
            } 
            
            else if (a == b && player[j].games < player[j + 1].games) {
                swapPlayers(player[j], player[j + 1]);
            }
        }
    }
}

bool accountExistsInFile(const char username[], const char password[], bool checkPassword) {
    ifstream fin;
    fin.exceptions(ios::goodbit);
    fin.open("users.txt");

    if (!fin) 
        return false;

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
    
    while (true){
        
        cin.getline(username, MAX_LENGTH);

        if (cin.fail()) {
            removeLine();
            cout << "Username is too long, please try again: " << endl;
            continue;
        }

        if (!isValidCredential(username)) {
            cout << "Username is too short or contains spaces. Try again (at least 3 characters, no spaces): " << endl;
            continue;
        }

        if (checkAccountExistanceReg(username)) {
            cout << "An account with this username already exists. Try again: " << endl;
            continue;
        }

        return;
    }
}

void readPassword(char password[]) {   //register
    cout << "Set your password: " << endl;

    while (true) {
        
        cin.getline(password, MAX_LENGTH);
        if (cin.fail()) {
            removeLine();
            cout << "Password is too long, please try again: " << endl;
            continue;
        }

        if (!isValidCredential(password)) {
            cout << "Password is too short or contains spaces. Try again (at least 3 characters, no spaces): " << endl;
            continue;
        }

        return;
    }
}

void addUserToFile(const char username[], const char password[]) {     //register
    ofstream fout;
    fout.exceptions(ios::goodbit);
    fout.open("users.txt", ios::app);

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

bool processLogin(char currentUser[]) {      //login
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

void login(char currentUser[]) {
    processLogin(currentUser);
}

void savePlayerToLeaderboard() {     //writes the player into the flie
    ofstream fout;
    fout.exceptions(ios::goodbit);
    fout.open("leaderboard.txt");

    if (!fout.is_open()) 
        return;
    
    for (int i = 0; i < playerCount; i++) {
        fout << player[i].username << " "
             << player[i].games << " "
             << player[i].wins << endl;
    }
    fout.close();
}

void addPlayerAtReg(const char username[]) {  //adds the registered user as a player
    if (playerCount >= MAX_PLAYERS)
        return;

    strCopy(player[playerCount].username, username, MAX_LENGTH);
    player[playerCount].games = 0;
    player[playerCount].wins = 0;
    playerCount++;
}

void registerNewUser(char currentUser[]) {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];

    readUsername(username);
    readPassword(password);
    addUserToFile(username, password);

    addPlayerAtReg(username);
    savePlayerToLeaderboard();

    strCopy(currentUser, username, MAX_LENGTH);
    
    cout << "Your account has been registered successfully" << endl;
}

int checkChoice(int choice, char currentUser[]) { //both

    if (choice == 1){ 
        login(currentUser);
        return 1;
    }
    
    if (choice == 2){
        registerNewUser(currentUser);
        return 1;
    }

    if (choice == 3)
        return 2;

    return 0;
}

bool menu(char currentUser[]) {
    int choice;

    cout << "___________________________________________" << endl;
    cout << "                   WORDLE                  " << endl;
    cout << "___________________________________________" << endl;
    cout << "| Press 1 to login      |" << endl;
    cout << "| Press 2 to register   |" << endl;
    cout << "| Press 3 to exit       |" << endl;
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

        int result = checkChoice(choice, currentUser);

        if (result == 0) {
            cout << "Invalid input. Please enter 1, 2 or 3: " << endl;;
            continue;
        }

        else if (result == 2) {
            cout << "Goodbye!" << endl;
            return false;
        }
        break;
    }
    return true; 
}

int countWords() {
    ifstream fin;
    fin.exceptions(ios::goodbit);
    fin.open("words.txt");

    if (!fin.is_open()){
        return 0;
    }

    int count = 0;
    char word[WORD_LENGTH];

    while (fin >> word){
        count++;
    }

    fin.close();
    return count;
}

bool pickWordAtIndex (const int i, char result[]) {
    ifstream fin;
    fin.exceptions(ios::goodbit);
    fin.open("words.txt");

    if (!fin.is_open()) return false;
    
    char word[WORD_LENGTH];
    int current = 0;

    while (fin >> word) {
        if (current == i) {
            strCopy(result, word, WORD_LENGTH);
            fin.close();
            return true;
        }
        current++;
    }

    fin.close();
    return false;

}

bool pickRandomWord (char result[]) {
    int wordCount = countWords();

    if (wordCount == 0) return false;

    int randomIndex = rand() % wordCount;
    return pickWordAtIndex(randomIndex, result);
}

bool isLower (const char letter) {
    return (letter >= 'a' && letter <= 'z');
}

bool isValidGuessFormat (const char guess[]) {
    for (int i = 0; i < WORD_LENGTH-1; i++) {
        if (guess[i] == '\0') 
            return false;
        if (!isLower(guess[i]))
            return false;
    }
    return guess[WORD_LENGTH-1] == '\0';
}

void readGuess (char guess[WORD_LENGTH]) {
    while (true) {
        cout << "Enter your guess (5 lowercase letters): " << endl;
        cin.getline(guess, WORD_LENGTH);

        if (cin.fail()) {
            removeLine();
            cout << "The word is too long. Please enter a word consisting of exactly 5 letters" << endl;
            continue;
        }

        if (!isValidGuessFormat(guess)) {
            cout << "Invalid guess. Use exactly 5 lowercase letters" << endl;
            continue;
        }

        return;
    }
}

void processGuess (const char guess[WORD_LENGTH], const char word[WORD_LENGTH], int letter[WORD_LENGTH-1]) {
    const int gray = 0;
    const int yellow = 1;
    const int green = 2;
    bool used[WORD_LENGTH-1] = {false};

    for (int i = 0; i < WORD_LENGTH-1; i++) letter[i] = gray; //turns all to gray

    for (int i = 0; i < WORD_LENGTH-1; i++) {
        if (guess[i] == word[i]) {
            letter[i] = green; //finds green
            used[i] = true;
        }
    }

    for (int i = 0; i < WORD_LENGTH-1; i++) {
        for (int j = 0; j < WORD_LENGTH-1; j++) {
            if (guess[i] == word[j] && letter[i] != green && used[j] == false) {
                letter[i] = yellow; //finds yellow
                used[j] = true;
                break;
            }
        }
    }
}

bool checkIfCorrect(const char guess[WORD_LENGTH], int letter[WORD_LENGTH-1]) {
    for (int i = 0; i < WORD_LENGTH-1; i++) {
        if (letter[i] != 2) 
            return false;
    }

    return true;
}

void colorGuess (const char guess[WORD_LENGTH], const int letter[WORD_LENGTH-1]) {
    for (int i = 0; i < WORD_LENGTH-1; i++) {
        if (letter[i] == 2)
            cout << "\033[42;30m" << guess[i] << " \033[0m";

        else if (letter[i] == 1)
            cout << "\033[43;30m" << guess[i] << " \033[0m";

        else cout << "\033[47;30m" << guess[i] << " \033[0m";
    }

    cout << endl;
}

int findPlayer(const char username[]) {
    for (int i = 0; i < playerCount; i++) {
        if (isEqual(player[i].username, username))
            return i;
    }

    return -1;
}

void updatePlayerAfterWin(const char currentUser[]) {
    int i = findPlayer(currentUser);

    if (i == -1) return;

    player[i].games++;
    player[i].wins++;
}

void updatePlayerAfterLoss(const char currentUser[]) {
    int i = findPlayer(currentUser);

    if (i == -1) return;

    player[i].games++;
}

void startGame(const char currentUser[]) {
    char word[WORD_LENGTH];
    
    if (!pickRandomWord(word)) {
        cout << "No words in the file" << endl;
        return;
    }

    if (pickRandomWord(word)){
        for (int i = 0; i < ATTEMPTS; i++) {
            char guess[WORD_LENGTH];
            int letter[WORD_LENGTH-1];

            readGuess(guess);
            processGuess(guess, word, letter); 
            colorGuess(guess, letter);

            cout << endl;
            if (checkIfCorrect(guess, letter)) {
                cout << "You won!" << endl;
                updatePlayerAfterWin(currentUser);
                savePlayerToLeaderboard();
                return;
            }
        }   
    }
    updatePlayerAfterLoss(currentUser);
    savePlayerToLeaderboard();
    cout << "You lost" << endl;
}

bool playAgain() {
    const int ANSWER_LENGTH = 2;
    char input[ANSWER_LENGTH];

    while (true) {
        cout << "Would you like to play again? Answer with y for yes or n for no" << endl;
        cin.getline(input, ANSWER_LENGTH);

        if (cin.fail()) {
            removeLine();
            cout << "Invalid answer. Please answer with y or n" << endl;
            continue;
        }

        if (input[0] == '\0') {
            cout << "Invalid answer. Please answer with y or n" << endl;
            continue;
        }

        if (input[0] == 'y' || input[0] == 'Y') 
            return true;

        if (input[0] == 'n' || input[0] == 'N') 
            return false;
        
        cout << "Invalid answer. Please answer with y or n" << endl;
    }
}

void loadLeaderboard() {
    playerCount = 0;

    ifstream fin;
    fin.exceptions(ios::goodbit);
    fin.open("leaderboard.txt");

    if (!fin.is_open()) return;

    while (playerCount < MAX_PLAYERS) {
        char name[MAX_LENGTH];
        int games, wins;

        if (!(fin >> name >> games >> wins))
            break;

        strCopy(player[playerCount].username, name, MAX_LENGTH);
        player[playerCount].games = games;
        player[playerCount].wins = wins;

        playerCount++;
    }

    fin.close();
}

void printLeaderboard() {
    cout << "-------------------- LEADERBOARD --------------------" << endl;
    cout << "User             Games   Wins   Winrate" << endl;
    cout << "-----------------------------------------------------" << endl;

    for (int i = 0; i < playerCount; i++) {
        double winrate = winRate(player[i]) * 100.0;

        cout << player[i].username;

        int nameLen = strLen(player[i].username);
        for (int i = nameLen; i < 16; i++) 
            cout << " ";

        cout << player[i].games << "       "
             << player[i].wins << "      "
             << (int)winrate << "%" << endl;
    }

    cout << "-----------------------------------------------------" << endl;
}

void leaderboardMenu() {
    while (true) {
        int choice;

        cout << "How would you like for the leaderboard to be sorted?" << endl;
        cout << "Sort by games played - Press 1" << endl;
        cout << "Sort by winrate - Press 2" << endl;
        cout << "I don't want to see the leaderboard - Press 3" << endl;
        cout << "Please enter your choice: ";

        cin >> choice;
        if (!cin) {
            cin.clear();
            cin.ignore(100000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }
        cin.ignore(100000, '\n');

        if(choice == 1) {
            sortByGamesPlayed();
            printLeaderboard();
            return;
        }

        if(choice == 2) {
            sortByWinRate();
            printLeaderboard();
            return;
        }

        if(choice == 3)
            return;

        else cout << "Invalid choice. Please enter 1, 2 or 3" << endl;
    }
}

int main(){
    cin.exceptions(ios::goodbit);
    cout.exceptions(ios::goodbit);
    cerr.exceptions(ios::goodbit);
    
    char currentUser[MAX_LENGTH];

    srand(time(0));
    loadLeaderboard();

    if(!menu(currentUser))
        return 0;

    do {
        startGame(currentUser);
    } while (playAgain());

    leaderboardMenu();

    cout << "Thanks for playing!" << endl;
    
    return 0;
}