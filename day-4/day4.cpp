#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map>
#include <set>

using namespace std;

class ScratchCard {
    public:
        ScratchCard() {}

        string _scratch_numbers;
        string _winning_numbers;
        set<int> _winning_numbers_set;

        int get_total_score(){
            return _total_score;
        }
        void play_scratch_cards(string line){
            _parse_scratch_card(line);

            // constructing stream from the string
            stringstream ss2(_scratch_numbers);
            string num;
            bool shouldDouble = false;
            int game_score = 0;

            while(getline(ss2, num, ' ')){
                if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                    int scratch_num = stoi(num);
                    if(_winning_numbers_set.find(scratch_num) != _winning_numbers_set.end()){
                        if(shouldDouble) game_score *= 2;
                        else {
                            game_score += 1;
                            shouldDouble = true;
                        }
                    }
                }
            }

            _total_score += game_score;
            _winning_numbers_set.clear();
        }
    
    private:
        int _total_score = 0;

        void _parse_scratch_card(string line){
            int n = line.size();
            int colonIndex = line.find(':');
            int pipeDelimiter = line.find('|');

            _winning_numbers = line.substr(colonIndex+1, pipeDelimiter-colonIndex-1);
            _scratch_numbers = line.substr(pipeDelimiter+1, n);

            // constructing stream from the string
            stringstream ss(_winning_numbers);
            string num;

            while(getline(ss, num, ' ')){
                // Process num only if it's not whitespace
                if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                    _winning_numbers_set.insert(stoi(num));
                }
            }
        }
};

int main(int argc, char *argv[]) {
    // read file
    string filename;
    if (argc != 2) {
        cout << "Filename not provided. Using default file input.txt..." << endl;
        filename = "input.txt";
    } else {
        filename = argv[1];
    }
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        return 1;
    }

    ScratchCard scratch_card_game = ScratchCard();

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            scratch_card_game.play_scratch_cards(line);
        }
    }

    cout << scratch_card_game.get_total_score() << endl;

    return 0;
}