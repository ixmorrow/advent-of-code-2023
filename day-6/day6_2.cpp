#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;

long long getData(string line){
    std::size_t colonIndex = line.find(':');
    if (colonIndex == std::string::npos) {
        // Handle error: colon not found in the input line
        std::cerr << "Error: Colon not found in the input line." << std::endl;
        return 0;
    }

    string line_of_data = line.substr(colonIndex+2);

    // constructing stream from the string
    string data_point;
    string result;
    stringstream ss(line_of_data);
    while(ss >> data_point){
        try {
            result += data_point;
        } catch (const std::out_of_range& e) {
            // Handle error: The number is too large
            std::cerr << "Error: Conversion failed. The number is too large." << std::endl;
            break;
        } catch (const std::invalid_argument& e) {
            // Handle error: Invalid argument (not a valid integer)
            std::cerr << "Error: Conversion failed. Invalid argument." << std::endl;
            break;
        }
    }

    
    return stoll(result);
}

int waysToWinRace(long long timeInMs, long long longestDistanceInMm){
    long long chargeTime = 0;
    long long speed = 0;
    int winCount = 0;

    while(chargeTime < timeInMs){
        chargeTime++;
        speed++;
        long long timeLeft = timeInMs - chargeTime;
        long long travelDistance = speed * timeLeft;
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
    auto time = getData(line);
    std::getline(myfile, line);
    auto distance = getData(line);

    int winPossibilities = waysToWinRace(time, distance);


    cout << "Win Possibilities: " << winPossibilities << endl;
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