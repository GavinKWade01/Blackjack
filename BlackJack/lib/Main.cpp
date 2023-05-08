#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <exception>
#include <stdexcept>

class card {
    public:
        int value;
        bool isAce;
        char number;
        card(){}
        card(int inValue, char inNumber){
            value = inValue;
            number = inNumber;
            if(number == 'A'){
                isAce = true;
            }
        }
        ~card(){}
};
//Creating the outline for the player and dealer. 
typedef struct player{
    int cardsInHand;
    card *hand[11];
}player_t;

//Deck holds the unplayed cards and usedDeck holds the cards that have been used. This is so the player doesn't get duplicates. 
card *deck[52];
bool usedDeck[52];
//Setting up the player and dealer for ease of clearing the board for the next game. 
player_t playerPC;
player_t dealer;

void cleanDeck(){
    int index;
    for(index = 0; index < 52; index++){
        usedDeck[index] = false;
    }
}

void initDeck(){
    int index;

    cleanDeck();
    /*
    Creates each suit in the deck, it does not actually note the suit but is running the process 4 times to simulate it.
    I have created it with the A for ace, and the others towards the end are T,J,Q, and K because it only accepts single characters. 
    */
    for(index = 0; index < 4; index++){
        deck[index * 13] = new card(1, 'A');
        deck[index * 13 + 1] = new card(2, '2');
        deck[index * 13 + 2] = new card(3, '3');
        deck[index * 13 + 3] = new card(4, '4');
        deck[index * 13 + 4] = new card(5, '5');
        deck[index * 13 + 5] = new card(6, '6');
        deck[index * 13 + 6] = new card(7, '7');
        deck[index * 13 + 7] = new card(8, '8');
        deck[index * 13 + 8] = new card(9, '9');
        deck[index * 13 + 9] = new card(10, 'T');
        deck[index * 13 + 10] = new card(10, 'J');
        deck[index * 13 + 11] = new card(10, 'Q');
        deck[index * 13 + 12] = new card(10, 'K');

    }
}

void resetHands(){
    playerPC.cardsInHand = 0;
    dealer.cardsInHand = 0;
}

void addCardPlayer(){
    int randNum = rand()%52;
    while(usedDeck[randNum]){ randNum = rand()%52; }

    playerPC.hand[playerPC.cardsInHand] = deck[randNum];

    playerPC.cardsInHand++; 
    usedDeck[randNum] = true;
}

void addCardDealer(){
    int randNum = rand()%52;
    while(usedDeck[randNum]){ randNum = rand()%52; }

    dealer.hand[dealer.cardsInHand] = deck[randNum];

    dealer.cardsInHand++; 
    usedDeck[randNum] = true;
}

int getPlayerHand(){
    int index;
    int playerCount, playerCountAce = 0;
    for(index = 0; index < playerPC.cardsInHand; index++){
        playerCountAce += playerPC.hand[index]->value;
        if (playerPC.hand[index]->number == 'A')
        {
            playerCount += 11;
        }else{
            playerCount += playerPC.hand[index]->value;
        }
    }
    if (playerCount <= 21)
        return playerCount;
    else 
        return playerCountAce;
}

int getDealerHand(){
    int index;
    int dealerCount, dealerCountAce = 0;
    for(index = 0; index < dealer.cardsInHand; index++){
        dealerCountAce += dealer.hand[index]->value;
        if (dealer.hand[index]->number == 'A')
        {
            dealerCount += 11;
        }else{
            dealerCount += dealer.hand[index]->value;
        }
    }
    if (dealerCount <= 21)
        return dealerCount;
    else 
        return dealerCountAce;
}

int main(){
    initDeck();
    srand(time(0));
    int gamesPlayed;
    int playerWins = 0;
    int playerLoss = 0;
    int playerTies = 0;

    for(gamesPlayed = 0; gamesPlayed < 100; gamesPlayed++){
        cleanDeck();
        resetHands();
        int index;
        int playerHand = 0;
        int newPlayerHand = 0;
        int dealerHand = 0;

        //This sets up the dealers hand and adds the total of those two cards values.
        addCardDealer();
        addCardDealer();
        getDealerHand();
        dealerHand = getDealerHand();
        
        //This sets up the player hand and adds the total of those two cards values.
        addCardPlayer();
        addCardPlayer();
        getPlayerHand();
        playerHand = getPlayerHand();
        while (playerHand < 17){
            addCardPlayer();
            getPlayerHand();
            playerHand = getPlayerHand();
        }
        //The lines to print out how the player does in there if wanted, they were useful for checking the logic.
        if (playerHand > 21){
            //std::cout << "Player Broke = " << playerHand << std::endl;  
            playerLoss++;
            continue;
        }else if(playerHand > dealerHand){
            //std::cout << "Player Won = " << playerHand << " Dealer Hand = " << dealerHand<< std::endl;  
            playerWins++;
            continue;
        }else if(playerHand < dealerHand){
            //std::cout << "Player Loss = " << playerHand << " Dealer Hand = " << dealerHand<< std::endl;  
            playerLoss++;
            continue;
        }else if(playerHand == dealerHand){
            //std::cout << "Player Tie = " << playerHand << " Dealer Hand = " << dealerHand<< std::endl;  
            playerTies++;
            continue;
        }
    }
    //This is just to print the wins, losses, ties, and total games to be read and count. 
    std::cout << "Player Wins = " << playerWins << std::endl;
    std::cout << "Player Loss = " << playerLoss << std::endl;
    std::cout << "Player Ties = " << playerTies << std::endl;
    std::cout << "Total Games = " << gamesPlayed << std::endl;
    return 0;
}