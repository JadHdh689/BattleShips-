#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Given the number which represents the ship, another function calls this when a ship has been sunk

void printWhichShipBot(char a, bool *Submarine, bool *Destroyer, bool *BattleShip, bool *Carrier)
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
}

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
// put water in the grid before the game starts
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

    // create a line A ---> J
    // then we print before each row number i
    // then print every element in the row
}

void chooseMove(int RadarP2, int SmokeP2, int ArtilleryP2, int TorpedoP2, char *x, int RadarSweepP1, bool recentHit)
{

    if (TorpedoP2 >= 1)
    {
        strcpy(x, "Torpedo");
    }
    if (ArtilleryP2 >= 1)
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
        else if (randomValue <= 75 && RadarP2 > 0 && !recentHit)
        {
            strcpy(x, "Radar");
        }
        else if (randomValue > 75 && SmokeP2 > 0 && RadarSweepP1 > 0)
        {
            strcpy(x, "Smoke");
        }
        else
        {
            strcpy(x, "Fire");
        }
    }
}
int getCOLUMNindex(char ltr)
{

    ltr = toupper(ltr); // only changes if given a letter
    int curr = (int)ltr;

    if (ltr >= 'A' && ltr <= 'Z')
        return (curr - 65);
    return curr - '1'; // If we are doing the case of Turpedo, we might receive a row instead of a column which is a number
                       // if given a number, return the number - 1 to get the index
}

// This function checks if the placement of the ship is valid. If yes, place the ship using the a number based on number of places it takes
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

// it calls check and add
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
        check = CheckAndAdd(row, col, isHorizontal, size, grid);
        free(Coordinates);

    } while (check == false);
}

