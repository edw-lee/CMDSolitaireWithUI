#include "Main.h"
using namespace std;

const int PLAY_INFO_STARTROW = 22;
const int PLAY_INFO_STARTCOL = 1;

void main()
{
	HideCursor();//hides the console cursor

	MainScreen();//Display the main screen

	srand(time(NULL));

	BuildDeck();//Generates the value of a deck
	ShuffleDeck();//Shuffle the generated deck
	SeperateCards();//Split the cards to Stock and Tableu
	showDeckCard = -1;//No card from the Stock(deck) will be displayed

	//Loop until game ends
	do
	{
		OpenCard();//Check if any card in the Tableu needs to be flipped
		DisplayPlayScreen();//Display the game interface
		GameFunctions();//Manage game keys, time and score
	} while (endGame == false);

	if (!completed)
		system("cls");

	Sleep(1500);

	EndGame();//Display end game screen
}

//Displays main screen
void MainScreen()
{
	ifstream mainScreenText;
	mainScreenText.open("TextFiles\\MainScreen.txt");//Read the text file from the main screen
	string text;

	//Display all the text in the MainScreen.txt file
	if (mainScreenText.is_open())
	{
		while (!mainScreenText.eof())
		{
			getline(mainScreenText, text, '\n');
			cout << text << endl;
		}

		mainScreenText.close();
	}

	char input;

	//Loop until one of the keys is pressed
	do
	{
		input = getch();
		switch (input)
		{
		case '\r'://ENTER - to start game(break out of the loop)
			break;
		case 'i'://Show information of the game
			InfoScreen();
			MainScreen();//Call the MainScreen function to clear the console and refresh the main screen
			break;
		default:
			input = NULL;
		}
	} while (!input);

	system("cls");
}

//Display infomation screen
void InfoScreen()
{
	system("cls");
	ifstream helpText;
	helpText.open("TextFiles\\HelpScreen.txt");//Read the text file from the information screen
	string text;

	//Display all the text in the HelpScreen.txt file
	if (helpText.is_open())
	{
		while (!helpText.eof())
		{
			getline(helpText, text, '\n');
			cout << text << endl;
		}

		helpText.close();
	}

	char input;

	//Loop until one of the keys is pressed
	do
	{
		input = getch();
		switch (input)
		{
		case '\t'://TAB to back to the main screen(breaks out of the loop and return to MainScreen() function)
			break;
		default:
			input = NULL;
		}
	} while (!input);

	system("cls");
}

//Display end game screen
void EndGame()
{
	system("cls");
	ifstream endGameText;
	endGameText.open("TextFiles\\EndGame.txt");//Read the text file from the end game screen
	string text, title, start;

	//Set the position to start display the text
	if (completed)
		start = "-won-";
	else
		start = "-gameover-";

	//Displays text until '-end-' is reached
	if (endGameText.is_open())
	{
		while (!endGameText.eof())
		{
			while (title != start)
				getline(endGameText, title, '\n');

			getline(endGameText, text, '\n');
			if (text == "-end-")
				break;

			cout << text << endl;
		}
	}

	endGameText.close();

	cout << endl << endl;

	//Displays the score and time
	if (completed)
	{
		cout << "\t\t\t\t  Score: " << score << endl << endl;
		cout << "\t\t\t\t  Time: " << currTime << "s" << endl << endl;
	}
	else
	{
		cout << "\t\t\t\t\tScore: " << score << endl << endl;
		cout << "\t\t\t\t\tTime: " << currTime << "s" << endl << endl;
	}

	cout << endl << endl << endl;

	Sleep(2000);
}

//Checks if the game has ended(completed the foundation)
bool CheckEndGame()
{
	//Check if the Foundation has all the Kings
	if (collectedCards[0][12].number == 12 && collectedCards[1][12].number == 12 && collectedCards[2][12].number == 12 && collectedCards[3][12].number == 12)
	{
		endGame = true;
		completed = true;
		return true;
	}

	return false;
}

//Quit game?
void QuitGame()
{
	MoveCursor(70, 11);
	cout << "> Quit game?(Y/N) <";

	char input;

	//Loop until one of the key is pressed
	do
	{
		//Updates time and score if the no keys were pressed
		while (!kbhit())
		{
			UpdateTime();
			UpdateScore();
		}

		input = getch();
		switch (tolower(input))
		{
		case 'y'://End game if 'yes'
			endGame = true;
			break;
		default://Other keys - clear quit game text and resume game
			MoveCursor(70, 11);
			cout << "                                    ";
			break;
		}
	} while (!input);
}

