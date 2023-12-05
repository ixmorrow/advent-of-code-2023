#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map>
#include <set>

using namespace std;

void run(const string filename) {
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    set<int> winning_numbers_count;
    int total_score = 0;

    string line;
    while(getline(myfile, line)) {
        int n = line.size();
        int colonIndex = line.find(':');
        int pipeDelimiter = line.find('|');

        string winning_numbers = line.substr(colonIndex+1, pipeDelimiter-colonIndex-1);
        string scratch_numbers = line.substr(pipeDelimiter+1, n);
        
        // constructing stream from the string
        stringstream ss(winning_numbers);
        string num;

        while(getline(ss, num, ' ')){
            // Process num only if it's not whitespace
            if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                winning_numbers_count.insert(stoi(num));
            }
        }

        // constructing stream from the string
        stringstream ss2(scratch_numbers);
        bool shouldDouble = false;
        int game_score = 0;

        while(getline(ss2, num, ' ')){
            if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                int scratch_num = stoi(num);
                if(winning_numbers_count.find(scratch_num) != winning_numbers_count.end()){
                    if(shouldDouble) game_score *= 2;
                    else {
                        game_score += 1;
                        shouldDouble = true;
                    }
                }
            }
        }

        total_score += game_score;
        winning_numbers_count.clear();
    }

    cout << total_score << endl;
}

int main() {
    const string filename = "./input.txt";

    try{
        run(filename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}