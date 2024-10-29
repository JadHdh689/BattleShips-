#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>




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
        if(i!=9)
        printf("%ld)  ", num);
        else printf("%ld) ", num);
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

int getCOLUMNindex(char ltr)
{

    ltr = toupper(ltr); // to upper does no change unless its a letter
    int curr = (int)ltr;

    if (ltr >= 'A' && ltr <= 'Z')
        return (curr - 65);
    return curr - '1'; // turpedo case....
                       // COLUMN I &S A LETTER, COLUMN INDEX RECEIVE LETTER AND COMPARE IT WITH CAPITAL A TO KNOW WHICH LETTER
                       // USE ASCII
}

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

void placeShips(char grid[10][10], char *name, int size){
bool check = false;
do
    {
        printArray(grid);
        printf("Please enter coordinates for the %s (%d cells) ", name, size);
        char *Coordinates = (char *)malloc(15 * sizeof(char));
        scanf("%14s", Coordinates);
        printf("\n");
        char temp = (int)Coordinates[1];
        int row = temp - '0';                     // B3 --> SECOND INDEX IS THE ROW
        int col = getCOLUMNindex(Coordinates[0]); // B3 --> FIRST INDEX IS THE COLUMN
        bool isHorizontal = true;                 // initializing
        if (Coordinates[3] == 'v' || Coordinates[3] == 'V'|| Coordinates[4] == 'V' || Coordinates[4] == 'v')   // B10, Vertical
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
    } while (check == false);
}

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

