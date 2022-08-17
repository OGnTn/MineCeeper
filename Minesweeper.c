/*
 ============================================================================
 Name        : Minesweeper.c
 Author      : Nathan Pesch
 Version     : 1.0
 Copyright   :
 Description : Minesweeper in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define fieldWidth 10	//These values do not need to be the same, an irregular playing field is possible.
#define fieldHeight 10

int mineAmount = 10;
int flagAmount;

void GetInput();
void NextTurn();		//Blueprints for some functions so that the compiler doesn't complain.
int WithinBounds(int x, int y);

struct Node {
	int isMine;
	int isFlagged;
	int mineAmount;
	int isRevealed;
};
struct Node board[fieldWidth][fieldHeight]; //Initialize the playing field.

void SetMine() {
	int randomX = rand() % fieldWidth;
	int randomY = rand() % fieldHeight;
	if (board[randomX][randomY].isMine == 1) {
		SetMine();			//if the random position already is a mine, choose another position
	}
	else {
		board[randomX][randomY].isMine = 1;		//Set the random position node to a mine
	}
}
void PlaceMines() {
	flagAmount = mineAmount;
	srand(time(NULL)); //generate a new seed every time
	for (int currentMine = 0; currentMine < mineAmount; currentMine++) {
		SetMine();
	}
}
int FindMineCount(int x, int y) {
	int count = 0;
	for (int currentX = x - 1; currentX < x + 2;currentX++) {
		for (int currentY = y - 1; currentY < y + 2; currentY++) {
			if (WithinBounds(currentX, currentY) == 1) {
				if (board[currentX][currentY].isMine == 1) {
					count++; // increment this counter for every mine that is near.
				}
			}
		}
	}
	return count;
}

void AssignMineAmount() {
	for (int currentX = 0; currentX < fieldWidth; currentX++) {
		for (int currentY = 0; currentY < fieldHeight; currentY++) {
			if (board[currentX][currentY].isMine == 0) {
				board[currentX][currentY].mineAmount = FindMineCount(currentX, currentY); // loop through the entire board and assign the amount of mines that are near.
			}
		}
	}
}
int WithinBounds(int x, int y) {
	if (x < fieldWidth && 0 <= x && y < fieldHeight && 0 <= y) {
		return 1; //return 1 if the coordinates are within the bounds of the board array.
	}
	else {
		return 0;
	}
}
void Print() {
	// print the entire board, revealed.
	printf("    ");
	for (int i = 0; i < fieldWidth; i++) {
		printf("%d", i);
		printf("   ");
	}
	printf("\n");
	printf("-----");
	for (int i = 0; i < fieldWidth; i++) {
		printf("----");

	}
	printf("\n");
	for (int y = 0; y < fieldHeight; y++) {
		printf("%d", y);
		printf("|| ");
		for (int x = 0; x < fieldWidth; x++) {
			if (board[x][y].isMine == 1) {
				printf("M | ");
			}
			else {
				printf("%d", board[x][y].mineAmount);
				printf(" | ");
			}
		}
		printf("\n");
	}
	printf("\n");
}
void PrintRevealed() {
	//print only the revealed nodes
	printf("Flags left: %d", flagAmount);
	printf("\n");
	printf("    ");
	for (int i = 0; i < fieldWidth; i++) {
		printf("%d", i);
		printf("   ");
	}
	printf("\n");
	printf("-----");
	for (int i = 0; i < fieldWidth; i++) {
		printf("----");
	}
	printf("\n");
	for (int y = 0; y < fieldHeight; y++) {
		printf("%d", y);
		printf("|| ");
		for (int x = 0; x < fieldWidth; x++) {
			if (board[x][y].isRevealed == 1) {
				printf("%d", board[x][y].mineAmount);
				printf(" | ");
			}
			else if (board[x][y].isFlagged == 1) {
				printf("F | ");
			}
			else {
				printf("  | ");
			}
		}
		printf("\n");
	}
	printf("\n");
}
void CheckWin() {
	//check if the game is won by counting the amount of correctly placed flags. Or by checking if every tile apart from those that are mines are revealed.
	int count = 0;
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			if (board[x][y].isMine == 1 && board[x][y].isFlagged == 1) {
				count++;
			}
		}
	}
	int revealedCount = 0;
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			if (board[x][y].isMine == 0 && board[x][y].isRevealed == 1) {
				revealedCount++;
			}
		}
	}

	if (count == mineAmount || revealedCount == (fieldWidth * fieldHeight) - mineAmount) {
		printf("\n");
		printf("You won! Congratulations! \n");
		Print();
	}
	else {
		NextTurn();
	}
}
void RevealCurrent(int x, int y) {
	//if the node is not yet revealed, recursively loop through the nodes that are near and reveal them.
	if (board[x][y].isRevealed == 0) {
		board[x][y].isRevealed = 1;

		if (board[x][y].mineAmount == 0) {
			int lowerX = x - 1;
			int upperX = x + 2;
			int lowerY = y - 1;
			int upperY = y + 2;

			for (int currentX = lowerX; currentX < upperX;currentX++) {
				for (int currentY = lowerY; currentY < upperY;currentY++) {
					if (WithinBounds(currentX, currentY) == 1) {
						if (board[currentX][currentY].mineAmount == 0) {
							RevealCurrent(currentX, currentY);
						}
						else {
							board[currentX][currentY].isRevealed = 1;
						}
					}
				}
			}
		}
	}
}
void Reveal(int x, int y) {
	if (board[x][y].isFlagged == 1) {
		printf("\n");
		printf("That node is flagged. Unflag it before revealing. \n");
		NextTurn();
	}
	else if (board[x][y].isMine == 1) {
		printf("\n");
		printf("BOOM! You lost. \n");
		Print();
	}
	else {
		RevealCurrent(x, y);
		CheckWin();
	}
}
void Flag(int x, int y) {
	//place or remove a flag
	if (board[x][y].isRevealed == 1) {
		printf("\n That node is already revealed. \n");
		NextTurn();
	}
	else {

		if (board[x][y].isFlagged == 1) {
			board[x][y].isFlagged = 0;
			flagAmount++;
		}
		else {
			board[x][y].isFlagged = 1;
			flagAmount--;
		}
		CheckWin();
	}
}
void GetInput() {
	//get the input and parse it.
	printf("Enter a command: \n");

	char input;
	int x;
	int y;
	scanf(" %c", &input);
	if (input == 'F' || input == 'R') {
		printf("enter an x coordinate: ");
		scanf("%i", &x);
		printf("enter a y coordinate: ");
		scanf("%i", &y);
		if (WithinBounds(x, y) == 1) {
			if (input == 'F') {
				Flag(x, y);
			}
			else {
				Reveal(x, y);
			}
		}
		else {
			printf("Input out of bounds. \n");
			GetInput();
		}

	}
	else if (input == 'P') {
		Print();
		NextTurn();
	}
	else {
		printf("Input not recognised. Maybe you typed it wrong? \n");
		GetInput();
	}
}
void NextTurn() {
	//activate the next turn
	PrintRevealed();
	GetInput();
}
void FirstTurn() {
	printf("Welcome to Minesweeper. Type either R x y or F x y. \n");
	PrintRevealed();
	GetInput();
}

void Initialize() {
	PlaceMines();
	AssignMineAmount();
	FirstTurn();
}
int main(void) {
	Initialize();

	return EXIT_SUCCESS;
}
