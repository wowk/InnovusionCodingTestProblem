#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <bitset>
#include <cassert>
#include <cerrno>
#include <cstdint>


using std::vector;
using std::stack;
using std::unordered_map;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

struct Cell {
    explicit Cell(uint32_t _x, uint32_t _y, uint32_t _pos) :
        x(_x), y(_y), pos(_pos) {}
    uint32_t x, y;
    uint64_t pos;
};

void visitGraph(unordered_map<uint64_t, vector<Cell>> & graph, 
        uint64_t pos, uint64_t destination,
        unordered_map<uint64_t, bool> visited,
        vector<pair<uint32_t, uint32_t>> & path,
        vector<vector<pair<uint32_t, uint32_t>>> & paths)
{
    struct Node {
        explicit Node(uint64_t _pos, vector<Cell>::iterator _iter) :
            pos(_pos), iter(_iter) {}
        uint64_t pos;
        vector<Cell>::iterator iter;
    };

    stack<Node> st;

    st.push(Node(0, graph[0].begin()));
    visited[0] = true;

    while(!st.empty()) {
        auto & top = st.top();
        
        if(top.iter == graph[top.pos].end()) {
            visited[top.pos] = false;
            st.pop();
            path.pop_back();
            continue;
        }

        if(visited[top.iter->pos]) {
            top.iter ++;
            continue;
        }
        
        path.push_back(make_pair(top.iter->x, top.iter->y));
        if(top.iter->pos == destination) {
            paths.push_back(path);
            path.pop_back();
        } else {
            visited[top.iter->pos] = true;
            st.push(Node(top.iter->pos, graph[top.iter->pos].begin()));
        }
        top.iter ++;
    }
}

//////////////////////////////////////////////////////////////////////////
//Function:             findCountOfPaths
//Input:
//      M:              the count of rows of the grid, a postive number
//      N:              the count of columns of the grid, a postive number
//      snakes:         the positions of snakes
//      paths:         the container used to hold the possible paths
//Ouput:
//      the possible paths
//  
int findCountOfPaths(uint32_t M, uint32_t N, 
        const vector<pair<uint32_t, uint32_t>> & snakes,
        vector<vector<pair<uint32_t, uint32_t>>> & paths) 
{
    assert(M && N);

    unordered_map<uint64_t, bool> snakePos;
    
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

    unordered_map<uint64_t, vector<Cell>> graph;
    //construct a graph of all possible nodes
    //every edge is a step that rabbit can choose
    for(unsigned int row = 0 ; row < M ; row ++) {
        for(unsigned int col = 0 ; col < N ; col ++) {
            uint64_t pos = row * N + col;
            if(snakePos[pos]) {
                continue;
            }

            uint64_t down = pos + N;
            if((row + 1 < M) && !snakePos[down]) {
                graph[pos].push_back(Cell(row + 1, col, down));        
            }

            uint64_t right = pos + 1;
            if((col + 1 < N) && !snakePos[right]) {
                graph[pos].push_back(Cell(row, col + 1, right));
            }

            uint64_t up = pos - N;
            if(up && (row >= 1) && !snakePos[up]) {
                graph[pos].push_back(Cell(row - 1, col, up));        
            }

            uint64_t left = pos - 1;
            if(left && (col >= 1) && !snakePos[left]) {
                graph[pos].push_back(Cell(row, col - 1, left));
            } 
        }
    }
    
    //visit the graph to find all paths
    uint64_t start = 0;
    uint64_t destination = (M * N -1);
    vector<pair<uint32_t, uint32_t>> path;
    unordered_map<uint64_t, bool> visited;

    path.push_back(make_pair(0, 0));
    if(M > 1 || N > 1) {
        visitGraph(graph, 0, destination, visited, path, paths);
    } else if(!snakePos[0]) {
        paths.push_back(path);
    } 

    return 0;
}


