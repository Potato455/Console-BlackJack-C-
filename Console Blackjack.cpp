#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

enum Rank {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
EIGHT, NINE, TEN, JACK, QUEEN, KING};

const Rank RankVector[] = {ACE, TWO, THREE, FOUR, FIVE, SIX, SEVEN,
EIGHT, NINE, TEN, JACK, QUEEN, KING};

enum Type {CLUBS, DIAMONDS, HEARTS, SPADES};

const Type TypeVector[] = {CLUBS, DIAMONDS, HEARTS, SPADES};

class Card{
public:
    // constructor with Rank and Type
    Card(Rank pRank, Type pType){
        this->aRank = pRank;
        this->aType = pType;
    }
    // map value to Rank
    int getValue(){
        int output = 0;
        switch(this->aRank)
        {
            case ACE:
                output = 1; break;
            case TWO:
                output = 2; break;
            case THREE:
                output = 3; break;
            case FOUR:
                output = 4; break;
            case FIVE:
                output = 5; break;
            case SIX:
                output = 6; break;
            case SEVEN:
                output = 7; break;
            case EIGHT:
                output = 8; break;
            case NINE:
                output = 9; break;
            case TEN:
                output = 10; break;
            case JACK:
                output = 10; break;
            case QUEEN:
                output = 10; break;
            case KING:
                output = 10; break;
        }
        return output;
    }
    // print Card
    void displayCard(){
        switch(this->aRank)
        {
            case ACE:
                cout << " 1";break;
            case TWO:
                cout << " 2";break;
            case THREE:
                cout << " 3";break;
            case FOUR:
                cout << " 4";break;
            case FIVE:
                cout << " 5";break;
            case SIX:
                cout << " 6";break;
            case SEVEN:
                cout << " 7";break;
            case EIGHT:
                cout << " 8";break;
            case NINE:
                cout << " 9";break;
            case TEN:
                cout << " 10";break;
            case JACK:
                cout << " J";break;
            case QUEEN:
                cout << " Q";break;
            case KING:
                cout << " K";break;
        }
        switch(this->aType)
        {
            case CLUBS:
                cout << "C"; break;
            case DIAMONDS:
                cout << "D"; break;
            case HEARTS:
                cout << "H"; break;
            case SPADES:
                cout << "S"; break;
        }
    }

private:
    Rank aRank;
    Type aType;
};

class Hand{
public:
    void add(Card c){
        this->cards.push_back(c);
    }
    void clear(){
        this->cards.clear();
    }
    int getTotal(){
        int total = 0;
        // keeps track of aces in hand to determine 1 or 10 value
        int aces = 0;
        for(Card c: this->cards){
            int value = c.getValue();
            // checks if it's an ACE
            if(value == 1){
                aces++;
            }
            total += value;
        }
        // see if we can get higher value because of ACE(s)
        for ( int i = 0; i<aces; i++){
            // see if we can add 10 value which makes the ACE worth 11
            if (total+10 <= 21){
                // then do it
                total+=10;
            }
            // if we can't then break
            else{
                break;
            }
        }
        return total;
    }
    // helper function to print out hand
    void print(){
        for(Card c: cards){
            c.displayCard();
        }
        cout << " [" << getTotal() << "]" << endl;
    }
private:
    vector<Card> cards;
};

class Deck{
public:
    void Populate(){
        // clear Deck first
        this->cards.clear();
        // loop through all rank and type to create all the cards
        for(Rank r: RankVector){
            for(Type t: TypeVector){
                Card c = Card(r, t);
                this->cards.push_back(c);
            }
        }
    }
    bool isEmpty(){
        return this->cards.empty();
    }
    void shuffle(){
        // get random number
        auto random = std::random_device {};
        auto rng = default_random_engine { random() };
        // using std::shuffle to shuffle cards
        std::shuffle(this->cards.begin(), this->cards.end(), rng);
    }
    Card deal(){
        // get the card from vector and destroy it
        Card c = this->cards.back();
        this->cards.pop_back();
        return c;
    }
    // void deal(ComputerPlayer p){
    //     // get the card from vector and destroy it
    //     Card c = this->cards.back();
    //     // add the card to the given Hand
    //     p.add(c);
    //     this->cards.pop_back();
    // }
    Card riggedDeal(){
        // get the card from vector and destroy it
        // loop through the cards to find the highest value (aka 2 10s)
        // if no 10's left then go to 9's
        for (int j = 10; j>8 ; j++){
            for (int i = 0; i<cards.size(); i++){
                if (cards.at(i).getValue() == j){
                    Card c = cards.at(i);
                    cards.erase(cards.begin()+i);
                    return c;
                }
            }
        }
        // if can't find any to rig then do normal deal
        return deal();
    }
private:
    vector<Card> cards;
};

class AbstractPlayer{
public:
    virtual bool isDrawing() const = 0;
    bool isBusted(){
        return this->aHand.getTotal() > 21;
    }
    void add(Card c){
        this->aHand.add(c);
    }
    int getWins(){
        return this->wins;
    }
    void incrementWins(){
        this->wins++;
    }
    // print hand 
    void print(){
        this->aHand.print();
    }
    // return score
    int getScore(){
        return this->aHand.getTotal();
    }
    void clear(){
        this->aHand.clear();
    }
    
protected:
    mutable Hand aHand;
    // keep track of wins
    mutable int wins = 0;
};