// fire function. returns what was hit. If the box aimed at is water --> miss. Else hit. We also change the private arrays.
char Fire(int row, int column, char givenPublic[10][10], char givenSecret[10][10], bool isHardDiff)
{
    if (row < 0 || column < 0 || row > 9 || column > 9)
        return '~'; // out of bounds -- no hit

    char ReturnChar = '0'; // initialize
    if (givenSecret[row][column] == '~')
    {
        if (!isHardDiff)
        {
            if (givenPublic[row][column] != '*') // if the spot isn't a sunken ship
                givenPublic[row][column] = 'o';  // just in the case of easy difficulty
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

// isSunk checks if the ship still exists
boolean isSunk(char given, char Matrix[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (Matrix[i][j] == given)
                return false; // 0 SUNK SHIPS ADDED WE FOUND A PIECE OF THE SHIP
        }
    }
    return true; // couldn't find the piece --> the ship has sunk
}

// SmokeScreen uses an alternative array to implement the functionality
// We will use && to check later if the area is a smoked area or a normal area
// Fill smoked boxes with number 1
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

// after each hit, check if a sink has been sunk. If a ship has been sunk, call the function to print which ship has been sunk.
// change recentSunk to true if a ship has been sunk
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

void total_fireBOT(char hit, char grid[10][10], int *sunkShips, int *smokeScreen, bool *recentSunk, bool *Submarine, bool *Destroyer, bool *BattleShip, bool *Carrier)
{
    if (hit != '~')
    {
        if (isSunk(hit, grid))
        {
            (*sunkShips)++;
            (*smokeScreen)++;
            *recentSunk = true;
            printWhichShipBot(hit, Submarine, Destroyer, BattleShip, Carrier);
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

// always call this function with shipSize = 4
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

    // USE DO WHILE LOOP : DO .... WHILE (BOOLEAN CURR == FALSE, DONT WORRY ABOUT ERROR , ERROR IS WRITTEN IN FUNCTION)

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

    int countRound = random; // helps in switching between turns (even = player1, odd = player 2)
    char hit = '0';          // we will have a variable that receives what got hit (either water if no target or A NUMBER IF PART OF A SHIP GOT HIT)
    int RadarSweepP1 = 3;    // Count how much radar sweeps left
    int RadarSweepP2 = 3;
    int SmokeScreenP1 = 0; // Count how much smokescreens left
    int SmokeScreenP2 = 0;
    int SmokeGridP1[10][10]; // if box != 1 no smoke, box = 1 smoke
    int SmokeGridP2[10][10];
    int probabilityGrid[10][10];
    int parityArray[20] = {0, 11, 22, 33, 44, 55, 66, 77, 88, 99, 5, 16, 27, 38, 49, 50, 61, 72, 83, 94};
    int parityRange = 20;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            SmokeGridP1[i][j] = 0;
            SmokeGridP2[i][j] = 0;
        }
    }
    // initialize smokegrids
    bool RecentSunkP1 = false;
    bool RecentSunkP2 = false;
    bool recentHit = false;
    bool submarine_up = true;
    bool destroyer_up = true;
    bool battleship_up = true;
    bool carrier_up = true;
    int recent_row = 0;      // for the bot
    int recent_column = 0;   // for the bot
    int current_ROW = 0;     // for after recenthit2
    int current_COLUMN = 0;  // for after recenthit2
    bool vertical = false;   // for the bot
    bool horizontal = false; // for the bot
    bool recentHit2 = false; // for the bot
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
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
                hit = Fire(row, column, gridp2PUBLIC, gridp2SECRET, isHardDiff);
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
                    hit = Fire(row, column, gridp2PUBLIC, gridp2SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row + 1, column, gridp2PUBLIC, gridp2SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row, column + 1, gridp2PUBLIC, gridp2SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp2SECRET, &SunkShipsP2, &SmokeScreenP1, &RecentSunkP1);

                    hit = Fire(row + 1, column + 1, gridp2PUBLIC, gridp2SECRET, isHardDiff);
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
                            hit = Fire(i, column, gridp2PUBLIC, gridp2SECRET, isHardDiff);
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
                            hit = Fire(column, i, gridp2PUBLIC, gridp2SECRET, isHardDiff);
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
            // player 2 turn
            printArray(gridp1PUBLIC);

            char *move = (char *)malloc(10 * sizeof(char));
            chooseMove(RadarSweepP2, SmokeScreenP2, RecentSunkP2, (RecentSunkP2 && SunkShipsP1 == 3), move, RadarSweepP1, recentHit);
            int row;
            int column;
            if (!recentHit)
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
                }
            } // if !recenthit GET coordinates for fire (row and column) either through carrier or prob grid

            else if ( recentHit && recentHit2)
            {

                if (left)
                {
                    if (current_COLUMN - 1 >= 0)
                        column = current_COLUMN - 1;
                    row = current_ROW;
                }
                else if (right)
                {
                    if (current_COLUMN + 1 <= 9)
                    {
                        column = current_COLUMN + 1;
                        row = current_ROW;
                    }
                }
                else if (up)
                {
                    if (current_ROW - 1 >= 0)
                        row = current_ROW - 1;
                    column = current_COLUMN;
                }
                else if (down)
                {
                    if (current_ROW + 1 <= 9)
                        row = current_ROW + 1;
                    column = current_COLUMN;
                }
            }

            else
             { // we have recent hit
                initializeProbability(probabilityGrid);
                calculateProbability(probabilityGrid, gridp1PUBLIC, submarine_up, destroyer_up, battleship_up, carrier_up, 5);
                int maxValue = 0;
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
                    if (recent_column - 1 >= 0 && probabilityGrid[recent_row][recent_column - 1] >= maxValue && gridp1PUBLIC[recent_row][recent_column -1] == '~')
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
                }
                
        
                

                //  recentHit2 --> keep going that side  --> recentSunk recentHit automatically goes
                //  4 booleans  --> up , left , down , right
                // recentHit 3ade
                // while having recentHit and hitting another hit
                // the other hit should decide the bool (up , left, down, right)
                // keep going up until recentHit goes away by the recentSunk condition....
            
            
            printf("Row = %d , Column = %d \n", row + 1, column + 1);
            printf("Move of bot is: %s \n", move);
            switch (move[0])
            {

            case 'F':
            case 'f':
                hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                if (hit != '~')
                {
                    printf("Hit\n");
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

                        // we hit the row and the column
                        // we want to compare current row and column with recent_row and recent_column
                        // then decide bool left, or bool right , or bool up , or bool down
                        // we should add to the conditions above that make the coordinates a new if(recentHit2)....
                    }
                    else
                    { // recentHit2 == true
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
                total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);
                if (RecentSunkP2)
                {
                    recentHit = false; // stop recentHit
                    recentHit2 = false;
                }
                break;
            case 'R':
            case 'r':
                if (RadarSweepP2 > 0)
                {
                    bool a = Radar(gridp1SECRET, SmokeGridP1, row, column);
                    if (a)
                    {
                        printf("Enemy ships found.\n");

                    }
                    else
                    {
                        printf("No enemy ships found.\n");
                    }
                    RadarSweepP2--;
                }
                else
                {
                    printf("No radar sweeps left. You lost your turn.\n");
                }

                break;
            case 'S':
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
                                if (gridp2SECRET[i][j] != '~')
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
                break;
            case 'A':
            case 'a':
                if (RecentSunkP2)
                {
                    bool did_hit = false;
                    int isSunkCurrent = SunkShipsP1;
                    hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);

                    hit = Fire(row + 1, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);

                    hit = Fire(row, column + 1, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);
                    hit = Fire(row + 1, column + 1, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);

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

            case 'T':
            case 't':
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
                            hit = Fire(CoordinateShoot, i, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                            if (hit != '~')
                            {
                                did_hit = true;
                            }
                            total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);
                        }
                    }

                    else
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(i, CoordinateShoot, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                            if (hit != '~')
                            {
                                did_hit = true;
                            }
                            total_fireBOT(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2, &submarine_up, &destroyer_up, &battleship_up, &carrier_up);
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

    if (SunkShipsP1 == 4)
        printf("Bot wins.\n");
    else
        printf("%s wins.\n", name1);
    free(name1);

    return 0;
}

// if hit -> go up if present or go down if present or go left or goo right and keep on going where present
//
