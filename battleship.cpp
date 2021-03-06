// Battleship Game
// battleship.cpp
// 04/12/2018
//
// This game allows you to play battleship through the terminal.
// You may play against a friend or against the computer.
//
// Include library files
#include "stdafx.h"
#include "battleship.h"

// Define maximum string length and board row and column lengths.
//
#define MAX_STRING_LEN 80
#define COLS 10
#define ROWS 10

// Function prototypes
//
int getInt(void);			 // Read and return an integer
double getDouble(void);		 // Read and return a double
char getCharacter(void);	 // Read and return a char
void getString(char list[]); // Read a string into "list", a char array input/output parameter.

// Enum for computer player or person
//
typedef enum
{
	human,
	computer
} playertype;

// Create struct to hold player data: name, own (secret) board, and guessed board
//
typedef struct
{
	playertype user;
	int secretGameBoard[ROWS][COLS];
	int guessGameBoard[ROWS][COLS];
} Player;

// Create enum to classify a battleship hit or miss
//
typedef enum
{
	water,
	boat,
	miss,
	hit
} outcome;

// Enum to describe ship placement orientation
//
typedef enum
{
	horizontal,
	vertical
} orientation;

// Enum for ship length
//
typedef enum
{
	destroyer = 2,
	cruiser,
	battleship,
	carrier
} ship;

// Random number generator in range for computer input
// inclusive of range ends
int getrandom(int low, int high)
{
	int randomnumber;
	randomnumber = rand() % (high + 1 - low) + low;
	return randomnumber;
}

// Initialize the game boards
void initializeGameBoards(Player *game)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			(*game).secretGameBoard[i][j] = water;
			(*game).guessGameBoard[i][j] = water;
		}
	}
}
// Print the game board
void printguessgameboard(Player *gameboard)
{
	printf("GUESS BOARD\n");
	int counter = 0;
	printf("    0  1  2  3  4  5  6  7  8  9\n");
	for (int i = 0; i < ROWS; i++)
	{
		printf("\n");
		printf("%d   ", counter);
		counter++;
		for (int j = 0; j < COLS; j++)
		{
			if ((*gameboard).guessGameBoard[i][j] == water)
			{
				printf("~  ");
			}
			else if ((*gameboard).guessGameBoard[i][j] == miss)
			{
				printf("O  ");
			}
			else if ((*gameboard).guessGameBoard[i][j] == hit)
			{
				printf("X  ");
			}
		}
	}
	printf("\n\n");
}
// print the game board
void printsecretgameboard(Player *gameboard)
{
	printf("SECRET BOARD\n");
	int counter = 0;
	printf("    0  1  2  3  4  5  6  7  8  9\n");
	for (int i = 0; i < ROWS; i++)
	{
		printf("\n");
		printf("%d   ", counter);
		counter++;
		for (int j = 0; j < COLS; j++)
		{
			printf("%d  ", (*gameboard).secretGameBoard[i][j]);
		}
	}
	printf("\n\n");
}

// Function to place ship on secret gameboard using the ship type, orientation, and coordinates. returns bool for counter.
//
bool placeship(Player *gameboard, int myship, orientation updown, int xcoord, int ycoord)
{

	// check orientation and reduce rows/cols accordingly for placement
	if (updown == horizontal)
	{
		int newrow = ROWS - myship;
		if (xcoord > newrow)
		{
			printf("\nPlease enter valid coordinates so ship is on board.\n");
			// exit early
			return false;
		}
		// check if valid entry, early exit if not
		int tempxcoord = xcoord;
		for (int i = 0; i < myship; i++)
		{
			if ((*gameboard).secretGameBoard[ycoord][tempxcoord] == boat)
			{
				return false;
			}
			tempxcoord++;
		}
		// set ship in secret game board represented by 'boat' enum
		int counter = 0;
		while (counter < myship)
		{

			// place boat, coordinates are backwards but whatever
			// check if ship already exists at coordinate, return false with message if so
			//
			if ((*gameboard).secretGameBoard[ycoord][xcoord] != boat)
			{
				(*gameboard).secretGameBoard[ycoord][xcoord] = boat;
				xcoord++;
				counter++;
			}
			else
			{
				printf("\nShip already exists at coordinate. Please try valid coords.\n\n");
				return false;
			}
		}
	}
	else if (updown == vertical)
	{
		int newcol = COLS - myship;
		if (ycoord > newcol)
		{
			printf("\nplease enter valid coordinates\n");
			// exit early
			return false;
		}
		// check if valid entry, early exit if invalid
		int tempycoord = ycoord;
		for (int i = 0; i < myship; i++)
		{
			if ((*gameboard).secretGameBoard[tempycoord][xcoord] == boat)
			{
				return false;
			}
			tempycoord++;
		}
		// set ship in secret game board
		int counter = 0;
		while (counter < myship)
		{
			// place boat, coordinates are backwards but whatever
			// check if ship already exists at coordinate, return false with message if so
			//
			if ((*gameboard).secretGameBoard[ycoord][xcoord] != boat)
			{
				(*gameboard).secretGameBoard[ycoord][xcoord] = boat;
				ycoord++;
				counter++;
			}
			else
			{
				printf("\nShip already exists at coordinate. Please try valid coords.\n\n");
				return false;
			}
		}
	}
	return true;
}

