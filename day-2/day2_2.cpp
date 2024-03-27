#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unordered_map>
#include <cctype>
#include <algorithm>

using namespace std;

class Game{
    public:
        Game(unordered_map<string, int> total_cubes) : _total_cubes{total_cubes} {
            _game_total = 0;
            _power = 1;
        }

        unordered_map<string, int> _total_cubes;

        int get_game_total(){
            return _game_total;
        }

        pair<string, vector<string>> parse_game(string line){
            std::string delimiter = ":";
            std::string game_num = line.substr(0, line.find(delimiter));
            std::string results = line.substr(game_num.size()+ 1, line.size());
            std::vector<string> results_vec = split(results, ";");

            return {game_num, results_vec};
        }

        void play_game(string line){
            auto [game_num, results_vec] = parse_game(line);
            game_num = game_num.substr(5, game_num.size()-1);
            _play_game(stoi(game_num), results_vec);
        }

    private:
        int _game_total;
        int _power;

        void _play_game(
            const int game,
            const vector<string>& results_vec
        ){
            std::vector<string> temp;
            std::vector<string> item_temp;
            bool is_possible = true;

            for (auto results : results_vec){
                temp = split(results, ",");
                for(auto item : temp) {
                    item_temp = split(item, " ");
                    trim(item_temp[1]); // Trim whitespace from color string
                    if(_total_cubes.find(item_temp[1]) == _total_cubes.end()){
                        cout << "Cannot find color in map. Color: " << item_temp[1] << endl;
                    }
                    else {
                        _total_cubes[item_temp[1]] = std::max(_total_cubes[item_temp[1]], stoi(item_temp[0]));
                    }
                }
            }

            for (auto i = _total_cubes.begin(); i != _total_cubes.end(); i++) {
                _power *= i->second;
            }

            _game_total += _power;
            _power = 1;
            _total_cubes["red"] = 0;
            _total_cubes["green"] = 0;
            _total_cubes["blue"] = 0;
        }

        // Helper function to trim whitespace from a string in place
        void trim(std::string& str) {
            // Trim leading whitespace
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](unsigned char c) {
                return !std::isspace(c);
            }));

            // Trim trailing whitespace
            str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char c) {
                return !std::isspace(c);
            }).base(), str.end());
        }

        std::vector<string> split(string str,   char* delimiter) {
            std::vector<string> v;
            char *token = strtok(const_cast<char*>(str.c_str()), delimiter);
            while (token != nullptr)
            {
                v.push_back(string(token));
                token = strtok(nullptr, delimiter);
            }

            return v;
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

    // hash of max total cubes in bag
    unordered_map<string, int> total_cubes;
    total_cubes["red"] = 0;
    total_cubes["green"] = 0;
    total_cubes["blue"] = 0;

    Game game = Game(total_cubes);

    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            game.play_game(line);
        }
    }

    cout << "Game result: " << game.get_game_total() << endl;
    return 0;
}