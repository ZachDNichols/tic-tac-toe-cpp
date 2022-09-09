#include <iostream>
#include <ios>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <chrono>
#include <thread>

/* Tic-Tac Toe in the Console
    By Zach Nichols
*/

//This checks if the input from the player is an actual number, since the game will be played using only number selections
bool check_number(std::string str)
{
    for (int i = 0; i < str.length(); i++)
        if (isdigit(str[i]) == false)
        {
            std::cout << std::endl << "Please enter a number." << std::endl << std::endl;
            return false;
        }

    return true;
}

//This checks if the input provided is outside the range, as to avoid the player going "out of bounds".
bool check_range(std::string str, int lowerLimit, int higherLimit)
{
    int temp = stoi(str);

    if (temp > higherLimit || temp < lowerLimit)
    {
        std::cout << std::endl << "Please enter a number within " << lowerLimit << " and " << higherLimit << "." << std::endl << std::endl;
        return false;
    }
    return true;
}

//Determines which console a user is using and clears it to make the game look nice!
int clear_console()
{
    #ifdef _WIN32
        system("cls");
    #endif

    #if __APPLE__
        system("clear");
    #endif

    #if __linux__
        system("clear");
    #elif __unix__
        system("clear");
    #endif

    return 0;
}

//Prints out the board shown under the comment in main()
void print_board(char positions[3][3])
{
    std::cout << " |     |     |     | " << std::endl;
    std::cout << "_|__" << positions[0][0] << "__|__" << positions[0][1] << "__|__" << positions [0][2] << "__|_" << std::endl;
    std::cout << "_|__" << positions[1][0] << "__|__" << positions[1][1] << "__|__" << positions [1][2] << "__|_" << std::endl;
    std::cout << "_|__" << positions[2][0] << "__|__" << positions[2][1] << "__|__" << positions [2][2] << "__|_" << std::endl;
    std::cout << " |     |     |     | " << std::endl << std::endl << std::endl;
}

//Makes sure the input is both a number and within desired range
bool input_valid(std::string input, int lower, int upper)
{
    bool isNumber = false;
    isNumber = check_number(input);
        if(isNumber)
            isNumber = check_range(input, lower, upper);
    
    if(isNumber)
        return true;
    
    return false;
}

//Checks for a winner with a thicc if statement
bool check_for_player_winner(char p[3][3], char x)
{
    // std::cout << "Checking for winner..." << std::endl;

    if (
        (p[0][0] == x && p[0][1] == x && p[0][2] == x) || 
        (p[0][2] == x && p[1][1] == x && p[2][0] == x) ||
        (p[0][0] == x && p[1][1] == x && p[2][2] == x) ||
        (p[2][0] == x && p[2][1] == x && p[2][2] == x) ||
        (p[1][0] == x && p[1][1] == x && p[1][2] == x) ||
        (p[0][0] == x && p[1][0] == x && p[2][0] == x) ||
        (p[0][1] == x && p[1][1] == x && p[2][1] == x) ||
        (p[0][2] == x && p[1][2] == x && p[2][2] == x)
        )
        {
            return true;
        }
    else
        return false;
}

//Validates a selection chosen by either party
bool make_selection(char p[3][3], int row, int column)
{
    if(p[row][column] != ' ')
        return false;
    
    return true;
}

//Checks for a tie
bool tie_check(char p[3][3])
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; i < 2; j++)
            if(p[i][j] == ' ')
                return false;
    
    return true;
}

