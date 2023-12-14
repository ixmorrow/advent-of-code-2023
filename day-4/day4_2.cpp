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

    unordered_map<int, int> points_per_game;
    unordered_map<int, int> game_multiples;
    set<int> winning_numbers_set;
    int total_score = 0;
    int game_count = 1;

    string line;
    while(getline(myfile, line)) {
        int n = line.size();
        int colonIndex = line.find(':');
        int pipeDelimiter = line.find('|');
        int game_number = 0;

        string game = line.substr(0, colonIndex);
        string winning_numbers = line.substr(colonIndex+1, pipeDelimiter-colonIndex-1);
        string scratch_numbers = line.substr(pipeDelimiter+1, n);

        // add winning numbers to a set
        stringstream gs(game);
        string game_number_str;
        while(getline(gs, game_number_str, ' ')){
            // Process num only if it's not whitespace
            if (!game_number_str.empty() && game_number_str.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                game_number = stoi(game_number_str);
                points_per_game[game_number] = 0;
            }
        }
        
        // Check if the string starts with "Card" and the rest is a valid integer
        // if (game.substr(0, 4) == "Card" && game.size() > 4 &&
        //     game.find_first_not_of("0123456789", 4) == std::string::npos) {
        //     try {
        //         game_number = std::stoi(game.substr(4));
        //         std::cout << "Adding: " << game << std::endl;
        //         // Process card_number as needed
        //         points_per_game[game_number] = 0;
        //     } catch (const std::exception& e) {
        //         // Handle stoi exception (e.g., if the conversion fails)
        //         std::cerr << "Error converting to integer: " << e.what() << std::endl;
        //     }
        // } else {
        //     std::cout << "Invalid format: " << game << std::endl;
        // }

        // add winning numbers to a set
        stringstream ss(winning_numbers);
        string num;
        while(getline(ss, num, ' ')){
            // Process num only if it's not whitespace
            if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                winning_numbers_set.insert(stoi(num));
            }
        }

        // constructing stream from the string
        stringstream ss2(scratch_numbers);
        int game_score = 0;

        while(getline(ss2, num, ' ')){
            if (!num.empty() && num.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                int scratch_num = stoi(num);
                if(winning_numbers_set.find(scratch_num) != winning_numbers_set.end()){
                    game_score += 1;
                }
            }

            for(int i = 1; i <= game_score; i++) {
                game_multiples[i+game_number] += 1;
            }
        }

        points_per_game[game_number] = game_score;
        winning_numbers_set.clear();
    }

    for(int i = 1; i < points_per_game.size(); i++){
        total_score += points_per_game[i] * game_multiples[i];
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