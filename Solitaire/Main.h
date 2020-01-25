#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>
#include <time.h>
#include <conio.h>
#include <fstream>
#include <sstream>
using namespace std;

//Enums
enum Colors
{
	BLACK, DARKBLUE, DARKGREEN, AQUA, MAROON, DARKPINK, GOLD, LIGHTGRAY, GRAY, BLUE, GREEN, LIGHTBLUE, RED, PINK, YELLOW, WHITE
};

enum ArrowKeys
{
	UP = 72, DOWN = 80, LEFT = 75, RIGHT = 77
};

enum Interface
{
	NONE = -1, DECK = 0, DECK_CARD = 1, COLLECTOR = 2, PLAYCARD = 3
};

//Structs
struct Card
{
	int number;
	int face;
	int color; //black = 240, red = 252, empty = 0
	bool open;
	COORD pos;
};

//Variable declarations
Card deck[52];
Card playCards[52];
Card playDeck[24];
Card collectedCards[4][13];
Card pointedCard;
Card selectedCard = {-1, -1, 0, 0, {0, 0}};

Colors selectedColor = LIGHTBLUE;

char pointer = 27;
char selectedIcon = 17;
char face[4] = {6, 3, 5, 4}; //Spades, Hearts, Clubs, Diamonds

string number[13] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

COORD pointerPos = {0, 0};

bool distribute = true;
bool endGame = false;
bool completed = false;

time_t startTime = -1;
int currTime;
int score;
int currInterface = NONE;
int selCardInterface = NONE;
int showDeckCard = -1;
int lastDeckCard = 23;
int deckCount = 24;

//Function prototypes
void MainScreen();
void InfoScreen();
void EndGame();
bool CheckEndGame();
void QuitGame();
void GameFunctions();
void BuildDeck();
void ShuffleDeck();
void SeperateCards();
void UpdateTime();
void UpdateScore();
void DisplayPlayScreen();
void DistributeCards();
void UpdateCards();
void MovePointer(char input);
void SelectCard(char input);
void Shortcuts(char input);
bool VerifyCardPlacement(Card card);
Card GetLastColumnCard(int x);
void ShowDeck();
void PlaceCard();
void MoveCards();
void CollectedToColumns();
void OpenCard();
void CollectCard(Card card, int slot);
void CollectAllCards();
int FindCollectorSlot(Card card);
void RemoveCard(Card card);
void PrintCard (Card card);
void PrintEmpty (Colors color);
Card EmptyCard();
int GetMaxYPointer();
void SetColor(int color);
int Color(int fontColor, int bgColor);
void MoveCursor(int x, int y);
COORD GetCursorPos();
void HideCursor();