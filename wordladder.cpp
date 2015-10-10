/*
 * File: WordLadder.cpp
 * --------------------
 * Name: Tina Ju && Elena Portz
 * Section: Chip Nguyen
 * This program finds a minimal word ladder between two words.
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "stack.h"
#include "queue.h"
#include "lexicon.h"
#include "filelib.h"
#include "simpio.h"

using namespace std;

/* Function prototypes */
void introduce();
bool getWords(string& start, string& end, Lexicon& dictionary);
Stack<string> wordLadder(string start, string end, Lexicon& dictionary);
Queue<string> generateNeighbors(string last, Lexicon& dictionary);

int main() {

    introduce();
    Lexicon dictionary( promptUserForFile("Dictionary file name? ") );
    cout << endl;

    while(true){

        /*Prompt user for start and end words*/
        string start, end;
        if(getWords(start, end, dictionary)){
            cout << "Have a nice day!";
            return 0;
        }

        /*Find and print a minimal word ladder*/
        Stack<string> ladder = wordLadder(start, end, dictionary);
        if(ladder.size() > 1){
            cout << "Found ladder from " << start << " to " << end << ": " << endl;
            while(!ladder.isEmpty()){
                cout << ladder.pop() << " ";
            }
            cout << endl;
        }else{
            cout << "No ladder found." << endl;
        }

        cout << endl;
    }

    return 0;
}

/*
 * Function: introduce word ladder to user
 * Usage: introduce();
 * --------------------------------
 * This function introduces the user to what a word ladder is and what the user should do.
 */
void introduce(){
    cout << "Welcome to CS 106B Word Ladder." << endl;
    cout << "Please give me two English words, and I will change the." << endl;
    cout << "first into the second by changing one letter at a time." << endl << endl;
}

/*
 * Function: get the start and end words
 * Usage: if(getWords(start, end, dictionary)) return 0;
 * --------------------------------
 * This function prompts the user to input the start and end words. If the words do not comply with
 * what WordLadder needs, the user will be reprimanded and reprompted. The function returns a boolean
 * so the program knows that the user hit "Enter" to quit. An if statement can be crafted so that if the
 * function returns true, the main program is exited.
 */
bool getWords(string& start, string& end, Lexicon& dictionary){

    start = getLine("Word #1 (or Enter to quit): ");
    if(start == ""){
        return true;
    }
    toLowerCase(start);

    end = getLine("Word #2 (or Enter to quit): ");
    if(end == ""){
        return true;
    }
    toLowerCase(start);

    if(start == end){
        cout << "The two words must be different." << endl << endl;
        return getWords(start, end, dictionary);
    }
    else if(!dictionary.contains(start) || !dictionary.contains(end)){
        cout << "Both words must be found in the dictionary." << endl << endl;
        return getWords(start, end, dictionary);
    }
    else if(start.length() != end.length()){
        cout << "The two words must be the same length." << endl << endl;
        return getWords(start, end, dictionary);
    }
    else{
        return false;
    }
}

/*
 * Function: find the minimal word ladder
 * Usage: Stack<string> ladder = wordLadder(start, end, english);
 * --------------------------------
 * This function finds the minimal word ladder between the start and end words
 * by employing a breadth-first search. All hops in the ladder must be a valid English word.
 */
Stack<string> wordLadder(string start, string end, Lexicon& dictionary){

    Queue<Stack<string>> possibleLadders;      /*Queue to store all possible partial-ladder stacks*/
    Lexicon usedWords;                          /*Lexicon to store words already used*/
    Stack<string> startLadder;                 /*The first ladder, based on the start word*/

    startLadder.push(start);
    possibleLadders.enqueue(startLadder);

    while(!possibleLadders.isEmpty()){

        Stack<string> ladder = possibleLadders.dequeue();
        string lastWord = ladder.peek();

        if(lastWord == end) return ladder;

        Queue<string> neighbors = generateNeighbors(lastWord, dictionary);

        while(!neighbors.isEmpty()){                //go through next possible words
            string next = neighbors.dequeue();
            if(!usedWords.contains(next)){          //if haven't been used before
                if(next == end){
                    ladder.push(next);
                    return ladder;
                } else {
                    Stack<string> newLadder = ladder;    //make a new ladder stack by copying it
                    newLadder.push(next);                //and adding the new word to the original stack
                    possibleLadders.enqueue(newLadder);  //add new ladder stack to queue
                    usedWords.add(next);
                }
            }
        }
    }

    return startLadder;
}

/*
 * Function: generate the next possible words
 * Usage: Queue<string> neighbors = generateNeighbors(last, dictionary);
 * --------------------------------
 * This function returns a queue of all the possible English words
 * that can be derived from the given word by only changing one character.
 */
Queue<string> generateNeighbors(string last, Lexicon& dictionary){

    Queue<string> nextWords;
    Lexicon usedWords;
    usedWords.add(last);

    for(int i = 0; i < last.length(); i++){     //go through the original word
        string original = last;
        for(int c = 'a'; c <= 'z'; c++){        //go through the alphabet
            string word = original.replace(i, 1, 1, c);
            if(dictionary.contains(word) && !usedWords.contains(word)){
                nextWords.enqueue(word);
                usedWords.add(word);
            }
        }
    }

    return nextWords;
}


