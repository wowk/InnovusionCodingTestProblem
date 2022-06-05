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
////////////////////////////////////////////////////////
// Function:        findCountOfPathes
// Input:
//      M:          the count of rows of the grid
//      N:          the count of columns of the grid
//      snakes:     the positions of all snakes
// Output:
//      the count of all possible pathes.
// 
// Errors:
//      if M, N or snakes are invalid, 
//      errcode EINVAL is returned
//
int findCountOfPathes(unsigned int M, unsigned int N, 
        const vector<pair<unsigned int, unsigned int>> & snakes) 
{
    assert(M && N);
    
    //swap the width and height if necessary so that we can
    //handle the problem more easier.
    bool exchg = false;
    if(M < N) {
        exchg = true;
        std::swap(M, N);
    }

    //construct a htable of positions of snakes
    //so that we can check if a location is ocuppied
    //by a snake quickly.
    unordered_map<int, bool> snakePos;

    unsigned int x, y;
    for(auto & snake : snakes) {
        // if we have swapped the width and the height of the grid
        // we also need to convert the locatioin of snakes
        x = snake.first;
        y = snake.second;
        if(exchg) {
            std::swap(x,y);
        }
        if(x == 0 && y == 0) {
            return 0;
        } else if(x == M - 1 && y == N - 1) {
            return 0;
        }
        assert(x < M && y < N);
        snakePos[x * N + y] = true;
    }

    vector<unsigned int> line(N, 0);
    //initialize the count of pathes to the nodes at fist raw
    for(unsigned int col = 0 ; col < N ; col ++) {
        if(snakePos[col]) {
            break;
        }
        line[col] = 1;
    }
    line[0] = 0;
    
    unsigned int count = 1;
    bool snakeEncounter = false;
    //now, calculate the count of pathes to the last node
    //according to equation: count[i][j] = count[i-1][j]  + count[i][j-1]
    for(unsigned int row = 1 ; row < M ; row ++) {
        //if we encounter snake at first column
        if(snakeEncounter || snakePos[row*N]) {
            snakeEncounter = true;
            count = 0;
        } else {
            count = 1;
        }

        for(unsigned int col = 1 ; col < N ; col ++) {
            unsigned int pos = row * N + col;
            if(snakePos[pos]) {
                //if a position is ocuppied by a snake
                count = 0;
                line[col] = 0;
                //cout << "Find Snake at (" << row << ", " << col << ")" << endl;
                continue;
            }
            line[col] += count;
            count = line[col];
            //cout << "Update grid[" << row << "][" << col << "] => " << line[col] << endl;
        }
    }
    
    return line[N-1];
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
    TEST_CASE("TestCase1", 1, 1, {}, 0);
    TEST_CASE("TestCase2", 1, 1, {{0,0}}, 0);
    
    TEST_CASE("TestCase3", 2, 2, {}, 2);
    TEST_CASE("TestCase4", 2, 2, {{0,0}}, 0);
    TEST_CASE("TestCase5", 2, 2, {{1,1}}, 0);
    TEST_CASE("TestCase6", 2, 2, {{1,0}}, 1);
    TEST_CASE("TestCase7", 2, 2, {{0,1}}, 1);
    
    TEST_CASE("TestCase8", 3, 3, {{0,1}, {0,2}}, 3);
    TEST_CASE("TestCase9", 3, 3, {{1,0}, {2,0}}, 3);
    TEST_CASE("TestCase10", 3, 3, {{1,1}}, 2);

    TEST_CASE("TestCase11", 5, 6, {{0,3},{4,0},{3,2},{2,5}}, 55);
    TEST_CASE("TestCase12", 6, 5, {{3,0},{0,4},{2,3},{5,2}}, 55);

    return 0;
}
 