// get human user to set all 4 ships
//
void userSetShips(Player *secboard)
{
	// Counter to confirm that ship is set
	//
	int counter = 0;
	int myboat = destroyer; // start at destroyer (2) then iterate to carrier (5)

	// open variables for user to enter data for ship placement
	// set coordinates above 10 to force user to enter valid coordinates
	// placeShip function should catch incorrect coordinates and return 'false' so counter
	// does not increment
	//
	int myorient = 20;
	int xcoord = 20;
	int ycoord = 20;

	// Notify user to place ships
	//
	printf("Hello user, please place your battleships. \n\n");

	// while counter is less than total number of ships to be successfully placed
	while (counter < 4)
	{
		// get user input for ship placement
		printf("Please enter a valid x-coordinate: \n");
		xcoord = getInt();
		printf("Please enter a valid y-coordinate: \n");
		ycoord = getInt();
		printf("Please enter orientation: '0' for horizontal or '1' for vertical: \n");
		myorient = getInt();

		// Convert user input (int) to enum
		orientation theorient = static_cast<orientation>(myorient);

		if (placeship(secboard, myboat, theorient, xcoord, ycoord) == true)
		{
			counter++;
			myboat++;
		}
		// show user secretgameboard after placement of a single ship
		//
		printsecretgameboard(secboard);
	}
}

// get computer to set all 4 ships
//
void computerSetShips(Player *secboard)
{
	// Counter to confirm that ship is set

	int counter = 0;
	int myboat = destroyer; // start at destroyer (2) then iterate to carrier (5)

	// open variables for user to enter data for ship placement
	// set coordinates above 10 to force user to enter valid coordinates
	// placeShip function should catch incorrect coordinates and return 'false' so counter
	// does not increment
	//
	int myorient = 20;
	int xcoord = 20;
	int ycoord = 20;

	// while counter is less than total number of ships to be successfully placed
	while (counter < 4)
	{

		// have computer select random numbers
		//
		xcoord = getrandom(0, 9);
		ycoord = getrandom(0, 9);
		myorient = getrandom(0, 1);

		// Convert user input (int) to enum
		orientation theorient = static_cast<orientation>(myorient);

		if (placeship(secboard, myboat, theorient, xcoord, ycoord) == true)
		{
			counter++;
			myboat++;
		}
		// show user secretgameboard after placement of a single ship
		// this is for demo purposes and should be removed if actually playing
		//
		printsecretgameboard(secboard);
	}
	printf("\n Computer has placed ships. \n\n");
}

