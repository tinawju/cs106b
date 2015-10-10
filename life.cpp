/*
 * File: life.cpp
 * --------------------
 * Name: Tina Ju
 * Section: Chip Nguyen
 * This program is a simplified version of The Game of Life, a simulation
 * of a bacterial colony over time.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;

/*Function prototypes*/
void introduce();
string promptForFile();
void inputToGrid(Grid<bool>& myGrid, string myFile);
void printGrid(Grid<bool>& myGrid);
void updateGeneration(Grid<bool>& myGrid);
int getNumOfNeighbors(Grid<bool>& myGrid, int myRow, int myCol);
void updateState(Grid<bool>& myGrid, Grid<bool>& oldGrid, int myRow, int myCol, int myNeighbors);
char promptUserInput();
int promptForFrames();
void tick(Grid<bool>& myGrid);
void animate(Grid<bool>& myGrid, int myFrames);

int main() {

    /*Introduce user to game.*/
    introduce();

    /*Prompt user for starter file.*/
    string filename = promptForFile();

    /*Set up grid*/
    Grid<bool> petriGrid;
    inputToGrid(petriGrid, filename);
    printGrid(petriGrid);

    /*Prompt user for actions*/
    while(true){
        switch (promptUserInput()){
            case 'a': animate(petriGrid, promptForFrames());
                break;
            case 't': tick(petriGrid);
                break;
            case 'q': cout << "Have a nice Life!" << endl;
                return 0;
    }
    }
   return 0;
}

/*
 * Function: Prints introduction to the game
 * Usage: introduce();
 * --------------------------------
 * This function prints an explanation fo the game and its rules for the user.
 */
void introduce(){
    cout << "Welcome to the Game of Life! This is a simulation of the life cycle of a bacterial colony." << endl;
    cout << "Cells (X) live and die depending on how many neighbors they have:" << endl;
    cout << "- A cell with 1 or fewer neighbors dies." << endl;
    cout << "- Locations with 2 neighbors remain stable." <<endl;
    cout << "- Locations with 3 neighbors will create life." << endl;
    cout << "- A cell with 4 or more neighbors dies." << endl << endl;
}

/*
 * Function: Prompts user for file name
 * Usage: filename = promptForFile();
 * --------------------------------
 * This function prompts the user for the file they would like to use
 * to set up the grid.
 */
string promptForFile(){
    string filename;
    cout << "Grid input file name? ";
    cin >> filename;
    if(!fileExists(filename)){
        cout << "Unable to open file. Try again!" << endl;
        return promptForFile();
    }
    return filename;
}

/*
 * Function: Configures the grid based on given file
 * Usage: animate(grid, frames);
 * --------------------------------
 * This function reads the given file then redefines the grid's dimensions
 * and fills the grid according to the file's specifications.
 */
void inputToGrid(Grid<bool>& myGrid, string myFile){
    //open file with stream
    ifstream infile;
    openFile(infile, myFile);

    //get row and col specs
    string rowStr, colStr;
    getLine(infile, rowStr);
    getLine(infile, colStr);
    int rowNum = stringToInteger(rowStr);
    int colNum = stringToInteger(colStr);

    //resize grid to specs
    myGrid.resize(rowNum, colNum);

    //fill grid
    string myLine;
    for(int row = 0; row < myGrid.numRows(); row++){
        getLine(infile, myLine);
        for(int col = 0; col < myGrid.numCols(); col++){
            if(myLine[col] == 'X'){
                myGrid.set(row, col, true);
            }
            else{
                myGrid.set(row, col, false);
            }
        }
    }
    infile.close();
}

/*
 * Function: Prints given grid
 * Usage: printGrid(myGrid);
 * --------------------------------
 * This function prints the grid by replacing a true with "X" to represent
 * a living bacterium and false with "-" to represent a dead one.
 */
void printGrid(Grid<bool>& myGrid){
    for(int row = 0; row < myGrid.numRows(); row++){
        for(int col = 0; col < myGrid.numCols(); col++){
            if(myGrid.get(row, col))
                cout << "X";
            else
                cout << "-";
        }
        cout << endl;
    }
}

