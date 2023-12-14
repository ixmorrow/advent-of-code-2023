#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map>
#include <set>

using namespace std;

void run(const string filename){
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    int sum = 0;
    unordered_map<int, int> prevLineInts;
    unordered_map<int, int> currentLineInts;
    set<int> prevLineSymbols;
    set<int> currentLineSymbols;

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            int n = line.size();
            for (int index = 0; index < n; index++) {
                // if symbol exists at index on prev line and if num exists at prev line index + 1
                if(prevLineSymbols.find(index) != prevLineSymbols.end() && prevLineInts.find(index) != prevLineInts.end()){
                    sum += prevLineInts[index];
                    prevLineInts.erase(index);
                }
                // if current char is an integer
                if(isdigit(line[index])) {
                    string number(1, line[index]);
                    // sliding window
                    int rightIndex = index + 1;
                    int startindex = index;
                    while (isdigit(line[rightIndex]) && rightIndex < n) {
                        number += line[rightIndex];
                        index = rightIndex;
                        rightIndex++;
                    }
                    // store index of start of num and end of num in map
                    currentLineInts[index] = stoi(number);
                    currentLineInts[startindex] = stoi(number);
                    
                    // check if there are any symbols at prev line at this index or prev/next index that have not been added to sum yet
                    if(prevLineSymbols.find(index-1) != prevLineSymbols.end()) {
                        sum += currentLineInts[index];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(index);
                        currentLineInts.erase(startindex);
                        prevLineSymbols.erase(index-1);
                    } else if (prevLineSymbols.find(index) != prevLineSymbols.end()) {
                        sum += currentLineInts[index];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(index);
                        currentLineInts.erase(startindex);
                        prevLineSymbols.erase(index);
                    } else if (prevLineSymbols.find(index+1) != prevLineSymbols.end()) {
                        sum += currentLineInts[index];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(index);
                        currentLineInts.erase(startindex);
                        prevLineSymbols.erase(index+1);
                    }
                    // check if there are any symbols at prev line at the start index of this num or prev/next index
                    if(prevLineSymbols.find(startindex-1) != prevLineSymbols.end()) {
                        sum += currentLineInts[startindex];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(startindex);
                        currentLineInts.erase(index);
                        prevLineSymbols.erase(startindex-1);                      
                    } else if (prevLineSymbols.find(startindex) != prevLineSymbols.end()) {
                        sum += currentLineInts[startindex];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(startindex);
                        currentLineInts.erase(index);
                        prevLineSymbols.erase(startindex);
                    } else if (prevLineSymbols.find(startindex+1) != prevLineSymbols.end()) {
                        sum += currentLineInts[startindex];
                        // remove elements from map to avoid adding again
                        currentLineInts.erase(startindex);
                        currentLineInts.erase(index);
                        prevLineSymbols.erase(startindex+1);
                    }
                }
                // symbol reached, check for ints at prev index and indices of prev line
                else if (line[index] != '.') {
                    // insert index to map of symbol indices
                    currentLineSymbols.insert(index);
                    // check current line for int at prev indices
                    if (currentLineInts.find(index-1) != currentLineInts.end()) {
                        sum += currentLineInts[index-1];
                        // cout << "Adding int to sum: " << currentLineInts[index-1] << endl;
                        currentLineInts.erase(index-1);
                    }
                    // check prev line for ints at indices
                    if (prevLineInts.find(index-1) != prevLineInts.end()) {
                        sum += prevLineInts[index-1];
                        // cout << "Adding int from prev line to sum: " << prevLineInts[index-1] << endl;
                        prevLineInts.erase(index-1);
                    }
                    if (prevLineInts.find(index) != prevLineInts.end()) {
                        sum += prevLineInts[index];
                        // cout << "Adding int from prev line to sum: " << prevLineInts[index] << endl;
                        prevLineInts.erase(index);
                    }
                    if (prevLineInts.find(index+1) != prevLineInts.end()) {
                        sum += prevLineInts[index+1];
                        // cout << "Adding int from prev line to sum: " << prevLineInts[index+1] << endl;
                        prevLineInts.erase(index+1);
                    }
                }
            }
            prevLineInts = currentLineInts;
            prevLineSymbols = currentLineSymbols;
            currentLineInts.clear();
            currentLineSymbols.clear();
        }
    }

    cout << "Sum: " << sum << endl;
}

int main() {
     // read file
    const string filename = "./input.txt";

    try{
        run(filename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}