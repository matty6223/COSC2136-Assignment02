#include "Game.h"
#include "Node.h"
#include "TileCodes.h"
#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <algorithm>
#include <sstream>

//constructor for game object -- In future needs to take in player names from main menu.
game::game(std::string playerNames[]){
    //setup a new game empty board.
    setBoardSize(26);
    setupGameboard();

    //initilize tile bag - randomly sort the bag.
    setupTileBag();

    //setup players with their name and hand list.
    for(int i = 0; i < NUM_PLAYERS; i++){
        LinkedList* hand = new LinkedList();
        // ~~initlize the player[i] with their hand.~~
        playerArr[i] = new Player(playerNames[i], i, 0, hand);
        for(int x = 0; x < 6; x++){
            playerArr[i]->getHand()->addTileEnd(tileBag.remove_front());
            // playerArr[i]->hand->placeTileEnd;
        }
    }

    //start gameplay loop
    gamePlayLoop();
}

//load game constructor
game::game(std::string fileName){
    std::ifstream saveFile(fileName);


    
    //load each players information - ID, name, score and hand
    for(int i = 0; i < NUM_PLAYERS; i++){

        int id;
        saveFile >> id;
        saveFile.ignore();

        std::string name;
        std::getline(saveFile, name);

        int score;
        saveFile >> score;

        LinkedList* playersHand = new LinkedList;

        for(int j = 0; j < 6; j++){
            char colour;
            saveFile >> colour;
            int shape;
            saveFile >> shape;
            saveFile.ignore();
            playersHand->addTileEnd(new Tile(colour, shape));
        }
        playerArr[i] = new Player(name, id, score, playersHand);
    }

    // load the tilebag
    std::string tileBagContents;
    saveFile >> tileBagContents;
    char delimiter = ',';
    std::vector<std::string> tiles;
    std::string tile;

    std::stringstream sstream(tileBagContents);
    while(std::getline(sstream, tile, delimiter)){
        tiles.push_back(tile);
    }

    for(int i = 0; i < (int)tiles.size(); i++){
        std::string temp = tiles[i];
        char colour = temp[0];
        int shape = temp[1];
        tileBag.addTileEnd(new Tile(colour, shape));
    }

    //load the turn tracker
    saveFile >> turnTracker;
    saveFile.ignore();

    // //load game size
    // std::string currentBoardSize;
    // saveFile >> currentBoardSize;
    // std::cout << "a" << currentBoardSize << std::endl;
    // saveFile.ignore();

    //load the game board state
    setupGameboard();
    int i = 0;
    std::string boardState;
    std::getline(saveFile, boardState);
    std::cout << boardState << std::endl;

    while(i < boardState.length()){
        char colour = boardState[i];
        int shape = boardState[i+1] - 48;
        char row = boardState[i+3];
        int col = boardState[i+4]- 48;
        if (boardState[i+5 != ',']){
            std::string sBase = std::to_string(boardState[i+4] - 48);
            std::string sAppend = std::to_string(boardState[i+5] - 48);
            std::string result = sBase + sAppend;
            col = std::stoi(result);
        }
        std::cout << row << col << std::endl;
        Tile* temp = new Tile(colour, shape);
        map[int(row)-65][col] = temp;
        i+=7;
    }
    displayBoard();
    gamePlayLoop();
}

//setup a randomly generated tilebag
void game::setupTileBag(){
    //generate all tiles - 6 colours with 6 different shapes and 2 of each type (72 in total).
    //and add them to the linkedlist
    char colour = RED;
    std::vector<Tile*> tempVector;

    for (int i = 0; i < 12; i++)
    {
        if (i >= 2  && i < 4)
        {   
            colour = ORANGE;
        }
        else if (i >= 4  && i < 6){
            colour = YELLOW;
        }
        else if (i >= 6  && i < 8){
            colour = GREEN;
        }
        else if (i >= 8  && i < 10){
            colour = BLUE;
        }
        else if (i >= 10  && i < 12){
            colour = PURPLE;
        }            

        tempVector.push_back(new Tile(colour, CIRCLE));
        tempVector.push_back(new Tile(colour, STAR_4));
        tempVector.push_back(new Tile(colour, DIAMOND));
        tempVector.push_back(new Tile(colour, SQUARE));
        tempVector.push_back(new Tile(colour, STAR_6));
        tempVector.push_back(new Tile(colour, CLOVER));
    }

    //randomize the order of the tiles.
    auto rng = std::default_random_engine {};
    std::shuffle(std::begin(tempVector), std::end(tempVector), rng);

    //add each tile to the linked list
    for(int i = 0; i < (int)tempVector.size(); ++i){
        //add each tile to the end of the linkedList
        tileBag.addTileEnd(tempVector[i]);
    }
}

