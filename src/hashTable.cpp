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
    //cout << "index" << indexSize << endl;
    this->table.resize(indexSize);
    
    this->calculate_v(this->v,k,d);
}

hashTable::~hashTable() {}


void hashTable::calculate_v(vector<vector<double>> &v,uint32_t k, uint32_t d)
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

void hashTable::calculate_t(uint32_t w,uint32_t k)
{
    srand( (unsigned)time( NULL ) );

    for (int i = 0; i < k; i++)
    {
        this->t = ((rand() % w) + 1);
    }
    
}

int hashTable::calculate_h(const vector<uint32_t> &point,const vector<double> &v,uint32_t k)
{
    int x=0 ,result = 0;
    
    calculate_t(this->w,k);
    for (int i = 0; i < point.size(); i++)
    {
        x +=point[i] * abs(v[i]);
    }

    result = floor(double(x + this->t )/ double(this->w));
    //cout << "pont size" << point.size() << endl;
    //cout << "x" << x << " t" << this->t << "w" << this->w << endl;
    return  result % this->M ;
}



void hashTable::insertItem(uint32_t g, int index, vector<uint32_t> &point)           //pair: provides a way to store two heterogeneous objects as a single unit
{
    //cout << "g" << g << " size" <<this->table.size() << endl;
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
