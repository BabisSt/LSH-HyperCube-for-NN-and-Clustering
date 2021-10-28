#include <iostream>
#include <queue>
#include <cmath>
#include <functional>

#include "../include/data.h"

using namespace std;

Data::Data()
{
    this->distanceFunction = &(this->EuclideanDistance);
}

int Data::InitMnistDataSet(ifstream &inputFile)
{
    int32_t a = 0 ;
    uint8_t b = 0;

    //magic number

    inputFile.read((char *)(&a), sizeof(a));
    if(!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }

    //number of images
    inputFile.read((char *)(&a), sizeof(a));
    if(!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }

    //from big endian to little endian
    this->n = __builtin_bswap32(a);
    cout << "number of images : " << this-> n << endl;

    inputFile.read((char *)(&a), sizeof(a));
    if(!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    int x = __builtin_bswap32(a);

    inputFile.read((char *)(&a), sizeof(a));
    if(!inputFile)
    {
        cerr << "Input file io error" << endl;
        return -1;
    }
    int y = __builtin_bswap32(a);

    this->d = x*y;

    for(int i =0; i< this->d;i++)
    {
        vector<uint8_t> image;

        for(int j=0; j < this->d; j++)
        {
            inputFile.read((char *)(&b), sizeof(b));
            if(!inputFile)
            {
                cerr << "read() from input file failed" << endl;
                return -1;
            }
            image.push_back(b);
        }
        this->data.push_back(image);
    }

    return 0;
    
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