//loops through gameplay until win condition is met
void game::gamePlayLoop(){
    bool winConditionMet = false;
    bool saveConditionMet = false;
    turnTracker = 0;

    //gameplay loop
    while (saveConditionMet == false)
    {
        //display the game board
        displayBoard();

        //display player turn details
        std::cout << playerArr[turnTracker]->getPlayerName() << "'s Hand:" << std::endl;
        std::cout << playerArr[turnTracker]->getHand()->getLength() << std::endl;
        
        //display Players hand
        for(int i = 0; i < playerArr[turnTracker]->getHand()->getLength(); i++){
            std::cout << playerArr[turnTracker]->getHand()->getTileAtIndex(i)->getColour() 
                      << playerArr[turnTracker]->getHand()->getTileAtIndex(i)->getShape() 
                      << " ";
        }
        std::cout << std::endl;

        //take input for user selection
        std::cout << "Awaiting user input: " << std::endl;
        std::string menuInput;
        std::cin >> menuInput;

        //place tile
        if(menuInput == "place")
        {
            //take in the colour and shape variables
            char colour;
            int shape;
            std::cin >> colour;
            std::cin >> shape;

            //create a temp tile and call function that removes the tile from player hand
            Tile* temp = playerArr[turnTracker]->getHand()->placeTile(new Tile(colour, shape));

            //Eat the word "at".
            std::cin >> menuInput;
        
            //take in the row and col values
            char row;
            int col;
            std::cin >> row;
            std::cin >> col;

            //convert row to ascii value and minus 65 so a = 0, b = 1 etc.
            map[int(row)-65][col] = temp;

            //add new tile to the end of players hand and remove tile from top of the tile bag.
            playerArr[turnTracker]->getHand()->addTileEnd(tileBag.remove_front());

            //ignore the leftover text in the input stream.
            std::cin.ignore();
        }
        //replace tile
        else if (menuInput == "replace")
        {
            //TODO
        }
        //save and exit
        else if (menuInput == "exit"){
            saveGame();
            saveConditionMet = true;
        }

        //change to next players turn
        if(turnTracker == 0){
            turnTracker = 1;
        }
        else {
            turnTracker = 0;
        }
    }
}

//saves the game to file
void game::saveGame(){
    //ask for save name
    std::string fileName = "test";
    // std::cout<< "please enter file name:" <<std::endl;
    // std::cin >> fileName;

    std::ofstream output(fileName + ".txt");

    //save each players information - ID, name, score and hand
    for(int i = 0; i < NUM_PLAYERS; i++){
        output << playerArr[i]->getID() << std::endl;
        output << playerArr[i]->getPlayerName() << std::endl;
        output << playerArr[i]->getScore() << std::endl;
        for(int j = 0; j < playerArr[i]->getHand()->getLength(); j++){
            output << playerArr[i]->getHand()->getTileAtIndex(j)->getColour() 
                        << playerArr[i]->getHand()->getTileAtIndex(j)->getShape() 
                        << ",";
        }
        output << std::endl;
    }
    //save the tilebag
    for(int i = 0; i <= tileBag.getLength(); i++){
        output << tileBag.getTileAtIndex(i)->getColour()
                  << tileBag.getTileAtIndex(i)->getShape()
                  << ",";
    }
    output << std::endl;

    //save turn tracker
    output << turnTracker << std::endl;
    char rowSymbol = 'A';

    //save game board state.
    for (size_t row = 0; row < map.size(); row++)
    {
        std::cout << row << std::endl;
        for (size_t col = 0; col < map.size(); col++)
        {
            
            if(map[row][col] != nullptr){
                std::cout << col << std::endl;
                output << map[row][col]->colour << map[row][col]->shape << "@" << rowSymbol << col << ", ";
            }            
        }
        rowSymbol++;
    }
    output << std::endl;
}

//setup an empty game board.
void game::setupGameboard(){
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        std::vector<Tile*> temp;
        for (int col = 0; col < BOARD_SIZE; col++)
        {
            Tile* tempNode = nullptr;
            temp.push_back(tempNode);
        }
        map.push_back(temp);
    }
}

void game::setBoardSize(int size){
    this->boardSize = size;
}

//display game board
void game::displayBoard(){
    std::cout << " ";
    for (int i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < map.size(); j++)
        {
            //display the col numbers on the first interation.
            if(i == 0){
                //ensuring the spacing is correct for the board display
                if(j <= 10){
                    std::cout<< "  " << j;
                }
                else{
                    std::cout<< " " << j;
                }
            }
            //display underlines on the 2nd iteration
            else if(j == 0){
                std::cout << "  ----";
            }
            else{
                std::cout << "---";
            }
        }
        std::cout<<std::endl;
    }
    char rowLetters = 'A';
    //loop through the map vector.
    for (size_t row = 0; row < map.size(); row++)
    {
        //display the Row letters.
        std::cout << rowLetters << " |";
        rowLetters++;
        
        //display the tile or empty space at row and col coordinates.
        for (size_t col = 0; col < map[row].size(); col++)
        {
                if(map[row][col] == nullptr){
                    std::cout << "  ";
                } else{
                    //~~~~need to implement displaying of tile color and shape.~~~~
                    std::cout << map[row][col]->colour << map[row][col]->shape;
                }
                std::cout << "|";            
        }
        std::cout << std::endl;
    }
}

// for in file testing only
main(){
    // std::string playerNames[2];
    // std::cout << "Player 1 Name: " << std::endl;
    // std::cin >> playerNames[0];
    // std::cout << "Player 2 Name: " << std::endl;
    // std::cin >> playerNames[1];

    // std::cin.ignore();

    // game* newGame = new game(playerNames);
    game* newGame = new game("test.txt");
    
}