void TEST_CASE(const char * message, uint32_t M, uint32_t N, 
        const vector<pair<uint32_t, uint32_t>> & snakes,
        const vector<vector<pair<uint32_t,uint32_t>>> & answeres)
{
    auto answerCompare = [](const vector<vector<pair<uint32_t,uint32_t>>> & a1, 
            const vector<vector<pair<uint32_t,uint32_t>>> & a2) -> bool 
    {
        if(a1.size() != a2.size()) {
            return false;    
        } else if(a1.size() == 0) {
            return true;
        }
        
        for(int i = 0 ; i < a1.size() ; i ++) {
            bool found = true;
            for(int j = 0 ; j < a2.size() ; j ++) {
                if(a1[i].size() != a2[j].size()) {
                    continue;
                }
                for(int k = 0 ; k < a1[i].size() ; k ++) {
                    if(a1[i][k].first != a2[j][k].first) {
                        found = false;
                        break;
                    }
                    if(a1[i][k].second != a2[j][k].second) {
                        found = false;
                        break;
                    }
                }
                if(found) {
                    return true;
                }
            }
        }

        return false;
    };

    vector<vector<pair<uint32_t,uint32_t>>> paths;
    findCountOfPaths(M, N, snakes, paths);
    bool result;
    if(answeres.size() == paths.size()) {
        result = answerCompare(paths, answeres);
    } else {
        result = false;
    }

    if(result) 
        cout << message << " Success" << endl;
    else 
        cout << message << " Failed" << endl;
    
    assert(result == true);
}

int main(int argc, char * argv[]) {
    
    TEST_CASE("TestCase1", 1, 1, {{0,0}}, {});
    TEST_CASE("TestCase2", 1, 1, {}, {{{0,0}}});

    TEST_CASE("TestCase3", 1, 2, {}, {{{0,0},{0,1}}});
    TEST_CASE("TestCase4", 2, 1, {}, {{{0,0},{1,0}}});

    TEST_CASE("TestCase5", 2, 2, {}, {
            {{0,0}, {1,0}, {1,1}},
            {{0,0}, {0,1}, {1,1}},
    });
    TEST_CASE("TestCase6", 2, 2, {{0,1}}, {
            {{0,0}, {1,0}, {1,1}},
    });
    TEST_CASE("TestCase7", 2, 2, {{1,0}}, {
            {{0,0}, {0,1}, {1,1}},
    });
    TEST_CASE("TestCase8", 2, 2, {{1,0},{0,1}}, {});
    TEST_CASE("TestCase9", 2, 2, {{0,0},{1,0},{0,1}}, {});
    TEST_CASE("TestCase10", 2, 2, {{1,1},{1,0},{0,1}}, {});
    TEST_CASE("TestCase11", 2, 2, {{0,0},{1,0},{0,1},{1,1}}, {});

    TEST_CASE("TestCase12", 3, 3, {}, {
            {{0,0}, {1,0}, {2,0}, {2,1}, {2,2}},
            {{0,0}, {1,0}, {1,1}, {2,1}, {2,2}},
            {{0,0}, {1,0}, {1,1}, {1,2}, {2,2}},

            {{0,0}, {0,1}, {1,1}, {2,1}, {2,2}},
            {{0,0}, {0,1}, {1,1}, {1,2}, {2,2}},
            {{0,0}, {0,1}, {0,2}, {1,2}, {2,2}},

            {{0,0}, {0,1}, {0,2}, {1,2}, {1,1}, {1,0}, {2,0}, {2,1}, {2,2}},
            {{0,0}, {0,1}, {0,2}, {1,2}, {1,1}, {2,1}, {2,2}},
            {{0,0}, {0,1}, {1,1}, {1,0}, {2,0}, {2,1}, {2,2}},

            {{0,0}, {1,0}, {2,0}, {2,1}, {1,1}, {0,1}, {0,2}, {1,2}, {2,2}},
            {{0,0}, {1,0}, {2,0}, {2,1}, {1,1}, {1,2}, {2,2}},
            {{0,0}, {1,0}, {1,1}, {0,1}, {0,2}, {1,2}, {2,2}},
    });
    TEST_CASE("TestCase13", 3, 3, {{1,1}}, {
            {{0,0}, {0,1}, {0,2}, {1,2}, {2,2}},
            {{0,0}, {1,0}, {2,0}, {2,1}, {2,2}},
    });
    TEST_CASE("TestCase14", 3, 3, {{1,1}}, {
            {{0,0}, {0,1}, {0,2}, {1,2}, {2,2}},
            {{0,0}, {1,0}, {2,0}, {2,1}, {2,2}},
    });
    TEST_CASE("TestCase15", 3, 3, {{2,1}, {1,2}}, {});
    TEST_CASE("TestCase16", 3, 3, {{2,2}}, {});
    TEST_CASE("TestCase17", 3, 3, {{0,1},{1,0}}, {});
    
    return 0;
} 

