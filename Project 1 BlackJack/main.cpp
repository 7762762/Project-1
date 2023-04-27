/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: diean
 *
 * Created on April 22, 2023, 1:47 PM
 */

#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

// Define the card structure
struct Card {
    int value;
    string name;
    string suit;
};

// Define the deck of cards
const int NUM_CARDS = 52;
Card deck[NUM_CARDS];

// Define the player structure
struct Player {
    string name;
    int money;
    Card hand[10];
    int num_cards;
    bool is_dealer;
    int getBet;
};

// Define the game structure
struct Game {
    int num_players;
    Player players[6];
    int dealer_cards[10];
    int num_dealer_cards;
    int getHandValue;
};

// Function to initialize the deck of cards
void initializeDeck() {
    string suits[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    string names[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};

    int card_count = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            deck[card_count].suit = suits[i];
            deck[card_count].name = names[j];
            if (j >= 10) {
                deck[card_count].value = 10;
            }
            else {
                deck[card_count].value = j + 1;
            }
            card_count++;
        }
    }
}

// Function to shuffle the deck of cards
void shuffleDeck() {
    random_shuffle(deck, deck + NUM_CARDS);
}

// Function to deal a card to a player
void dealCard(Player& player, bool show_card = true) {
    player.hand[player.num_cards] = deck[0];
    player.num_cards++;

    // Remove the dealt card from the deck
    for (int i = 0; i < NUM_CARDS; i++) {
        if (deck[i].suit == player.hand[player.num_cards - 1].suit && deck[i].name == player.hand[player.num_cards - 1].name) {
            for (int j = i; j < NUM_CARDS - 1; j++) {
                deck[j] = deck[j + 1];
            }
            break;
        }
    }

    if (show_card) {
        cout << "Dealt " << player.hand[player.num_cards - 1].name << " of " << player.hand[player.num_cards - 1].suit << " to " << player.name << endl;
    }
}

// Function to calculate the value of a hand
int calculateHandValue(Card hand[], int num_cards) {
    int value = 0;
    int num_aces = 0;

    for (int i = 0; i < num_cards; i++) {
        value += hand[i].value;
        if (hand[i].name == "Ace") {
            num_aces++;
        }
    }

    // Adjust for aces
    while (value > 21 && num_aces > 0) {
        value -= 10;
        num_aces--;
    }

    return value;
}

// Function to display a player's hand
void displayHand(Player player, bool show_all_cards = true) {
    cout << player.name << "'s hand: ";
    for (int i = 0; i < player.num_cards; i++) {
        if (i == 0 && show_all_cards == false && player.is_dealer) {
            cout << "?? of ?? ";
        } else {
            cout << player.hand[i].name << " of " << player.hand[i].suit << " ";
        }
    }
    cout << endl;
}


// Function to get player bet
int getPlayerBet(Player player) {
    int bet = 0;
    while (bet <= 0 || bet > player.money) {
        cout << player.name << ", you have $" << player.money << ". Please enter your bet: ";
        cin >> bet;
    }
    return bet;
}


// Function to play the game
void playGame(Game& game) {
    // Declare and initialize the game structure
    game.num_dealer_cards = 0;
    
    // Deal two cards to each player
    for (int i = 0; i < game.num_players; i++) {
        for (int j = 0; j < 2; j++) {
            dealCard(game.players[i], true);
        }
        if (game.players[i].is_dealer) {
            displayHand(game.players[i], false);
            int hand_value = calculateHandValue(game.players[i].hand, game.players[i].num_cards);
            game.dealer_cards[0] = hand_value;
            game.num_dealer_cards++;
        } else {
            displayHand(game.players[i]);
        }
    }
    
    // Ask each player for their bet
    for (int i = 0; i < game.num_players; i++) {
        if (!game.players[i].is_dealer) {
            int bet = getPlayerBet(game.players[i]);
            game.players[i].money -= bet;
        }
    }

    // Play the game
    for (int i = 0; i < game.num_players; i++) {
        if (!game.players[i].is_dealer) {
            bool done = false;
            while (!done) {
                int hand_value = calculateHandValue(game.players[i].hand, game.players[i].num_cards);
                cout << game.players[i].name << ", your hand is currently worth " << hand_value << endl;
                cout << "Do you want to hit (h) or stand (s)? ";
                char choice;
                cin >> choice;

                if (choice == 'h') {
                    dealCard(game.players[i]);
                    displayHand(game.players[i]);
                    int new_hand_value = calculateHandValue(game.players[i].hand, game.players[i].num_cards);
                    if (new_hand_value > 21) {
                        cout << game.players[i].name << ", your hand is now worth " << new_hand_value << ", which is over 21. You lose." << endl;
                        done = true;
                    }
                } else {
                    done = true;
                }
            }
        }
    }
}


// Dealer's turn
cout << "Dealer's turn..." << endl;
displayHand(game.players[0]);

