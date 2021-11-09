#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <math.h>

#include "../include/input.h"
#include "../include/kmeansplusplus.h"

using namespace std;

int main(int argc, char *argv[])
{
    int flag = true;

    Input input;

    srand(time(NULL));

    if(input.parseCmdOptions(argc, argv) == -1)                 //pairnoume to input pou mas dinei o xrhsths
    {
        cerr << "input::ParseCmdOptions() failed" << endl;
    }

    while (flag)
    {
        string file;
        Data data;

        while (!input.inputFile.is_open())                  
        {
            cout << "Please provide a path to an input file" << endl;   //dinoume path gia to dataset
            cin >> file;
            input.OpenInputFile(file);                                  //anoigei to arxeio
        }

        if(data.InitDataSet(input.inputFile) == -1)                     //diabazei ta dedomena tou arxeio kai ta ftiaxnei
        {
            cerr << "Data::InitDataset() failed" << endl;
            return -1;
        }

        if(input.mode == _cluster)                                      //me cluster
        {
            kmeansplusplus *kmeans;

            if(!strcmp(input.method, "Classic"))                        //klassikh periptwsh gia kmeansplusplus
            {
                kmeans = new kmeansplusplus(input.nClusters, input.complete, data);
            }
            else if(!strcmp(input.method, "LSH"))                      //gia LSH
            {
                kmeans = new kmeansplusplus(input.nClusters, input.complete, input.lsh_k, input.L, data);
            }
            else if(!strcmp(input.method, "HyperCube"))                 //gia hypercube
            {
                kmeans = new kmeansplusplus(input.nClusters, input.complete, input.cube_k, input.M, input.probes, data);
            }
            else
            {
                cout << "Method: " << input.method << " not recognized " << endl;
                return -1;
            }

            kmeans->Run(input.outputFile);

            delete kmeans;
        }
        else                                                    //xwris cluster gia queries
        {
            while (!input.queryFile.is_open())                  //pairnoume to arxeio queries
            {
                cout << "Please provide a path to a query file" << endl;
                cin >> file;
                input.OpenQueryFile(file);
            }

            if(data.ReadQueryFile(input.queryFile) == -1)     //to diabazoume
            {
                cerr << "Data::ReadQueryFile() failed" << endl;
                return -1;
            }

            if(input.mode == _lsh)                          //an einai lsh
            {
                LSH *lsh = new LSH(input.lsh_k, input.L, data, 10000, 200);

                if(lsh->Run(data.queries, input.outputFile, input.N, input.R) == -1)
                {
                    cerr << "LSH::Run() failed" << endl;
                }
                delete lsh;
            }
            else                                            //an einai hypercube
            {
                HyperCube *hc = new HyperCube(floor(log2(data.n)), input.M, input.probes, data);

                if(hc->Run(data.queries, input.outputFile, input.N, input.R) ==  -1)
                {
                    cerr << "HyperCube::hyperCubeRun() failed" << endl;
                }

                delete hc;
            }
            
        }
        
        string str;
        while (1)                                           //epanalambanei gia diaforetikes ekteleseis tou xrhsth
        {
            cout << "Would you like to run again with different input? [Y, N]" << endl;
            cin >> str;

            if(!str.compare("Y") || !str.compare("N"))
            {
                input.inputFile.close();
                input.queryFile.close();
                break;
            }
            else if(!str.compare("N") || !str.compare("n"))
            {
                flag = false;
                break;
            }
        }
        
    }
    
    return 0;
}