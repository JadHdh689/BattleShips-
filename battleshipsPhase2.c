#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>




/*

requires: The character ship must be associated with the
ship that has been sunk.
For example, 
the carrier has number 5 since it is 5 cells long.
If the carrier was destroyed, the char ship should be equal to 5.


effects:
All the cells in the sunk_ship_grid that are equal to the char ship become equal to 'o' indicating
that the ship is sunk in the grid.

*/
void UpdateSunkShipsGrid(char ship, char sunk_ships_grid[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (sunk_ships_grid[i][j] == ship)
                sunk_ships_grid[i][j] = 'o';
        }
    }
}


/*

requires: 
The character ship must be associated with the
ship that has been sunk.
For example, 
the carrier has number 5 since it is 5 cells long.
If the carrier was destroyed, the char ship should be equal to 5.


Effect:
This function is only called when a ship has been sunk.
The function prints which ship has been sunk based on the given char.
The function also updates the boolean of the ship that has been sunk to false.
The function also calls another function called UpdateSunkShipsGrid.



*/
void printWhichShipBot(char a, bool *Submarine, bool *Destroyer, bool *BattleShip, bool *Carrier, char sunkShipsGrid[10][10])
{
    if (a == '2')
    {
        printf("A submarine has been sunk\n");
        *Submarine = false;
    }
    if (a == '3')
    {
        printf("A destroyer has been sunk\n");
        *Destroyer = false;
    }
    if (a == '4')
    {
        printf("A battleship has been sunk\n");
        *BattleShip = false;
    }
    if (a == '5')
    {
        printf("A carrier has been sunk\n");
        *Carrier = false;
    }
    UpdateSunkShipsGrid(a, sunkShipsGrid);
}



/*

requires: 
The character ship must be associated with the
ship that has been sunk.
For example, 
the carrier has number 5 since it is 5 cells long.
If the carrier was destroyed, the char ship should be equal to 5.


Effect:
This function is only called when a ship has been sunk.
The function prints which ship has been sunk based on the given char.
The function also updates the boolean of the ship that has been sunk to false.




*/



void printWhichShip(char a)
{
    if (a == '2')
        printf("A submarine has been sunk\n");
    if (a == '3')
        printf("A destroyer has been sunk\n");
    if (a == '4')
        printf("A battleship has been sunk\n");
    if (a == '5')
        printf("A carrier has been sunk\n");
}



/*

requires:
Nothing.
This function is called to calculate the coordinates of the next fire / artillery.

effects:
The function returns true if there is a ship that has destroyed parts but it isn't sunk yet.
If it returns true, the function also changes the recentRow and recentColumn to a cell of the 
ship that has been sunk, since the hit phase will be activated in the main.

If it returns false, recentRow and recentColumn won't change.

*/


bool Destroy_remaining_ships(char sunkenShips[10][10], char gridPublic[10][10], int *recentRow, int *recentColumn)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (gridPublic[i][j] == '*' && sunkenShips[i][j] != 'o' && sunkenShips[i][j] != '~')
            {
                if (gridPublic[i - 1][j] == '~' || gridPublic[i][j - 1] == '~' || gridPublic[i][j + 1] == '~' || gridPublic[i + 1][j] == '~')
                {
                    *recentRow = i;
                    *recentColumn = j;
                    return true;
                }
            }
        }
    }
    return false;
}


/*

requires: nothing.
This function is called several times at the beginning of the game to initialize the arrays of the player grid and the bot grid.

effects:
This function places water in all the cells of the 2D array given.


*/

void initializeArray(char given[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            given[i][j] = '~';
        }
    }
}

/*
requires: nothing.
This function is called when the bot follows the probability strategy to calculate coordinates of the next move.


Effect:
This function initializes the probability of all the cells of the array to 0 since it prepares for the probability
to be recalculated.

*/


void initializeProbability(int grid[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grid[i][j] = 0;
        }
    }
}

/*

requires: Nothing
This function is at each turn of either the bot or player to see the public grid of the opponent.

Effect:
It prints the public grid of the opponent with the letters A --> J as columns at the first row.
It prints before each row a number from 1 --> 10, and the numbers are printed in order.

*/
void printArray(char given[10][10])
{
    printf("    A B C D E F G H I J \n");
    for (int i = 0; i < 10; i++)
    {
        int num = i + 1;
        if (i != 9)
            printf("%ld)  ", num);
        else
            printf("%ld) ", num);
        for (int j = 0; j < 10; j++)
        {
            printf("%c ", given[i][j]);
        }
        printf("\n");
    }

    printf("\n");

}



/*


requires: nothing.
This function is called when the bot wants to choose a move.

Effect:
It returns a string that chooses the bots move.
The string is chosen based on the given integers, boolean, and based on chance.

*/

void chooseMove(int RadarP2, int SmokeP2, bool ArtilleryP2, bool TorpedoP2, char *x, int RadarSweepP1, bool recentHit, bool RadarHit)
{

    if (TorpedoP2)
    {
        strcpy(x, "Torpedo");
    }
    else if (ArtilleryP2)
    {
        strcpy(x, "Artillery");
    }
    else
    {
        int randomValue;
        srand(time(NULL));
        randomValue = rand() % 100 + 1;

        if (randomValue <= 50)
        {
            strcpy(x, "Fire");
        }
        else if (randomValue <= 75 && RadarP2 > 0 && !recentHit && !RadarHit)
        {
            strcpy(x, "Radar");
        }
        else if (randomValue > 75 && SmokeP2 > 0 && RadarSweepP1 > 0 && !recentHit && !RadarHit)
        {
            strcpy(x, "Smoke");
        }
        else
        {
            strcpy(x, "Fire");
        }
    }
}



