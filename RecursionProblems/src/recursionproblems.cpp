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

int convertStringToIntegerHelper(string exp);
bool isBalancedHelper(string exp, string target);
void drawSierpinskiTriangleHelper(GWindow& gw, double x, double y, int size, int order);
Map<string, Vector<string>> inputGrammar(istream& input);
string expandGrammar(Map<string, Vector<string>>& grammar, string key);
string expandGrammarHelper(Map<string, Vector<string>>& grammar, string);

int countKarelPaths(int street, int avenue) {
    // TODO: write this function
    return 0;
}

int convertStringToInteger(string exp) {
    if(exp.at(0) == '-'){
        return -1 * convertStringToIntegerHelper(exp.substr(1));
    }else{
        return convertStringToIntegerHelper(exp);
    }
}

int convertStringToIntegerHelper(string exp){
    if(exp.length() == 0){
        return 0;
    }else{
        string numbers = "0123456789";
        char lastNumStr = exp.at(exp.length()-1);
        int lastNum = numbers.find(lastNumStr);
        if(lastNum != string::npos){
            exp.erase(exp.length()-1, 1);
            return lastNum + 10*convertStringToIntegerHelper(exp);
        }
        else{
            throw string("exception ! error");
        }
    }
}

bool isBalanced(string exp) {
    if(exp.length() == 0){
        return true;
    } else {
        return isBalancedHelper(exp, "()") || isBalancedHelper(exp, "[]")
                || isBalancedHelper(exp, "<>") || isBalancedHelper(exp, "{}");
        }
    }

bool isBalancedHelper(string exp, string target){
    int index = exp.find(target);
    if(index != string::npos){
        exp.erase(index, target.length());
        return isBalanced(exp);
    } else {
        return false;
    }
}

double weightOnKnees(int row, int col, Vector<Vector<double>>& weights) {
    if(row < 0 || col < 0 || col > weights.get(row).size() - 1){
        return 0.0;
    } else {
        return weights[row][col] + 0.5 * weightOnKnees(row - 1, col - 1, weights) + 0.5 * weightOnKnees(row - 1, col, weights);
    }
}

void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    gw.drawLine(x, y, x + size, y);
    gw.drawLine(x, y, x + size/2, y + size*sqrt(3)/2);
    gw.drawLine(x + size, y, x + size/2, y + size*sqrt(3)/2);
    drawSierpinskiTriangleHelper(gw, x + size/4, y + size*sqrt(3)/4, size/2, order - 1);
}

void drawSierpinskiTriangleHelper(GWindow& gw, double x, double y, int size, int order){
   if(order > 0)
        gw.drawLine(x, y, x + size, y);
        gw.drawLine(x, y, x + size/2, y - size*sqrt(3)/2);
        gw.drawLine(x + size, y, x + size/2, y - size*sqrt(3)/2);
        drawSierpinskiTriangleHelper(gw, x - size/4, y - size*sqrt(3)/4, size/2, order-1);
        drawSierpinskiTriangleHelper(gw, x + 3*size/4, y - size*sqrt(3)/4, size/2, order-1);
        drawSierpinskiTriangleHelper(gw, x + size/4, y + size*sqrt(3)/4, size/2, order-1);
    }
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
