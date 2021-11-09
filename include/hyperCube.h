#include <list>
#include <unordered_set>
#include <vector>
#include <string>
#include <chrono>
#include <random>

#include "hashTable.h"
#include "data.h"

using namespace std;

class f
{
    private:
        unordered_set<string> setofZeros;
        unordered_set<string> setofOnes;

    public:
        f();
        ~f();

        int calculate_f(string key);
    
};

class HyperCube
{
    private:
        int k, M, probes;
        Data &data;
        hashTable *ht;
        vector<f *> fTable;

        uint32_t calculate_h(const vector<uint8_t> &x, const vector<int> &s);
        int calculate_a(const uint8_t &xi, const int &si);

        string toBinary(int n, int size);

        int hamming(string str1, string str2);

        list<string> HammingDist(string s, int probes);

        void print(
            ofstream &outputfile,
            const int &query,
            vector<pair<int, int>> cubeResult,
            vector<pair<int, int>> trueResult,
            const int64_t &tCube, const int64_t &tTrue,
            vector<pair<int, int>> rangeSearch);
        
        void hashData();
        void hyperCubeInsert(const string &s, int index, vector<uint8_t> &point);
    
    public:
        HyperCube(int k, int M, int probes, Data &data, uint32_t w = 40000, uint32_t m = pow(2, 30) - 5);
        ~HyperCube();

        int Run(const vector<vector<uint8_t>> &queries, ofstream &outputFileint, const int &N, const int &R);
        vector<pair<int, int>> exec_query(const vector<uint8_t> &query, const int &N);
};