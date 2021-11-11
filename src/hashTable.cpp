#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>

#include "../include/hashTable.h"

using namespace std;



hashTable::hashTable(int indexSize, int k, int d, int w,int M)    //thelei init oi metablites
{
    this->table.resize(indexSize);
    this->M =2;
    this->t = 2;
    this->w= 2;
    this->M =2;
    
    this->calculate_v(this->v,k,d);
}


void hashTable::calculate_v(vector<vector<double>> &v,int k, int d)
{
    default_random_engine re(chrono::steady_clock::now().time_since_epoch().count());
    normal_distribution<double> distribution(0, 1);

    v.resize(k, vector<double>(d));

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < d; j++)
        {
           
            this->v[i][j] = distribution(re);
        }
        
    }

}


void hashTable::calculate_t(int w,int k)
{
    srand( (unsigned)time( NULL ) );

    for (int i = 0; i < k; i++)
    {
        this->t = ((rand() % w) + 1);
        cout << this->t << endl;
    }
    
}

int hashTable::calculate_h(vector<uint32_t> &point, vector<double> &v)
{
    int x=0 ,result = 0;
    
    for (int i = 0; i < point.size(); i++)
    {
        x +=point[i] * v[i];
    }

    result = floor(double(x + this->t )/ double(this->w));
    return  result % this->M ;
}


hashTable::~hashTable() {}


void hashTable::insertItem(uint32_t g, int index, vector<uint32_t> &point)           //pair: provides a way to store two heterogeneous objects as a single unit
{
    this->table[g % this->table.size()].emplace_back(g,index,ref(point));           //emplace_back : inserts a new element at the end of the vector
}

vector<pair<int,reference_wrapper<vector<uint32_t>>>> hashTable::getItems(const uint32_t &g)
{
    vector<pair<int, reference_wrapper<vector<uint32_t>>>> result;

    for (auto &bucket: this->table[g % this->table.size()])     // auto: directs the compiler to use the initialization expression of a declared variable to deduce its type
    {
        result.emplace_back(get<1>(bucket), get<2>(bucket));
    }

    return result;
}

//get_index_size