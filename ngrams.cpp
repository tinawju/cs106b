/*
 * File: ngrams.cpp
 * ----------------------
 * Name: Tina Ju && Elena Portz
 * Section: Chip Nguyen
 * This program generates random text with some semblance of sensicality
 * based on a Markov model derived from a given text.
 *
 */

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "map.h"
#include "random.h"
#include "strlib.h"
#include "vector.h"
#include "simpio.h"
#include "filelib.h"

using namespace std;

/* Function prototypes */
void introduce();
void promptForParameters(string& file, int& N, int& wordNum);
void inputData(string fileName, Map<Vector<string>, Vector<string>>& model, int N);
void updateModel(Map<Vector<string>, Vector<string>>& model, Vector<string>& nextKey, string next);
void generateRandomText(Map<Vector<string>, Vector<string>>& model, int N, int wordNum);
Vector<string> getRandomKey(Map<Vector<string>, Vector<string>>& model);

int main() {

    introduce();

    /*Prompt user for text parameters*/
    string fileName;
    int N, wordNum;
    promptForParameters(fileName, N, wordNum);

    /*Develop the model and generate the random text*/
    Map<Vector<string>, Vector<string>> model;    /*The Markov model to be used*/
    inputData(filename, model, N);
    generateRandomText(model, N, wordNum);

    return 0;
}

/*
 * Function: introduces the user to N-grams
 * Usage: introduce()
 * --------------------------------
 * This function introduces the user to N-grams and explains the input and output.
 */
void introduce(){
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for group" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

/*
 * Function: prompts user for text parameters
 * Usage: promptForParameters(file, N, wordNum)
 * --------------------------------
 * This function takes the parameters needed for the text and prompts the user
 * to input them. Changes the parameters directly.
 */
void promptForParameters(string& file, int& N, int& wordNum){
    file = promptUserForFile("Please enter filename containing source text: ");
    N = getInteger("Value of N? ");
    while(N < 2){
        cout << "N cannot be less than 2 - otherwise you'd be generating words at complete random!" << endl;
        N = getInteger("Value of N? ");
    }
    wordNum = getInteger("Number of random words to generate (0 to quit)? ");
    while(wordNum < N){
        cout << "Any number less than N does not make sense.";
        wordNum = getInteger("Number of random words to generate (0 to quit)? ");
    }
    cout << endl << endl;
}

/*
 * Function: input data into Markov model
 * Usage: inputData(filename, model, N);
 * --------------------------------
 * This function inputs the information from the source text into the Markov model.
 * It develops the model based on the N assigned.
 */
void inputData(string fileName, Map<Vector<string>, Vector<string>>& model, int N){

    ifstream infile;
    openFile(infile, fileName);

    string next;
    Vector<string> nextKey;     //vector to contain the N-1 window of words
    Vector<string> firstWords;  //vector to contain first N-1 words to use for wrap around of map

    while(infile >> next){
        if(nextKey.size() < N-1){      //get window of N-1 words
            nextKey.add(next);
            firstWords.add(next);
        }else{
            updateModel(model, nextKey, next);
        }
    }

    for(int i = 0; i < N-1; i++){
        next = firstWords.get(i);
        updateModel(model, nextKey, next);
    }

    infile.close();
}

/*
 * Function: updates the model based on new value
 * Usage: inputData(filename, model, N);
 * --------------------------------
 * This function updates the Markov model with the new word read from the text.
 * If the key has already been inputted, the word is added to the value vector.
 * If the key is not in the model, a new vector is made and the word is added to it.
 * That new vector is then added to the model with its corresponding key.
 */
void updateModel(Map<Vector<string>, Vector<string>>& model, Vector<string>& nextKey, string next){
    //if the map already has key, update the corresponding value vector
    if(model.containsKey(nextKey)){
        Vector<string> newValue = model.get(nextKey);
        newValue.add(next);
        model.put(nextKey, newValue);
    }else{  //if the map does not yet have the key, create a new vector with new word
        Vector<string> value;
        value.add(next);
        model.put(nextKey, value);  //add both new key and vector
    }
    nextKey.remove(0);
    nextKey.add(next);
}

/*
 * Function: generate random text from model
 * Usage: generateRandomText(model, N, wordNum)
 * --------------------------------
 * This function takes the Markov model and generates random text for a given
 * number of words based on N. It starts the text with a random key in the map.
 */
void generateRandomText(Map<Vector<string>, Vector<string>>& model, int N, int wordNum){

    Vector<string> key = getRandomKey(model);
    for(string s : key){
        cout << s << " ";
    }

    for(int i = N; i <= wordNum; i++){
        int randomInt = randomInteger(0, model.get(key).size()-1);  //get random integer within value vector's indices
        string nextWord = model.get(key).get(randomInt);    //get random string from value vector
        cout << nextWord << " ";
        key.remove(0);
        key.add(nextWord);
    }
}

/*
 * Function: get random key from map
 * Usage: generateRandomText(model, N, model)
 * --------------------------------
 * This function generates a vector of keys from the map. It then randomly
 * selects one of the keys to be used as the first key in the text.
 */
Vector<string> getRandomKey(Map<Vector<string>, Vector<string>>& model){
    Vector<Vector<string>> keys = model.keys();
    return keys.get( randomInteger(0,keys.size()-1) );
}

