#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>

#include "../include/hashTable.h"

using namespace std;



hashTable::hashTable(int indexSize, uint32_t k, uint32_t d, uint32_t w,uint32_t M)    //thelei init oi metablites
    : d(d), w(w), M(M)
{
    this->table.resize(indexSize);
    this->calculate_v(this->v,k,d);
}

hashTable::~hashTable() {}


void hashTable::calculate_v(vector<vector<double>> &v,uint32_t k, uint32_t d)
{
    default_random_engine re(chrono::steady_clock::now().time_since_epoch().count());
    normal_distribution<double> distribution(0, 1);

    v.resize(k, vector<double>(d));

    for (uint32_t i = 0; i < k; i++)
    {
        for (uint32_t j = 0; j < d; j++)
        {
           
            this->v[i][j] = distribution(re);
        }
        
    }

}

void hashTable::calculate_t(uint32_t w,uint32_t k)
{
    srand( (unsigned)time( NULL ) );

    for (uint32_t i = 0; i < k; i++)
    {
        this->t.push_back(((rand() % w) + 1));
    }
    
}

uint32_t hashTable::calculate_h(const vector<uint32_t> &point,const vector<double> &v,uint32_t k)
{
    uint32_t x=0 ;
    uint32_t result = 0;
    
    for (uint32_t i = 0; i < point.size(); i++)
    {
        calculate_t(this->w,k);
        if(this->t[i] != 0)
        {
        x +=point[i] * abs(v[i]);
        result = floor( double(x + this->t[i] )/ double(this->w) );

        }
    }

    return  result % this->M ;
}


void hashTable::Insert_Item(uint32_t g, int index, vector<uint32_t> &point)           
{
   
    this->table[g % this->table.size()].emplace_back(g,index,ref(point));           
}


vector<pair<int,reference_wrapper<vector<uint32_t>>>> hashTable::Get_Items(const uint32_t &g)
{
    vector<pair<int, reference_wrapper<vector<uint32_t>>>> result;

    for (auto &bucket: this->table[g % this->table.size()])     
    {
        result.emplace_back(get<1>(bucket), get<2>(bucket));
    }

    return result;
}
