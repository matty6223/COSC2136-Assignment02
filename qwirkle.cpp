
// #include "LinkedList.h"
#include "Game.h"

#include <iostream>
#include <fstream>

#define EXIT_SUCCESS    0

void displayCredits() {
   std::string names[4] = {"Michael Tamasauskas", "Matthew Rosin",  
                           "Seth Underdown", "Varshan Shankar"};
   std::string ids[4] = {"s3562292", "s3879003", 
                         "s3794214", "s3886861"};
   std::string emails[4] = {"s3562292@student.rmit.edu.au", 
                            "s3879003@student.rmit.edu.au", 
                            "s3794214@student.rmit.edu.au", 
                            "s3886861@student.rmit.edu.au"};
   for(int i = 0; i < 4; i++) {
      std::cout << "Name: " << names[i] << std::endl;
      std::cout << "Student ID: " << ids[i] << std::endl;
      std::cout << "Email: " << emails[i] << std::endl;
      std::cout << std::endl;
   }
}

bool saveFileFormatCorrect() { //testing
   return true;
}

bool saveFileFormatCorrect(std::string fileName) { //testing
   return true;
}

int main(void) {
   bool inGame = true;
   int menuSelection = 0;


   while (inGame != false && !std::cin.eof()) {
      std::cout << "Menu" << std::endl;
      std::cout << "----" << std::endl;
      std::cout << " 1. New Game" << std::endl;
      std::cout << " 2. Load Game" << std::endl;
      std::cout << " 3. Credits (Show student information)" << std::endl;
      std::cout << " 4. Quit" << std::endl;

      std::cin >> menuSelection;

      switch (menuSelection) {
         case 1: { // New Game 
            // TODO
            std::string playerNames[2];
            std::cout << "Player 1 Name: " << std::endl;
            std::cin >> playerNames[0];
            std::cout << "Player 2 Name: " << std::endl;
            std::cin >> playerNames[1];

            std::cin.ignore();

            game* newGame = new game(playerNames);
            newGame->gamePlayLoop();
         break; }
         case 2: { // Load Game
            std::string inputTmp;
            std::string loadGameFile;
            std::cout << "Enter the filename from which load a game" << std::endl;
            // user to enter "save1" (not save1.save)
            std::cin >> inputTmp;

            // check that the file exists
            std::ifstream checkFile;
            checkFile.open(inputTmp + ".save");
            if (checkFile) { 
               // TODO: check file format ***TO CREATE bool saveFileFormatCorrect*** here or Game.cpp?
               // if(saveFileFormatCorrect(checkFile)) { //passing in file OR
               // if(saveFileFormatCorrect(inputTmp + ".save")) { //passing in filename
               if(saveFileFormatCorrect()) { //TESTING
                  loadGameFile = inputTmp + ".save";
                  std::cout << "Qwirkle game successfully loaded" << std::endl;
                  game* newGame = new game(loadGameFile);
                  newGame->gamePlayLoop();
               } else {
                  std::cout << "Error: file not correct format" << std::endl;
               }
            } else {
               std::cout << "Error: file does not exist" << std::endl;
            }
         break; }
         case 3: // Credits (Show student information)
            // TODO - implement credits function
            displayCredits();
            // after printing, return to the main menu
         break;
         case 4: // Quit (also quit on EOF)
            std::cout << "Goodbye" << std::endl;
            inGame = false;
         break;
         default:
            std::cout << "Invalid Input" << std::endl;
            std::cin >> menuSelection;
         break;
   }

   return EXIT_SUCCESS;
   }
}