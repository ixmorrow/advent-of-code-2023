#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

enum class Hand_Type {
    FiveOfAKind, // 0
    FourOfAKind, // 1
    FullHouse, // 2
    ThreeOfAKind, // 3
    TwoPair, // 4
    OnePair, // 5
    HighCard, // 6
    UNKNOWN // 7
};

struct Hand {
    string hand;
    int multiplier;
    Hand_Type hand_type;
    int winnings;
    int joker_count;

    // Constructor
    Hand(string h, int m) : hand(h), multiplier(m), hand_type(Hand_Type::UNKNOWN), winnings(0), joker_count(0) {}

};

unordered_map<char, int> cardRankings = 
    {
        {'A', 14},
        {'K', 13},
        {'Q', 12},
        {'T', 10},
        {'9', 9},
        {'8', 8},
        {'7', 7},
        {'6', 6},
        {'5', 5},
        {'4', 4},
        {'3', 3},
        {'2', 2},
        {'J', 1}
    };

bool compareCard(char card1, char card2){
    return (cardRankings[card1] < cardRankings[card2]);
}

Hand_Type calculateHand(string cards, Hand& hand){
        unordered_map<char, int> card_count;

        // Count the occurrences of each card
        for(auto card : cards) {
            if(card_count.find(card) != card_count.end()) card_count[card] += 1;
            else card_count[card] = 1;
        }

        hand.joker_count = card_count['J'];
        int fullHouse = 0;
        int threeOfKind = 0;
        int twoPair = 0;
        int onePair = 0;

        for(auto it = card_count.begin(); it != card_count.end(); it++){
            if(it->second == 5) {
                return Hand_Type::FiveOfAKind;
            } else if(it->second == 4) {
                if(hand.joker_count == 1) {
                    return Hand_Type::FiveOfAKind;
                } else {
                    return Hand_Type::FourOfAKind;
                }
            } else if (it->second == 3){
                if(hand.joker_count == 2) {
                    return Hand_Type::FiveOfAKind;
                } else if(hand.joker_count == 1){
                    return Hand_Type::FourOfAKind;
                } else if(onePair == 1) {
                    return Hand_Type::FullHouse;
                } else {
                    threeOfKind = 1;
                }
            } else if (it->second == 2){
                if(hand.joker_count == 3) {
                    return Hand_Type::FiveOfAKind;
                } else if(hand.joker_count == 2) {
                    return Hand_Type::FourOfAKind;
                } else if(hand.joker_count == 1){
                    if(onePair == 1) {
                        return Hand_Type::FullHouse;
                    } else {
                        threeOfKind = 1;
                    }
                }
                if(threeOfKind == 1) {
                    return Hand_Type::FullHouse;
                } else if(onePair == 1) {
                    return Hand_Type::TwoPair;
                } else {
                    onePair = 1;
                }
            }
        }

        if(threeOfKind == 1) {
            return Hand_Type::ThreeOfAKind;
        } else if (onePair == 1) {
            return Hand_Type::OnePair;
        } else {
            if(hand.joker_count == 0) 
                return Hand_Type::HighCard;
            else if(hand.joker_count == 1) 
                return Hand_Type::OnePair;
            else if (hand.joker_count == 2) 
                return Hand_Type::ThreeOfAKind;
            else if (hand.joker_count == 3) 
                return Hand_Type::FourOfAKind;
            else 
                return Hand_Type::FiveOfAKind;
        }
    }

void sortHands(const Hand& hand, std::list<Hand>& hands) {
    bool breakout = false;

    if (hands.empty()) {
        hands.push_back(hand);
    } else {
        auto it = hands.begin();

        while (it != hands.end() && it->hand_type >= hand.hand_type && !breakout) {
            if (it->hand_type == hand.hand_type) {
                // Iterate over each individual card
                int i;
                for (i = 0; i < 5; i++) {
                    if (hand.hand[i] == it->hand[i]) {
                        // If the cards are equal, move on to the next one
                        continue;
                    } else {
                        if (compareCard(hand.hand[i], it->hand[i])) {
                            breakout = true;
                        } else {
                            break;
                        }
                    }
                }

                // Break out of the loop if all cards are equal
                if (i == 5) {
                    break;
                }
            }

            if (!breakout) {
                ++it;
            }
        }

        hands.insert(it, hand);
    }
}

vector<Hand> buildHands(const string filename){
    // read file
    ifstream myfile(filename);

    if (!myfile.is_open()) {
        cerr << "Error: Unable to open file '" << filename << "'" << endl;
        throw "Unable to opepn file!";
    }

    vector<Hand> hands;
    std::string line;
    while(getline(myfile, line)){
        std::istringstream iss(line);
        string hand;
        int multiplier;

        if (iss >> hand >> multiplier) {
            Hand hand_struct = {hand, multiplier};
            hand_struct.hand_type = calculateHand(hand, hand_struct);
            hands.push_back(hand_struct);
        } else {
            // Handle error if the extraction fails
            std::cerr << "Error extracting data from the line." << std::endl;
        }
    }

    return hands;
}

void run(const string filename) {
    auto hands = buildHands(filename);

    // sorted list of hands
    list<Hand> sorted_hands;
    for(auto hand : hands){
        sortHands(hand, sorted_hands);
    }

    int total_winnings = 0;
    int rank = 1;
    for (auto hand : sorted_hands) {
        hand.winnings = hand.multiplier * rank;
        total_winnings += hand.winnings;

        // cout << "Rank: " << rank << ", Hand: " << hand.hand << ", Multiplier: " << hand.multiplier << ", Winnings: " << hand.winnings << ", Type: " << static_cast<int>(hand.hand_type) << ", Joker Count: " << hand.joker_count << endl;
        rank++;
    }

    cout << "Total Winnings: " << total_winnings << endl;
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