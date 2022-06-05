#include <cfloat>
#include <climits>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;
using std::unordered_map;
using std::vector;
using std::pair;
using std::make_pair;


static void findStonePairsWithSameWeight(const vector<double> & stones,
                vector<pair<unsigned int, unsigned int>> & result)
{
    unordered_map<double, bool> stoneSet;
    unordered_map<double, unsigned int> tbl;

    for(unsigned int i = 0 ; i < stones.size() ; i ++) {
        auto itr = tbl.find(stones[i]);
        if(itr != tbl.end()) {
            if(stoneSet.end() == stoneSet.find(stones[i])) {
                stoneSet[stones[i]] = true;
                result.push_back(make_pair(itr->second, i));
            }
        } else {
            tbl[stones[i]] = i;
        }
    } 
}

//////////////////////////////////////////////////////////////////////
//Function:         findAllStonePairs
//Input:
//      stones:     positions of stones
//      delta:      difference of the stone pairs that we want to find
//Output:
//      result:     all stone pairs matched.
//Notes:
//      the max value of weight is UINT_MAX
//
void findAllStonePairs(const vector<double> & stones,
        int delta, vector<pair<unsigned int, unsigned int>> & result)
{
    if(stones.size() < 2) {
        return;
    }
    
    //if delta is 0, we go easy way
    if(delta == 0) {
        findStonePairsWithSameWeight(stones, result);
        return;
    }
    
    if(delta < 0) {
        delta = -delta;
    }

    double little, big;
    double boundary = UINT_MAX - delta;
    unordered_map<double, unsigned int> tbl;
    unordered_map<double, bool> answer;
   
    //construct a htable of stones' weight
    for(unsigned int i = 0 ; i < stones.size() ; i ++) {
        tbl[stones[i]] = i;
    }

    for(auto & p : tbl) {
        //in case of int overflow
        if(p.second > boundary) {
            continue;
        }

        //find stone pair
        auto itr = tbl.find(stones[p.second] + delta);
        if(itr == tbl.end()) {
            continue;
        }
        
        //found a pair, now we check if any duplicated pair exists
        if(p.first > itr->first) {
            little  = itr->first;
        } else {
            little  = p.first;
        }
        
        if(answer.end() != answer.find(little)) {
            continue;
        }
        
        //it's a new pair, add it into answer list
        result.push_back(make_pair(itr->second, p.second));
        answer[little] = true;
    }
}


void TEST_CASE(const char * message, const vector<double> & stones, double delta,
        const vector<pair<unsigned int, unsigned int>> & answer)
{
    vector<pair<unsigned int, unsigned int>> pairs;
    findAllStonePairs(stones, delta, pairs);

    bool result = false;
    if(pairs.size() == answer.size()) {
        for(auto & p : pairs) {
            bool found = false;
            for(auto & ans : answer) {
                if((p.first == ans.first && p.second == ans.second) 
                        || (p.first == ans.second && p.second == ans.first)) {
                    found = true;
                    break;
                }
            }
            if(!found) {
                goto out;
            }
        }
        result = true;
    }

out:
    if(result) 
        cout << message << " Success" << endl;
    else
        cout << message << " Failed" << endl;

    assert(result == true);
}

int main(int argc, char * argv[]) {

    TEST_CASE("TestCase1", {UINT_MAX, UINT_MAX-1.0}, 1.0, {{0,1}});
    TEST_CASE("TestCase2", {}, 1.0, {});
    TEST_CASE("TestCase2", {1,2,3,4,5,6,7,8,9,10}, 2.0, {
            {0,2},{1,3},{2,4},{3,5},{4,6},{5,7},{6,8},{7,9}
    });
    //TEST_CASE .....
    //
    return 0;
}

