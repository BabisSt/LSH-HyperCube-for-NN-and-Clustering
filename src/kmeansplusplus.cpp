#include <cstdlib>
#include <cmath>
#include <cstring>
#include <random>
#include <chrono>
#include <algorithm>
#include <queue>
#include <unordered_set>

#include "../include/kmeansplusplus.h"

using namespace std;

// //classic 
kmeansplusplus::kmeansplusplus(const int &mClusters, const bool &complete, Data &data)
    :nClusters(mClusters), complete(complete), data(data)
{
    this->method = _Classic;
}

//lsh
kmeansplusplus::kmeansplusplus(const int &Clusters, const bool &complete,const int &lsh_k, const int &L, Data &data)
    : nClusters(Clusters), complete(complete), lsh_k(lsh_k), L(L), data(data)
{
    this->method = _LSH;
    
    this->lsh = new LSH(lsh_k, L, data);
}

//hypercube
kmeansplusplus::kmeansplusplus(const int &Clusters, const bool &complete,const int &cube_k, const int &M, const int &probes, Data &data)
    : nClusters(Clusters), complete(complete), cube_k(cube_k), M(M), probes(probes), data(data)
{
    this->method = _HyperCube;

    this->cube = new HyperCube(cube_k, M, probes, data);
}

kmeansplusplus::~kmeansplusplus()
{
    if(this->lsh !=nullptr)
        delete this->lsh;
    if(this->cube != nullptr)
        delete this->cube;
}

int kmeansplusplus::Run(ofstream &outputFile)
{
    vector<vector<int>> clusters;
    int totalChange = this->minChange +1;
    auto start = chrono::high_resolution_clock::now();

    this->initCentroids();

    int iterations =0;

    while (totalChange > this->minChange && iterations < this->maxIterations)
    {
        switch (this->method)
        {
        case _Classic:
            clusters = this->LloydsClustering();
            break;
        
        case _LSH:
            clusters = this->LSHClustering();
            break;
        case _HyperCube:
            clusters = this->HyperCubeClustering();
            break;
        }

        totalChange = 0;
        for (int i = 0; i < this->nClusters; i++)
        {
            int clusterChange =0;
            
            for (int j = 0; j < this->data.d; j++)
            {
                int mean =0;

                for (int &index : clusters[i])
                {
                    mean += int(this->data.data[index][j]);
                }
                

                mean /= clusters[i].size();
                clusterChange += abs(int(this->centroids[i][j]) - mean);

                this->centroids[i][j] = mean;
            }
            
            totalChange += clusterChange;
        }
        iterations++;
        cout << "total " << totalChange << endl;
    }

    auto stop = chrono::high_resolution_clock::now();
    cout << "iterations: " << iterations << endl;

    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

    this->print(clusters, outputFile, duration.count(), this->Silouette(clusters));

    return 0;
    
}

void kmeansplusplus::initCentroids()
{
    default_random_engine re(chrono::system_clock::now().time_since_epoch().count());

    //picking first centroid at random
    this->centroids.push_back(this->data.data[rand() % this->data.n]);

    for (int i = 1; i < nClusters; i++)
    {
        double maxDi = 0;
        vector<double> P(this->data.n + 1); //plus one for P[0] = 0

        for (auto &point : this->data.data)
        {
            double Di = double(this->minDinstance_from_Centroids(point));

            if(Di > maxDi)
                maxDi = Di;
        }
         
        P[0] = 0;

        for (int j = 1; j < this->data.n; j++)
        {
            double Di = this->minDinstance_from_Centroids(this->data.data[j]);

            P[j] = pow(Di / maxDi, 2) + P[j-1];
        }

        uniform_real_distribution<double> unif(0.0, P[this->data.n]);
        double x = unif(re);

        this->centroids.push_back(this->data.data[this->findNextCentroid(P, x) - 1]);
        
    }
    
}

uint32_t kmeansplusplus::minDinstance_from_Centroids(const vector<uint32_t> &point)
{
    vector<int> D;

    for (auto &centroid : this->centroids)
    {
        D.push_back(this->data.distanceFunction(point, centroid));
    }

    return (D.size() == 0) ? 0 : *min_element(begin(D), end(D));
    
}

int kmeansplusplus::findNextCentroid(const vector<double> &P, const double x)
{
    for (int i = 1; i <= this->data.n; i++)
    {
        if( x <= P[i])
            return i;
    }
    
    return -1;
}

int kmeansplusplus::median(vector<uint32_t> &v)
{
    nth_element(v.begin(), v.begin() + v.size() / 2, v.end());

    return (v.size() == 0 ? 0 : v[v.size() / 2]);
}


vector<vector<int>> kmeansplusplus::LloydsClustering()
{
    vector<vector<int>> clusters(this->nClusters); //holds all data points for every centroid

    for (int i = 0; i < this->data.n; i++)
    {
        clusters[this->minCentroid(this->data.data[i])].push_back(i);
    }
    
    return clusters;
}

