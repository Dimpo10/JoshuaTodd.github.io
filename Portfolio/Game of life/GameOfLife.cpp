#include <iostream> 
#include <conio.h> 
#include <windows.h> 
#include <ctime>

#define KEY_DOWN(vk_code) GetAsyncKeyState(vk_code) // Define virtual key codes
#define KEY_UP(vk_code) GetAsyncKeyState(vk_code) 

using namespace std; // Using standard library

					 // Declare functions
void clearScreen(void); // Clear screen
void gotoxy(int, int); // Move cusor to coodinates XY
void textcolor(int); // Change text colour
void randomColours(void); // Random colours function
void gameOfLife(void); // Game of life function
void movingBlock(void); // Moving block function
void menu(void); // Menu to select different programs

				 // Time is measured in milliseconds as a special kind of integer called a DWORD
DWORD tickCount1;
DWORD tickCount2;

// Create 2 arrays to use in the Game of Life
int a[80][25];
int b[80][25];

// Create variable type char and pass it the ascii code for a block
char block = 219;
int color = 1;

// Program runs here
void main(void)
{
	clearScreen(); // Clear the screen
	menu(); // Calls menu function and runs the menu
}

void menu(void)
{
	clearScreen(); // Clears the screen to make sure no text is left over
	textcolor(15); // Sets text colour to white

	cout << "Choose from the list below using the number keys.\n\n";
	cout << "\t1. Moving block program\n";
	cout << "\t2. Random coloured blocks program\n";
	cout << "\t3. Game of life program\n";
	cout << "\t4. Exit";

	// Run this until number 4 has been pressed
	do {

		if (KEY_DOWN(0x31)) // If 1 is pressed run movingBlock function
		{
			clearScreen();// Clears the screen to make sure no text is left over
			movingBlock();
		}

		else if (KEY_DOWN(0x32)) // If 2 is pressed run randomColours function
		{
			clearScreen(); // Clears the screen to make sure no text is left over
			randomColours();
		}

		else if (KEY_DOWN(0x33)) // If 3 is pressed run gameOfLife function
		{
			clearScreen(); // Clears the screen to make sure no text is left over
			gameOfLife();
		}

	} while (KEY_DOWN(0x34) == false); // If 4 is pressed, exit
}

void movingBlock(void)
{
	//Declare variables for the coordiantes of the block
	int blockx = 0;
	int blocky = 0;
	int blockdx = 0;
	int blockdy = 0;

	// Pass the value of tickCount into GetTickCount
	tickCount1 = GetTickCount();

	//Start of do-loop. This will run until the while condition at the end is false.
	do {

		//Get the time in milliseconds now
		tickCount2 = GetTickCount();

		//Work out the difference from the last time we measured the time to now
		DWORD timeDifference = tickCount2 - tickCount1;

		//If that difference is greater than 500ms (1/2 a second)...
		if (timeDifference > 100)
		{
			//Clear the screen
			clearScreen();

			// If the right arrow is pressed move the block to the right
			if (KEY_DOWN(VK_RIGHT))
			{
				blockdx = 1; // Increment the value of blockdx by 1
			}

			// If the left arrow is pressed move the block to the left
			else if (KEY_DOWN(VK_LEFT))
			{
				blockdx = -1; // decrement the value of blockdx by 1
			}

			// If the up arrow is pressed move the block upwards
			else if (KEY_DOWN(VK_UP))
			{
				blockdy = -1; // decrement the value of blockdy by 1
			}

			// If the right arrow is pressed move the block downwards
			else if (KEY_DOWN(VK_DOWN))
			{
				blockdy = 1; // Increment the value of blockdy by 1
			}

			// If none of the above conditions are true keep the values the same
			else
			{
				blockdx = 0;
				blockdy = 0;
			}

			// Pass new values into blockx and blocky
			blockx = blockx + blockdx;
			blocky = blocky + blockdy;

			// Draw an block ant the coordinates of blockx and blocky
			gotoxy(blockx, blocky);
			cout << block;

			// Set the time1 variable to time2, which is now the last time we measured the time
			tickCount1 = tickCount2;
		}

		// Move cusor to X.2 Y.27
		gotoxy(2, 27);
		cout << "Use the arrow keys to move the block";
		gotoxy(2, 28);
		cout << "Press escape to go back to the menu";
		gotoxy(2, 29);
		cout << "Press 4 to exit";

		// If Escape is pressed go back to menu
		if (KEY_DOWN(VK_ESCAPE))
		{
			clearScreen();
			menu();
		}

		// Keep doing this until the 4 key is pressed
	} while (KEY_DOWN(0x34) == false);

	//Once 4 is pressed, the program will exit
}

