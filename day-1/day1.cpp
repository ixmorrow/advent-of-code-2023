#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

using namespace std;

void run(const string filename){
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    int total = 0;
    bool first_digit_found = false;
    int first_digit = 0;
    int second_digit = 0;
    int current = 0;

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
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

            first_digit_found = false;
            first_digit = 0;
            second_digit = 0;
            current = 0;
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