//Manage the game functionalities
void GameFunctions()
{
	//Check if game ended - if true, return to main()
	if (CheckEndGame())
		return;

	//Loop until any key is pressed
	while (!kbhit())
	{
		//Updates time and score if start time was set
		if (startTime >= 0)
		{
			UpdateTime();
			UpdateScore();
		}
	}

	char input = getch();
	MovePointer(input);//Moves pointer if arrow key was pressed
	SelectCard(input);//Select card if ENTER was pressed
	Shortcuts(input);//Run shortcut functions if one of the shortcut key was pressed
}

//Creates an arranged deck 
void BuildDeck()
{
	for (int i = 0; i < 52; i++)
	{
		deck[i].face = i / 13;
		deck[i].number = i % 13;
		int color = ((i / 13) % 2 == 0) ? Color(BLACK, WHITE) : Color(RED, WHITE);
		deck[i].color = color;
		deck[i].open = true;
	}
}

//Shuffle the newly created deck
void ShuffleDeck()
{
	int randomCard;
	Card tempDeck[52];

	for (int i = 0; i < 52; i++)
	{
		/*Loop until the randomized card is not an
		/empty array slot in the deck*/
		do
		{
			randomCard = rand() % 52;
		} while (deck[randomCard].color == 0);

		//Assign the randomized card to a temporary deck array
		tempDeck[i] = deck[randomCard];

		//Remove the card from the deck so that it will not be repeated
		deck[randomCard] = EmptyCard();
	}

	//Copy the temporary deck order to the main deck
	memcpy(deck, tempDeck, sizeof tempDeck);
}

//Seperate the shuffled deck to Stock(deck) and Tableu(cards on the table)
void SeperateCards()
{
	for (int i = 0; i < 52; i++)
	{
		if (i < 28)
			playCards[i] = deck[i];
		else
		{
			playDeck[i - 28] = deck[i];
			playCards[i] = EmptyCard();
		}
	}
}

//Update and display time
void UpdateTime()
{
	//Updates every 1 second - return if less than 1 second
	if (difftime(difftime(time(0), startTime), currTime) < 1 && startTime >= 0)
		return;

	MoveCursor(51, 0);

	/*Set the difference between current system time with
	the time the game was started as the game time*/
	currTime = difftime(time(0), startTime);

	cout << "Time: " << currTime << "s";

	//Deduct score by 2 every 30 seconds
	if (currTime > 0 && currTime % 30 == 0)
		score -= 2;
}

//Update and display score
void UpdateScore()
{
	MoveCursor(0, 0);

	if (score <= 0)
		score = 0;

	cout << "Score: " << score << "          ";
}

