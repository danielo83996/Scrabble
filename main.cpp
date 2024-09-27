#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

//sets size of board, hand
const int ROWS = 15;
const int COLUMNS = 15;
const int HANDSIZE = 7;
const int MAXSCORE = 25;

//declaring functions
void DisplayBoard(string (*arr)[ROWS]);
void CreateHand(string arr[HANDSIZE]);
void DisplayHand(string arr[HANDSIZE]);
bool CheckMove(string &letter, char direction, string hand[HANDSIZE],string (*board)[ROWS], int row, int column);
void UpdateBoard(int row, int column, string word, char direction,string (*board)[ROWS]);
void UpdateScore(int &score, bool legalMove, string letter);
bool UpdateHand(string hand[HANDSIZE], string letter);

int main() {
    //initializes board
    string board[ROWS][COLUMNS];
    for(int rows =0; rows < ROWS; rows++){
        for(int columns = 0; columns < COLUMNS; columns++){
            board[rows][columns] = "0";
        }
    }

    //initialize player hands and players choices
    string player1[HANDSIZE];
    string player2[HANDSIZE];
    int rowChoice,colChoice,player1Score = 0,player2Score = 0;
    string letterChoice;
    char directionChoice;
    bool move;

    while(player1Score <MAXSCORE || player2Score < MAXSCORE){
        //creates players hands and board
        DisplayBoard(board);
        //player ones turn
        move = false;
        while(!move){
            CreateHand(player1);
            cout << "Player one choose what column you would like to play" << endl;
            cin >> rowChoice;
            cout << "choose what row you would like to play:" << endl;
            cin >> colChoice;
            cout << "What letter(s) would you like to play:" << endl;
            DisplayHand(player1);
            cout << endl;
            cin >> letterChoice;
            if (letterChoice.length() > 1) {
                cout << "would you like to play word horiziontal or vertial? H or V:" << endl;
                cin >> directionChoice;
            }
            move = CheckMove(letterChoice, directionChoice, player1, board, rowChoice, colChoice);
        }
        UpdateScore(player1Score, move,letterChoice);
        cout << "Player ones score is: "<< player1Score << "\nPlayer twos score is: "<<player2Score << endl;

        //player twos turn and creates his hand
        CreateHand(player2);
        DisplayBoard(board);
        move = false;

        while(!move){
            cout << "player two choose what column you would like to play" << endl;
            cin >> rowChoice;
            cout << "choose what row you would like to play:" << endl;
            cin >> colChoice;
            cout << "What letter(s) would you like to play:" << endl;
            DisplayHand(player2);
            cout << endl;
            cin >> letterChoice;
            if (letterChoice.length() > 1) {
                cout << "would you like to play word horiziontal or vertial? H or V:" << endl;
                cin >> directionChoice;
            }
            move = CheckMove(letterChoice, directionChoice, player2, board, rowChoice, colChoice);
        }
        UpdateScore(player2Score, move,letterChoice);
        cout << "Player ones score is: "<< player1Score << "\nPlayer twos score is: "<<player2Score << endl;

    }


    return 0;
}

//prints out the board
void DisplayBoard(string (*arr)[ROWS]){
    for(int columns =0; columns < COLUMNS; columns++) {
        for (int rows = 0; rows < ROWS; rows++) {
            cout << arr[rows][columns] << "    ";
        }
        cout << endl;
    }
}

//prints the users hand
void DisplayHand(string arr[HANDSIZE]){
    for(int hand = 0; hand < HANDSIZE; hand++){
        if(hand == (HANDSIZE - 1)){
            cout << arr[hand];
            break;
        }
        cout << arr[hand] << ", ";
    }
}

//Makes sure the players move is legal within the rules of scrabble (i.e. not an existing letter in the spot, adjacent
//letters make a legal word, and is touching another letter.
bool CheckMove(string &letter, char direction,string hand[HANDSIZE],string (*board)[ROWS], int row, int column){
    //uploads our file of legal scrabble words onto hash map that is paired with
    //that will be used to check if players move is a real word
    ifstream inputFile;
    inputFile.open("LegalWords.txt");
    if(!inputFile){
        cout<< "error opening file"<<endl;
        return 0;
    }
    string read, word;

    map<string, string> dictionary;

    while(getline(inputFile,read)){
        inputFile >> word;
        dictionary[word] = word;
    }

    //checks to see if word fits on the board
    if(row+letter.length() > 15 || column+letter.length() > 15){
        cout<<"illegal move! Out of bounds. Try again."<<endl;
        return false;
    }

    //checks to see if letters given are in users hand if not function ends
    if(!UpdateHand(hand, letter)){
        cout<<"illegal move! You don't have the appropriate letter(s). Try again."<<endl;
        return false;
    }

    //places the letters on the board
    word = "";
    char tempWord;
    int incriment = 1;

    if(!(dictionary.find(letter) != dictionary.end())){
        if (board[row][column] == "0") {
            board[row][column] = letter[0];
            word += letter[0];
            for (int length = 1; length < letter.length(); length++) {
                if (toupper(direction) == 'H') {

                    if(board[row + length][column] != "0"){
                        word += board[row + length][column];
                    }
                    word += letter[incriment];
                    incriment++;
                }else if (toupper(direction) == 'V'){
                    if(board[row][column + length] != "0"){
                        word +=  board[row][column + length];
                    }
                    word += letter[incriment];
                    incriment++;
                }
            }


        }else{
            word += (board[row][column]);

            for (int length = 1; length <= letter.length(); length++) {
                if (toupper(direction) == 'H') {
                    if(board[row + length][column] != "0"){
                        word += board[row + length][column];
                    }else{
                        board[row + length][column] = letter[length-1];
                        word += letter[length-1];
                        hand[length] = "";
                    }
                }else if (toupper(direction) == 'V'){
                    if(board[row][column + length] != "0"){
                        word +=  board[row][column+ length];
                    }else {
                        board[row][column + length] = letter[length-1];
                        word += letter[length-1];
                        hand[length] = "";
                    }
                }
            }
        }
    }

    letter = word;

    for( int i = 0; i < word.length(); i++){
        tempWord = toupper(word[i]);
        word[i] = tempWord;
    }
    if(dictionary.find(word) != dictionary.end()){
        cout << word << endl;
        UpdateBoard(row,column,word,direction,board);
        UpdateHand(hand,letter);
        return true;
    }
    cout<<"illegal move! Word does not exist. Try again."<<endl;
    cout << word << endl;
    return false;
}