/*

requires: The char ltr is a letter from A to J. 
This function is called when a player chooses coordinates.

Effect:
The function returns a corresponding number to the letter between 0 -- > 9 to be used 
as a column in the main.
If given a number, it returns the number as is.
*/

int getCOLUMNindex(char ltr)
{

    ltr = toupper(ltr); 
    int curr = (int)ltr;

    if (ltr >= 'A' && ltr <= 'Z')
        return (curr - 65);
    return curr - '1'; 
}

/*

requires: nothing.
This function is called when a player is placing a ship. 
It is called for each of the four ships at least once.

Effect:
The function places a ship in the given grid if the ship placement is valid, meaning without any collisions and in bounds.
If the ship placement is valid, the function returns true and places the ship in the grid.
If the ship placement is invalid, the function returns false and print "Error"

*/
bool CheckAndAdd(int row, int column, bool isHorizontal, int sizeOfShip, char given[10][10])
{

    row = row - 1;
    if (row > 9 || column > 9 || row < 0 || column < 0) // OUT OF BOUNDS
    {
        printf("Error \n");
        return false; // false = error
    }
    if (isHorizontal)
    {
        if (10 - column < sizeOfShip) // CHECK IF EXTENDS OUTSIDE
        {
            printf("Error \n");
            return false;
        }
    }
    else
    {
        if (10 - row < sizeOfShip)
        {
            printf("Error \n");
            return false;
        }
    }

    if (isHorizontal)
    {
        // stay at row , ONLY CHANGE COLUMNS

        for (int i = column; i < sizeOfShip + column; i++)
        {
            if (given[row][i] != '~') // EITHER HAS A 2, 3, 4, 5 ( CHAR )
            {
                printf("Error \n");
                return false;
            }
        }

        for (int i = column; i < sizeOfShip + column; i++)
        {

            given[row][i] = sizeOfShip + '0';
        }
    }
    else
    {
        // stay at column , ONLY CHANGE ROWS

        for (int i = row; i < row + sizeOfShip; i++)
        {
            if (given[i][column] != '~')
            {
                printf("Error \n");

                return false;
            }
        }

        for (int i = row; i < row + sizeOfShip; i++)
        {

            given[i][column] = sizeOfShip + '0';
        }
    }
    return true;
}
/*

requires: nothing.
This function is called when the bot is placing a ship. 
It is called for each of the four ships at least once.

Effect:
The function places a ship in the given grid if the ship placement is valid, meaning without any collisions and in bounds.
If the ship placement is valid, the function returns true and places the ship in the grid.
If the ship placement is invalid, the function returns false.

*/
bool CheckAndAddBot(int row, int column, bool isHorizontal, int sizeOfShip, char given[10][10])
{

    row = row - 1;
    if (row > 9 || column > 9 || row < 0 || column < 0) // OUT OF BOUNDS
    {

        return false; // false = error
    }
    if (isHorizontal)
    {
        if (10 - column < sizeOfShip) // CHECK IF EXTENDS OUTSIDE
        {

            return false;
        }
    }
    else
    {
        if (10 - row < sizeOfShip)
        {

            return false;
        }
    }

    if (isHorizontal)
    {
        // stay at row , ONLY CHANGE COLUMNS

        for (int i = column; i < sizeOfShip + column; i++)
        {
            if (given[row][i] != '~') // EITHER HAS A 2, 3, 4, 5 ( CHAR )
            {

                return false;
            }
        }

        for (int i = column; i < sizeOfShip + column; i++)
        {

            given[row][i] = sizeOfShip + '0';
        }
    }
    else
    {
        // stay at column , ONLY CHANGE ROWS

        for (int i = row; i < row + sizeOfShip; i++)
        {
            if (given[i][column] != '~')
            {

                return false;
            }
        }

        for (int i = row; i < row + sizeOfShip; i++)
        {

            given[i][column] = sizeOfShip + '0';
        }
    }
    return true;
}

/*

requires: nothing.
This function is called when a player is placing a ship. 
It is called for each of the four ships at least once.

Effect:
The function asks the player for coordinates for a given ship.
The function then calls the checkAndAdd function to check if the placement is valid.
If the placement is valid, the ship is placed normally in the grid in the CheckAndAdd function.
If the placement is invalid, the player is constantly asked to give new coordinates until valid coordinates are given.

*/
void placeShips(char grid[10][10], char *name, int size)
{
    bool check = false;
    do
    {
        printArray(grid);
        printf("Please enter coordinates for the %s (%d cells) ", name, size);
        char *Coordinates = (char *)malloc(15 * sizeof(char));
        scanf("%14s", Coordinates);
        printf("\n");
        char temp = (int)Coordinates[1];
        int row = temp - '0';                                                                                 // B3 --> SECOND INDEX IS THE ROW
        int col = getCOLUMNindex(Coordinates[0]);                                                             // B3 --> FIRST INDEX IS THE COLUMN
        bool isHorizontal = true;                                                                             // initializing
        if (Coordinates[3] == 'v' || Coordinates[3] == 'V' || Coordinates[4] == 'V' || Coordinates[4] == 'v') // B10, Vertical
        {
            isHorizontal = false; // vertical
        }
        if (Coordinates[2] == '0' && row == 1)
        {
            row = 10;
        }
        else if (Coordinates[2] == '0')
        {
            row = 20;
        }
        check = CheckAndAdd(row, col, isHorizontal, size, grid);
        free(Coordinates);

    } while (check == false);
}
/*

requires: nothing.
This function is called when a bot is placing a ship. 
It is called for each of the four ships at least once.

Effect:
The function generates random coordinates for a given ship.
The function then calls the checkAndAdd function to check if the placement is valid.
If the placement is valid, the ship is placed normally in the grid in the CheckAndAdd function.
If the placement is invalid, the function will constantly generate new coordinates until valid coordinates are given.

*/