void gameOfLife(void)
{
	// Pass the value of tickCount into GetTickCount
	tickCount1 = GetTickCount();

	//Seed the random number generator
	srand(time(NULL)*GetTickCount());

	//Setup the entire console to have random blocks
	// If x is less than 80, increment x
	for (int x = 0; x < 80; x++)
	{
		// If y is less than 25 increment y
		for (int y = 0; y < 25; y++)
		{
			a[x][y] = rand() % 10; // Generate random number for the array
		}
	}

	// Do this until while condition is true
	do {

		// Pass value of tickCount2 into GetTickCount
		tickCount2 = GetTickCount();

		// Make timeDifference the value of tickCount2 minus tickCount1
		DWORD timeDifference = tickCount2 - tickCount1;

		// Run every half a second
		if (timeDifference > 500)
		{
			//Clear the screen
			clearScreen();

			// Make tickCount1 the same as tickCount2
			tickCount1 = tickCount2;

			// Print out the console with the random blocks
			// If x is less than 80, increment x
			for (int x = 0; x < 80; x++)
			{
				// If y is less than 25 increment y
				for (int y = 0; y < 25; y++)
				{
					// Go to specified coordinates
					gotoxy(x, y);

					// Compare a to 1
					if (a[x][y] == 1)
					{
						cout << block; // If true, print out block
					}
				}
			}

			// If x is less than 80, increment x
			for (int x = 0; x < 80; x++)
			{
				// If y is less than 25 increment y
				for (int y = 0; y < 25; y++)
				{

					// Calculate how many neighbours we have
					int numberOfNeighbours = 0;

					// Top row
					if (a[x - 1][y - 1] == 1)
					{
						// If true, increment, same for the others below
						numberOfNeighbours = numberOfNeighbours + 1;
					}
					if (a[x][y - 1] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}
					if (a[x + 1][y - 1] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}

					//Middle row
					if (a[x - 1][y] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}
					if (a[x + 1][y] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}

					//Bottom row
					if (a[x - 1][y + 1] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}
					if (a[x][y + 1] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}
					if (a[x + 1][y + 1] == 1)
					{
						numberOfNeighbours = numberOfNeighbours + 1;
					}

					//Do something with the number of neighbours we have

					if (numberOfNeighbours < 2)
					{
						b[x][y] = 0;
					}

					if (numberOfNeighbours == 3)
					{
						b[x][y] = 1;
					}


					if (numberOfNeighbours > 3)
					{
						b[x][y] = 0;
					}
				}
			}

			for (int x = 0; x < 80; x++)
			{
				// If y is less than 25 increment y
				for (int y = 0; y < 25; y++)
				{
					a[x][y] = b[x][y];
				}
			}
			// Go to coordinates
			gotoxy(2, 27);
			// Print out instructions
			cout << "Press escape to go back to the menu";
			gotoxy(2, 26);
			cout << "Press spacebar to reset";
			gotoxy(2, 28);
			cout << "Press 4 to exit";

			// IF escape key is pressed, go back to the menu
			if (KEY_DOWN(VK_ESCAPE))
			{
				clearScreen();
				menu();
			}

			//Space bar pressed resets the game of life
			if (KEY_DOWN(VK_SPACE))
			{
				clearScreen();

				for (int x = 0; x < 80; x++)
				{
					// If y is less than 25 increment y
					for (int y = 0; y < 25; y++)
					{
						a[x][y] = rand() % 10; // Generate random number for the array
					}
				}

				for (int x = 0; x < 80; x++)
				{
					// If y is less than 25 increment y
					for (int y = 0; y < 25; y++)
					{
						// Go to specified coordinates
						gotoxy(x, y);

						// Compare a to 1
						if (a[x][y] == 1)
						{
							cout << block; // If true, print out block
						}
					}
				}
			}
		}
	} while (KEY_DOWN(0x34) == false); // ends if key 4 is pressed
}

void randomColours(void)
{
	// Go to coordinates and print instructions
	gotoxy(2, 27);
	cout << "Press escape to go back to the menu";
	gotoxy(2, 28);
	cout << "Press 4 to exit";

	// Do until while condition is true
	do {

		//Print out the console with the random blocks
		// If x is less than 80, increment x
		for (int x = 0; x < 80; x++)
		{
			// If y is less than 25 increment y
			for (int y = 0; y < 25; y++)
			{
				// Go to coordiantes
				gotoxy(x, y);
				if (a[x][y] == 1)
				{
					// Generate random number for the text colour up to 10
					textcolor(rand() % 10);
					cout << block;
				}
			}
		}

		// If x is less than 80, increment x
		for (int x = 0; x < 80; x++)
		{
			// If y is less than 25 increment y
			for (int y = 0; y < 25; y++)
			{
				// Generate random number up to 2
				a[x][y] = (rand() % 2);

			}
		}
		// Go back to menu is escape is pressed
		if (KEY_DOWN(VK_ESCAPE))
		{
			clearScreen();
			menu();
		}
	} while (KEY_DOWN(0x34) == false);
}

void clearScreen(void)
{
	//Clears the screen and puts the cursor back to default coordinates position 
	static HANDLE hStdout = NULL;
	static CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0,0 };
	DWORD dummy;

	if (!hStdout)
	{
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
		GetConsoleScreenBufferInfo(hStdout, &csbi);
	}
	FillConsoleOutputCharacter(hStdout, ' ', csbi.dwSize.X * csbi.dwSize.Y, startCoords, &dummy);

	hStdout = NULL;
	COORD coord;

	coord.X = 0;
	coord.Y = 0;

	if (!hStdout) { hStdout = GetStdHandle(STD_OUTPUT_HANDLE); }
	SetConsoleCursorPosition(hStdout, coord);
}

void gotoxy(int x, int y)
{
	// Generates coordinates for the cursor and sets the postition when 2 variables are passed into it.
	static HANDLE hStdout = NULL;
	COORD coord;

	coord.X = x;
	coord.Y = y;

	if (!hStdout) { hStdout = GetStdHandle(STD_OUTPUT_HANDLE); }
	SetConsoleCursorPosition(hStdout, coord);
}

void textcolor(int c)
{
	// Sets the text colour using the standard library
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, c);
}