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

class SeedMapper{
    public:
        SeedMapper(const std::string& myfile) {
            _parseFile(myfile);
            _lowest_location = std::numeric_limits<long long>::max(); // Initialize to maximum possible int value;
        }

        // Define vectors to store the data
        std::vector<std::vector<long long>> _seedToSoilMap;
        std::vector<std::vector<long long>> _soilToFertilizerMap;
        std::vector<std::vector<long long>> _fertilizerToWaterMap;
        std::vector<std::vector<long long>> _waterToLightMap;
        std::vector<std::vector<long long>> _lightToTemperatureMap;
        std::vector<std::vector<long long>> _temperatureToHumidityMap;
        std::vector<std::vector<long long>> _humidityToLocationMap;

        vector<long long> get_seeds(){
            return _seed_vals;
        }

        long long get_lowest_destination_value(){
            return _lowest_location;
        }

        void calculate_lowest_location(){
            for(auto seed : _seed_vals){
                cout << "Seed: " << seed << endl;
                long long soilValue = _getDestinationValue(seed, _seedToSoilMap);
                long long fertilizerValue = _getDestinationValue(soilValue, _soilToFertilizerMap);
                long long waterValue = _getDestinationValue(fertilizerValue, _fertilizerToWaterMap);
                long long lightValue = _getDestinationValue(waterValue, _waterToLightMap);
                long long tempValue = _getDestinationValue(lightValue, _lightToTemperatureMap);
                long long humidityValue = _getDestinationValue(tempValue, _temperatureToHumidityMap);
                long long location = _getDestinationValue(humidityValue, _humidityToLocationMap);

                _lowest_location = min(location, _lowest_location);
            }
        }

    private:
        vector<long long> _seed_vals;
        long long _lowest_location;

        
        void _getSeeds(string line){
            std::size_t colonIndex = line.find(':');
            if (colonIndex == std::string::npos) {
                // Handle error: colon not found in the input line
                std::cerr << "Error: Colon not found in the input line." << std::endl;
            }

            string seeds = line.substr(colonIndex+2);
            // constructing stream from the string
            string seed;
            stringstream ss(seeds);
            while(ss >> seed){
                try {
                    // Convert the string to a long long and add to the vector
                    _seed_vals.push_back(std::stoll(seed));
                } catch (const std::out_of_range& e) {
                    // Handle error: The number is too large
                    std::cerr << "Error: Conversion failed. The number is too large." << std::endl;
                    _seed_vals.clear();  // Clear the vector to indicate an error
                    break;
                } catch (const std::invalid_argument& e) {
                    // Handle error: Invalid argument (not a valid integer)
                    std::cerr << "Error: Conversion failed. Invalid argument." << std::endl;
                    _seed_vals.clear();  // Clear the vector to indicate an error
                    break;
                }
            }
        }
        
        void _parseFile(const std::string& filename) {
            std::ifstream inputFile(filename);
            if (!inputFile.is_open()) {
                std::cerr << "Error: Unable to open file '" << filename << "'" << std::endl;
                return;
            }

            std::string line;

            // Parse seeds section
            if (!std::getline(inputFile, line)) {
                std::cerr << "Error: Unable to read file '" << filename << "'" << std::endl;
                return;
            }
            _parseSeeds(line);

            // Continue parsing other sections...
            while (std::getline(inputFile, line)) {
                if (line.empty()) {
                    // Skip empty lines
                    continue;
                }

                if (line.find("seed-to-soil map:") != std::string::npos) {
                    cout << "Reading seed-to-soil map" << endl;
                    // Parse seed-to-soil map section
                    readMap(inputFile, _seedToSoilMap);
                } else if (line.find("fertilizer-to-water map:") != std::string::npos) {
                    cout << "Reading fertilizer-to-water map" << endl;
                    // Parse fertilizer-to-water map section
                    readMap(inputFile, _fertilizerToWaterMap);
                } else if (line.find("water-to-light map:") != std::string::npos) {
                    cout << "Reading water-to-light map" << endl;
                    // Parse water-to-light map section
                    readMap(inputFile, _waterToLightMap);
                } else if (line.find("light-to-temperature map:") != std::string::npos) {
                    cout << "Reading light-to-temperature map" << endl;
                    // Parse light-to-temperature map section
                    readMap(inputFile, _lightToTemperatureMap);
                } else if (line.find("temperature-to-humidity map:") != std::string::npos) {
                    cout << "Reading temp-to-humidity map" << endl;
                    // Parse temperature-to-humidity map section
                    readMap(inputFile, _temperatureToHumidityMap);
                } else if (line.find("humidity-to-location map:") != std::string::npos) {
                    cout << "Reading humidity-to-location map" << endl;
                    // Parse humidity-to-location map section
                    readMap(inputFile, _humidityToLocationMap);
                }
            }
        }

        void _parseSeeds(string line){
            // Parse seeds section
                cout << "Reading seeds" << endl;
                std::istringstream iss(line.substr(line.find(":") + 1));
                long long value;
                while (iss >> value) {
                    _seed_vals.push_back(value);
                }
        }

        // Function to read a map from the file
        void readMap(std::istream& input, std::vector<std::vector<long long>>& map) {
            std::string line;
            while (std::getline(input, line) && !line.empty()) {
                std::istringstream iss(line);
                std::vector<long long> values;
                long long value;
                while (iss >> value) {
                    values.push_back(value);
                }
                map.push_back(values);
            }
        }

        long long _getDestinationValue(long long src_value, vector<vector<long long>>& map){
            if(map.empty()) {
                cout << "Destination map is empty. Returning 0" << endl;
                return 0;
            }

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

    SeedMapper seed_mapper = SeedMapper(filename);
    seed_mapper.calculate_lowest_location();

    cout << "Lowest Location: " << seed_mapper.get_lowest_destination_value() << endl;

    // for(int i=0; i<seed_mapper._fertilizerToWaterMap.size(); i++){
    //     for(int j=0; j<seed_mapper._fertilizerToWaterMap[i].size(); j++){
    //         cout << seed_mapper._fertilizerToWaterMap[i][j] << endl;
    //     }
    // }

    return 0;
}