//Display the main game interface
void DisplayPlayScreen()
{
	//Print score
	UpdateScore();

	//Print time
	UpdateTime();

	MoveCursor(0, 2);

	//Print Stock(deck)
	if (deckCount > 0)
	{
		//Print Aqua colored box(back of the card) if it is not the last card 
		if (showDeckCard < lastDeckCard)
			PrintEmpty(AQUA);
		else//Print Black(nothing) if it is the last card
			PrintEmpty(BLACK);

	}
	else//If no card in Stock - print nothing
		PrintEmpty(BLACK);

	//Show pointer if it is pointing Stock else show empty space
	if (pointerPos.X == 0 && pointerPos.Y == 0)
		cout << pointer;
	else
		cout << " ";

	//Print Waste(card) from (Stock)deck
	if (showDeckCard >= 0)
		PrintCard(playDeck[showDeckCard]);//Show card
	else
		PrintCard(EmptyCard());//Show nothing

	//Show pointer if it is pointing Waste
	if (pointerPos.X == 1 && pointerPos.Y == 0)
	{
		//If Waste is selected - print blue pointer 
		if (selectedCard.face == playDeck[showDeckCard].face && selectedCard.number == playDeck[showDeckCard].number)
		{
			SetColor(Color(selectedColor, BLACK));//selected color = blue
			cout << pointer;
			SetColor(Color(WHITE, BLACK));//return to default console color
		}
		else
		{
			if (showDeckCard >= 0)//If not selected - print white pointer
				cout << pointer;
			else//if no card is shown, skip pointer to Tableu
				cout << " ";
		}
	}
	else//If not pointing at Waste
	{
		if (showDeckCard >= 0)
		{
			//Print blue arrow if selected 
			if (selectedCard.face == playDeck[showDeckCard].face && selectedCard.number == playDeck[showDeckCard].number)
			{
				SetColor(Color(selectedColor, BLACK));
				cout << selectedIcon;
				SetColor(Color(WHITE, BLACK));
			}
			else//Print space if not selected
				cout << " ";
		}
		else//print space if not card is shown in Waste
			cout << " ";
	}

	//Print Foundation
	MoveCursor(27, GetCursorPos().Y);

	//Set the value of first slot of Foundation as empty card if it is empty
	for (int i = 0; i < 4; i++)
	{
		if (collectedCards[i][0].color == EmptyCard().color)
			collectedCards[i][0] = EmptyCard();
	}

	/*Go through each slot in each Foundation slot until an empty card
	is found and print the card in the previous slot*/
	for (int i = 0; i < 4; i++)//Go through each Foundation slot
	{
		Card topCard = EmptyCard();
		for (int j = 0; j < 13; j++)//Go through each slot in selected Foundation slot
		{
			if (collectedCards[i][j].color == EmptyCard().color)//If the slot is empty
			{
				//If it is not the first slot print the card in the previous slot
				if (j > 0)
				{
					PrintCard(collectedCards[i][j - 1]);
					topCard = collectedCards[i][j - 1];
				}
				else//If it is the first slot, print empty slot
					PrintEmpty(LIGHTGRAY);
				break;
			}

			/*(If the loop did not break)At last slot,
			print last card(King) if it is not empty*/
			if (j == 12)
			{
				if (collectedCards[i][j].color != EmptyCard().color)
					PrintCard(collectedCards[i][j]);
			}
		}

		//Show pointer if it is pointing a Foundation slot
		if (pointerPos.X == 2 + i && pointerPos.Y == 0)
		{
			if (selectedCard.color != EmptyCard().color)//If selected card is not empty
			{
				if (selectedCard.number == topCard.number && selectedCard.face == topCard.face)//If it is a card in the Foundation slot
					SetColor(Color(selectedColor, BLACK));//Set console color - blue foreground and black background
				else//If it is not a card in the Foundation slot
					SetColor(Color(WHITE, BLACK));//Set default console color
			}
			cout << pointer;//Print pointer
			SetColor(Color(WHITE, BLACK));//Set back to default console color
		}
		else//If not pointing at Foundation
		{
			if (selectedCard.number == topCard.number && selectedCard.face == topCard.face)//If selected card is the card in the Foundation slot
			{
				if (selectedCard.color != EmptyCard().color)//If selected card is not empty - for reassurance purpose
				{
					SetColor(Color(selectedColor, BLACK));
					cout << selectedIcon;
					SetColor(Color(WHITE, BLACK));
				}
				else//If it is empty, print space
					cout << " ";
			}
			else//If not a card from the Foundation slot
				cout << " ";
		}
	}

	if (completed)//If game completed, end game
	{
		endGame = true;
		return;
	}

	cout << endl << endl << endl;

	//Print Tableu (7 columns of cards)
	if (distribute)//If card needs distribution, distribute card else update cards' positions
	{
		DistributeCards();
		distribute = false;
	}
	else
		UpdateCards();

	//Print shortcut keys instructions
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW);

	cout << "= Arrow keys - move pointer";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 1);
	cout << "= ENTER - select";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 2);
	cout << "= D - show Stock(deck)";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 3);
	cout << "= C - Tableu to Foundation";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 4);
	cout << "= Q - pointer to Waste(flipped deck card).";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 5);
	cout << "= W,E,R,T - pointer to Foundation slots (1 -> 4)";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 6);
	cout << "= 1 -> 7 - move pointer to Tableu";
	MoveCursor(PLAY_INFO_STARTCOL, PLAY_INFO_STARTROW + 7);
	cout << "= TAB - quit game";
}

//Distribute the cards to Tableu
void DistributeCards()
{
	int counter = 0;
	for (int i = 0; i < 7; i++) //Loop through Y position
	{
		for (int j = 0; j < 7; j++) //Loop through X position
		{
			if (j < i)//Fills up empty spaces at the beginning of each row
			{
				PrintEmpty(BLACK);
				cout << " ";
			}
			else
			{
				if (j > i)
					playCards[counter].open = false;//Close cards if it is not the last card in the column

				PrintCard(playCards[counter]);

				playCards[counter].pos.X = j;//Set the x position
				playCards[counter].pos.Y = i + 1;//Set the y position

				cout << " ";

				counter++;
			}
		}
		cout << endl << endl;
	}
}

