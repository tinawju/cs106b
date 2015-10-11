/*
 * File: recursionproblems.cpp
 * --------------------
 * Name: Tina Ju && Elena Portz
 * Section: Chip Nguyen
 * Hella recursion, all day every day. 
 */

// Please feel free to add any other #includes you need!
#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
using namespace std;

Map<string, Vector<string>> inputGrammar(istream& input);
string expandGrammar(Map<string, Vector<string>>& grammar, string key);
string expandGrammarHelper(Map<string, Vector<string>>& grammar, string);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

int convertStringToInteger(string exp) {
    // TODO: write this function
    return 0;
}

bool isBalanced(string exp) {
    // TODO: write this function
    return false;
}

double weightOnKnees(int row, int col, Vector<Vector<double> >& weights) {
    // TODO: write this function
    return 0.0;
}

void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    // TODO: write this function

}

int floodFill(GBufferedImage& image, int x, int y, int color) {
    // TODO: write this function
    return 0;
}

Vector<string> grammarGenerate(istream& input, string symbol, int times) {
    Map<string, Vector<string>> grammar = inputGrammar(input);
    Vector<string> randomGrammars;

    for(int i = 0; i < times; i++){
        if(!grammar.containsKey(symbol)){
            randomGrammars.add(symbol);
        } else {
        randomGrammars.add(expandGrammar(grammar, symbol));
        }
    }
    return randomGrammars;
}

Map<string, Vector<string>> inputGrammar(istream& input){
    Map<string, Vector<string>> grammar;
    string line;
    while(getline(input, line)){
        Vector<string> lineVect = stringSplit(line, "::=");
        Vector<string> valueVect = stringSplit(lineVect.get(1), "|");
        grammar.put(lineVect.get(0), valueVect);
    }
    return grammar;
}

string expandGrammar(Map<string, Vector<string>>& grammar, string key){
    Vector<string> rules = grammar.get(key);
    string next = rules.get(randomInteger(0,rules.size()-1));

    Vector<string> rulesVect = stringSplit(next, " ");
    string expanded;

    for(string str: rulesVect){
    expanded += expandGrammarHelper(grammar, str);
    }

    return expanded;
}

string expandGrammarHelper(Map<string, Vector<string>>& grammar, string next){
    if(grammar.containsKey(next)){
        return expandGrammar(grammar, next);
    } else {
        return next + " ";
    }
}