int main()
{
    using namespace std::this_thread;
    using namespace std::chrono;
    /*
    Basic layout of the game
         |     |     |     | 
        _|__X__|__X__|__X__|_
        _|__X__|__X__|__X__|_
        _|__X__|__X__|__X__|_
         |     |     |     | 
    */
   
    //The selection the player will later provide
    int playerChoice;
    //The positions on the game board. Represented by spaces to be empty at the start.
    char positions[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

    //Row and column will be used by the player to select where they wish to play their spot.
    int row;
    int column;

    int cpuRow;
    int cpuColumn;
    
    //Saves if the player and computer are an X or O
    char player;
    char cpu;

    //Used to determine at the end of the game if the player wants to play again
    int playAgain;
    
    //Used to evaluate player inputs as proper numbers
    bool isNumber = false;
    std::string input;
    isNumber = false;

    //Use to check for a winner
    bool pWinner = false;
    bool cWinner = false;

    //Used to make sure the spot selected is open
    bool validSelection = false;

    //Used for the computer to determine if it has found a good spot, if not it resorts to going forward a row and column from the player.
    bool cpuSelected;

    //Asks the player if they wish to be an X or an O
    while(!isNumber)
    {
        std::cout << "Do you wish to be an X or an O?" << std::endl << std::endl << "X: 1" << std::endl << "O: 2" << std::endl << std::endl;
        std::cin >> input;
        isNumber = input_valid(input, 1, 2);
    }

    playerChoice = stoi(input);

    clear_console();

    //Assigns the player to either X or O based on their selection.
    if(playerChoice == 1)
    {
        std::cout << "You are X's!" << std::endl;
        player = 'X';
        cpu = 'O';
    }
    else
    {
        std::cout << "You are O's!" << std::endl;
        player = 'O';
        cpu = 'X';
    }


    //This loop is the game
    while(!pWinner && !cWinner)
    {
        print_board(positions);
        isNumber = false;

        //Makes sure a player puts in a number between 1 and 3
        while(!validSelection)
        {
        //Player selects a row, gets checked for valid input, converted to int, and saved
        while(!isNumber)
        {
            std::cout << "Player, please select a row." << std::endl << std::endl;
            std::cin >> input;

            isNumber = input_valid(input, 1, 3);
        }

        row = stoi(input);
        std::cout << "Row " << row << " selected." << std::endl << std::endl;
        row--;
        isNumber = false;
        
        //Player selects a column, gets checked for valid input, converted to int, and saved
        while(!isNumber)
        {
            std::cout << "Player, please select a column." << std::endl << std::endl;
            std::cin >> input;

            isNumber = input_valid(input, 1, 3);
        }

        column = stoi(input);
        std::cout << "Column " << column << " selected." << std::endl << std::endl;
        column--;

        validSelection = make_selection(positions, row, column);

        //Checks if the player didn't choose a spot that was already chosen
        if(!validSelection)
            std::cout << "Invalid selection. Please try again." << std::endl;
        }

        //Saves the coordinates in a 3D array
        positions[row][column] = player;
        

        clear_console();

        print_board(positions);
        std::cout << "This is your selection." << std::endl;
        
        //Checks if the player won and stops the game if true
        pWinner = check_for_player_winner(positions, player);

        //Breaks if the player won
        if(pWinner)
            break;

        //Tie check after player turn
        if(tie_check(positions))
        {
            while (!isNumber)
            {
                std::cout << "It was a tie!" << std::endl << "Would you like to play again?" << std::endl << std::endl << "1: Yes" << std::endl << "2: No" << std::endl << std::endl;
                std::cin >> input;

                isNumber = input_valid(input, 1, 2);
            }

            playAgain = stoi(input);
            clear_console();
            if(playAgain == 1)
                main();
            
            break;
        }

        //This makes the program wait a second in order for the player to get an idea of what is happening on screen and make some text readable.
        //Without these statements, text would flash on screen and be difficult to play and understand what was going on.
        sleep_for(seconds(1));
        sleep_until(system_clock::now() + seconds(1));
        
        std::cout << "Computer turn..." << std::endl;

        sleep_for(seconds(1));
        sleep_until(system_clock::now() + seconds(1));

        clear_console();
        validSelection = false;

        //A very messy, ineffeicent algorithm for the computer to try and determine the best place to put their mark.
        //First part evaluates if the computer has an opportunity to win, if not it moves on to stopping the player from winning.

        //Checks for three in a direct line like | or ---
        for(int i = 0; i < 2; i++)
        {
            if (positions[i][0] == cpu && positions[i][1] == cpu)
            {
                cpuRow = i;
                cpuColumn = 2;
                cpuSelected = true;
                break;
            }
            if (positions[i][0] == cpu && positions[i][2] == cpu)
            {
                cpuRow = i;
                cpuColumn = 1;
                cpuSelected = true;
                break;
            }
            if (positions[i][1] == cpu && positions[i][2] == cpu)
            {
                cpuRow = i;
                cpuColumn = 0;
                cpuSelected = true;
                break;
            }
        }
        // Checks for diagonal like '/'
        if(positions[0][2] == cpu && positions[1][1] == cpu)
        {
            cpuRow = 2;
            cpuColumn = 0;
            cpuSelected = true;
        }
        if(positions[0][2] == cpu && positions[2][0] == cpu)
        {
            cpuRow = 1;
            cpuColumn = 1;
            cpuSelected = true;
        }
        if(positions[1][1] == cpu && positions[2][0] == cpu)
        {
            cpuRow = 0;
            cpuColumn = 2;
            cpuSelected = true;
        }

        //Checks for diagonal likle '\'
        if(positions[0][0] == cpu && positions[1][1] == cpu)
        {
            cpuRow = 2;
            cpuColumn = 2;
            cpuSelected = true;
        }
        if(positions[0][0] == cpu && positions[2][2] == cpu)
        {
            cpuRow = 1;
            cpuColumn = 1;
            cpuSelected = true;
        }
        if(positions[2][2] == cpu && positions[1][1] == cpu)
        {
            cpuRow = 0;
            cpuColumn = 0;
            cpuSelected = true;
        }

        //Now trying to stop the player from winning

        //Checks for three in a direct line like | or ---
        for(int i = 0; i < 2; i++)
        {
            if (positions[i][0] == player && positions[i][1] == player)
            {
                cpuRow = i;
                cpuColumn = 2;
                cpuSelected = true;
                break;
            }
            if (positions[i][0] == player && positions[i][2] == player)
            {
                cpuRow = i;
                cpuColumn = 1;
                cpuSelected = true;
                break;
            }
            if (positions[i][1] == player && positions[i][2] == player)
            {
                cpuRow = i;
                cpuColumn = 0;
                cpuSelected = true;
                break;
            }
        }
        // Checks for diagonal like '/'
        if(positions[0][2] == player && positions[1][1] == player)
        {
            cpuRow = 2;
            cpuColumn = 0;
            cpuSelected = true;
        }
        if(positions[0][2] == player && positions[2][0] == player)
        {
            cpuRow = 1;
            cpuColumn = 1;
            cpuSelected = true;
        }
        if(positions[1][1] == player && positions[2][0] == player)
        {
            cpuRow = 0;
            cpuColumn = 2;
            cpuSelected = true;
        }

        //Checks for diagonal likle '\'
        if(positions[0][0] == player && positions[1][1] == player)
        {
            cpuRow = 2;
            cpuColumn = 2;
            cpuSelected = true;
        }
        if(positions[0][0] == player && positions[2][2] == player)
        {
            cpuRow = 1;
            cpuColumn = 1;
            cpuSelected = true;
        }
        if(positions[2][2] == player && positions[1][1] == player)
        {
            cpuRow = 0;
            cpuColumn = 0;
            cpuSelected = true;
        }

        if(!cpuSelected)
        {
            cpuRow = row + 1;
            cpuColumn = column + 1;
        }
        
        //Validates to make sure the computer did not choose an already taken spot
        while(!validSelection)
        {
            validSelection = make_selection(positions, cpuRow, cpuColumn);
            if(validSelection)
                break;
            if (cpuRow < 2)
                cpuRow++;
            else
                cpuRow = 0;
            validSelection = make_selection(positions, cpuRow, cpuColumn);
            if(validSelection)
                break;
            if (cpuColumn < 2)
                cpuColumn++;
            else
                cpuColumn = 0;
        }

        //Marks the computer's spot
        positions[cpuRow][cpuColumn] = cpu;

        print_board(positions);

        std::cout << "This is the computer's selection." << std::endl << std::flush;
        sleep_for(seconds(1));
        sleep_until(system_clock::now() + seconds(1));
        clear_console();

        //Checks if the computer won
        cWinner = check_for_player_winner(positions, cpu);
        
        //If computer won then the game ends
        if(cWinner)
            break;

        isNumber = false;
        //Checks for a tie after the computer's turn
        if(tie_check(positions))
        {
            while (!isNumber)
            {
                std::cout << "It was a tie!" << std::endl << "Would you like to play again?" << std::endl << std::endl << "1: Yes" << std::endl << "2: No" << std::endl << std::endl;
                std::cin >> input;

                isNumber = input_valid(input, 1, 2);
            }

            playAgain = stoi(input);
            clear_console();
            if(playAgain == 1)
                main();
            
            break;
        }

        validSelection = false;
    }

    isNumber = false;

//Checks if computer won
    if(cWinner)
    {
        std::cout << "You lost to the computer, shameful..." << std::endl;

        //Loop to makes sure valid selection on player again
        while(!isNumber)
        {
                std::cout << "Would you like to play again?" << std::endl << std::endl << "1: Yes" << std::endl << "2: No" << std::endl << std::endl;
                std::cin >> input;

                isNumber = input_valid(input, 1, 2);
        }
        //Clears console and calls main if play again was selected, other wise main will return and end the program.
        playAgain = stoi(input);
        clear_console();
        if(playAgain == 1)
            main();
    }

    //Checks if player won
    if (pWinner)
    {
        std::cout << "You won against to the computer! Don't get too cocky..." << std::endl;

        //Loop to makes sure valid selection on player again
        while(!isNumber)
        {
                std::cout << "Would you like to play again?" << std::endl << std::endl << "1: Yes" << std::endl << "2: No" << std::endl << std::endl;
                std::cin >> input;

                isNumber = input_valid(input, 1, 2);
        }
        //Clears console and calls main if play again was selected, other wise main will return and end the program.
        playAgain = stoi(input);
        clear_console();
        if(playAgain == 1)
            main();
    }
    return 0;
}

