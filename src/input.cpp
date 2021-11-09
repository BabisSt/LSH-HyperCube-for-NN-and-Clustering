#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>

#include "../include/input.h"

using namespace std;

Input::Input() 
{}

Input::~Input()
{
    if(inputFile.is_open())
    {
        inputFile.close();
    }

    if(queryFile.is_open())
    {
        queryFile.close();
    }

    if(outputFile.is_open())
    {
        outputFile.close();
    }

    if(method != nullptr)
    {
        free(method);
    }
}

char *Input::getCmdOption(char **begin, char **end, const string &option)
{
    char **itr = find(begin,end, option);

    if(itr != end && ++itr != end)
    {
        return *itr;
        cout << itr << endl;
    }

    return nullptr;
}

int Input::OpenInputFile(const string &file)
{
    this->inputFile.open(file, ifstream::in | ifstream::binary);
    if(!this->inputFile)
    {
        perror("open()");
    }

    return 0;
}

int Input::OpenQueryFile(const string &file)
{
    this->queryFile.open(file, ifstream::in | ifstream::binary);
    if(!this->queryFile)
    {
        perror("open()");
    }

    return 0;
}

int Input::parseCmdOptions(const int &argc, char *argv[])
{
    char *val = nullptr;
    string str = argv[0];

    if((val = this->getCmdOption(argv,argv + argc, "-i")) !=nullptr)
    {
        try
        {
            this->inputFile.open(val, ifstream::in | ifstream::binary);
        }
        catch(const ifstream::failure &e)
        {
            perror("open()");
            cerr<< "Failed to open " << val << endl;
        }
        
    }

    if((val = this->getCmdOption(argv,argv + argc, "-o")) !=nullptr)
    {
        try
        {
            this->outputFile.open(val, ofstream::out | ofstream::trunc);
        }
        catch(const ofstream::failure &e)
        {
            perror("open()");
            cerr<< "Failed to open " << val << endl;
        }
        
    }
    else
    {
        try
        {
            this->outputFile.open("./logs/logs.txt", ofstream::out | ofstream::trunc);
            cout << "Output file ./logs/logs.txt" << endl;
        }
        catch(const ofstream::failure &e)
        {
            perror("open()");
            cerr<< "Failed to open ./logs/logs.txt " << endl;
        }
        
    }

    if(string(argv[0]).find("cluster") != string::npos)
    {
        this->mode = _cluster;
        this->lsh_k = 4;
        this->L = 3;
        this->cube_k = 3;
        this->M = 10;
        this->probes = 2;

        if((val = this->getCmdOption(argv,argv + argc, "-c")) !=nullptr)
        {
            ifstream confFile;

            try
            {
                confFile.open(val, ifstream::in);
            }
            catch(const ifstream::failure &e)
            {
                perror("open()");
                cerr << e.what() << endl;
                cerr<< "Failed to open " << val << endl;
                return -1;
            }
            
            for (string line; getline(confFile, line);)
            {
                istringstream ss(line);
                istream_iterator<string> begin(ss), end;

                vector<string> options(begin,end);

                if(!options[0].compare("number_of_clusters:"))
                {
                    this->nClusters = stoi(options[1]);
                }
                else if(!options[0].compare("number_of_vector_hash_tables:"))
                {
                    this->L = stoi(options[1]);
                }
                else if(!options[0].compare("number_of_vector_hash_functions:"))
                {
                    this->lsh_k = stoi(options[1]);
                }
                else if(!options[0].compare("max+number_M_hypercube:"))
                {
                    this->M = stoi(options[1]);
                }
                else if(!options[0].compare("number_of_hypercube_dimensions:"))
                {
                    this->cube_k = stoi(options[1]);
                }
                else if(!options[0].compare("number_of_probes:"))
                {
                    this->probes = stoi(options[1]);
                }
                else
                {
                    cout << "unknown option in .conf file" << endl;
                }
            
            }
            confFile.close();
            
        }
        else
        {
            cerr << "Configuration file required" << endl;
            return -1;
        }

        if((val = this->getCmdOption(argv,argv + argc, "-complete")) !=nullptr)
        {
            this->complete = true;
        }

        if((val = this->getCmdOption(argv,argv + argc, "-m")) !=nullptr)
        {
            if (this->method == nullptr)
            {
                this->method = strdup(val);
            }
            
        }
        
    }
    else
    {
        this->N = 1;
        this->R = 10000;

        if((val = this->getCmdOption(argv,argv + argc, "-q")) !=nullptr)
        {
            try
            {
                this->queryFile.open(val, ifstream::in);
            }
            catch(const ifstream::failure &e)
            {
                perror("open()");
                cerr<< "Failed to open " << val << endl;
                return -1;
            }
        }

        if((val = this->getCmdOption(argv,argv + argc, "-N")) !=nullptr)
        {
            sscanf(val, "%d", &(this->N));
        }

        if((val = this->getCmdOption(argv,argv + argc, "-R")) !=nullptr)
        {
            sscanf(val, "%d", &(this->R));
        }

        if(string(argv[0]).find("lsh") != string::npos)
        {
            this->mode = _lsh;
            this->lsh_k = 4;
            this->L = 5;

            if((val = this->getCmdOption(argv,argv + argc, "-L")) !=nullptr)
            {
                sscanf(val, "%d", &(this->L));
            }

            if((val = this->getCmdOption(argv,argv + argc, "-k")) !=nullptr)
            {
                sscanf(val, "%d", &(this->lsh_k));
            }
        }
        else
        {
            this->mode = _cube;
            this->cube_k = 14;
            this->M = 10;
            this->probes = 2;

            if((val = this->getCmdOption(argv,argv + argc, "-k")) !=nullptr)
            {
                sscanf(val, "%d", &(this->cube_k));
            }

            if((val = this->getCmdOption(argv,argv + argc, "-M")) !=nullptr)
            {
                sscanf(val, "%d", &(this->M));
            }

            if((val = this->getCmdOption(argv,argv + argc, "-probes")) !=nullptr)
            {
                sscanf(val, "%d", &(this->probes));
            }

        }
    }

    return 0;
}