//Update the Tableu interface
void UpdateCards()
{
	for (int i = 1; i <= 19; i++)//Loop through Y position
	{
		for (int j = 0; j < 7; j++)//Loop through X position
		{
			//Loop through every card in the Tableu array(playCards) until the card match the positions(j and i)
			for (int k = 0; k < 52; k++)
			{
				if (playCards[k].pos.X == j && playCards[k].pos.Y == i)
				{
					PrintCard(playCards[k]);//Display the card

					//Show the pointer if it is pointing at the card
					if (pointerPos.X == playCards[k].pos.X && pointerPos.Y == playCards[k].pos.Y)
					{
						pointedCard = playCards[k];//Assign the card values to a variable for comparisons in other functions

						//If selected card is this card, show blue pointer
						if (selectedCard.pos.X == pointedCard.pos.X && selectedCard.pos.Y == pointedCard.pos.Y)
						{
							SetColor(Color(selectedColor, BLACK));//Set color to selected pointer color - blue
							cout << pointer;
							SetColor(Color(WHITE, BLACK));
						}
						else//If not selected, show white pointer
							cout << pointer;
					}
					else//If not pointing at the card
					{
						//If selected card is this card, show blue arrow 
						if (selectedCard.pos.X == playCards[k].pos.X && selectedCard.pos.Y == playCards[k].pos.Y)
						{
							SetColor(Color(selectedColor, BLACK));
							cout << selectedIcon;
							SetColor(Color(WHITE, BLACK));
						}
						else//If not selected show space
							cout << " ";
					}

					break;
				}

				//If no card at this position (current i(y) and j(x)),print empty space
				if (k == 51)
				{
					PrintEmpty(BLACK);//Display black box
					if (pointerPos.X == j && pointerPos.Y == i)//If pointer at this position, show pointer
					{
						pointedCard = EmptyCard();
						cout << pointer;
					}
					else//If pointer not at this position, show empty space
						cout << " ";
				}
			}
		}
		cout << endl << endl;
	}
}

//Updates pointer position
void MovePointer(char input)
{
	if (input == 0 || input == -32)
	{
		input = getch();

		if (input == LEFT)
		{
			if (pointerPos.Y == 0)//At the first row(Stock, Waste, Foundation row)
			{
				if (pointerPos.X > 0)//Prevents negative value
					pointerPos.X--;

				/*If no card(Waste) is showing from the deck and pointer is pointing at it,
				decrease pointer x position by one (to Stock(deck) position)*/
				if (pointerPos.X == 1 && showDeckCard < 0)
					pointerPos.X--;
			}
			else//At Tableu row
			{
				if (pointerPos.X > 0)//Prevents negative value
					pointerPos.X--;

				//Automatically go to last card if the pointer y poisition is more than the selected column
				if (pointerPos.Y > GetMaxYPointer())
					pointerPos.Y = GetMaxYPointer();
			}
		}
		else if (input == RIGHT)
		{
			if (pointerPos.Y == 0)
			{
				if (pointerPos.X < 5)//Prevents x value to be more than 6
					pointerPos.X++;

				/*If no card(Waste) is showing from the deck and pointer is pointing at it,
				increase pointer x position by one (to Stock(deck) position)*/
				if (pointerPos.X == 1 && showDeckCard < 0)
					pointerPos.X++;
			}
			else
			{
				if (pointerPos.X < 6)//Prevents x value to be more than 7
					pointerPos.X++;

				//Automatically go to last card if the pointer y poisition is more than the selected column
				if (pointerPos.Y > GetMaxYPointer())
					pointerPos.Y = GetMaxYPointer();
			}
		}
		else if (input == UP)
		{
			//To adjust the pointer so that the position matches the first row(Stock, Waste, Foundation row)
			if (pointerPos.Y == 1)
			{
				if (pointerPos.X >= 3)
					pointerPos.X--;
				else
				{
					if (showDeckCard < 0)
						pointerPos.X = 0;
					else
					{
						if (pointerPos.X == 2)
							pointerPos.X--;
					}
				}

			}

			if (pointerPos.Y > 0)//Prevents x value to be less than 0
				pointerPos.Y--;
		}
		else if (input == DOWN)
		{
			//To adjust the pointer so that the position matches the Tableu first row
			if (pointerPos.Y == 0)
			{
				if (pointerPos.X >= 2)
					pointerPos.X++;
			}

			if (pointerPos.Y < GetMaxYPointer())//Prevents pointer from going down to more than the last card
				pointerPos.Y++;
		}
	}

	//If there is no card in Stock, skip to Foundation first slot
	if (pointerPos.X == 0 && pointerPos.Y == 0)
	{
		if (lastDeckCard <= 0)
			pointerPos.X = 2;
	}
}

