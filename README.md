# Finding Nearest Neighbor(s) with Locality Sensitive Hashing and HyperCube - Data Clustering

## Project Overview

This is the first project for the Software Development course on Algorithmic Problems. The project implements Locality Sensitive Hashing (LSH) and HyperCube algorithms in d-dimensional space using the L2 metric to find:

- The nearest neighbor
- N nearest neighbors

Additionally, K-means++ and Lloyd's algorithms are implemented for clustering. The clustering functionality is not fully functional.

## Project Structure

The project consists of the following folders and files:

- **assets**: Contains dataset files.
- **bin**: Contains executable files.
- **build**: Contains object files (`.o`).
- **include**: Contains header files (`.h`).
- **logs**: Contains `logs.txt`, which holds the execution logs.
- **src**: Contains source files (`.cpp`).
- **cluster.conf**: Configuration file for clustering.
- **Makefile**: Build instructions.
- **README**: This file.

## Running the Project

After building the project with `make`, there are two ways to run it:

1. **Manually running commands**:
   - Navigate to the `bin` folder and run the commands as specified in the project description.
   
2. **Running predefined commands from the Makefile**:
   - Run predefined commands such as `run-lsh`, `run-hc` from the Makefile. Some of these commands do not require all arguments, and the program will use default values.

When running with the first method, the program asks for input and query files. With the second method, the program runs directly using the files found in the Makefile. Afterward, it allows the user to run with different parameters.

The results are saved in the `logs/logs.txt` file.

## Functionality

- The program reads and stores datasets for both input and query.
- It operates in **cluster mode** or **non-cluster mode**, depending on the mode selected.
- **In non-cluster mode**, it processes queries and creates either LSH or HyperCube.
    - **LSH**: Can find times and make necessary prints using brute force or by getting the closest neighbors.
    - **Hypercube**: In addition to the above, it creates `f` values and calculates Hamming distances.
- For the hash tables, the program computes hash functions from the data chunks and constructs tables with their respective buckets.

## How It Works

- **LSH**: The program hashes the data using hash tables, finds `g` values, and executes queries.
- **Hypercube**: In addition to the operations of LSH, the program creates `f` values and calculates Hamming distances.
- **Hash tables**: We calculate the hash function from data chunks and build the hash tables with their respective buckets.

## Execution Logs

The logs of the program are stored in the `logs/logs.txt` file, which contains the output of the program execution.

---

### Developed by:
**Charalambos Stevis**
Student ID: `sdi1600278`
