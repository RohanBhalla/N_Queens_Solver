//
//  main.cpp
//  AI_Project2
//
//  Created by Rohan Bhalla and Mirna Ashour.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
using namespace std;

vector<vector<int>> gameBoard {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};
//Maybe implement the 2 boards below as characters
vector<vector<int>> horiCSBoard {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};
//Vertical constraint satisfaction board
vector<vector<int>> vertiCSBoard {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};
//Degree board
vector<vector<int>> degBoard {
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0}
};
//MRV board
vector<vector<int>> mrvBoard {
    {0, 5, 5, 5, 5},
    {5, 5, 5, 0, 5},
    {5, 5, 5, 5, 0},
    {5, 5, 5, 5, 5},
    {5, 5, 5, 5, 5},
};

//  open file stream
void openFile(ifstream& ifs, string fileName);

//  read from file and fill in information
void readFile(ifstream& ifs);

//print board to screen
void printBoard(vector<vector<int>> board);

//Backtracking Search function (recursive ???) (Might need additional paramaters as well)
bool backTrackSearch();

//Big boy degree
pair<int,int> degreeHeuristic(vector<pair<int, int>> minPositions);

// Help with degree calc
void degreeHelper(int i, int j);

//Consistent Row and Col assignment check
bool checkRow(int row, int col, int val);

bool checkCol(int row, int col, int val);

//Select unassigned variable function (calls mrv and deg accordingly)
pair<int, int> selectUnassignedVar();

// mrv function
void marvin();

// mrv helper for checking number constraints in row and col
int mrvRowCol(size_t row, size_t col);

// mrv helper for checking horizontal and vertical constraints
int mrvConstraints(size_t row, size_t col);

// returns an array of the positions of the min values in the MRV board
vector<pair<int,int>> minMRV();

//Constraint satisfaction checkers
bool numConditions(int row, int col, int val);

//  checks for an empty cell on the board (returns true if there is)
bool checkEmptySpot();

int emptySpots();

int main(int argc, const char * argv[]) {
    cout << "AI PROJECT 2\n";
    //  intializing file stream and initialBoard array
    ifstream ifs;
    string fileName = "";

    //  checks if a file name was passed an argument, if not, it exits with an error
    if(argc == 2) {
        fileName = argv[1];
    }
    else {
        cout << "File name not provided." << endl;
        exit(1);
    }
    openFile(ifs, fileName);
    readFile(ifs);
    printBoard(gameBoard);
    cout << endl;
    printBoard(horiCSBoard);
    cout << endl;
    printBoard(vertiCSBoard);
    cout << endl;
    
    cout << "TESTING\n";
    backTrackSearch();

//    pair<int, int> varPos = selectUnassignedVar();
    printBoard(gameBoard);
//    cout << endl;
//    printBoard(mrvBoard);
//    cout << endl << "Min MRV pos: " << varPos.first << " " << varPos.second << endl;
    
    return 0;
}


//  open file stream
void openFile(ifstream& ifs, string fileName) {
    ifs.open(fileName);
    if(!ifs){   // prints error and exits if file is not found
        cout << "File cannot be opened." << endl;
        exit(1);
    }
}


//  read from file and fill in information
void readFile(ifstream& ifs) {
    int readInt;
    char readChar;
    //  populates intital board
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++){
            ifs >> readInt;
            gameBoard[i][j] = readInt;
        }
    }
    //Horizontal constraints board is filled here
    for(int i = 0; i < 5; i++) {
        for(int j = 1; j < 5; j++){
            ifs >> readChar;
            int num  = int(readChar);
            if(num == 48)
                num = 0;
            else if (num == 60) // Lesser than < --> 1
                num = 1;
            else if (num == 62) // Greater than > --> 2
                num = 2;
            horiCSBoard[i][j] = num;
        }
    }
    //Vertical constraints board is filled here
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 5; j++){
            ifs >> readChar;
            int num  = int(readChar);
            if(num == 48)
                num = 0;
            else if (num == 94) // Lesser than ^ --> 1, 94 = ^
                num = 1;
            else if (num == 118) // Greater than v --> 2, 118 = v
                num = 2;
            vertiCSBoard[i][j] = num;
        }
    }


    ifs.close();
}

//Backtracking Search function (recursive ???)
bool backTrackSearch()
{
    //Base case check
    if(checkEmptySpot() == false) {
        return true;
    }
    //Print empty spots
//    cout << "Empty spots: "  << emptySpots() <<endl;
    //Recursive case
    pair<int, int> varPos = selectUnassignedVar();
    
    for(int val = 1; val <= 5; ++val) {
        int saveMrv = mrvBoard[varPos.first][varPos.second];
        //Check all constraints are satisfied by chosen value
        if(numConditions(varPos.first, varPos.second, val) && checkRow(varPos.first, varPos.second, val) && checkCol(varPos.first, varPos.second, val)) {
            //Assign value to the position
            gameBoard[varPos.first][varPos.second] = val;
            //Update MRV
            mrvBoard[varPos.first][varPos.second] = 0;
            bool result = backTrackSearch();
            if(result){
                return result;
            }
            //Remove assigned value if it didn't work
            gameBoard[varPos.first][varPos.second] = 0;
            //Reset MRV
            mrvBoard[varPos.first][varPos.second] = saveMrv;
        }
        
        
    }
    
    return false;
}



