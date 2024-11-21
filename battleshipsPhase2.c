#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Given the number which represents the ship, another function calls this when a ship has been sunk

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

void chooseMove(int RadarP2, int SmokeP2, int ArtilleryP2, int TorpedoP2, char *x)
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
        else if (randomValue <= 75 && RadarP2 > 0)
        {
            strcpy(x, "Radar");
        }
        else if (randomValue > 75 && SmokeP2 > 0)
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
        else if (countRound % 2 == 1)
        { // ALL FUNCTIONS SAME AS PLAYER 1
            printf("It's the bot's turn %s\n\n");
            // player 2 turn
            printArray(gridp1PUBLIC);
            //    printf("Choose one of the following options: \n");
            //    printf("1. Fire (Fire B3)\n");
            //    printf("2. Radar Sweep (Radar B3) (%d left)\n", RadarSweepP2);
            //    printf("3. SmokeScreen (Smoke B3) (%d left)\n", SmokeScreenP2);
            //    printf("4. Artillery (Artillery B3) (%d left)\n", RecentSunkP2);
            //    printf("5. Torpedo (Torpedo B/3) (%d left)\n", (RecentSunkP2 && SunkShipsP1 ==3));

            char *move = (char *)malloc(10 * sizeof(char));
            char *coordinates = (char *)malloc(10 * sizeof(char));
            chooseMove(RadarSweepP2, SmokeScreenP2, RecentSunkP2, (RecentSunkP2 && SunkShipsP1 == 3), move);
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
                hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                if (hit != '~')
                    printf("Hit\n");
                else
                    printf("Miss\n");
                total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);
                break;
            case 'R':
            case 'r':
                if (RadarSweepP1 > 0)
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
                    SmokeScreen(SmokeGridP2, row, column);
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
                    total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);

                    hit = Fire(row + 1, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);

                    hit = Fire(row, column + 1, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);

                    hit = Fire(row + 1, column + 1, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                    if (hit != '~')
                        did_hit = true;
                    total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);

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
                    if (isColLetter)
                    {
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(i, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                            if (hit != '~')
                                did_hit = true;
                            total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);
                        }
                    }
                    else
                    {
                        if (column == 0 && coordinates[1] == '0')
                            column = 9;
                        for (int i = 0; i < 10; i++)
                        {
                            hit = Fire(column, i, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                            if (hit != '~')
                                did_hit = true;
                            total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);
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

            default:
                break;
            }
            countRound++;

            free(move);
            free(coordinates);
        }

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }

    if (SunkShipsP1 == 4)
        printf("%s wins.\n", name2);
    else
        printf("%s wins.\n", name1);
    free(name1);
    
    return 0;
}







// if hit -> go up if present or go down if present or go left or goo right and keep on going where present
//