void placeShipsBot(char grid[10][10], char *name, int size)
{
    bool check = false;
    srand(time(NULL));
    do
    {
        char *Coordinates = (char *)malloc(15 * sizeof(char));

        int row = rand() % 10 + 1; // random row from 0 --> 9
        int col = rand() % 10;     // random column from 0 --> 9
        bool isHorizontal = rand() % 2;
        check = CheckAndAddBot(row, col, isHorizontal, size, grid);
        free(Coordinates);

    } while (check == false);
}

/*

requires: nothing.
This function is called when the player or the bot chooses to fire.

Effect:
If the coordinates given are out of bounds, a '~' is returned indicating a miss.

If the target is a miss, either because the target was already hit before or the target is water,
the function returns the content of the cell which is '~' indicating a miss.
If the difficulty is easy, the target of the miss changes to 'o' in the public grid and in the sunkShipsGrid.
If the difficuty is hard, no changes are done to the public grid.
No changes are done to the private / secret grid.


If the target is a hit, the function returns the content of the cell which is a number from 1 --> 4 based 
on the ship that was hit indicating a hit.
If the difficulty is easy, the target of the hit changes to '*' in the public grid.
If the difficulty is hard, the target of the hit changes to '*' in the public grid.
In the private grid, the target of the hit changes to '~'.

*/
char Fire(int row, int column, char givenPublic[10][10], char givenSecret[10][10], char sunkShipsGrid[10][10], bool isHardDiff, bool botTurn)
{
    if (row < 0 || column < 0 || row > 9 || column > 9)
        return '~'; // out of bounds -- no hit

    char ReturnChar = '0'; // initialize
    if (givenSecret[row][column] == '~')
    {
        if (!isHardDiff || botTurn)
        {
            if (givenPublic[row][column] != '*') // if the spot isn't a sunken ship
                givenPublic[row][column] = 'o';  // just in the case of easy difficulty
            if (botTurn)
                sunkShipsGrid[row][column] = 'o';
        }
        ReturnChar = '~'; // if miss return water
    }
    else
    {
        givenPublic[row][column] = '*';
        ReturnChar = givenSecret[row][column]; // GET THE NUMBER TO CHECK IF THE SHIP IS SUNKEN
        givenSecret[row][column] = '~';
    }

    return ReturnChar;

    /* first if: if the box that he is aiming at is water at SecretGrid
    this means that he missed
    we check if we are playing easy difficulty: if the place isn't sunken
    we put o (miss water)  in the public grid
    in hard: if we miss  we dont change to o, keep the water sign
    in this case change nothing in the secret grid
    return water sign



    else : if the box that he is aiming at is A SHIP at SecretGrid
    change the ship to water in secret grid
    show at the public grid sign * for sunken ship for both easy and hard diff


    */
}

/*

requires: given must be a char between 1 and 4.
This function is called after every hit is made.

Effect:
This function returns false if the given char is found at the grid, meaning the ship wasn't sunk.
The function returns true if the given char is not found at the given grid, meaning the ship was sunk.

*/
boolean isSunk(char given, char Matrix[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Matrix[i][j] == given)
                return false; 
        }
    }
    return true; 
}

// SmokeScreen uses an alternative array to implement the functionality
// We will use && to check later if the area is a smoked area or a normal area
// Fill smoked boxes with number 1


/*
requires: nothing.
This function is called when the player or the bot chooses the smokescreen move.

Effect: If the coordinates are out of bound, the function does nothing.
If the coordinates are in bound, the function changes
the 2x2 of the corresponding coordinates to 1 in the smokeGrid.
*/
void SmokeScreen(int SmokeGrid[10][10], int row, int col)
{
    if (row < 0 || col < 0 || row > 9 || col > 9)
        return;
    SmokeGrid[row][col] = 1;
    if (row != 9)
        SmokeGrid[row + 1][col] = 1;
    if (col != 9)
        (SmokeGrid[row][col + 1] = 1);
    if (row != 9 && col != 9)
        SmokeGrid[row + 1][col + 1] = 1;
}

// Uses && to check if the spots are hidden, if we find 1 ship and not hidden return true


/*

requires: nothing.
This function is called when the player or the bot chooses to do a Radar.

Effect:
Returns a boolean if the 2x2 corresponding to the coordinates are out of bounds,
or if the 2x2 is hidden by a smoke, or if there is no ships in the 2x2.

Returns ships found if there is ships in the 2x2 and no smoke is covering them.
 
*/
bool Radar(char grid[10][10], int SmokeGrid[10][10], int row, int col)
{
    if (row < 0 || col < 0 || row > 9 || col > 9)
        return false;
    if (grid[row][col] != '~' && SmokeGrid[row][col] != 1) // if ship seen and not smoked
        return true;
    if (row != 9)
    {
        if (grid[row + 1][col] != '~' && SmokeGrid[row + 1][col] != 1)
            return true;
    }
    if (col != 9)
    {
        if (grid[row][col + 1] != '~' && SmokeGrid[row][col + 1] != 1)
            return true;
    }
    if (row != 9 && col != 9)
    {
        if (grid[row + 1][col + 1] != '~' && SmokeGrid[row + 1][col + 1] != 1)
            return true;
    }
    return false;
    // check all b3 c3 b4 c4
    // if b9 or at the edge dont do it
}

// after each hit, check if a ship has been sunk. If a ship has been sunk, call the function to print which ship has been sunk.
// change recentSunk to true if a ship has been sunk