//ENTER key functions
void SelectCard(char input)
{
	if (input == '\r')//If ENTER is pressed
	{
		//Sets selected interfaces
		if (pointerPos.Y == 0)
		{
			switch (pointerPos.X)
			{
			case 0:
				currInterface = DECK;
				break;
			case 1:
				currInterface = DECK_CARD;
				break;
			default:
				currInterface = COLLECTOR;
				break;
			}
		}
		else
			currInterface = PLAYCARD;


		//Set ENTER function for selected interface
		switch (currInterface)
		{
		case DECK://At Stock
			ShowDeck();//Show card from Stock(deck)
			break;
		case DECK_CARD://At Waste
			//If selected card is the current Waste card, reset selected card to nothing
			if (selectedCard.face == playDeck[showDeckCard].face && selectedCard.number == playDeck[showDeckCard].number)
			{
				selectedCard = EmptyCard();
				break;
			}

			//If selected card is not the current Waste card, set the selected card as the current Waste card
			if (selectedCard.face != playDeck[showDeckCard].face || selectedCard.number != playDeck[showDeckCard].number)
			{
				selectedCard = playDeck[showDeckCard];
				selCardInterface = DECK_CARD;//Sets the selected card interface
			}
			break;
		case PLAYCARD://At Tableu
			//If selected card is the current pointed card, reset to nothing
			if (selectedCard.face == pointedCard.face && selectedCard.number == pointedCard.number)
			{
				selectedCard = EmptyCard();
				break;
			}

			//Selects the pointed card if nothing is selected
			if (selectedCard.color == EmptyCard().color)
			{
				if (pointedCard.open)
				{
					selectedCard = pointedCard;
					selCardInterface = PLAYCARD;//Sets the selected card interface
				}
			}
			else//If a card is selected
			{
				if (selCardInterface == PLAYCARD)//If the selected card is from Tableu
					MoveCards();//Move cards from Tableu to a selected place

				if (selCardInterface == DECK_CARD)//If the selected card is from Waste
					PlaceCard();//Place card from Waste to Tableu

				if (selCardInterface == COLLECTOR)//If the selected card is from Foundation
					CollectedToColumns();//Place card from Foundation to Tableu
			}
			break;
		case COLLECTOR://At Foundation
			if (selectedCard.color != EmptyCard().color)//If a card is selected
			{
				CollectCard(selectedCard, pointerPos.X - 2);//Try collecting the card to Foundation
				selectedCard = EmptyCard();//Reset the selected card to nothing
			}
			else//If nothign is selected
			{
				//Go through the loop to find the last card in the Foundation array
				for (int i = 0; i < 13; i++)
				{
					if (collectedCards[pointerPos.X - 2][i].color == EmptyCard().color)
					{
						selectedCard = collectedCards[pointerPos.X - 2][i - 1];//Selected the last card in the Foundation slot
						selCardInterface = COLLECTOR;//Sets the selected card interface
						break;
					}
				}
			}
			break;
		}
	}
}

//Shortcut keys functions
void Shortcuts(char input)
{
	if (input == 0 || input == -32)
		return;

	if (!isdigit(input))//If the entered key is not a number
	{
		switch (tolower(input))
		{
		case 'c'://Collect all valid cards from Tableu to Foundation
			CollectAllCards();
			break;
		case 'd'://Show card from Stock
			ShowDeck();
			break;
		case 'q'://Move pointer to Waste
			if (showDeckCard >= 0)
			{
				pointerPos.Y = 0;
				pointerPos.X = 1;
			}
			break;
		case 'w'://Move pointer to Foundation 1st slot
			pointerPos.Y = 0;
			pointerPos.X = 2;
			break;
		case 'e'://Move pointer to Foundation 2nd slot
			pointerPos.Y = 0;
			pointerPos.X = 3;
			break;
		case 'r'://Move pointer to Foundation 3rd slot
			pointerPos.Y = 0;
			pointerPos.X = 4;
			break;
		case 't'://Move pointer to Foundation 4th slot
			pointerPos.Y = 0;
			pointerPos.X = 5;
			break;
		case '\t'://TAB - quit game
			QuitGame();
			break;
		}
	}
	else//If it is a number
	{
		if (input - 49 < 7 && input - 49 >= 0)//Ignore numbers more than 7 and less than 1
		{
			pointerPos.Y = 1;
			pointerPos.X = input - 49;
		}
	}
}

