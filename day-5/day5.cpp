#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>
#include <unordered_map>
#include <set>
#include <vector>
#include <limits>

using namespace std;

vector<long long> getSeeds(string line){
    vector<long long> seed_vals;
    std::size_t colonIndex = line.find(':');
    if (colonIndex == std::string::npos) {
        // Handle error: colon not found in the input line
        std::cerr << "Error: Colon not found in the input line." << std::endl;
        return seed_vals;  // Return an empty vector indicating an error
    }

    string seeds = line.substr(colonIndex+2);

    // constructing stream from the string
    string seed;
    stringstream ss(seeds);
    while(ss >> seed){
        try {
            // Convert the string to a long long and add to the vector
            seed_vals.push_back(std::stoll(seed));
        } catch (const std::out_of_range& e) {
            // Handle error: The number is too large
            std::cerr << "Error: Conversion failed. The number is too large." << std::endl;
            seed_vals.clear();  // Clear the vector to indicate an error
            break;
        } catch (const std::invalid_argument& e) {
            // Handle error: Invalid argument (not a valid integer)
            std::cerr << "Error: Conversion failed. Invalid argument." << std::endl;
            seed_vals.clear();  // Clear the vector to indicate an error
            break;
        }
    }

    return seed_vals;
}

// Function to read a map from the file
std::vector<std::vector<long long>> readMap(std::ifstream& inputFile) {
    std::vector<std::vector<long long>> map;
    std::string line;

    if(line.empty()) getline(inputFile, line);

    while (getline(inputFile, line) && !line.empty()) {
        std::vector<long long> values;
        std::istringstream iss(line);
        
        long long value;
        while (iss >> value) {
            values.push_back(value);
        }

        map.push_back(values);
    }

    return map;
}

long long getDestinationValue(long long src_value, vector<vector<long long>> map){
    if(map.empty()) return 0;

    long long destination = 0;

    for(auto vec : map){
        if (vec.size() >= 3){
            long long range = vec[2];
            long long sourceUpperBound = vec[1] + range;
            long long sourceLowerBound = vec[1];

            // val outside given range
            if(sourceLowerBound > src_value || src_value > sourceUpperBound){
                destination = src_value;
            } else {
                long long distance = src_value - sourceLowerBound;
                destination = vec[0] + distance;
                break;
            }
        }
    }

    return destination;
}

void run(const string filename) {
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    // get the seed values from the first line of the text file
    std::string line;
    std::getline(myfile, line);
    auto seeds = getSeeds(line);

    // Define vectors to store the data
    std::vector<std::vector<long long>> seedToSoilMap;
    std::vector<std::vector<long long>> soilToFertilizerMap;
    std::vector<std::vector<long long>> fertilizerToWaterMap;
    std::vector<std::vector<long long>> waterToLightMap;
    std::vector<std::vector<long long>> lightToTemperatureMap;
    std::vector<std::vector<long long>> temperatureToHumidityMap;
    std::vector<std::vector<long long>> humidityToLocationMap;

    seedToSoilMap = readMap(myfile);
    soilToFertilizerMap = readMap(myfile);
    fertilizerToWaterMap = readMap(myfile);
    waterToLightMap = readMap(myfile);
    lightToTemperatureMap = readMap(myfile);
    temperatureToHumidityMap = readMap(myfile);
    humidityToLocationMap = readMap(myfile);

    long long lowestLocation = std::numeric_limits<long long>::max(); // Initialize to maximum possible int value;

    for(auto seed : seeds){
        long long soilValue = getDestinationValue(seed, seedToSoilMap);

        long long fertilizerValue = getDestinationValue(soilValue, soilToFertilizerMap);

        long long waterValue = getDestinationValue(fertilizerValue, fertilizerToWaterMap);

        long long lightValue = getDestinationValue(waterValue, waterToLightMap);

        long long tempValue = getDestinationValue(lightValue, lightToTemperatureMap);

        long long humidityValue = getDestinationValue(tempValue, temperatureToHumidityMap);

        long long location = getDestinationValue(humidityValue, humidityToLocationMap);

        lowestLocation = min(location, lowestLocation);
    }

    cout << "Lowest Location: " << lowestLocation << endl;
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