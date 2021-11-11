#include <iostream>
#include <cmath>
#include <bits/stdc++.h>

#include "../include/hyperCube.h"

using namespace std;

// /*
// use 2 unordered sets instead of map as they dont add time complexity for searching
// since 2 O(1) == O(1)
// while using less space since they only store the keys and not the values asssociated with them
// */

// f::f() {}

// f::~f() {}

// int f::calculate_f(string key)
// {
//     int num;
//     default_random_engine re(chrono::system_clock::now().time_since_epoch().count());
//     uniform_int_distribution<int> unif(0, 2);

//     if(this->setofZeros.find(key) != this->setofZeros.end())
//         return 0;
//     else if (this->setofOnes.find(key) != this->setofOnes.end())
//         return 1;
//     else
//     {
//         num = unif(re) % 2;
//         if (num == 0)
//         {
//             this->setofZeros.insert(key);
//             return 0;
//         }
//         else
//         {
//             this->setofOnes.insert(key);
//             return 1;
//         }
//     }

//     return -1;
// }


// HyperCube::HyperCube(int k, int M, int probes, Data &data, uint32_t w, uint32_t m)
//     : k(k), M(M), probes(probes), data(data)
// {
//     this->ht = new hashTable(pow(2, k), k, data.d, w, m, pow(2, 32/ this->k));

//     //initialize array of f functions

//     for (int i = 0; i < k; i++)
//     {
//         f *f_instance = new f();
//         this->fTable.push_back(f_instance);
//     }

//     hashData();
    
// }

// HyperCube::~HyperCube()
// {
//     for (int i = 0; i < k; i++)
//     {
//         delete this->fTable[i];
//     }
//     delete this->ht;
    
// }

// int HyperCube::Run(const vector<vector<uint8_t>> &queries, ofstream &outputFileint, const int &N, const int &R)
// {
//     for (int i = 0; i < int(queries.size()); i++)
//     {
//         auto cubeStart = chrono::high_resolution_clock::now();
//         vector<pair<int, int>> cubeResult = this->exec_query(queries[i], N);
//         auto cubeStop = chrono::high_resolution_clock::now();

//         auto tCube = chrono::duration_cast<chrono::milliseconds>(cubeStop - cubeStart);

//         auto tStart = chrono::high_resolution_clock::now();
//         vector<pair<int, int>> trueResult = this->data.BruteForceNeighbors(queries[i], N);
//         auto tStop = chrono::high_resolution_clock::now();

//         auto tTrue = chrono::duration_cast<chrono::milliseconds>(tStop - tStart);

//         this->print(outputFileint, i, cubeResult, trueResult, tCube.count(), tTrue.count(), this->data.RangeSearch(queries[i], R));
//     }

//     return 0;
    
// }

// void HyperCube::hashData()
// {
//     for (int i = 0; i < this->data.n; i++)
//     {
//         string s = "";
//         for (int j = 0; j < this->k; j++)
//         {
//             s +=to_string(this->fTable[j]->calculate_f(to_string(this->ht->calculate_h(this->data.data[i], this->ht->S[j]))));
//         }

//         this->hyperCubeInsert(s,i, this->data.data[i]);
        
//     }
    
// }

// void HyperCube::hyperCubeInsert(const string &s, int index, vector<uint8_t> &point)
// {
//     this->ht->insertItem(stoi(s, nullptr, 2), index, point);
// }

// vector<pair<int, int>> HyperCube::exec_query(const vector<uint8_t> &query, const int &N)
// {
//     vector<pair<int, vector<uint8_t>>> possible_neighbors;
//     list<string> bucketList;
//     list<string>::iterator it;
//     string s = "";
//     int counter = 0;
//     int m;

//     for (int i = 0; i < this->k; i++)
//     {
//         s = s + to_string(this->fTable[i]->calculate_f(to_string(this->ht->calculate_h(query, this->ht->S[i]))));
//     }

//     bucketList = HammingDist(s, probes);
//     bucketList.push_front(s);

//     it = bucketList.begin();
//     while (counter < M && it != bucketList.end())
//     {
//         m  = stoi(*it, nullptr, 2);
//         for (auto &image : this->ht->getItems(m))
//         {
//             possible_neighbors.emplace_back(image.first, image.second);
//             counter++;
//             if(counter >= M)
//                 break;
//         }
//         ++it;
        
//     }

//     return this->data.GetClosestNeighbors(query, possible_neighbors, N);
    
// }

// string HyperCube::toBinary(int n, int size)
// {
//     string r;

//     while (n !=0)
//     {
//         r = (n % 2 == 0 ? "0" : "1") + r;
//         n /=2;
//     }

//     while (int(r.size()) < size)
//     {
//         r = "0" + r;
//     }
    
//     return r;
    
// }

// int HyperCube::hamming(string str1, string str2)
// {
//     int i = 0, count =0;

//     while (str1[i] != '\0')
//     {
//         if(str1[i] != str2[i])
//         {
//             count++;
//         }
//         i++;
//     }
    
//     return count;
// }

// list<string> HyperCube::HammingDist(const string s, int probes)
// {
//     list<string> l;
//     string curr;
//     int level =1;
//     int counter = 0;
//     int dist;

//     while (1)
//     {
//         for (int  i = 0; i < pow(2, s.size()); i++)
//         {
//             curr = toBinary(i, s.size());
//             dist = hamming(s,curr);
//             if(dist == level)
//             {
//                 l.push_back(curr);
//                 counter++;
//             }

//             if(counter >= probes)
//                 return l;
//         }

//         level++;
//     }
    
//     return l;
// }

// void HyperCube::print(ofstream &outputfile,const int &query,vector<pair<int, int>> cubeResult,vector<pair<int, int>> trueResult,const int64_t &tCube, const int64_t &tTrue,vector<pair<int, int>> rangeSearch)
// {
//     outputfile << "Query: " <<query << endl;

//     for (int i = 0; i < int(cubeResult.size()); i++)
//     {
//         outputfile << "Nearest neighbor-" << i << ": " << cubeResult[i].second << endl;
//         outputfile << "distanceCube: " << cubeResult[i].first << endl;
//         outputfile << "distanceTrue: " << trueResult[i].first << endl << endl;
//     }

//     outputfile << "tCube: " << tCube << endl;
//     outputfile << "tTrue: " << tTrue << endl;
//     outputfile << "R-near neighbors: " << endl;

//     for (const auto &point : rangeSearch)
//     {
//         outputfile << point.second << endl;
//     }
    
// }