//Verify card placement validity
bool VerifyCardPlacement(Card card)
{
	//If it is a King, can only place on empty slots
	if (selectedCard.number == 12)
	{
		if (card.color == EmptyCard().color)
			return true;
		else
			return false;
	}

	//Check if the selected card color and number is valid compared to the pointer card
	if (selectedCard.color != card.color && selectedCard.number == card.number - 1)
		return true;
	else
		return false;
}

//Get the last card of the selected column
Card GetLastColumnCard(int x)
{
	int y = 0;
	Card lastColumnCard = EmptyCard();

	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].pos.X == x)
		{
			if (playCards[i].pos.Y > y)
			{
				y = playCards[i].pos.Y;
				lastColumnCard = playCards[i];
			}
		}
	}

	return lastColumnCard;
}

//Check if the card is the last card of the selected column
bool IsLastCardIncolumn(Card card)
{
	if (card.face == GetLastColumnCard(card.pos.X).face && card.number == GetLastColumnCard(card.pos.X).number)
		return true;
	else
		return false;
}

//Show a card from the Stock(deck)
void ShowDeck()
{
	//Sets the start time if the game has just started
	if (startTime == -1)
		time(&startTime);

	selectedCard = EmptyCard();//Resets selected card to nothing

	//If it is not the last card from the Stock(deck) being shown
	if (showDeckCard < lastDeckCard)
	{
		//Loop through until a card after the current showing card is found
		do
		{
			if (showDeckCard == lastDeckCard)//If it is the last card, show nothing
			{
				showDeckCard = -1;
				score -= 100;
				break;
			}
			showDeckCard++;
		} while (playDeck[showDeckCard].color == EmptyCard().color);
	}
	else//If it is the last card from the Stock(deck) show nothing
	{
		showDeckCard = -1;
		score -= 100;
	}

	//If no card is being shown and pointer is pointed at Waste, set pointer to point at Stock
	if (showDeckCard < 0)
	{
		if (pointerPos.Y == 0 && pointerPos.X == 1)
			pointerPos.X = 0;
	}
}

//Place a card from Stock(deck) to Tableu
void PlaceCard()
{
	Card lastColumnCard = GetLastColumnCard(pointedCard.pos.X);

	//If selected card is not from Waste, return
	if (playDeck[showDeckCard].face != selectedCard.face || playDeck[showDeckCard].number != selectedCard.number)
		return;

	//Check if card placement is valid
	if (!VerifyCardPlacement(lastColumnCard))
	{
		selectedCard = EmptyCard();
		return;
	}

	//Find empty slot in Tableu array for new card
	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].color == EmptyCard().color)
		{
			if (pointedCard.color != EmptyCard().color)
			{
				//Set the selected card placement to the pointed card column and as the last card
				selectedCard.pos.X = lastColumnCard.pos.X;
				selectedCard.pos.Y = lastColumnCard.pos.Y + 1;
			}
			else//If the pointed slot is empty, set the selected card position as the empty slot position
				selectedCard.pos = pointerPos;

			playCards[i] = selectedCard;//Fill in the empty slot

			//Remove card from the card's original array
			RemoveCard(selectedCard);

			score += 5;

			break;
		}
	}

	selectedCard = EmptyCard();//Resets selected card to nothing

	//Set start time if game just started
	if (startTime == -1)
		time(&startTime);
}

//Move the cards from one column to another
void MoveCards()
{
	Card lastColumnCard = GetLastColumnCard(pointedCard.pos.X);

	//Check if selected card is from Tableu, return if it is not
	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].face == selectedCard.face && playCards[i].number == selectedCard.number)
			break;

		if (i == 51)
			return;
	}

	//Verify card placement
	if (!VerifyCardPlacement(lastColumnCard))
	{
		selectedCard = EmptyCard();
		return;
	}

	//Find the selected card from the Tableu array
	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].pos.X == selectedCard.pos.X && playCards[i].pos.Y >= selectedCard.pos.Y)
		{
			//Change the position of the selected card
			if (pointedCard.color != EmptyCard().color)
			{
				playCards[i].pos.X = lastColumnCard.pos.X;
				playCards[i].pos.Y = lastColumnCard.pos.Y + 1 + (playCards[i].pos.Y - selectedCard.pos.Y);
			}
			else//For King placement
			{
				playCards[i].pos.X = pointerPos.X;
				playCards[i].pos.Y = pointerPos.Y + (playCards[i].pos.Y - selectedCard.pos.Y);
			}

		}
	}

	//Set start time if game just started
	if (startTime == -1)
		time(&startTime);

	//Resets selected card to nothing
	selectedCard = EmptyCard();
}

