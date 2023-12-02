#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map> 

using namespace std;

int main() {
    // read file
    const string filename = "./input.txt";
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return 1;
    }

    int total = 0;
    bool first_digit_found = false;
    int first_digit = 0;
    int second_digit = 0;

    // hash of spelling to int
    unordered_map<string, int> number_spellings;
    number_spellings["one"] = 1;
    number_spellings["two"] = 2;
    number_spellings["three"] = 3;
    number_spellings["four"] = 4;
    number_spellings["five"] = 5;
    number_spellings["six"] = 6;
    number_spellings["seven"] = 7;
    number_spellings["eight"] = 8;
    number_spellings["nine"] = 9;
    number_spellings["ten"] = 10;

    // init range for sliding window
    int left = 0;

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            for (int right = 1; right < line.size(); right++) {
                // while right != digit AND right - left > 1
                while (!isdigit(line[left]) && !isdigit(line[right]) && right - left >= 1 && right < line.size()) {
                    auto substr = line.substr(left, right-left+1);
                    // cout << "Current substring: " << substr << endl;
                    // if substring is in hash map, then it's a number
                    if (number_spellings.find(substr) != number_spellings.end()) {
                        // cout << "Found number: " << substr << endl;
                        if (!first_digit_found) {
                            // cout << "First digit found: " << number_spellings[substr] * 10 << endl;
                            total += number_spellings[substr] * 10;
                            first_digit_found = true;
                            left = right+1;
                            right+=2;
                            // cout << "Running total: " << total << endl;
                        }
                        else {
                            second_digit = number_spellings[substr];
                            // cout << "Potential Second Digit Found: " << second_digit << endl;
                            left = right;
                            right++;
                        }
                    }
                    // move sliding window right if we have not reached maxlength
                    else if (right - left < 5) {
                        right++;
                    } 
                    // move sliding window to left one and bring right side two away from left
                    else {
                        left++;
                        right = left + 1;
                    }
                }
                if (isdigit(line[left]) && !first_digit_found) {
                        int digit = line[left] - '0';

                        total += digit * 10;
                        first_digit_found = true;
                        first_digit = digit;
                        left++;
                        right = left + 1;
                }
                else if (isdigit(line[right]) && right - left <= 2) {
                    int digit = line[right] - '0';

                    if (!first_digit_found) {
                        total += digit * 10;
                        first_digit_found = true;
                        first_digit = digit;
                        left = right + 1;
                        right += 2;
                    }
                    else {
                        // cout << "Potential Second Digit Found: " << digit << endl;
                        second_digit = digit;
                        left = right + 1;
                        right++;
                    }
                } 
                else {
                    left++;
                    right = left + 1;
                }
            }
            if (second_digit == 0) total += first_digit;
            else total += second_digit;

            // cout << "Running total: " << total << endl;

            first_digit_found = false;
            first_digit = 0;
            second_digit = 0;
            left = 0;
        }
        myfile.close();
    } else std::cout << "Unable to open file" << endl;

    std::cout << "Total: " << total << endl;
    return 0;
}