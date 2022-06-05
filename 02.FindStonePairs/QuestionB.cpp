//
// Space complexity O(N)
// Time complexity O(N)
// Cornor Cases:
//      
//

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


static bool findOneStonePairWithSameWeight(const vector<double> & stones,
                pair<unsigned int, unsigned int> & result)
{
    unordered_map<double, unsigned int> tbl;
    for(unsigned int i = 0 ; i < stones.size() ; i ++) {
        auto itr = tbl.find(stones[i]);
        if(itr != tbl.end()) {
            result = make_pair(itr->second, i);
            return true;
        } else {
            tbl[stones[i]] = i;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////
//Function:         findOneStonePair
//Input:
//      stones:     positions of stones
//      delta:      difference of the stone pairs that we want to find
//Output:
//      result:     all stone pairs matched.
//Notes:
//      the max value of weight is UINT_MAX
//
bool findOneStonePair(const vector<double> & stones, 
        double delta, pair<unsigned int,unsigned int> & result) 
{
    if(stones.size() < 2) {
        return false;
    }

    //if delta is 0, we go easy way
    if(delta == 0) {
        return findOneStonePairWithSameWeight(stones, result);
    }

    if(delta < 0) {
        delta = -delta;
    }
    
    double boundary = UINT_MAX - delta;
    unordered_map<double, unsigned int> tbl;

    //construct a htable of stones' weight
    for(unsigned int i = 0 ; i < stones.size() ; i ++) {
        tbl[stones[i]] = i;
    }

    //find stone pair
    for(int i = 0 ; i < stones.size() ; i ++) {

        //in case of int overflow
        if(stones[i] > boundary) {
            continue;
        }

        //found a pair, return
        auto itr = tbl.find(stones[i] + delta);
        if(itr != tbl.end()) {
            result = make_pair(i, itr->second);
            return true;
        }
    }
    
    return false;
}

 
void TEST_CASE(const char * message, const vector<double> & stones, double delta,
        bool exist, const pair<unsigned int, unsigned int> & answer = {0,0})
{
    pair<unsigned int, unsigned int> pair;

    bool result = false;
    if(exist != findOneStonePair(stones, delta, pair)) 
        goto out;

    if(exist) {
        if((pair.first == answer.first && pair.second == answer.second) 
                || (pair.first == answer.second && pair.second == answer.first)) {
            result = true;
        }
    } else {
        result = true;
    }

out:
    if(result) 
        cout << message << " Success" << endl;
    else
        cout << message << " Failed" << endl;

    //assert(result == true);
}

int main(int argc, char * argv[]) {

    TEST_CASE("TestCase1", {UINT_MAX, UINT_MAX-0.5, UINT_MAX - 1.0}, 1.0, true, {0,2});
    TEST_CASE("TestCase2", {UINT_MAX, UINT_MAX-0.5, UINT_MAX - 1.0}, 1.5, false);
    TEST_CASE("TestCase3", {}, 1, false);
    TEST_CASE("TestCase4", {1.0, 2.0}, 1.5, false);

    return 0;
}
 
