#include <iostream>
#include <random>
#include <chrono>

#include "../include/hashTable.h"

using namespace std;

hashTable::hashTable(int indexSize, int k, int d, uint32_t w, uint32_t m, uint32_t M)
    : d(d), w(w), m(m), M(M)                        //initialisation list
{
    this->md.resize(this->d, 0);
    this->md[1] = this->m % this-> M;

    this->table.resize(indexSize);
    this->calculate_s(this->S,k,d,w);
}

hashTable::~hashTable() {}

uint32_t hashTable::calculate_h(const vector<uint8_t> &x, const vector<int> &s)
{
    uint32_t h = uint32_t(calculate_a(x[this->d -1], s[this->d-1])) % this->M;

    for (int i = this->d-2; i >=0; i--)
    {
        if(this->md[this->d-1-i] == 0)
        {
            this->md[this->d-1-i] = (this->md[this->d-2-i]* this->md[1]) % this->M;
        }

        int a = calculate_a(x[i], s[i]);

        if(a!=0)
        {
            h += (((a % this->M) * this->md[this->d -1 -i]) % this->M) % this->M;
        }
    }

    return h % this->M;
}

int hashTable::calculate_a(const uint8_t &xi, const int &si)
{
    return floor(double((int(xi) - si)) / double(this->w));
}

void hashTable::calculate_s(vector<vector<int>> &S, int k, int d, int w)
{
    default_random_engine re(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> unif(0,w);                                            //Produces random integer 

    S.resize(k, vector<int>(d));

    for (int i=0; i < k ; i++)
    {
        for(int j=0; j<d ; j++)
        {
            S[i][j] = unif(re);
        }
    }
}

void hashTable::insertItem(uint32_t g, int index, vector<uint8_t> &point)           //pair: provides a way to store two heterogeneous objects as a single unit
{
    this->table[g % this->table.size()].emplace_back(g,index,ref(point));           //emplace_back : inserts a new element at the end of the vector
}

vector<pair<int,reference_wrapper<vector<uint8_t>>>> hashTable::getItems(const uint32_t &g)
{
    vector<pair<int, reference_wrapper<vector<uint8_t>>>> result;

    for (auto &bucket: this->table[g % this->table.size()])     // auto: directs the compiler to use the initialization expression of a declared variable to deduce its type
    {
        result.emplace_back(get<1>(bucket), get<2>(bucket));
    }

    return result;
}