class HumanPlayer: public AbstractPlayer{
public:
    bool isDrawing() const {
        string input;
        while(true){
            cout << "Do you want to draw? (y/n): ";
            cin >> input;
            if (input.compare("y") == 0){
                return true;
            }
            else if (input.compare("n") == 0){
                return false;
            }
            // if user doesn't input y or n
            else {
                cout << "Invalid input, try again" << endl;
            }
        }
        
    }
    // prints information on end game
    // returns -1 if player loses and 1 if player wins, 0 if push
    int announce(int ComputerScore){
        int output = 3;
        int score = this->aHand.getTotal();
        // check player bust
        if (this->isBusted()){
            cout << "Player busts." << endl;
            cout << "Casino Wins." << endl;
            output = -1;
        }
        // check computer bust
        else if(ComputerScore > 21){
            cout << "Casino busts." << endl;
            cout << "Player wins." << endl;
            output = 1;
        }
        // check player wins
        else if ((score>ComputerScore && score<=21)){
            cout << "Player wins." << endl;
            output = 1;
        }
        // check for push (equal score)
        else if(score == ComputerScore){
            cout << "Push: No one wins." << endl;
            output = 0;
        }
        // else casino wins
        else {
            cout << "Casino Wins." << endl;
            output = -1;
        }
        return output;
    }
};

class ComputerPlayer: public AbstractPlayer{
public:
    bool isDrawing() const {
        bool output = false;
        // draws until total is more than 16
        if(this->aHand.getTotal() <= 16){
            output = true;
        }
        // otherwise it's above 16 and under 21
        else {
            output = false;
        }
        return output;
    }
};

class BlackJackGame{
public:
    void play(){
        bool casinoDraws = false;
        // check if requires populate (replayability)
        if (m_deck.isEmpty()){
            m_deck.Populate();
            // shuffle
            m_deck.shuffle();
        }
        // increment rounds by the amount of hands
        rounds+=humanList.size();
        // casino draws 1 card
        // check if we should rigg it towards casino's favor
        if (shouldRigg()){
            m_casino.add(m_deck.riggedDeal());
        }
        // otherwise no cheating draw
        else{
            m_casino.add(m_deck.deal());
        }
        // display casino cards
        cout << "Casino:";
        m_casino.print();
        // for each separate "player"
        // draw cards one after the other
        for (int i = 0; i<humanList.size(); i++){
            // player draws 2 cards
            humanList[i].add(m_deck.deal());
            humanList[i].add(m_deck.deal());
            // display player cards
            cout << "Player:";
            humanList[i].print();

            // drawing phase for player
            while (humanList[i].isDrawing()){
                // draw card
                humanList[i].add(m_deck.deal());
                // print new cards
                cout << "Player:";
                humanList[i].print();

                // check if busted
                if(humanList[i].isBusted()){
                    humanList[i].announce(m_casino.getScore());
                    // in this scenario for every hand won by casino is counted 
                    // in the win percentage for the rigging.
                    m_casino.incrementWins();
                    break;
                }
            }
        }
        // casino only draws if at least one human didn't bust
        for (HumanPlayer p: humanList){
            if (!p.isBusted()){
                casinoDraws = true;
                break;
            }
        }
        
        if (casinoDraws){
            // casino draws
            while(m_casino.isDrawing()){
                // check if we need to rigg
                if(shouldRigg()){
                    m_casino.add(m_deck.riggedDeal());
                }
                else {
                    m_casino.add(m_deck.deal());
                }
                // print new cards
                cout << "Casino:";
                m_casino.print();
            }
        }
        
        // anounce winner
        for (int i = 0; i<humanList.size(); i++){
            // if already busted, don't need to say again
            if (humanList[i].isBusted()){
                humanList[i].clear();
                continue;
            }
            cout << "For player hand:";
            humanList[i].print();
            int outcome = humanList[i].announce(m_casino.getScore());
            if (outcome == 1){
                humanList[i].incrementWins();
            }
            else if (outcome == -1){
                m_casino.incrementWins();
            }
            // clear hand
            humanList[i].clear();
        }
        
        // clear hand
        m_casino.clear();
        
    }
    
    // checks wether or not we should rigg to casino's favor
    bool shouldRigg(){
        // only check for rigging every 10 rounds
        if (rounds%10==0){
            // calculate percentage winning for Casino
            float perc = (float)m_casino.getWins()/(float)rounds;
            // only rigg if winning less than 55%
            if (perc < 0.55){
                return true;
            }
            else {
                return false;
            }
        }
        else {
            return false;
        }
        
    }
    void setHands(int hands){
        for (int i = 0; i<hands; i++){
            HumanPlayer p;
            humanList.push_back(p);
        }
    }
private:
    Deck m_deck;
    ComputerPlayer m_casino;
    vector<HumanPlayer> humanList;
    // will check percentage every 10 rounds
    int rounds = 0;
};

int main()
{
    cout << "\tWelcome to the Comp322 Blackjack game!" << endl << endl;
    BlackJackGame game;
    int hands = 1;
    cout << "How many hands would you like to play with? (1-3): ";
    cin >> hands;
    game.setHands(hands);
    // The main loop of the game
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
        game.play();
        // Check whether the player would like to play another round
        cout << endl << endl;
        cout << "Would you like another round? (y/n): ";
        cin >> answer;
        cout << endl << endl;
        playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Gave over!";
    return 0;
}