// Function to check if guess coordinates were a match with the secretboard,
// if so modify guessboard
//
void humanGuessCoords(Player *gameboard, int *hitcounter)
{
	// declare variables as invalid input then require user to select from 0 to 9
	int guessx = 20;
	int guessy = 20;
	int counter = 0;

	// Ensure valid coordinates and a location not previously guessed
	while ((guessx < 0 && guessx > 10) || (guessy < 0 && guessy > 10) || (counter == 0))
	{
		// reset counter
		counter = 0;

		// get user input for guess
		printf("\nEnter coordinates to shoot your opponent's battleship!\n");
		printf("\nPlease enter a valid x-coordinate: \n");
		guessy = getInt();
		printf("Please enter a valid y-coordinate: \n");
		guessx = getInt();

		// check if location already guessed
		//
		if (((*gameboard).guessGameBoard[guessx][guessy] == miss) || ((*gameboard).guessGameBoard[guessx][guessy] == hit))
		{
			// intentionally mess up a coordinate to remain in while loop
			guessx = 20;
			printf("\nYou have entered a previously entered coordinate, please try again. \n");
		}
		// check if a ship is present
		else if ((*gameboard).secretGameBoard[guessx][guessy] == boat)
		{
			// guessboard gets adjusted to be a hit
			(*gameboard).guessGameBoard[guessx][guessy] = hit;
			counter++;
			// increment hit counter if hit
			(*hitcounter)++;
		}
		// else it is water
		else
		{
			(*gameboard).guessGameBoard[guessx][guessy] = miss;
			// increment counter since valid shot
			counter++;
		}
	}
}

// Computer guesscoords function
//
void computerGuessCoords(Player *gameboard, int *hitcounter)
{
	// declare variables as invalid input then require user to select from 0 to 9
	int guessx = 20;
	int guessy = 20;
	int counter = 0;

	// Ensure valid coordinates and a location not previously guessed
	while ((guessx < 0 && guessx > 10) || (guessy < 0 && guessy > 10) || (counter == 0))
	{
		// reset counter
		counter = 0;

		// get random number guess
		//
		guessx = getrandom(0, 9);
		guessy = getrandom(0, 9);

		// check if location already guessed
		//
		if (((*gameboard).guessGameBoard[guessx][guessy] == miss) || ((*gameboard).guessGameBoard[guessx][guessy] == hit))
		{
			// intentionally mess up a coordinate to remain in while loop
			guessx = 20;
		}
		// check if a ship is present
		else if ((*gameboard).secretGameBoard[guessx][guessy] == boat)
		{
			// guessboard gets adjusted to be a hit
			(*gameboard).guessGameBoard[guessx][guessy] = hit;
			counter++;
			// increment hit counter if hit
			(*hitcounter)++;
		}
		// else it is water
		else
		{
			(*gameboard).guessGameBoard[guessx][guessy] = miss;
			// increment counter since valid shot
			counter++;
		}
	}
}

// Function to play the game, each player guesses until all ships are hit.
// Make sure to print guess board and if user wins, print all boards.
//
bool playgame(Player *human, Player *computer)
{

	// declare variable for end-of-game hit count and a standard hit count
	//
	int totalhitcount = destroyer + cruiser + battleship + carrier;
	int hitcounthuman = 0;
	int hitcountcomp = 0;

	// Set ships
	//
	userSetShips(human);
	computerSetShips(computer);

	// Enter infinite while loop to guess coordinates until game over
	//
	while (true)
	{

		humanGuessCoords(computer, &hitcounthuman);
		printf("Computer's Board\n");
		printguessgameboard(computer);
		computerGuessCoords(human, &hitcountcomp);
		printf("Human's Board\n");
		printguessgameboard(human);

		printf("Hit count human %d \n", hitcounthuman);
		printf("Hit count comp %d \n\n", hitcountcomp);
		// Check if human won
		//
		if (hitcounthuman == totalhitcount)
		{
			printf("\n\n*****\n\n THE HUMAN HAS SUCCEEDED IN DEFEATING THE COMPUTER !! \n\n Congratulations! \n\n*****\n");
			break;
		}
		// check if computer won
		//
		if (hitcountcomp == totalhitcount)
		{
			printf("\n\n*****\n\n BEEP BOOP ROBOT WON BOO BOO BEEP. \n\n*****\n");
			break;
		}
	}
	return true;
}

// main function
//
int main()
{
	// declare two players, first human, second computer
	//
	Player player1{};
	Player player2{};

	// initialize both gameboards
	//
	initializeGameBoards(&player1);
	initializeGameBoards(&player2);

	// play the game of Battleship, you vs. computer
	//
	playgame(&player1, &player2);

	// exit program
	//
	return 0;
}
// end of program
