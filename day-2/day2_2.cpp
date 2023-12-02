#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;

std::vector<string> split(string str,   char* delimiter)
{
    std::vector<string> v;
    char *token = strtok(const_cast<char*>(str.c_str()), delimiter);
    while (token != nullptr)
    {
        v.push_back(string(token));
        token = strtok(nullptr, delimiter);
    }

    return v;
}

int main() {
    // read file
    const string filename = "./input.txt";
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

    int power = 1;
    int game_total = 0;


    string line;
    if (myfile.is_open()) {
        while(getline(myfile, line)) {
            std::string delimiter = ":";
            std::string game_num = line.substr(0, line.find(delimiter));
            

            std::string results = line.substr(game_num.size()+ 1, line.size());
            std::vector<string> results_vec = split(results, ";");
            std::vector<string> temp;
            std::vector<string> item_temp;

            game_num = game_num.substr(5, game_num.size()-1);
            int game = stoi(game_num);
            // cout << "Game: " << game << endl;

            for (auto results : results_vec){
                temp = split(results, ",");
                for(auto item : temp) {
                    item_temp = split(item, " ");
                    total_cubes[item_temp[1]] = std::max(total_cubes[item_temp[1]], stoi(item_temp[0]));
                }
            }

            for (auto i = total_cubes.begin(); i != total_cubes.end(); i++) {
                power *= i->second;
            }

            game_total += power;
            power = 1;
            total_cubes["red"] = 0;
            total_cubes["green"] = 0;
            total_cubes["blue"] = 0;
        }
    }

    cout << game_total << endl;
    return 0;
}