bool done = false;
while (!done) {
    int hand_value = calculateHandValue(game.players[0].hand, game.players[0].num_cards);
    if (hand_value >= 17) {
        done = true;
    }
    else {
        dealCard(game.players[0]);
        displayHand(game.players[0], false);
    }
}
game.dealer_cards[game.num_dealer_cards] = calculateHandValue(game.players[0].hand, game.players[0].num_cards);
game.num_dealer_cards++;

// Determine the winner
int player_hand_value, dealer_hand_value;
for (int i = 0; i < game.num_players; i++) {
    player_hand_value = 0;
    dealer_hand_value = 0;
    if (!game.players[i].is_dealer) {
        player_hand_value = calculateHandValue(game.players[i].hand, game.players[i].num_cards);
        dealer_hand_value = game.dealer_cards[0];

        if (player_hand_value > 21) {
            cout << game.players[i].name << ", you busted with a hand worth " << player_hand_value << ". You lose." << endl;
        }
        else if (dealer_hand_value > 21) {
            cout << game.players[i].name << ", the dealer busted with a hand worth " << dealer_hand_value << ". You win!" << endl;
            game.players[i].money += 2 * game.players[i].bet;
        }
        else if (dealer_hand_value > player_hand_value) {
            cout << game.players[i].name << ", the dealer's hand is worth " << dealer_hand_value << ", which is higher than your hand worth " << player_hand_value << ". You lose." << endl;
        }
        else if (player_hand_value > dealer_hand_value) {
            cout << game.players[i].name << ", your hand is worth " << player_hand_value << ", which is higher than the dealer's hand worth " << dealer_hand_value << ". You win!" << endl;
            game.players[i].money += 2 * game.players[i].bet;
        }
        else {
            cout << game.players[i].name << ", your hand is worth " << player_hand_value << ", which is the same as the dealer's hand worth " << dealer_hand_value << ". It's a push." << endl;
            game.players[i].money += game.players[i].bet;
        }
    }
}

int main() {
    // Initialize the game
    srand(time(NULL));
    Game game = createGame();

    // Play the game until the players run out of money or choose to quit
    bool done = false;
    while (!done) {
        // Reset the game
        resetGame(game);

        // Play a round of the game
        playGame(game);

        // Check if the players want to continue playing
        bool valid_input = false;
        char choice;
        while (!valid_input) {
            cout << "Do you want to play another round (y/n)? ";
            cin >> choice;
            if (choice == 'y' || choice == 'n') {
                valid_input = true;
            }
        }
        if (choice == 'n') {
            done = true;
            break; // Added a break statement to exit the loop immediately
        }

        // Check if any players have run out of money
        for (int i = 0; i < game.num_players; i++) {
            if (game.players[i].money <= 0) {
                cout << game.players[i].name << ", you have run out of money. Game over." << endl;
                done = true;
                break;
            }
        }
    }

    return 0;
}

// Shuffle a deck of cards
void shuffleDeck(Deck& deck) {
    if (deck.size() < 2) {
        cerr << "Error: Not enough cards in deck to shuffle" << endl;
        exit(1); // exit program with error status
    }
    random_shuffle(deck.begin(), deck.end());
}

// Deal cards to the players
void dealCards(Game& game) {
    if (game.deck.size() < (2 * game.num_players + 2)) {
        cerr << "Error: Not enough cards in deck to deal" << endl;
        exit(1); // exit program with error status
    }
    for (int i = 0; i < game.num_players; i++) {
        game.players[i].hand.push_back(game.deck.back());
        game.deck.pop_back();
        game.players[i].hand.push_back(game.deck.back());
        game.deck.pop_back();
    }
    game.dealer_hand.push_back(game.deck.back());
    game.deck.pop_back();
    game.dealer_hand.push_back(game.deck.back());
    game.deck.pop_back();
}

// Print the cards in a player's hand
void printHand(const vector<Card>& hand) {
    if (hand.empty()) {
        cout << "Empty hand" << endl;
        return;
    }
    for (int i = 0; i < hand.size(); i++) {
        cout << "(" << hand[i].rank << " " << hand[i].suit << ") ";
    }
    cout << endl;
}

// Get the value of a player's hand
int getHandValue(const vector<Card>& hand) {
    int value = 0;
    int num_aces = 0;
    for (int i = 0; i < hand.size(); i++) {
        if (hand[i].rank == "Ace") {
            num_aces++;
            value += 11;
        }
        else if (hand[i].rank == "King" || hand[i].rank == "Queen" || hand[i].rank == "Jack") {
            value += 10;
        }
        else {
            value += stoi(hand[i].rank);
        }
    }
    while (value > 21 && num_aces > 0) {
        value -= 10;
        num_aces--;
    }
    return value;
}

// Ask a player for their bet
int getBet(const Player& player) {
    int bet = 0;
    bool valid_input = false;
    int attempts = 0;
    while (!valid_input && attempts < 3) {
        cout << player.name << ", you have " << player.money << " dollars. How much do you want to bet? ";
        if (!(cin >> bet)) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            attempts++;
        }
        else if (bet < 1 || bet > player.money) {
            cout << "Invalid bet. Please enter a bet between 1 and " << player.money << "." << endl;
            attempts++;
        }
        else {
            valid_input = true;
        }
    }
    if (attempts >= 3) {
        cout << "Too many invalid attempts. Exiting program." << endl;
        exit(1);
    }
    return bet;
}


