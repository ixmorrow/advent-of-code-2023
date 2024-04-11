#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>

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
        unordered_set<string> starting_nodes;
        int starting_points = 0;

        void print_nodes(){
            for(const auto& pair : nodes){
                cout << pair.first << ": "<< "(" << pair.second.left_node << " " << pair.second.right_node << ")" << endl;
            }
        }

        int traverse_map() {
            std::queue<Node> q;
            for (auto node : starting_nodes) {
                q.push(nodes[node]);
            }

            int steps = 0;

            while (!q.empty()) {
                int queue_size = q.size(); // Number of nodes at current level
                // Track whether all nodes reach the end node with name ending in 'Z' in this step
                bool all_end_in_Z_this_step = true;

                for (int i = 0; i < queue_size; i++) {
                    Node current_node = q.front();
                    q.pop();

                    // Check if the current node ends in 'Z'
                    if (current_node.current_node.back() != 'Z') {
                        all_end_in_Z_this_step = false;
                    }

                    // Process left and right nodes
                    if (!current_node.left_node.empty()) {
                        q.push(nodes[current_node.left_node]);
                    }
                    if (!current_node.right_node.empty()) {
                        q.push(nodes[current_node.right_node]);
                    }
                }

                // Check if all nodes reach the end node with name ending in 'Z' in this step
                if (all_end_in_Z_this_step) {
                    return steps; // All nodes reach the end node with name ending in 'Z' in the same level
                }
                steps++; // Increment the number of steps
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
                // vector<string> node_strings = extractStrings(line);
                auto pair = extractNodeAndStrings(line);
                string node_identifier = pair.first;
                vector<string> node_strings = pair.second;
                Node node;
                if (node_strings.size() == 2 && (nodes.find(node_identifier) == nodes.end())) {
                    node.current_node = node_identifier;
                    node.left_node = node_strings[0];
                    node.right_node = node_strings[1];
                    nodes[node.current_node] = node;

                    // if node ends in A, add to set of starting nodes
                    if(node.current_node.back() == 'A'){
                        cout << "Found starting node: " << node.current_node << endl;
                        starting_nodes.insert(node.current_node);
                        starting_points+=1;
                    }
                }
            }
        };

        // Function to extract node identifier and strings of letters from a line
        std::pair<std::string, std::vector<std::string>> extractNodeAndStrings(const std::string& line) {
            std::pair<std::string, std::vector<std::string>> result;
            std::stringstream ss(line);
            std::string node_identifier;
            std::vector<std::string> strings;
            ss >> node_identifier; // Extract node identifier
            while (ss >> std::ws) {
                std::string word;
                ss >> word;
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
            result.first = node_identifier;
            result.second = strings;
            return result;
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