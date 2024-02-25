#include <iostream>

int size = 0;
int number_of_mines = 0;

int myStrcmp(const char* first, const char* second)
{
    if (!first || !second)
        return 0;

    while ((*first == *second) && (*first) && (*second))
    {
        first++;
        second++;
    }

    return (*first - *second);
}

void init(char** matrix, char value)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = value;
        }
    }
}

void print(char** grid, int numberOfMines) 
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            std::cout << grid[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "We are left with " << numberOfMines << " number of mines." << std::endl;

}

bool areValidIndices(char** matrix, int x, int y)
{
    return x >= 0 && x < size && y >= 0 && y < size && matrix[x][y] != '*';
}

bool areValid (int x, int y)
{
    return x >= 0 && x < 10 && y >= 0 && y < 10;
}

void inputValidation(char** matrix, int& x, int& y)
{
    std::cout << "Enter coordinates: \n";
    std::cin >> x >> y;

    while (!areValid(x, y))
    {
        std::cout << "Try again with different coordinates: \n";
        std::cin >> x >> y;
    }
}

void generateMinesweeperGrid(char** grid, int numberOfMines) 
{
    init(grid, '0');

    srand(time(NULL));

    int placedMines = 0;
    while (placedMines < numberOfMines)
    {
        int row = rand() % size;
        int coll = rand() % size;
        if (grid[row][coll] != '*')
        {
            grid[row][coll] = '*';
            placedMines++;

            for (int i = row - 1; i <= row + 1; i++) 
            {
                for (int j = coll - 1; j <= coll + 1; j++) 
                {
                    if (areValidIndices(grid, i, j))
                    {
                        grid[i][j]++;
                    }
                }
            }
        }
    }
}

bool inRange(int x, int y, int i, int j)
{
    return x + i >= 0 && x + i < 10 && y + j >= 0 && y + j < 10;
}

void reveal(char** matrix, char** resultMatrix, int x, int y)
{
    if (matrix[x][y] == '*')
        return;

    if (!areValid(x, y) || resultMatrix[x][y] != '_')
    {
        return;
    }
    resultMatrix[x][y] = matrix[x][y];
    if (matrix[x][y] == '0')
    {
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (inRange(x, y, i, j) && resultMatrix[x + i][y + j] == '_')
                {
                    reveal(matrix, resultMatrix, x + i, y + j);
                }
            }
        }
    }
}

void open(char** matrix, char** resultMatrix, int x, int y, int numberOfMines)
{
    if (resultMatrix[x][y] == 'X')
    {
        return;
    }

    if (matrix[x][y] == '*')
    {
        resultMatrix[x][y] = matrix[x][y];
        print(resultMatrix, numberOfMines);
        std::cout << "End of the game!" << std::endl;
        return;
    }
    else if (matrix[x][y] != '0')
    {
        resultMatrix[x][y] = matrix[x][y];
    }
    else if (matrix[x][y] == '0')
    {
        reveal(matrix, resultMatrix, x, y);
    }
}

void mark(char** matrix, char** resultMatrix, int x, int y, int& numberOfMines)
{
    if (resultMatrix[x][y] == '_')
    {
        resultMatrix[x][y] = 'X';
    }
    numberOfMines--;
}

void unmark(char** matrix, char** resultMatrix, int x, int y, int& numberOfMines)
{
    if (resultMatrix[x][y] == 'X')
    {
        resultMatrix[x][y] = '_';
        numberOfMines++;
    }
}

bool allRevelaed(char** matrix, char** resultMatrix, int x, int y, int numberOfMines)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (resultMatrix[i][j] == '_')
            {
                return false;
            }
        }
    }

    return numberOfMines == 0;
}

void getBackOldMines(char** resultMatrix, int x, int y)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (resultMatrix[x][y] == 'X')
            {
                resultMatrix[x][y] = '*';
            }
        }
    }
}

void inputMatrixSize(int& matrix_size)
{
    std::cout << "Enter matrix size: \n";
    std::cin >> matrix_size;

    while (matrix_size < 3 || matrix_size > 10)
    {
        std::cout << "Try again with different size: \n";
        std::cin >> matrix_size;
    }
}

void inputNumberOfMines(int number_of_mines, int matrix_size)
{
    std::cout << "Enter number of mines: \n";
    std::cin >> number_of_mines;

    while (number_of_mines < 1 || number_of_mines > 3 * matrix_size)
    {
        std::cout << "Try again with different number: \n";
        std::cin >> number_of_mines;
    }
}

char** allocateMatrix(int n)
{
    char** matrix = new char* [n];
    for (int i = 0; i < n; i++)
    {
        matrix[i] = new char[n];
    }

    return matrix;
}

void free(char** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        delete[] matrix[i];
    }

    delete[] matrix;
}

int main()
{
    int x, y;
    char command[1024];
    inputMatrixSize(size);
    inputNumberOfMines(number_of_mines, size);
    std::cin.ignore();

    char** matrix = new char*[size]; // already generated matrix
    char** result = new char*[size]; // matrix for print
    
    matrix = allocateMatrix(size);
    result = allocateMatrix(size);
    
    generateMinesweeperGrid(matrix, number_of_mines); // function for generating a matrix
    init(result, '_'); // initialize the matrix we want to print
    print(result, number_of_mines); // function for print

    for (;;)
    {
        std::cout << "Enter command: \n";
        std::cin.getline(command, 1024);
        if (myStrcmp(command, "open") == 0)
        {
            inputValidation(matrix, x, y);
            std::cin.ignore();
            open(matrix, result, x, y, number_of_mines);
            if (result[x][y] == '*')
                break;

            print(result, number_of_mines);
        }
        else if (myStrcmp(command, "mark") == 0)
        {
            inputValidation(matrix, x, y);
            std::cin.ignore();
            mark(matrix, result, x, y, number_of_mines);
            print(result, number_of_mines);
        }
        else if (myStrcmp(command, "unmark") == 0)
        {
            inputValidation(matrix, x, y);
            std::cin.ignore();
            unmark(matrix, result, x, y, number_of_mines);
            print(result, number_of_mines);
        }
        else if (myStrcmp(command, "exit") == 0)
        {
            std::cout << "Invalid command!" << std::endl;
            break;
        }
        // check if we have a winner
        bool isFinished = allRevelaed(matrix, result, x, y, number_of_mines);
        if (isFinished)
        {
            getBackOldMines(result, x, y);
            print(result, number_of_mines);
            std::cout << "Congratulations, you win the game!" << std::endl;
            break;
        }
    }
    
    free(matrix, size);
    free(result, size);

	return 0;
}