// Ask a player if they want to hit or stand
bool getHitOrStand(const Player& player) {
    bool valid_input = false;
    char choice;
    while (!valid_input) {
        cout << player.name << ", your hand is worth " << getHandValue(player.hand) << ". Do you want to hit (h) or stand (s)? ";
        cin >> choice;
        // Convert the input to lowercase for case-insensitive comparison
        choice = tolower(choice);
        if (choice == 'h' || choice == 's') {
            valid_input = true;
        } else {
            // Clear the input stream if the input is invalid
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter 'h' to hit or 's' to stand." << endl;
        }
    }
    return choice == 'h';
}

// Play a round of the game
void playGame(Game& game) {
    // Shuffle the deck and deal cards to the players
    shuffleDeck(game.deck);
    dealCards(game);

    // Get each player's bet and deduct it from their money
    for (int i = 0; i < game.num_players; i++) {
        game.players[i].bet = getBet(game.players[i]);
        game.players[i].money -= game.players[i].bet;
    }

    // Print the dealer's face-up card
    cout << "The dealer's face-up card is (" << game.dealer_hand[1].rank << " " << game.dealer_hand[1].suit << ")" << endl;

    // Ask each player to hit or stand
    for (int i = 0; i < game.num_players; i++) {
        bool done = false;
        while (!done) {
            cout << game.players[i].name << "'s hand is: ";
            printHand(game.players[i].hand);
            if (getHandValue(game.players[i].hand) == 21) {
                cout << game.players[i].name << " has 21!" << endl;
                done = true;
            }
            else if (getHandValue(game.players[i].hand) > 21) {
                cout << game.players[i].name << " has busted!" << endl;
                done = true;
            }
            else {
                bool hit = getHitOrStand(game.players[i]);
                if (hit) {
                    game.players[i].hand.push_back(game.deck.back());
                    game.deck.pop_back();
                }
                else {
                    done = true;
                }
            }
        }
    }
}

// Play the dealer's hand
cout << "The dealer's hand is: ";
printHand(game.dealer_hand);
while (getHandValue(game.dealer_hand) < 17) {
    game.dealer_hand.push_back(game.deck.back());
    game.deck.pop_back();
    cout << "The dealer hits." << endl;
    cout << "The dealer's hand is now: ";
    printHand(game.dealer_hand);
    if (getHandValue(game.dealer_hand) > 21) {
        cout << "The dealer has busted!" << endl;
        return;
    }
}
cout << "The dealer stands." << endl;
}


// Determine the winner of each hand and adjust the players' money accordingly
for (int i = 0; i < game.num_players; i++) {
    int player_value = getHandValue(game.players[i].hand);
    int dealer_value = getHandValue(game.dealer_hand);
    if (player_value > 21) {
        cout << game.players[i].name << " loses " << game.players[i].bet << " dollars." << endl;
    }
    else if (dealer_value > 21) {
        game.players[i].money += 2 * game.players[i].bet;
        cout << game.players[i].name << " wins " << game.players[i].bet << " dollars." << endl;
    }
    else if (player_value > dealer_value) {
        game.players[i].money += 2 * game.players[i].bet;
        cout << game.players[i].name << " wins " << game.players[i].bet << " dollars." << endl;
    }
    else if (player_value < dealer_value) {
        cout << game.players[i].name << " loses " << game.players[i].bet << " dollars." << endl;
    }
    else {
        game.players[i].money += game.players[i].bet;
        cout << game.players[i].name << " pushes." << endl;
    }
}

// Remove the cards from each player's hand and the dealer's hand
for (int i = 0; i < game.num_players; i++) {
    game.players[i].hand.clear();
}
game.dealer_hand.clear();

}

// Play the game until the players are out of money or choose to quit
void playBlackjack() {
    cout << "Welcome to Blackjack!" << endl;
    int num_players;
    bool valid_input = false;
    while (!valid_input) {
        cout << "How many players will be playing (1-6)? ";
        cin >> num_players;
        if (num_players >= 1 && num_players <= 6) {
            valid_input = true;
        }
    }
    Game game(num_players);
    while (true) {
        for (int i = 0; i < game.num_players; i++) {
            if (game.players[i].money == 0) {
                cout << game.players[i].name << " is out of money and can no longer play." << endl;
                game.players[i].active = false;
                game.num_active_players--;
            }
        }
        if (game.num_active_players == 0) {
            cout << "All players are out of money. Game over!" << endl;
            break;
        }
        bool play_again = getPlayAgain();
        if (!play_again) {
            break;
        }
        playRound(game);
    }
}

int main() {
    initializeDeck();
    shuffleDeck();
    Game game;
    playBlackjack();
    return 0;
}     