//updates the board when pieces are interacting with each other
void UpdateBoard(int row, int column, string word, char direction,string (*board)[ROWS]){
    for(int length = 0; length < word.length(); length++){
        if (toupper(direction) == 'H') {
            board[row+length][column] = word[length];
        }else if (toupper(direction) == 'V'){
            board[row][column+length] = word[length];
        }
    }
}

//updates player hand to remove the letters from their hand
bool UpdateHand(string hand[HANDSIZE], string letter){
    for (int i = 0; i < letter.length(); i++) {
        string character = string(1, toupper(letter[i])); // Convert letter to uppercase
        bool found = false;
        for (int j = 0; j < HANDSIZE; j++) {
            if (hand[j] == character) {
                hand[j] = ""; // Remove the letter from the hand
                found = true;
                break;
            }
        }
        if (!found) {
            return false; // Letter not found in the hand
        }
    }
    return true; // All letters in the word were found in the hand and removed
}

//checks to see if the move the player made was leagal and if so updates the score
//according to the letter.
void UpdateScore(int &score, bool legalMove, string letter){
    if(legalMove){
        for(int length = 0; length < letter.length(); length++){
            switch(toupper(letter[length])){
                case 'A':
                case 'E':
                case 'I':
                case 'O':
                case 'U':
                case 'L':
                case 'N':
                case 'S':
                case 'T':
                case 'R':
                    score +=1;
                    break;
                case 'D':
                case 'G':
                    score += 2;
                    break;
                case 'B':
                case 'C':
                case 'M':
                case 'P':
                    score += 3;
                    break;
                case 'F':
                case 'H':
                case 'V':
                case 'W':
                case 'Y':
                    score += 4;
                    break;
                case 'K':
                    score += 5;
                    break;
                case 'J':
                case 'X':
                    score += 8;
                    break;
                case 'Q':
                case 'Z':
                    score += 10;
                    break;
                default:
                    break;
        }

        }
    }
}

//creates the hand for the players
void CreateHand(string arr[HANDSIZE]){
    int letter;
    srand(time(NULL));
    for(int hand = 0; hand < HANDSIZE; hand++){
        letter = rand() % 100;
        if(arr[hand].empty() || arr[hand] == ""){
            if(letter>=0 && letter<=9){
                arr[hand] = "A";
            }else if(letter >=10 && letter <=11){
                arr[hand] = "B";
            }else if(letter>=12 && letter<=13){
                arr[hand] = "C";
            }else if(letter>=14 && letter<=17){
                arr[hand] = "D";
            }else if(letter>=18 && letter <=29){
                arr[hand] = "E";
            }else if(letter>=30 && letter<=31){
                arr[hand] = "F";
            }else if(letter>=32 && letter<=34){
                arr[hand] = "G";
            }else if(letter>=35 && letter<=36){
                arr[hand] = "H";
            }else if(letter>=37 && letter<=45){
                arr[hand] = "I";
            }else if(letter==46){
                arr[hand] = "J";
            }else if(letter==47){
                arr[hand] = "K";
            }else if(letter>=48 && letter<=51){
                arr[hand] = "L";
            }else if(letter>=52 && letter<=53){
                arr[hand] = "M";
            }else if(letter>=54 && letter<=59){
                arr[hand] = "N";
            }else if(letter>=60 && letter<=67){
                arr[hand] = "O";
            }else if(letter>=68 && letter<=69){
                arr[hand] = "P";
            }else if(letter==70){
                arr[hand] = "Q";
            }else if(letter>=71 && letter<=76){
                arr[hand] = "R";
            }else if(letter>=77 && letter<=80){
                arr[hand] = "S";
            }else if(letter>=81 && letter<=86){
                arr[hand] = "T";
            }else if(letter>=87 && letter<=90){
                arr[hand] = "U";
            }else if(letter>=91 && letter<=92){
                arr[hand] = "V";
            }else if(letter>=93 && letter<=94){
                arr[hand] = "W";
            }else if(letter==95){
                arr[hand] = "X";
            }else if(letter>=96 && letter<=97){
                arr[hand] = "Y";
            }else{
                arr[hand] = "Z";
            }
        }
        }

}