vector<vector<int>> kmeansplusplus::LSHClustering()
{
    vector<vector<int>> clusters(this->nClusters);
    unordered_set<int> pickedPoints;

    for (int i = 0; i < this->nClusters; i++)
    {
        for (auto &point: this->lsh->exec_query(this->centroids[i], this->data.n / this->nClusters))
        {
            if(pickedPoints.find(point.second) == pickedPoints.end())
            {
                pickedPoints.insert(point.second);
                clusters[i].push_back(point.second);
            }
        }
        
    }

    if(int(pickedPoints.size()) < this->data.n)
    {
        for (int i = 0; i < this->data.n; i++)
        {
            if(pickedPoints.find(i) == pickedPoints.end())
            {
                clusters[this->minCentroid(this->data.data[i])].push_back(i);
            }
        }
        
    }

    return clusters;
    
}

vector<vector<int>> kmeansplusplus::HyperCubeClustering()
{
    vector<vector<int>> clusters(this->nClusters);
    unordered_set<int> pickedPoints;


    for (int i = 0; i < this->nClusters; i++)
    {
        for (auto &point: this->cube->exec_query(this->centroids[i], this->data.n / this->nClusters))
        {
            if(pickedPoints.find(point.second) == pickedPoints.end())
            {
                pickedPoints.insert(point.second);
                clusters[i].push_back(point.second);
            }
        }
        
    }

    if(int(pickedPoints.size()) < this->data.n)
    {
        for (int i = 0; i < this->data.n; i++)
        {
            if(pickedPoints.find(i) == pickedPoints.end())
            {
                clusters[this->minCentroid(this->data.data[i])].push_back(i);
            }
        }
        
    }

    return clusters;
}

int kmeansplusplus::minCentroid(const vector<uint32_t> &point)
{
    int index = -1, minDistance = INT32_MAX;

    for (int i = 0; i < this->nClusters; i++)
    {
        int d = this->data.distanceFunction(this->centroids[i], point);
        if(d < minDistance)
        {
            minDistance =d;
            index  = i;
        }
    }
    
    return index;
}

int kmeansplusplus::nextCentroid(const vector<uint32_t> &point)
{
    auto cmp = [](pair<int, int> left, pair<int, int> right){
        return left.first > right.first;
    };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp) > q(cmp);

    for (int i = 0; i < this->nClusters; i++)
    {
        q.push(make_pair(this->data.distanceFunction(this->centroids[i], point), i));
    }

    q.pop();

    return q.top().second;
    
}

vector<double> kmeansplusplus::Silouette(vector<vector<int>> clusters)
{
    vector<double> result;
    vector<int> ai = vector<int>(this->data.n, 0);
    vector<int> bi = vector<int>(this->data.n, 0);
    vector<double> si = vector<double>(this->data.n, 0);

    for (const auto &cluster: clusters)
    {
        for (const int &i : cluster)
        {
            for (const int &j : cluster)
            {
                if( i != j)
                    ai[i] += this->data.distanceFunction(this->data.data[i], this->data.data[j]);
            }
            
            for (const int &j : clusters[this->nextCentroid(this->data.data[i])])
            {
                bi[i] += this->data.distanceFunction(this->data.data[i], this->data.data[j]);
            }
            
            si[i] = double(bi[i] - ai[i]) / double((ai[i] < bi[i]) ? bi[i] : ai[i]);
            
        }
        
        double mean = 0;

        for (const int &i : cluster)
        {
            mean += si[i];
        }
        result.push_back(mean / double(cluster.size()));
        
    }
    
    return result;
}

void kmeansplusplus::print(const vector<vector<int>> &clusters, ofstream &outputFile, int64_t time, vector<double> silouette)
{
    switch (this->method)
    {
    case _Classic:
        outputFile << "Algorithm: Llooyds" << endl;
        break;
    case _LSH:
        outputFile << "Algorithm: LSH" << endl;
        break;
    case _HyperCube:
        outputFile << "Algorithm: HyperCube" << endl;
        break;
    
    }

    for (int i = 0; i < this->nClusters; i++)
    {
        outputFile << "CLUSTER-" << i << " {size :" << clusters[i].size() << ", centroid: [";
        for (const auto &i : this->centroids[i])
        {
            outputFile << int(i) << ", ";
        }
        outputFile << "\b\b]}" << endl << endl;
        
    }

    outputFile << "clustering_time: " << time << endl;
    outputFile << "Silouetter: [";

    double sktotal = 0;
    for (const double &i : silouette)
    {
        outputFile << i << ", ";
        sktotal +=i;
    }

    outputFile << "stotal = " << sktotal / double(silouette.size()) << "]" << endl << endl;
    
    if(this->complete)
    {
        for (int i = 0; i < this->nClusters; i++)
        {
            outputFile << "CLUSTER-" << i << " {size: " << clusters[i].size() << ", centroid: [";
            for (const auto &i : clusters[i])
            {
                outputFile << i << ", ";
            }
            
            outputFile << "\b\b]}" << endl << endl;
        }
        
    }
    
}