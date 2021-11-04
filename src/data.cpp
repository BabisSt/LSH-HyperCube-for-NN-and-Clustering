#include <iostream>
#include <queue>
#include <cmath>
#include <functional>

#include "../include/data.h"

using namespace std;

Data::Data()
{
    this->distanceFunction = &(this->ManhattanDistance);
}

int Data::InitDataSet(ifstream &inputFile)
{

    
}

int Data::ReadQueryFile(ifstream &queryFile)
{
    int count =0;
    uint8_t b;
    vector<uint8_t> point(this->d);

    while (1)
    {
        queryFile.read((char *)(&b), sizeof(b));
        if(queryFile.eof())
        {  
            if(count !=0)
            {
                cerr << "Corupter query file" << endl;
                return -1;
            }
            break;
        }

        point[count] = b;
        if(++count == this->d)
        {
            this->queries.push_back(point);
            count = 0;
        }
        
    }
    
    return 0;
}

int Data::EuclideanDistance(const vector<uint8_t> &p1,const vector<uint8_t> &p2)
{
    int d =0;

    for(size_t i =0 ; i < p1.size(); i++)
    {
        d += pow(p2[i] - p1[i], 2);
    }

    return sqrt(d);
}

int Data::ManhattanDistance(const vector<uint8_t> &p1, const vector<uint8_t> &p2)
{
    int d=0;

    for (int i = 0; i < int(p1.size()); i++)
    {
        d += abs(p2[i] - p1[i]);
    }
    return d;    
}

vector<pair<int, int>> Data::RangeSearch(vector<uint8_t> query, float R)
{
    vector<pair<int, int>> result;

    for (int i=0; i < this->n; i++)
    {
        int d = this->distanceFunction(this->data[i], query);
        if(d < R)
        {
            result.emplace_back(d,i);
        }
    }

    return result;
}


vector<pair<int, int>> Data::GetClosestNeighbors(const vector<uint8_t> &query, const vector<pair<int, vector<uint8_t>>> &data, const int &N)
{
    vector<pair<int, int>> result;

    auto cmp = [](pair<int,int> left, pair<int,int> right){
        return left.first > right.first;
    };

    priority_queue<pair<int, int>, vector<pair<int,int>> , decltype(cmp)> q(cmp);

    for( int i=0; i< int(data.size()) ; i++)
    {
        q.push(make_pair(this->distanceFunction(data[i].second, query), data[i].first));
    }

    int min = (n<int(q.size())) ? N : q.size();

    for (int i=0; i < min; i++)
    {
        result.push_back(q.top());
        q.pop();
    }

    return result;
}

vector<pair<int, int>> Data::BruteForceNeighbors(const vector<uint8_t> &query, const int &N)
{
    vector<pair<int, int>> result;

    auto cmp = [](pair<int,int> left, pair<int,int> right){
        return left.first > right.first;
    };

    priority_queue<pair<int, int>, vector<pair<int,int>> , decltype(cmp)> q(cmp);

    for( int i=0; i< int(this->data.size()) ; i++)
    {
        q.push(make_pair(this->distanceFunction(this->data[i], query), i));
    }

    int min = (N <int(q.size())) ? N : q.size();

    for (int i=0; i < min; i++)
    {
        result.push_back(q.top());
        q.pop();
    }

    return result;
}