/*

requires: hit must be either '~' or a number between 1 an 4
This function is called after a fire is done by the player.

Effect: 
If the fire is a miss, recentSunk is set to false.
If the fire is a hit, the function calls the isSunk function to check if the ship that was hit has been sunk.
If the ship was sunk, the function increments sunkShips by 1 (the number of sunk ships of the opponent) and increments smokescreen by 1 (the number of smokes the current player can make),
sets recentSunk to true, and calls printWhichShip function to print which ship was sunk.
If the ship wasn't sunk, the fuction sets recentSunk to false.

*/
void total_fire(char hit, char grid[10][10], int *sunkShips, int *smokeScreen, bool *recentSunk)
{
    if (hit != '~')
    {
        if (isSunk(hit, grid))
        {
            (*sunkShips)++;
            (*smokeScreen)++;
            *recentSunk = true;
            printWhichShip(hit);
        }
        else
        {
            *recentSunk = false;
        }
    }
    else
    {
        *recentSunk = false;
    }
}



/*

requires: hit must be either '~' or a number between 1 an 4
This function is called after a fire is done by the player.

Effect: 
If the fire is a miss, recentSunk is set to false.
If the fire is a hit, the function calls the isSunk function to check if the ship that was hit has been sunk.
If the ship was sunk, the function increments sunkShips by 1 (the number of sunk ships of the opponent) and increments smokescreen by 1 (the number of smokes the current player can make),
sets recentSunk to true, sets RecentHit and RecentHit2 to false to exit the hit phase ,and calls printWhichShipBot function to print which ship was sunk.
If the ship wasn't sunk, the fuction sets recentSunk to false.

*/



void total_fireBOT(char hit, char grid[10][10], char sunkShipsGrid[10][10], int *sunkShips, int *smokeScreen, bool *recentSunk, bool *Submarine, bool *Destroyer, bool *BattleShip, bool *Carrier, bool *recentHit, bool *recentHit2)
{
    if (hit != '~')
    {
        if (isSunk(hit, grid))
        {
            (*sunkShips)++;
            (*smokeScreen)++;
            *recentSunk = true;
            printWhichShipBot(hit, Submarine, Destroyer, BattleShip, Carrier, sunkShipsGrid);
            *recentHit = false;
            *recentHit2 = false;
        }
        else
        {
            *recentSunk = false;
        }
    }
    else
    {
        *recentSunk = false;
    }
}

