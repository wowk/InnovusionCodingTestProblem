#include <iostream>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <cerrno>


using std::vector;
using std::unordered_map;
using std::pair;
using std::cout;
using std::endl;


////////////////////////////////////////////////////////////////////////
//Function:             findCountOfPathes
//Input:
//      M:              the count of rows of the grid, a postive number
//      N:              the count of columns of the grid, a postive number
//      snakes:         the positions of snakes
//Ouput:
//      the count of possible pathes
//
int findCountOfPathes(unsigned int M, unsigned int N, 
        const vector<pair<unsigned int, unsigned int>> & snakes) 
{
    assert(M && N);

    unordered_map<int, bool> snakePos;

    //construct a position htable of the snakes
    for(auto & snake : snakes) {
        if(snake.first == 0 && snake.second == 0) {
            return 0;
        } else if(snake.first == M - 1 && snake.second == N - 1) {
            return 0;
        }
        assert(snake.first < M && snake.second < N);
        snakePos[snake.first * N + snake.second] = true;
    }

    vector<vector<int>> grid(M, vector<int>(N, 0));
    //initialize the count of pathes to the nodes at fist column
    for(unsigned int col = 0 ; col < N ; col ++) {
        if(snakePos[col]) {
            break;
        }
        grid[0][col] = 1;
        //cout << "Initialize grid[0][" << col << "] => " << grid[0][col] << endl;
    }

    //initialize the count of pathes to the nodes at fist raw
    for(unsigned int row = 0 ; row < M ; row ++) {
        if(snakePos[row * N]) {
            break;
        }
        grid[row][0] = 1;
        //cout << "Initialize grid[" << row << "][0] => " << grid[row][0] << endl;
    }
    
    grid[0][0] = 0;

    //now, calculate the count of pathes to the last node
    //according to equation: count[i][j] = count[i-1][j]  + count[i][j-1]
    for(unsigned int row = 1 ; row < M ; row ++) {
        for(unsigned int col = 1 ; col < N ; col ++) {
            unsigned int pos = row * N + col;
            if(snakePos[pos]) {
                //cout << "Find Snake at (" << row << ", " << col << ")" << endl;
                continue;
            }
            grid[row][col] = grid[row-1][col] + grid[row][col-1];                
            //cout << "Update grid[" << row << "][" << col << "] => " << grid[row][col] << endl;
        }
    }

    return grid[M-1][N-1];
}


void TEST_CASE(const char * message, unsigned int M, unsigned int N, 
        const vector<pair<unsigned int, unsigned int>> & snakes, unsigned int answer) 
{
    unsigned int ret = findCountOfPathes(M, N, snakes);
    if(answer == ret)
        cout << message << " Success" << endl;
    else {
        cout << message << " Failed, wrong answer: "  << ret << endl;
    }
    assert(answer == ret);
}


int main(int argc, char * argv[]) {
    TEST_CASE("TestCase2", 1, 1, {}, 0);
    TEST_CASE("TestCase3", 1, 1, {{0,0}}, 0);
    
    TEST_CASE("TestCase4", 2, 2, {}, 2);
    TEST_CASE("TestCase5", 2, 2, {{0,0}}, 0);
    TEST_CASE("TestCase6", 2, 2, {{1,1}}, 0);
    TEST_CASE("TestCase7", 2, 2, {{1,0}}, 1);
    TEST_CASE("TestCase8", 2, 2, {{0,1}}, 1);
    
    TEST_CASE("TestCase9", 3, 3, {{0,1}, {0,2}}, 3);
    TEST_CASE("TestCase10", 3, 3, {{1,0}, {2,0}}, 3);
    TEST_CASE("TestCase11", 3, 3, {{1,1}}, 2);

    TEST_CASE("TestCase12", 5, 6, {{0,3},{4,0},{3,2},{2,5}}, 55);

    return 0;
}

