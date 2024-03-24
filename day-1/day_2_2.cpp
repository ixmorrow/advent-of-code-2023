#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map>
#include <vector>

using namespace std;

struct NumberString {
    string str;
    int len;
    int value;

    // Constructor to initialize NumberString
    NumberString(const string& s, int l, int v) : str(s), len(l), value(v) {}
};


class NumberOperator {
    private:
        void construct_number_map(){
            for(int i=0; i<10; i++){
                string spelling = _number_spellings[i];
                _number_mappings[spelling[0]].push_back(NumberString(spelling, spelling.size(), i));
            }
        }
    public:
    NumberOperator(){
        _number_spellings[0] = "zero";
        _number_spellings[1] = "one";
        _number_spellings[2] = "two";
        _number_spellings[3] = "three";
        _number_spellings[4] = "four";
        _number_spellings[5] = "five";
        _number_spellings[6] = "six";
        _number_spellings[7] = "seven";
        _number_spellings[8] = "eight";
        _number_spellings[9] = "nine";

        construct_number_map();
    };

    bool is_num_str(char c){

    }

    unordered_map<char, vector<NumberString>> _number_mappings;
    unordered_map<int, string> _number_spellings;

};

void checkLine(int &total, string &line){
    bool first_digit_found = false;
    int first_digit = 0;
    int second_digit = 0;
    int current = 0;

    for (auto character : line) {
        if(isdigit(character)) {
            int digit = character - '0';
            if (!first_digit_found) {
                total += digit * 10;
                first_digit_found = true;
                first_digit = digit;
            } else second_digit = digit;
        }
    }
    if (second_digit == 0) total += first_digit;
    else total += second_digit;
}

void run(const string filename){
    NumberOperator number_operator = NumberOperator();

    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    int total = 0;

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            checkLine(total, line);
        }
        myfile.close();
    } else std::cout << "Unable to open file" << endl;

    std::cout << "Total: " << total << endl;
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