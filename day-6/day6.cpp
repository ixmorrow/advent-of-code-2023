#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <vector>

using namespace std;

vector<int> getData(string line){
    vector<int> data;
    std::size_t colonIndex = line.find(':');
    if (colonIndex == std::string::npos) {
        // Handle error: colon not found in the input line
        std::cerr << "Error: Colon not found in the input line." << std::endl;
        return data;  // Return an empty vector indicating an error
    }

    string line_of_data = line.substr(colonIndex+2);

    // constructing stream from the string
    string data_point;
    stringstream ss(line_of_data);
    while(ss >> data_point){
        try {
            data.push_back(std::stoi(data_point));
        } catch (const std::out_of_range& e) {
            // Handle error: The number is too large
            std::cerr << "Error: Conversion failed. The number is too large." << std::endl;
            data.clear();  // Clear the vector to indicate an error
            break;
        } catch (const std::invalid_argument& e) {
            // Handle error: Invalid argument (not a valid integer)
            std::cerr << "Error: Conversion failed. Invalid argument." << std::endl;
            data.clear();  // Clear the vector to indicate an error
            break;
        }
    }

    return data;
}

int waysToWinRace(int timeInMs, int longestDistanceInMm){
    int chargeTime = 0;
    int speed = 0;
    int winCount = 0;

    while(chargeTime < timeInMs){
        chargeTime++;
        speed++;
        int timeLeft = timeInMs - chargeTime;
        int travelDistance = speed * timeLeft;
        // cout << "Travel Distance: " << travelDistance << endl;
        // cout << "Longest Distance: " << longestDistanceInMm << endl;
        if(travelDistance > longestDistanceInMm) winCount++;
    }

    return winCount;
}

void run(const string filename) {
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    // get the seed values from the first two lines of the text file
    std::string line;
    std::getline(myfile, line);
    auto times = getData(line);
    std::getline(myfile, line);
    auto distances = getData(line);

    vector<int> winPossibilities;

    for(int i = 0; i < times.size(); i++){
        int waysToWin = waysToWinRace(times[i], distances[i]);
        winPossibilities.push_back(waysToWin);
    }

    int marginOfError = 1;
    for(auto count : winPossibilities){
        // cout << "Count: " << count << endl;
        marginOfError *= count;
    }

    cout << "Margin of Error: " << marginOfError << endl;
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