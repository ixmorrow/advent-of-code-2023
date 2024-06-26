#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <list>

using namespace std;

enum class HandType {
    FiveOfAKind = 0,
    FourOfAKind = 1,
    FullHouse = 2,
    ThreeOfAKind = 3,
    TwoPair = 4,
    OnePair = 5,
    HighCard = 6,
    UNKNOWN = 7
};

class Hand {
    public:
        // Constructor
        Hand(string h, int m) : cards(h), multiplier(m), winnings(0) {
            hand_type = _calculate_hand(h);
        }

        string cards;
        int multiplier;
        HandType hand_type;
        int winnings;
        int _joker_count;

    private:
        HandType _calculate_hand(string cards){
            unordered_map<char, int> card_count;
            for(auto card : cards) {
                if(card_count.find(card) != card_count.end()) {
                    card_count[card] += 1;
                }
                else card_count[card] = 1;
            }

            _joker_count = card_count['J'];
            int fullHouse = 0;
            int threeOfKind = 0;
            int twoPair = 0;
            int onePair = 0;

            for(auto it = card_count.begin(); it != card_count.end(); it++){
                if(it->second == 5) return HandType::FiveOfAKind;
                else if(it->second == 4) return HandType::FourOfAKind;
                else if (it->second == 3){
                    if(onePair == 1) return HandType::FullHouse;
                    else threeOfKind = 1;
                }
                else if (it->second == 2){
                    if(threeOfKind == 1) return HandType::FullHouse;
                    else if(onePair == 1) return HandType::TwoPair;
                    else onePair = 1;
                }
            }

            if(threeOfKind == 1) return HandType::ThreeOfAKind;
            else if (onePair == 1) return HandType::OnePair;
            else return HandType::HighCard;
        }
};
class CamelCards {
    public:
        CamelCards() { }

        void play(const string filename) {
            auto hands = _build_hands(filename);

            // sorted list of hands
            list<Hand> sorted_hands;
            for(auto hand : hands){
                _sort_hands(hand, sorted_hands);
            }

            int total_winnings = 0;
            int rank = 1;
            for (auto hand : sorted_hands) {
                hand.winnings = hand.multiplier * rank;
                total_winnings += hand.winnings;
                rank++;
            }

            cout << "Total Winnings: " << total_winnings << endl;
        }

    private:
        unordered_map<char, int> cardRankings = 
        {
            {'A', 14},
            {'K', 13},
            {'Q', 12},
            {'J', 11},
            {'T', 10},
            {'9', 9},
            {'8', 8},
            {'7', 7},
            {'6', 6},
            {'5', 5},
            {'4', 4},
            {'3', 3},
            {'2', 2}
        };

        vector<Hand> _build_hands(const string filename){
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
                    hands.push_back(hand_struct);
                } else {
                    // Handle error if the extraction fails
                    std::cerr << "Error extracting data from the line." << std::endl;
                }
            }

            return hands;
        }

        void _sort_hands(const Hand& hand, std::list<Hand>& hands) {
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
                            if (hand.cards[i] == it->cards[i]) {
                                // If the cards are equal, move on to the next one
                                continue;
                            } else {
                                if (_compare_card(hand.cards[i], it->cards[i])) {
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

        bool _compare_card(char card1, char card2){
            return (cardRankings[card1] < cardRankings[card2]);
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

    CamelCards camel_cards = CamelCards();
    try{
        camel_cards.play(filename);
    } catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}