//   print board to screen
void printBoard(vector<vector<int>> board) {
    for(size_t i = 0; i <board.size(); i++) {
        for(size_t j = 0; j < board[i].size(); j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}


pair<int, int> selectUnassignedVar() {
    vector<pair<int, int>> minPositions;
    pair<int, int> cellPos;
    
    marvin();
    minPositions = minMRV();
    if(minPositions.size() > 1) {
        cellPos = degreeHeuristic(minPositions);
    }
    else
    {
        cellPos = minPositions[0];
    }
//    cout << "CELL POS: " << cellPos.first << " " << cellPos.second << endl;
    return cellPos;
}


vector<pair<int,int>> minMRV() {
    int min = INT_MAX;
    vector<pair<int, int>> minPositions;
    
    for(int i = 0; i < mrvBoard.size(); i++) {
        for(int j = 0; j < mrvBoard[i].size(); ++j){
            if(mrvBoard[i][j] < min && (gameBoard[i][j] == 0) ) {
                min = mrvBoard[i][j];
                minPositions.clear();
                minPositions.push_back(make_pair(i, j));
            }
            else if(mrvBoard[i][j] == min && (gameBoard[i][j] == 0)) {
                minPositions.push_back(make_pair(i, j));
            }
        }
    }
    
    return minPositions;
}

//MRV heuristic
void marvin(){
    int totalNumCons = 0;
    
    for(size_t i=0; i < gameBoard.size(); ++i){
        for(size_t j=0; j < gameBoard[i].size(); ++j){
            if(gameBoard[i][j] != 0)//has an assigned value
            {
                mrvBoard[i][j] = 0;
            }
            else{
                totalNumCons += mrvRowCol(i, j);
                totalNumCons += mrvConstraints(i, j);
                mrvBoard[i][j] = 5 - totalNumCons;
                totalNumCons = 0;
            }
        }
    }
}


int mrvRowCol(size_t row, size_t col) {
    //MRV board will be pre initialized with domain sizes for all positions (0 if value has been assigned)
    
    int numCons = 0;
    //Row scan
    for(size_t i = 0; i < gameBoard.size(); ++i){
        if(i != col && (gameBoard[row][i] != 0))
        {
            numCons++;
        }
    }
    //Col scan
    for(size_t j=0; j < gameBoard.size(); ++j){
        if(j != row && (gameBoard[j][col] != 0))
        {
            numCons++;
        }
        
    }
//    mrvBoard[row][col] -= numCons;
    return numCons;
    
}

int mrvConstraints(size_t row, size_t col)
{
    int numCons = 0;
    //If already assigned
    if(gameBoard[row][col] != 0)
    {
        return 0;
    }
    
    //Check the cell itself for inequality (horizontal)
    if(horiCSBoard[row][col] == 2 || (horiCSBoard[row][col] == 1) ){
        numCons++;
        
    }
    //Check right cell for inequality
    if(col < 4)
    {
        if(horiCSBoard[row][col+1] == 1 || (horiCSBoard[row][col+1] == 2) ){
            numCons++;
        }
    }
    

    //Check the cell itself for inequality (vertical)
    if(vertiCSBoard[row][col] == 2 || (vertiCSBoard[row][col] == 1) ) {
        numCons++;
    }
    //Check above cell for inequality
    if(row > 0){
        if(vertiCSBoard[row-1][col] == 2 || (vertiCSBoard[row-1][col] == 1) ) {
            numCons++;
        }
    }
    
//    mrvBoard[row][col] -= numCons;
    return numCons;
    
}


//Degree heruristic checking number of unassigned neighbors for a cell
pair<int,int> degreeHeuristic(vector<pair<int, int>> minPositions) {
    int max = INT_MIN;
    pair<int, int> maxPos;
    //Reset degree board
    for(int i=0; i < degBoard.size(); ++i)
    {
        for(int j=0; j < degBoard[i].size(); ++j)
        {
            degBoard[i][j] = 0;
        }
    }
    //Loop through board and calculate values
    for(int i=0; i < minPositions.size(); ++i)
    {
        degreeHelper(minPositions[i].first, minPositions[i].second);
    }
    //Take max of degreeBoard
    //COUNTER SOLUTION
    for(int i=0; i < minPositions.size(); ++i)
    {
        if(degBoard[minPositions[i].first][minPositions[i].second] > max)
        {
            max = degBoard[minPositions[i].first][minPositions[i].second];
            maxPos = make_pair(minPositions[i].first, minPositions[i].second);
        }
    }
    
//    for(int i = 0; i < degBoard.size(); i++) {
//        for(int j = 0; j < degBoard[i].size(); ++j){
//            if(degBoard[i][j] > max){
//                max = degBoard[i][j];
//                maxPos = make_pair(i, j);
//            }
//        }
//    }
    
    return maxPos;
}


//Degree heuristic helper
void degreeHelper(int i, int j) {
    if((i != 0)) {  //Up
        if(gameBoard[i-1][j] == 0) {
            degBoard[i][j]++;
        }
        
    }
    if((i != 4)) { //Down
        if(gameBoard[i+1][j] == 0) {
            degBoard[i][j]++;
        }
    }
    if((j != 0)) { //Left
        if(gameBoard[i][j-1] == 0) {
            degBoard[i][j]++;
        }
        
    }
    if((j != 4)) { //Right
        if(gameBoard[i][j+1] == 0) {
            degBoard[i][j]++;
        }
    }
}

//Row consistency check for given value, position
bool checkRow(int row, int col, int val)
{
    bool ans = true;
    for(int i=0; i < gameBoard[0].size(); i++){
        if(gameBoard[row][i] == val){
            ans = false;
            break;
        }
    
    }
    return ans;
}
//Col consistency check for given value, position
bool checkCol(int row, int col, int val)
{
    bool ans = true;
    for(int i=0; i < gameBoard[0].size(); i++){
        if(gameBoard[i][col] == val){
            ans = false;
            break;
        }
    
    }
    return ans;
}
//Inequality consistency checker for given value, position (INCOMPLETE)
bool numConditions(int row, int col, int val)
{
    //For the given cell (row, col) check the hConsts and VConsts
    
    //Horizontal checking
    
    //Check the cell itself for > sign
    if(horiCSBoard[row][col] == 2 && (col != 0)){ // contains  > sign
        if(gameBoard[row][col-1] !=0 && (gameBoard[row][col-1] < val)){ //check val on left to see if it is greater or not
            return false;
        }
        
    }
    //Check the cell itself for < sign
    else if(horiCSBoard[row][col] == 1 && (col != 0)){ // contains  < sign
        if(gameBoard[row][col-1] !=0 && (gameBoard[row][col-1] > val)){ //check val on left to see if it is lesser or not
            return false;
        }
    }
    
    //Check right cell for inequality
    if(col < 4){
        if(horiCSBoard[row][col+1] == 1){     //Right cell has < sign
            if(gameBoard[row][col+1] !=0 && (gameBoard[row][col+1] < val)){
                return false;
            }
        }
        else if(horiCSBoard[row][col+1] == 2) {     // Right cell has  > sign
            if(gameBoard[row][col+1] !=0 && (gameBoard[row][col+1] > val)){
                return false;
            }
        }
    }
    
    //Vertical checking
    if(vertiCSBoard[row][col] == 2 && (row != 4)) {   // contains V sign (greater than)
        if(gameBoard[row+1][col] != 0 && (gameBoard[row+1][col] > val)){      // checks if val below is greater or not
            return false;
        }
    }
    else if(vertiCSBoard[row][col] == 1 && (row != 4)) {  //Check cell itself for ^ (less than)
        if(gameBoard[row+1][col] != 0 && (gameBoard[row+1][col] < val)) {       // checks if val below is lesser or not
            return false;
        }
    }
    //Check above cell for inequality
    if(row > 0) {
        if(vertiCSBoard[row-1][col] == 2) {   // checks if cell above contains V (greater than value)
            if(gameBoard[row-1][col] != 0 && (val > gameBoard[row-1][col]) ) {
                return false;
            }
        }
        
        else if(vertiCSBoard[row-1][col] == 1) {     // checks if cell above contains ^ (less than value)
            if(gameBoard[row-1][col] != 0 && (val < gameBoard[row-1][col]) ) {
                return false;
            }
        }
    }
    return true;
}

//Stopping condition check
bool checkEmptySpot()
{
    bool ans = false;
    for(int i = 0; i <gameBoard.size(); i++) {
        for(int j = 0; j < gameBoard[i].size(); j++) {
            if(gameBoard[i][j] == 0)
            {
                ans = true;
                return  ans;
            }
                
        }
    }
    return ans;
}

int emptySpots()
{
    int ans = 0;
    for(int i = 0; i <gameBoard.size(); i++) {
        for(int j = 0; j < gameBoard[i].size(); j++) {
            if(gameBoard[i][j] == 0)
            {
                ans++;
            }
                
        }
    }
    return ans;
}