//Move cards from foundation to selected column
void CollectedToColumns()
{
	Card lastColumnCard = GetLastColumnCard(pointedCard.pos.X);

	//Check if selected card is from Foundation
	for (int i = 0; i < 4; i++)
	{
		if (collectedCards[i][0].color == EmptyCard().color)
			continue;

		for (int j = 0; j < 13; j++)
		{
			if (collectedCards[i][j].color != EmptyCard().color)
			{
				if (collectedCards[i][j].face == selectedCard.face && collectedCards[i][j].number == selectedCard.number)
					break;
			}
		}
	}

	//Verify card placement
	if (!VerifyCardPlacement(lastColumnCard))
	{
		selectedCard = EmptyCard();
		return;
	}

	//Find empty slot in Tableu array for new card
	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].color == EmptyCard().color)
		{
			if (pointedCard.color != EmptyCard().color)
			{
				//Set the selected card placement to the pointed card column and as the last card
				selectedCard.pos.X = lastColumnCard.pos.X;
				selectedCard.pos.Y = lastColumnCard.pos.Y + 1;
			}
			else//If the pointed slot is empty, set the selected card position as the empty slot position
				selectedCard.pos = pointerPos;

			playCards[i] = selectedCard;//Fill in the empty slot

			//Remove card from the card's original array
			RemoveCard(selectedCard);

			score -= 15;

			break;
		}
	}

	//Resets selected card to nothing
	selectedCard = EmptyCard();
}

//Flip turn over cards if it is the last card of the column 
void OpenCard()
{
	for (int i = 0; i < 52; i++)
	{
		if (IsLastCardIncolumn(playCards[i]))
		{
			if (!playCards[i].open)
			{
				playCards[i].open = true;
				score += 5;
			}
		}
	}
}

//Move selected cards from Tableu to Foundation
void CollectCard(Card card, int slot)
{
	//Check if card to be collected is from Foundation, if it is return
	for (int i = 0; i < 4; i++)
	{
		if (collectedCards[i][0].color == EmptyCard().color)
			continue;

		for (int j = 0; j < 13; j++)
		{
			if (collectedCards[i][j].color != EmptyCard().color)
			{
				if (collectedCards[i][j].face == card.face && collectedCards[i][j].number == card.number)
					return;
			}
		}
	}


	if (card.number == 0)//If the card is an Ace
	{
		//Place the card in the first empty slot in the selected Foundation slot
		if (collectedCards[slot][0].color == EmptyCard().color)
		{
			collectedCards[slot][0] = card;
			RemoveCard(card);
			score += 10;

			//Set the pointer position to card before the collected card
			if (card.pos.Y == pointerPos.Y && card.pos.X == pointerPos.X)
				if (pointerPos.Y > 1)
					pointerPos.Y--;
		}
	}
	else//Other cards
	{
		//If selected slot is empty, return(cards with other values cannot be placed in an empty slot)
		if (collectedCards[slot][0].color == EmptyCard().color) {
			return;
		}

		//If the selected slot has the same face as the selected card
		if (collectedCards[slot][0].face == card.face)
		{
			//Check if the last card of the Foundation slot has a number less that the selected card
			for (int i = 1; i < 13; i++)
			{				
				if (collectedCards[slot][i - 1].number == card.number - 1)
				{
					collectedCards[slot][i] = card;
					RemoveCard(card);
					score += 10;					
					//Set the pointer position to card before the collected card
					if (card.pos.Y == pointerPos.Y && card.pos.X == pointerPos.X)
						if (pointerPos.Y > 1)
							pointerPos.Y--;
					break;
				}
			}
		}
	}

	//Set start time if game just started
	if (startTime == -1)
		time(&startTime);

	//Resets selected card to nothing
	selectedCard = EmptyCard();
}