/*

requires: ship_size = 5
This function is called before the bot chooses a move to initialize the probability grid for the bot to use it to calculate coordinates.

Effects:
The function changes the cells in the probability grid to be numbers that correspond to the probability of having a ship there.
Probabilities of each cell are calculated based on how many different permutations of ships that would occupy this cell. 
The more ships you can place, the higher the probability is. 
The function depends on the booleans given.


*/
void calculateProbability(int probabilityGrid[10][10], char grid[10][10], bool submarine_up, bool destoryer_up, bool battleship_up, bool carrier_up, int ship_size)
{
    if (ship_size == 5 && carrier_up || ship_size == 4 && battleship_up || ship_size == 3 && destoryer_up || ship_size == 2 && submarine_up)
    {
        for (int row = 0; row < 10; row++)
        {
            for (int column = 0; column <= 10 - ship_size; column++)
            {
                if (grid[row][column + 4] != '~' && ship_size >= 5)
                    column += 4;
                else if (grid[row][column + 3] != '~' && ship_size >= 4)
                    column += 3;
                else if (grid[row][column + 2] != '~' && ship_size >= 3)
                    column += 2;
                else if (grid[row][column + 1] != '~')
                    column++;
                else if (grid[row][column] == '~')
                {
                    probabilityGrid[row][column]++;
                    probabilityGrid[row][column + 1]++;
                    if (ship_size >= 3)
                        probabilityGrid[row][column + 2]++;
                    if (ship_size >= 4)
                        probabilityGrid[row][column + 3]++;
                    if (ship_size >= 5)
                        probabilityGrid[row][column + 4]++;
                }
            }
        }
        for (int column = 0; column < 10; column++)
        {
            for (int row = 0; row <= 10 - ship_size; row++)
            {
                if (grid[row + 4][column] != '~' && ship_size >= 5)
                    row += 4;
                else if (grid[row + 3][column] != '~' && ship_size >= 4)
                    row += 3;
                else if (grid[row + 2][column] != '~' && ship_size >= 3)
                    row += 2;
                else if (grid[row + 1][column] != '~')
                    row++;
                else if (grid[row][column] == '~')
                {
                    probabilityGrid[row][column] = probabilityGrid[row][column] + 1;
                    probabilityGrid[row + 1][column]++;
                    if (ship_size >= 3)
                        probabilityGrid[row + 2][column]++;
                    if (ship_size >= 4)
                        probabilityGrid[row + 3][column]++;
                    if (ship_size >= 5)
                        probabilityGrid[row + 4][column]++;
                }
            }
        }
    }
    if (ship_size == 5)
    {
        calculateProbability(probabilityGrid, grid, submarine_up, destoryer_up, battleship_up, carrier_up, ship_size - 1);
        calculateProbability(probabilityGrid, grid, submarine_up, destoryer_up, battleship_up, carrier_up, ship_size - 2);
        calculateProbability(probabilityGrid, grid, submarine_up, destoryer_up, battleship_up, carrier_up, ship_size - 3);
    }
}
int main()
{
    srand(time(0)); // used for randomness
    printf("Choose BattleShip difficulty: (easy/ hard) == (0/1) ");
    int num = 0;
    scanf("%ld", &num);
    bool isHardDiff = true;
    if (num == 0)
        isHardDiff = false;

    printf("\n");
    printf("Please enter your name: ");
    char *name1 = (char *)malloc(50 * sizeof(char));
    scanf("%49s", name1);
    printf("\n");

    int random = rand() % 2; // takes a random integer --> even == 0, odd == 1
    // we will use it for the counter (counter = random)
    char gridp1SECRET[10][10];
    char gridp2SECRET[10][10];
    char gridp1PUBLIC[10][10];
    char gridp2PUBLIC[10][10];

    initializeArray(gridp1SECRET);
    initializeArray(gridp2SECRET);
    initializeArray(gridp1PUBLIC);
    initializeArray(gridp2PUBLIC);
    // We use for each player a secret array and a public array
    // if random = 0, start player 1 and player 2 normally
    // if random = 1, swap names so that player 2 starts first

    printf("\n\n\n\n\n");
    printf("%s please put all your ships.\n", name1);
    // place carrier
    placeShips(gridp1SECRET, "Carrier", 5);
    // place battleship
    placeShips(gridp1SECRET, "Battleship", 4);
    // put destroyer
    placeShips(gridp1SECRET, "Destroyer", 3);
    // put submarine
    placeShips(gridp1SECRET, "Submarine", 2);


    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    // place carrier
    placeShipsBot(gridp2SECRET, "Carrier", 5);
    // place battleship
    placeShipsBot(gridp2SECRET, "Battleship", 4);
    // put destroyer
    placeShipsBot(gridp2SECRET, "Destroyer", 3);
    // put submarine
    placeShipsBot(gridp2SECRET, "Sumbarine", 2);

    // place ships for both players by using placeShips function

    int SunkShipsP1 = 0;
    int SunkShipsP2 = 0;

    int countRound = random; // helps in switching between turns (even = player1, odd = bot)
    char hit = '0';          // we will have a variable that receives what got hit (either water if no target or A NUMBER IF PART OF A SHIP GOT HIT)
    int RadarSweepP1 = 3;    // Count how much radar sweeps left
    int RadarSweepP2 = 3;
    int SmokeScreenP1 = 0; // Count how much smokescreens left
    int SmokeScreenP2 = 0;
    int SmokeGridP1[10][10]; // if box != 1 no smoke, box = 1 smoke
    int SmokeGridP2[10][10];
    int probabilityGrid[10][10];
    int parityArray[20] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 5, 16, 27, 38, 49, 50, 61, 72, 83, 94}; 
    // these coordinates will be used when the carrier has not been sunk for the parity strategy
    int parityRange = 20;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            SmokeGridP1[i][j] = 0;
            SmokeGridP2[i][j] = 0;
        }
    }

    // initialize sunkShips grid to be same as the secret grid at the start
    char sunkShipGrid[10][10];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            sunkShipGrid[i][j] = gridp1SECRET[i][j];
        }
    }
  




    // initialize smokegrids
    bool RecentSunkP1 = false;    // this indicates if player 1 has sunk a ship in the last round
    bool RecentSunkP2 = false;    // this indicates if the bot has sunk a ship in the last round
    bool recentHit = false;       // this is when a ship has been hit but not sunk yet
    bool submarine_up = true;     // each ship will have a boolean to indicate if it has been sunk yet
    bool destroyer_up = true;
    bool battleship_up = true;
    bool carrier_up = true;
    int recent_row = 0;      // for the bot
    int recent_column = 0;   // for the bot
    int current_ROW = 0;     // for after recenthit2
    int current_COLUMN = 0;  // for after recenthit2
    bool vertical = false;   // for the bot
    bool horizontal = false; // for the bot
    bool recentHit2 = false; // this indicates that the bot is moving at a certain direction during firing to sink a ship
    bool left = false;   // during recentHit2 this indicates which direction the bot is moving
    bool right = false;  
    bool up = false;
    bool down = false;

    bool RadarHit = false;  // indicates if the bot found ships after doing Radar
    int radar_row = 0; // this is the original row of the radar that the bot made
    int radar_column = 0; // this is the original column of the radar that the bot made

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    while (SunkShipsP1 != 4 && SunkShipsP2 != 4)
    {
        if (countRound % 2 == 0)
        {
            printf("It's your turn %s\n\n", name1);
            // player 1 turn
            printArray(gridp2PUBLIC);
            printf("Choose one of the following options: \n");
            printf("1. Fire (Fire B3)\n");
            printf("2. Radar Sweep (Radar B3) (%d left)\n", RadarSweepP1);
            printf("3. SmokeScreen (Smoke B3) (%d left)\n", SmokeScreenP1);
            printf("4. Artillery (Artillery B3) (%d left)\n", RecentSunkP1);
            printf("5. Torpedo (Torpedo B/3) (%d left)\n", (RecentSunkP1 && SunkShipsP2 == 3));

            char *move = (char *)malloc(10 * sizeof(char));
            char *coordinates = (char *)malloc(10 * sizeof(char));
            scanf("%s", move);
            scanf("%s", coordinates);
            char col = toupper(coordinates[0]);
            boolean isColLetter;
            if (col >= 'A' && col <= 'Z')
                isColLetter = true;
            else
                isColLetter = false;
            //        ltr = toupper(ltr);
            // int curr = (int)ltr;

            // if (ltr >= 'A' && ltr <= 'Z')
            int column = getCOLUMNindex(coordinates[0]); // COLUMN INDEX OF THE LETTER
            int row = coordinates[1] - '1';              // incase of t garbage values AND ASCII
            if (coordinates[2] == '0' && row == 0)
            {
                row = 9; // CASE IF ROW = 10
            }
            else if (coordinates[1] >= '1' && coordinates[1] <= '9' && coordinates[2] >= '1' && coordinates[2] <= '9')
            {
                row = 20; // give error
            }
            switch (move[0])
            {

            case 'F':
            case 'f':
                hit = Fire(row, column, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                if (hit != '~')
                    printf("Hit\n");
                else
                    printf("Miss\n");
                total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);
                break;
                // Fire uses 2 functions
            case 'R':
            case 'r':
                if (RadarSweepP1 > 0)
                {
                    bool a = Radar(gridp2SECRET, SmokeGridP2, row, column);
                    if (a)
                    {
                        printf("Enemy ships found.\n");
                    }
                    else
                    {
                        printf("No enemy ships found.\n");
                    }
                    RadarSweepP1--;
                }
                else
                {
                    printf("No radar sweeps left. You lost your turn.\n");
                }
                // Radar returns true or false --> if true alert about the ships, if false don't
                // Just use the function Radar that checks the smoke grids and secret grids using &&
                break;
            case 'S':
            case 's':
                if (SmokeScreenP1 > 0)
                {
                    SmokeScreen(SmokeGridP1, row, column);
                    SmokeScreenP1--;
                }
                else
                {
                    printf("No smokescreens left. You lost your turn.\n");
                }
                RecentSunkP1 = false;
                break;
                // SmokeScreen changes the 2x2 boxes to 1 in the smokeGrid of the current player
            case 'A':
            case 'a':
                if (RecentSunkP1)
                {
                    int isSunkCurrent = SunkShipsP2;
                    bool did_hit = false;
                    hit = Fire(row, column, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row + 1, column, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row, column + 1, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row + 1, column + 1, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    if (did_hit)
                        printf("hit\n");
                    else
                        printf("miss\n");
                    if (SunkShipsP2 != isSunkCurrent)
                        RecentSunkP1 = true;
                }
                else
                {
                    printf("You didn't sink a ship in the round before. You lost your turn.\n");
                }
                break;
                // fire on B3,C3,B4,C4

            case 'T': // ignore coordinates[1] use coordinates[0]
            case 't':
                if (SunkShipsP2 == 3 && RecentSunkP1)
                {
                    bool did_hit = false;
                    if (isColLetter)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(i, column, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                            if (hit != '~')
                                did_hit = true;
                            total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);
                        }
                    }
                    else
                    {
                        if (column == 0 && coordinates[1] == '0')
                            column = 9;
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(column, i, gridp2PUBLIC, gridp2SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                            if (hit != '~')
                                did_hit = true;
                            total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);
                        }
                    }
                    if (did_hit)
                        printf("Hit\n");
                    else
                        printf("Miss\n");
                }
                else
                {
                    printf("You don't meet the conditions to do a torpedo. You lost your turn.\n");
                }
                break;
                // in torpedo use a for loop to either fire on an entire row or column
            default:
                break;
            }
            free(move);
            free(coordinates);
            countRound++;
        }

        // BOT TURN BOT TURN BOT TURN BOT TURN BOT TURN BOT TURN

        else if (countRound % 2 == 1)
        { // ALL FUNCTIONS SAME AS PLAYER 1
            printf("It's the bot's turn \n\n");

            printArray(gridp1PUBLIC);
            if (!recentHit)
            {
                if (Destroy_remaining_ships(sunkShipGrid, gridp1PUBLIC, &recent_row, &recent_column))
                {
                    recentHit = true;
                    recentHit2 = false;
                }
            }
            
            // If there is no recentHit and there is a ship that is not sunk and broken parts, enter hit phase and let the bot hit all parts of the ship.

            char *move = (char *)malloc(10 * sizeof(char));
            chooseMove(RadarSweepP2, SmokeScreenP2, RecentSunkP2, (RecentSunkP2 && SunkShipsP1 == 3), move, RadarSweepP1, recentHit, RadarHit);
            int row;
            int column;
            bool Loop = true;
            while (Loop)
            {
                
                // If the radar found a hit, choose the coordinates of the next move to be the cell that has the highest probability in the 2x2.
                if (RadarHit)
                {
                    initializeProbability(probabilityGrid);
                    calculateProbability(probabilityGrid, gridp1PUBLIC, submarine_up, destroyer_up, battleship_up, carrier_up, 5);
                    int maxValue = 0;
                    // Check self
                    if (probabilityGrid[radar_row][radar_column] > maxValue)
                    {
                        maxValue = probabilityGrid[radar_row][radar_column];
                        row = radar_row;
                        column = radar_column;
                    }

                    // Check down
                    if (radar_row + 1 < 10 && probabilityGrid[radar_row + 1][radar_column] > maxValue)
                    {
                        maxValue = probabilityGrid[radar_row + 1][radar_column];
                        row = radar_row + 1;
                        column = radar_column;
                    }

                    // Check right and down
                    if (radar_column + 1 < 10 && radar_row + 1 < 10 && probabilityGrid[radar_row + 1][radar_column + 1] > maxValue)
                    {
                        maxValue = probabilityGrid[radar_row + 1][radar_column + 1];
                        row = radar_row + 1;
                        column = radar_column + 1;
                    }

                    // Check right
                    if (radar_column + 1 < 10 && probabilityGrid[radar_row][radar_column + 1] > maxValue)
                    {
                        maxValue = probabilityGrid[radar_row][radar_column + 1];
                        row = radar_row;
                        column = radar_column + 1;
                    }

                    if (gridp1SECRET[row][column] != '~')
                        RadarHit = false;

                    Loop = false;
                }
                 
                 // if not recentHit, choose the next coordinates based on parity or based on the probability strategy.
                else if (!recentHit)
                { // if we dont have a recent hit
                    if (carrier_up)
                    { // use parity until carrier is sunk
                        srand(time(NULL));
                        int index = rand() % parityRange;
                        int coor = parityArray[index];
                        row = coor / 10;
                        column = coor % 10;
                        parityArray[index] = parityArray[parityRange - 1];
                        parityArray[parityRange - 1] = coor;
                        parityRange--;
                        Loop = false;
                    }
                    else
                    { // use probability grid
                        initializeProbability(probabilityGrid);
                        calculateProbability(probabilityGrid, gridp1PUBLIC, submarine_up, destroyer_up, battleship_up, carrier_up, 5);
                        int max = 0;
                        for (int i = 0; i < 10; i++)
                        {
                            for (int j = 0; j < 10; j++)
                            {
                                if (probabilityGrid[i][j] > max)
                                {
                                    row = i;
                                    column = j;
                                    max = probabilityGrid[i][j];
                                }
                            }
                        }
                        Loop = false;
                    }
                } // if !recenthit GET coordinates for fire (row and column) either through carrier or prob grid


                // if there is a recentHit and there is a direction, choose the next coordinates based on the direction     
                else if (recentHit && recentHit2)
                {
                    int oldRow = row;
                    int oldCol = column;
                    if (left)
                    {
                        if (current_COLUMN - 1 >= 0 && gridp1PUBLIC[current_ROW][current_COLUMN - 1] == '~')
                            column = current_COLUMN - 1;
                        row = current_ROW;
                    }
                    else if (right)
                    {
                        if (current_COLUMN + 1 <= 9 && gridp1PUBLIC[current_ROW][current_COLUMN + 1] == '~')
                        {
                            column = current_COLUMN + 1;
                            row = current_ROW;
                        }
                    }
                    else if (up)
                    {
                        if (current_ROW - 1 >= 0 && gridp1PUBLIC[current_ROW - 1][current_COLUMN] == '~')
                            row = current_ROW - 1;
                        column = current_COLUMN;
                    }
                    else if (down)
                    {
                        if (current_ROW + 1 <= 9 && gridp1PUBLIC[current_ROW + 1][current_COLUMN] == '~')
                            row = current_ROW + 1;
                        column = current_COLUMN;
                    }
                    Loop = false;
                    if (oldRow == row && oldCol == column)
                    {
                        recentHit2 = false;
                        Loop = true;
                    }
                }
                 
                // if there is a recentHit and no direction, choose a direction
                else
                {

                    int oldRow = row;
                    int oldCol = column;
                    initializeProbability(probabilityGrid);
                    calculateProbability(probabilityGrid, gridp1PUBLIC, submarine_up, destroyer_up, battleship_up, carrier_up, 5);
                    int maxValue = -1;
                    if (recent_row - 1 >= 0 && probabilityGrid[recent_row - 1][recent_column] >= maxValue && gridp1PUBLIC[recent_row - 1][recent_column] == '~')
                    {
                        maxValue = probabilityGrid[recent_row - 1][recent_column];
                        row = recent_row - 1;
                        column = recent_column;
                    }

                    // Check down
                    if (recent_row + 1 < 10 && probabilityGrid[recent_row + 1][recent_column] >= maxValue && gridp1PUBLIC[recent_row + 1][recent_column] == '~')
                    {
                        maxValue = probabilityGrid[recent_row + 1][recent_column];
                        row = recent_row + 1;
                        column = recent_column;
                    }

                    // Check left
                    if (recent_column - 1 >= 0 && probabilityGrid[recent_row][recent_column - 1] >= maxValue && gridp1PUBLIC[recent_row][recent_column - 1] == '~')
                    {
                        maxValue = probabilityGrid[recent_row][recent_column - 1];
                        row = recent_row;
                        column = recent_column - 1;
                    }

                    // Check right
                    if (recent_column + 1 < 10 && probabilityGrid[recent_row][recent_column + 1] >= maxValue && gridp1PUBLIC[recent_row][recent_column + 1] == '~')
                    {
                        maxValue = probabilityGrid[recent_row][recent_column + 1];
                        row = recent_row;
                        column = recent_column + 1;
                    }

                    Loop = false;
                    if (oldRow == row && oldCol == column)
                    {
                        recentHit = false;
                        Loop = true;
                    }
                }
            }

            if (move[0] != 's' && move[0] != 'S')
                printf("Row = %d , Column = %d \n", row + 1, column + 1);
            printf("Move of bot is: %s \n", move);
            switch (move[0])
            {

            case 'F':
            case 'f':
                hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                if (hit != '~')
                {
                    printf("Hit\n");
                 
                    // change stuff based on having recentHit and recentHit2
                    // recentHit only --> no direction yet
                    // recentHit2 --> there is a direction
                    if (!recentHit)
                    {
                        recentHit = true;
                        recent_row = row;
                        recent_column = column;
                    }
                    else if (recentHit && !recentHit2)
                    {
                        recentHit2 = true;
                        current_ROW = row;
                        current_COLUMN = column;

                        if (current_ROW > recent_row)
                        {
                            down = true;
                            up = false;
                            right = false;
                            left = false;
                        }
                        else if (current_ROW < recent_row)
                        {
                            down = false;
                            up = true;
                            right = false;
                            left = false;
                        }
                        else if (current_COLUMN > recent_column)
                        {
                            down = false;
                            up = false;
                            right = true;
                            left = false;
                        }
                        else
                        {
                            down = false;
                            up = false;
                            right = false;
                            left = true;
                        }

                    }
                    else
                    {  
                        current_ROW = row;
                        current_COLUMN = column;
                    }
                }
                else
                {
                    printf("Miss\n");
                    recentHit2 = false;
                    up = false;
                    down = false;
                    left = false;
                    right = false;
                }
                total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);
                if (RecentSunkP2)
                {
                    recentHit = false; // stop recentHit
                    recentHit2 = false;
                }
                break;
            case 'R':    // Radar move
            case 'r':
                if (RadarSweepP2 > 0)
                {
                    bool a = Radar(gridp1SECRET, SmokeGridP1, row, column);
                    if (a)
                    {
                        printf("Enemy ships found.\n");
                        RadarHit = true;
                        radar_row = row;
                        radar_column = column;
                    }
                    else
                    {
                        printf("No enemy ships found.\n");
                        RadarHit = false;
                        if (SmokeScreenP1 == SunkShipsP2)
                        {
                            gridp1PUBLIC[row][column] = 'o';
                            gridp1PUBLIC[row + 1][column] = 'o';
                            gridp1PUBLIC[row][column + 1] = 'o';
                            gridp1PUBLIC[row + 1][column + 1] = 'o';
                        }
                    }
                    RadarSweepP2--;
                }
                else
                {
                    printf("No radar sweeps left. You lost your turn.\n");
                }

                /* Call the radar function, if true change RadarHit and radar_row and radar_column for the next round
                if false change the grid of player 1
                
                */

                break;
            case 'S': // Smoke
            case 's':
                if (SmokeScreenP2 > 0)
                {
                    int row_SMOKE = 0;
                    int column_SMOKE = 0;
                    bool found = false;
                    for (int i = 0; i < 10; i++)
                    {
                        for (int j = 0; j < 10; j++)
                        {
                            if (gridp2SECRET[i][j] != '~' && SmokeGridP2[i][j] != 1)
                            {

                                row_SMOKE = i;
                                column_SMOKE = j;
                                found = true;
                                break;
                            }
                        }
                        if (found)
                            break;
                    }

                    SmokeScreen(SmokeGridP2, row_SMOKE, column_SMOKE);
                    SmokeScreenP2--;
                }
                else
                {
                    printf("No smokescreens left. You lost your turn.\n");
                }
                RecentSunkP2 = false;


                /*
                
                Smoke just changes the smokeGrid. 
                Smoke chooses coordinates based on the first ship that has not been sunk for the bot.
                */
                break;
            case 'A': // Artillery
            case 'a':
         /*
         Artillery fires a 2x2 based on the calculated coordinates. */

                if (RecentSunkP2)
                {
                    bool did_hit = false;
                    int isSunkCurrent = SunkShipsP1;
                    hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                    {
                        did_hit = true;
                        recentHit = true;
                        recent_column = column;
                        recent_row = row;
                    }
                    total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);

                    hit = Fire(row + 1, column, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                    {
                        did_hit = true;
                        recentHit = true;
                        recent_column = column;
                        recent_row = row + 1;
                    }
                    total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);

                    hit = Fire(row, column + 1, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                    {
                        did_hit = true;
                        recentHit = true;
                        recent_column = column + 1;
                        recent_row = row;
                    }
                    total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);
                    hit = Fire(row + 1, column + 1, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                    if (hit != '~')
                    {
                        did_hit = true;
                        recentHit = true;
                        recent_column = column + 1;
                        recent_row = row + 1;
                    }
                    total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);

                    if (did_hit)
                        printf("hit\n");
                    else
                        printf("miss\n");

                    if (SunkShipsP1 != isSunkCurrent)
                        RecentSunkP2 = true;
                }
                else
                {
                    printf("You didn't sink a ship in the round before. You lost your turn.\n");
                }
                break;

            case 'T': //Torpedo
            case 't':
            /*
         Torpedo calculates the probabilites for every row and column
         and then chooses to fire the row or column with the highest probability.*/
                if (SunkShipsP1 == 3 && RecentSunkP2)
                {
                    bool did_hit = false;
                    bool torpedoRow = false;
                    bool torpedoColumn = false;
                    initializeProbability(probabilityGrid);
                    calculateProbability(probabilityGrid, gridp1PUBLIC, submarine_up, destroyer_up, battleship_up, carrier_up, 5);

                    int CoordinateShoot = 0;
                    int max = 0;
                    for (int i = 0; i < 10; i++)
                    {
                        int currentSum = 0;
                        for (int j = 0; j < 10; j++)
                        {
                            currentSum += probabilityGrid[i][j];
                        }
                        if (currentSum > max)
                        {
                            max = currentSum;
                            torpedoRow = true;
                            torpedoColumn = false;
                            CoordinateShoot = i;
                        }
                    }

                    for (int i = 0; i < 10; i++)
                    {
                        int currentSum = 0;
                        for (int j = 0; j < 10; j++)
                        {
                            currentSum += probabilityGrid[j][i];
                        }
                        if (currentSum > max)
                        {
                            max = currentSum;
                            torpedoColumn = true;
                            torpedoRow = false;
                            CoordinateShoot = i;
                        }
                    }

                    if (torpedoRow)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(CoordinateShoot, i, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                            if (hit != '~')
                            {
                                did_hit = true;
                                recentHit = true;
                                recent_row = CoordinateShoot;
                                recent_column = i;
                            }
                            total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);
                        }
                    }

                    else
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(i, CoordinateShoot, gridp1PUBLIC, gridp1SECRET, sunkShipGrid, isHardDiff, countRound % 2);
                            if (hit != '~')
                            {
                                did_hit = true;
                                recentHit = true;
                                recent_row = i;
                                recent_column = CoordinateShoot;
                            }
                            total_fireBOT(hit, gridp1SECRET, sunkShipGrid, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up, &recentHit, &recentHit2);
                        }
                    }
                }
                else
                {
                    printf("You don't meet the conditions to do a torpedo. You lost your turn.\n");
                }
                break;

            default:
                break;
            }
            countRound++;

            free(move);
        }

        printf("\n\n\n\n\n");
    }
    
    // Game ended

    if (SunkShipsP1 == 4)
        printf("Bot wins.\n");
    else
        printf("%s wins.\n", name1);
    free(name1);

    return 0;
} 

// if hit -> go up if present or go down if present or go left or goo right and keep on going where present
//