/*
 * Function: Update the grid by one generation
 * Usage: updateGeneration(grid);
 * --------------------------------
 * This function updates the grid by one generation based on the rules
 * given by the simulation.
 */
void updateGeneration(Grid<bool>& myGrid){
    Grid<bool> newGrid(myGrid.numRows(), myGrid.numCols());
    for(int row = 0; row < myGrid.numRows(); row++){
        for(int col = 0; col < myGrid.numCols(); col++){
            updateState(newGrid, myGrid, row, col, getNumOfNeighbors(myGrid, row, col));
        }
    }
    myGrid = newGrid;
}

/*
 * Function: Calculates the number of neighbors
 * Usage: int numOfNeighbors = getNumOfNeighbors(grid, row, col);
 * --------------------------------
 * This function calculates the number of neighbors surrounding a given
 * location on the grid.
 */
int getNumOfNeighbors(Grid<bool>& myGrid, int myRow, int myCol){
    int neighbors = 0;
    for(int dRow = -1; dRow <= 1; dRow++){
        for(int dCol = -1; dCol <= 1; dCol++){
            if(!(dRow == 0 && dCol == 0)){  //do not count the original location
                int newRow = myRow + dRow;
                int newCol = myCol + dCol;
                if(myGrid.inBounds(newRow, newCol) && myGrid.get(newRow, newCol))
                    neighbors++;
            }
    }
    }
    return neighbors;
}

/*
 * Function: Updates the state of a location on the grid
 * Usage: updateState(grid, oldGrid, row, col, neighborNum);
 * --------------------------------
 * This function updates what a given location on a grid will be, based on
 * the number of neighbors surrounding it.
 */
void updateState(Grid<bool>& myGrid, Grid<bool>& oldGrid, int myRow, int myCol, int myNeighbors){
    if(myNeighbors < 1 || myNeighbors >= 4)
        myGrid.set(myRow, myCol, false);
    else if(myNeighbors == 3)
        myGrid.set(myRow, myCol, true);
    else if(myNeighbors == 2)
        myGrid.set(myRow, myCol, oldGrid.get(myRow, myCol));
}

/*
 * Function: Prompts user for their choice of action
 * Usage: char choice = promptUserInput();
 * --------------------------------
 * This function prompts the user to input whether they would like
 * to animate, tick, or quit - or reprompts when the user chooses
 * none of the above.
 */
char promptUserInput(){
    string choice;
    cout << "a)nimate, t)ick, q)uit? ";
    cin >> choice;
    choice = toLowerCase(choice);

    if(choice.compare("a")==0 || choice.compare("t")==0 || choice.compare("q")==0)
        return stringToChar(choice);
    else {
        cout << "Choice not recognized. Try again!" << endl;
        return promptUserInput();
}
}

/*
 * Function: Prompt user for number of animation frames
 * Usage: int numOfFrames = promptForFrames();
 * --------------------------------
 * This function prompts the user for how many frames
 * they would like to see in the animation.
 */
int promptForFrames(){
    string frameStr;
    cout << "How many frames? ";
    cin >> frameStr;
    if(stringIsInteger(frameStr))
        return stringToInteger(frameStr);
    else{
        cout << "Illegal integer format. Try again!" << endl;
        return promptForFrames();
    }
}

/*
 * Function: Go to the next generation
 * Usage: tick(grid);
 * --------------------------------
 * This function updates and displays the next generation of the bacterial colony.
 */
void tick(Grid<bool>& myGrid){
    updateGeneration(myGrid);
    printGrid(myGrid);
}

/*
 * Function: Animate a given number of generations
 * Usage: animate(grid, frames);
 * --------------------------------
 * This function plays an animation of the bacterial colony
 * over a given number of generations at 50 ms per generation.
 */
void animate(Grid<bool>& myGrid, int myFrames){
    for(int i = 0; i < myFrames; i++){
        updateGeneration(myGrid);
        printGrid(myGrid);
        pause(50);
        clearConsole();
    }
}