/* smoke screen: make radar think this 2x2 stuff is water
radar: goes by a 2x2, if there is a ship, return ship found
radar can go over grid that has ship and smoke screen --> Return true if ship not hidden

both utilize if given b3 --> does on b3, (b+1 = col+1)3, b(3+1 = row + 1 = 4) , c4 (row +1 and col + 1)

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
    if (row != 9 && col != 9){
        if (grid[row+1][col+1] != '~' && SmokeGrid[row + 1][col + 1] != 1)
            return true; }
            return false;
    // check all b3 c3 b4 c4
    // if b9 or at the edge dont do it
}
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
    else{
        *recentSunk = false;
    }
}
int main()
{
    srand(time(0));
    printf("Choose difficulty: (easy/ hard) == (0/1) ");
    int num = 0;
    scanf("%ld", &num);
    bool isHardDiff = true;
    if (num == 0)
        isHardDiff = false;

    printf("\n");
    printf("Please enter name 1: ");
    char *name1 = (char *)malloc(50 * sizeof(char)); // 50 chars allowed
    scanf("%49s", name1);                            // name1 hye already pointer, fa fina n3ml hek
    printf("\n");
    printf("Please enter name 2: ");
    char *name2 = (char *)malloc(50 * sizeof(char)); // 50 chars allowed
    scanf("%49s", name2);

    int random = rand() % 2; // takes a random integer --> even == 0, odd == 1

    char gridp1SECRET[10][10]; // usually give size to function, but we wont
    char gridp2SECRET[10][10];
    char gridp1PUBLIC[10][10];
    char gridp2PUBLIC[10][10];

    initializeArray(gridp1SECRET);
    initializeArray(gridp2SECRET);
    initializeArray(gridp1PUBLIC);
    initializeArray(gridp2PUBLIC);

    int initialPointColumn = 65;

    // if random = 0, start player 1 and player 2 normally
    // if random = 1, swap names so that player 2 starts first
    if (random == 1)
    {
        //
        char *temp = (char *)malloc(50 * sizeof(char)); // Temporary buffer for swapping
        strcpy(temp, name1);                            // Store name1 in temp
        strcpy(name1, name2);                           // Copy name2 to name1
        strcpy(name2, temp);                            // Copy temp (original name1) to name2
    }
    printf("\n\n\n\n\n");
    printf("%s please start first by putting all your ships.\n", name1);
    // place carrier
    placeShips(gridp1SECRET, "Carrier", 5);
    // place battleship
    placeShips(gridp1SECRET, "Battleship", 4);
    // put destroyer
    placeShips(gridp1SECRET, "Destroyer", 3);
    // put submarine
    placeShips(gridp1SECRET, "Submarine", 2);

    // USE DO WHILE LOOP : DO .... WHILE (BOOLEAN CURR == FALSE, DONT WORRY ABOUT ERROR , ERROR IS WRITTEN IN FUNCTION)

    // printf("%s %s", name1, name2);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("%s please put all your ships.\n", name2);
    // place carrier
    placeShips(gridp2SECRET, "Carrier", 5);
    // place battleship
    placeShips(gridp2SECRET, "Battleship", 4);
    // put destroyer
    placeShips(gridp2SECRET, "Destroyer", 3);
    // put submarine
    placeShips(gridp2SECRET, "Sumbarine", 2);

    

    

    int SunkShipsP1 = 0;
    int SunkShipsP2 = 0;

    int countRound = 0; // helps in switching between turns (even = player1, odd = player 2)
    char hit = '0';     // we will have a variable that receives what got hit (either water if no target or A NUMBER IF PART OF A SHIP GOT HIT)
    int RadarSweepP1 = 3;
    int RadarSweepP2 = 3;
    int SmokeScreenP1 = 0;
    int SmokeScreenP2 = 0;
    int Art1 = 0;
    int Art2 = 0;
    int TorpedoP1 = 0;
    int TorpedoP2 = 0;
    int SmokeGridP1[10][10]; // if box != 1 no smoke, box = 1 smoke , garbage values can't be = 1
    int SmokeGridP2[10][10];
    for(int i = 0; i< 10; i++){
        for(int j = 0; j<10 ; j++){
            SmokeGridP1[i][j] =0;
            SmokeGridP2[i][j] = 0;
        }
    }
    bool RecentSunkP1 = false;
    bool RecentSunkP2 = false;
      printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    while (SunkShipsP1 != 4 && SunkShipsP2 != 4)
    {
        if (countRound % 2 == 0)
        {
            printf("It's your turn %s\n\n" , name1);
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
                // hit is the NUMBER in char, check in SECRET IF IT IS NOT PRESENT, REMEMBER WE WOULDN'T RECEIVE IT UNLESS WE HIT IT
                // IF IS SUNK IS TRUE --> ISSUNK = 1
                // IF IT IS FALSE --> ISSUNK = 0
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
                /* how artillery works:
                boolean p = false;
                given B3
                hit B3, hit B4, hit C3, hit C4 (the function takes all correctly)
                and for each hit we check, if hit != water in any case --> p = true
                based on p give hit or miss
                 */
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
                        if(SunkShipsP2 != isSunkCurrent) RecentSunkP1 = true;
                }
                else
                {
                    printf("You didn't sink a ship in the round before. You lost your turn.\n");
                }
                break;

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
                        if(column == 0 && coordinates[1] == '0')
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

            default:
                break;
            }

            countRound++;
        }
        else if (countRound % 2 == 1)
        {
            printf("It's your turn %s\n\n", name2);    
            // player 2 turn
            printArray(gridp1PUBLIC);
            printf("Choose one of the following options: \n");
            printf("1. Fire (Fire B3)\n");
            printf("2. Radar Sweep (Radar B3) (%d left)\n", RadarSweepP2);
            printf("3. SmokeScreen (Smoke B3) (%d left)\n", SmokeScreenP2);
            printf("4. Artillery (Artillery B3) (%d left)\n", RecentSunkP2);
            printf("5. Torpedo (Torpedo B/3) (%d left)\n", (RecentSunkP2 && SunkShipsP1 ==3));

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
                hit = Fire(row, column, gridp1PUBLIC, gridp1SECRET, isHardDiff);
                if (hit != '~')
                    printf("Hit\n");
                else
                    printf("Miss\n");
                total_fire(hit, gridp1SECRET, &SunkShipsP1, &SmokeScreenP2, &RecentSunkP2);
                break;
                // hit is the NUMBER in char, check in SECRET IF IT IS NOT PRESENT, REMEMBER WE WOULDN'T RECEIVE IT UNLESS WE HIT IT
                // IF IS SUNK IS TRUE --> ISSUNK = 1
                // IF IT IS FALSE --> ISSUNK = 0
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
                /* how artillery works:
                boolean p = false;
                given B3
                hit B3, hit B4, hit C3, hit C4 (the function takes all correctly)
                and for each hit we check, if hit != water in any case --> p = true
                based on p give hit or miss
                 */
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

                        if(SunkShipsP1 != isSunkCurrent) RecentSunkP2 = true;
                }
                else
                {
                    printf("You didn't sink a ship in the round before. You lost your turn.\n");
                }
                break;

            case 'T': // ignore coordinates[1] use coordinates[0]
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
                        if(column == 0 && coordinates[1] == '0')
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
        }
    
    
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    
    
    
    
    }

if(SunkShipsP1 == 4) printf("%s wins.\n", name2);
else printf("%s wins.\n", name1);
return 0;

    
}
