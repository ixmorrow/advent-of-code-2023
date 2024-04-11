#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct Node {
        string current_node;
        string left_node;
        string right_node;
};

class Map {
    public:
        Map(const string filename) {
            _build_map(filename);
        };

        vector<char> directions;
        unordered_map<string, Node> nodes;

        void print_nodes(){
            for(const auto& pair : nodes){
                cout << pair.first << ": "<< "(" << pair.second.left_node << " " << pair.second.right_node << ")" << endl;
            }
        }

        int traverse_map(){
            int steps = 0;
            Node current_node = nodes["AAA"];
            
            while(current_node.current_node != "ZZZ"){
                int index = steps%directions.size();
                if(directions[index] == 'L'){
                    current_node = nodes[current_node.left_node];
                } else {
                    current_node = nodes[current_node.right_node];
                }
                steps++;
            }

            return steps;
        }

    private:
        void _build_map(const string filename){
            // read file
            ifstream myfile(filename);

            if (!myfile.is_open()) {
                cerr << "Error: Unable to open file '" << filename << "'" << endl;
                throw "Unable to opepn file!";
            }

            string line;
            getline(myfile, line);
            
            for (char c : line) {
                if (!std::isspace(c)) {
                    directions.push_back(c);
                }
            }

            while(getline(myfile, line)){
                vector<string> node_strings = extractStrings(line);
                Node node;
                if (node_strings.size() == 3 && (nodes.find(node_strings[0]) == nodes.end())) {
                    node.current_node = node_strings[0];
                    node.left_node = node_strings[1];
                    node.right_node = node_strings[2];
                    nodes[node.current_node] = node;
                }
            }
        };

        // Function to extract strings of letters from a line
        std::vector<std::string> extractStrings(const std::string& line) {
            std::vector<std::string> strings;
            std::stringstream ss(line);
            std::string word;
            while (ss >> word) {
                std::string str;
                for (char c : word) {
                    if (isalpha(c)) {
                        str += c;
                    }
                }
                if (!str.empty()) {
                    strings.push_back(str);
                }
            }
            return strings;
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

    Map map = Map(filename);
    int steps = map.traverse_map();
    cout << "Steps: " << steps << endl;

    return 0;
}