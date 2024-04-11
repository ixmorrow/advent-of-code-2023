#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

class PipeMaze{
    public:
        PipeMaze(const string filename){
            _build_matrix(filename);
        }
        // Initialize the 2D vector with specific dimensions and initial value
        std::vector<std::vector<char>> matrix;
        pair<int, int> starting_coordinate;
        int furthest_distance = 0;

        int calc_distance(){
            _bfs_calc_distances();
            return furthest_distance;
        }

    private:
        int _rows = 0;
        int _cols = 0;

        void _build_matrix(const string filename){
            // read file
            ifstream myfile(filename);

            if (!myfile.is_open()) {
                cerr << "Error: Unable to open file '" << filename << "'" << endl;
                throw "Unable to opepn file!";
            }

            int row_num = 0;
            std::string line;
            while(getline(myfile, line)){
                vector<char> row;
                for(int i=0; i < line.size(); i++){
                    row.push_back(line[i]);
                    if(line[i] == 'S') {
                        starting_coordinate = {row_num, i};
                    }
                }
                matrix.push_back(row);
                row_num++;
            }

            _rows = matrix.size();
            _cols = matrix[0].size();
        }

        // calc distances using BFS
        void _bfs_calc_distances(){
            vector<vector<bool>> visited(_rows, vector<bool>(_cols, false));

            queue<pair<pair<int, int>, int>> q;

            // Mark the starting point as visited and enqueue it with distance 0
            visited[starting_coordinate.first][starting_coordinate.second] = true;
            q.push({starting_coordinate, 0});

            // Perform BFS
            while (!q.empty()){
                // Dequeue a cell
                pair<pair<int, int>, int> cell = q.front();
                q.pop();
                int row = cell.first.first;
                int col = cell.first.second;
                int dist = cell.second;

                furthest_distance = max(furthest_distance, dist);

                // Print the distance from the starting point to the current cell
                cout << "Distance from (" << starting_coordinate.first << ", " << starting_coordinate.second << ") to (" << row << ", " << col << "): " << dist << endl;

                vector<pair<int, int>> potential_next_steps = _get_potential_coordinates(row, col);
                for (int i = 0; i < potential_next_steps.size(); i++) {
                    int newRow = potential_next_steps[i].first;
                    int newCol = potential_next_steps[i].second;

                    // Check if the new cell is within the grid and not visited yet
                    if (_isValid(newRow, newCol) && !visited[newRow][newCol] && _is_connected({row, col}, {newRow, newCol})) {
                        // Mark the new cell as visited and enqueue it with distance incremented by 1
                        visited[newRow][newCol] = true;
                        q.push({{newRow, newCol}, dist + 1});
                    }
                }
            }
        }

        vector<pair<int, int>> _get_potential_coordinates(int row, int col){
            vector<pair<int,int>> potential_coordinates;
            switch(matrix[row][col]){
                case '|':
                    potential_coordinates.push_back({row+1, col});
                    potential_coordinates.push_back({row-1, col});
                    break;
                case '-':
                    potential_coordinates.push_back({row, col+1});
                    potential_coordinates.push_back({row, col-1});
                    break;
                case 'L':
                    potential_coordinates.push_back({row+1, col});
                    potential_coordinates.push_back({row, col+1});
                    break;
                case 'J':
                    potential_coordinates.push_back({row+1, col});
                    potential_coordinates.push_back({row, col-1});
                    break;
                case '7':
                    potential_coordinates.push_back({row-1, col});
                    potential_coordinates.push_back({row, col-1});
                case 'F':
                    potential_coordinates.push_back({row-1, col});
                    potential_coordinates.push_back({row, col+1});
                    break;
                case 'S':
                    potential_coordinates.push_back({row, col+1});
                    potential_coordinates.push_back({row+1, col});
                    potential_coordinates.push_back({row-1, col});
                    potential_coordinates.push_back({row, col+1});
                    break;
            }

            return potential_coordinates;
        }

        bool _is_connected(pair<int, int> a, pair<int, int> b){
            // checks if a is connected to b, looks at point b
            switch(matrix[b.first][b.second]){
                case '|':
                    return (abs(a.first - b.first) == 1);
                    break;
                case '-':
                    return (abs(a.second - b.second) == 1);
                    break;
                case 'L':
                    return a.first + 1 == b.first || a.second - 1 == b.second;
                    break;
                case 'J':
                    return a.first - 1 == b.first || a.second + 1 == b.second;
                    break;
                case '7':
                    return a.first + 1 == b.first || a.second + 1 == b.second;
                    break;
                case 'F':
                    return a.first + 1 == b.first || a.second - 1 == b.second;
                    break;
                default:
                    return false;
                    break;
            }
        }

        bool _isValid(int row, int col){
            if(matrix[row][col]=='.'){
                return false;
            } else {
                return (row >= 0) && (row <= _rows) && (col >= 0) && (col <= _cols);
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

    PipeMaze pipe_maze = PipeMaze(filename);
    try{
        cout << "Largest distance: " << pipe_maze.calc_distance() << endl;
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}