//Move all valid cards to Foundation
void CollectAllCards()
{
	for (int i = 0; i < 4; i++)//Loops through each face
	{
		for (int j = 0; j < 13; j++)//Loops through each number
		{
			//Check and see if the card from Waste can be placed in Foundation
			if (playDeck[showDeckCard].face == i && playDeck[showDeckCard].number == j)
			{
				selCardInterface = DECK_CARD;
				CollectCard(playDeck[showDeckCard], FindCollectorSlot(playDeck[showDeckCard]));
			}

			//Check and see if any last card of a column from the Tableu can be placed in Foundation
			for (int k = 0; k < 52; k++)
			{
				if (playCards[k].open)
				{
					if (playCards[k].face == i && playCards[k].number == j)
					{
						Card card = playCards[k];
						if (card.face == GetLastColumnCard(card.pos.X).face && card.number == GetLastColumnCard(card.pos.X).number)
						{
							selCardInterface = PLAYCARD;
							CollectCard(card, FindCollectorSlot(card));
							break;
						}
					}
				}
			}
		}
	}
}

//Find the correct Foundation slot from the selected card
int FindCollectorSlot(Card card)
{
	//Search for the same face as the selected card
	for (int i = 0; i < 4; i++)
	{
		if (collectedCards[i][0].face == card.face)
			return i;
	}

	//Else search for an empty slot
	for (int i = 0; i < 4; i++)
	{
		if (collectedCards[i][0].color == EmptyCard().color)
			return i;
	}
}

//Remove selected card from a place(Array) if it does not exist there
void RemoveCard(Card card)
{
	//Remove card from Waste
	if (selCardInterface == DECK_CARD)
	{
		playDeck[showDeckCard] = EmptyCard();

		//Get the value of the new last card in  the deck
		if (showDeckCard == lastDeckCard)
		{
			do
			{
				lastDeckCard--;
			} while (playDeck[lastDeckCard].color == EmptyCard().color);
		}

		//Get the value of the card to be shown next in Waste
		do
		{
			showDeckCard--;
		} while (playDeck[showDeckCard].color == EmptyCard().color);

		if (showDeckCard < 0)
			pointerPos.X = 0;

		deckCount--;
		return;
	}

	//Remove card from Tableu
	if (selCardInterface == PLAYCARD)
	{
		for (int i = 0; i < 52; i++)
		{
			if (playCards[i].face == card.face && playCards[i].number == card.number)
				playCards[i] = EmptyCard();
		}
	}

	//Remove card from Foundation
	if (selCardInterface == COLLECTOR)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				if (collectedCards[i][j].face == card.face && collectedCards[i][j].number == card.number)
					collectedCards[i][j] = EmptyCard();
			}
		}
	}
}

//Display a given card
void PrintCard(Card card)
{
	int color = (card.open == true) ? card.color : Color(AQUA, AQUA);
	SetColor(color);

	cout << " " << setw(5) << left;
	cout << number[card.number] << face[card.face] << " ";

	SetColor(Color(WHITE, BLACK));

	cout << setw(0);
}

//Display empty slots(black boxes)
void PrintEmpty(Colors color)
{
	SetColor(Color(color, color));

	cout << setw(8) << left;
	cout << " ";

	SetColor(Color(WHITE, BLACK));

	cout << setw(0);
}

//Set values for empty card(black boxes/empty slots)
Card EmptyCard()
{
	Card empty;
	empty.number = -1;
	empty.color = 0;
	empty.face = -1;
	empty.pos.X = -1;
	empty.pos.Y = -1;
	empty.open = true;

	return empty;
}

//Get the maximum Y position that the pointer can move to
int GetMaxYPointer()
{
	int maxY = 0;
	for (int i = 0; i < 52; i++)
	{
		if (playCards[i].pos.X == pointerPos.X)
		{
			if (playCards[i].pos.Y > maxY)
				maxY = playCards[i].pos.Y;
		}
	}

	if (maxY == 0)
		maxY = 1;

	return maxY;
}

//Set the console color scheme
void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

//Calculate the console value of a color
int Color(int fontColor, int bgColor)
{
	int color = fontColor + bgColor * 16;
	return color;
}

//Move the console cursor to a give position
void MoveCursor(int x, int y)
{
	COORD coordinate = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinate);
}

//Get the console cursor position
COORD GetCursorPos()
{
	CONSOLE_SCREEN_BUFFER_INFO bufferInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &bufferInfo);

	return bufferInfo.dwCursorPosition;
}

//Hides the console cursor
void HideCursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}