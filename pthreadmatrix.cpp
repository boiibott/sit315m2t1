#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib> // For random number generation
#include <pthread.h>

using namespace std;

const int MATRIX_SIZE = 100; // Define the size of the matrices
const int NUM_THREADS = 2;   // Number of threads to use

// Structure to pass data to thread function
struct ThreadData {
    vector<vector<int>>* A;
    vector<vector<int>>* B;
    vector<vector<int>>* C;
    int startRow;
    int endRow;
};

// Function to generate a random matrix
void generateRandomMatrix(vector<vector<int>>& matrix) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            matrix[i][j] = rand() % 100; // Generating random numbers between 0 and 99
        }
    }
}

// Function to display a matrix
void displayMatrix(const vector<vector<int>>& matrix) {
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Thread function to calculate dot products
void* calculateDotProducts(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);

    for (int i = data->startRow; i < data->endRow; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                (*data->C)[i][j] += (*data->A)[i][k] * (*data->B)[k][j];
            }
        }
    }

    return nullptr;
}

int main() {
    // Initialize matrices
    vector<vector<int>> A(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> B(MATRIX_SIZE, vector<int>(MATRIX_SIZE));
    vector<vector<int>> C(MATRIX_SIZE, vector<int>(MATRIX_SIZE, 0)); // Initialize C with zeros

    // Generate random matrices
    generateRandomMatrix(A);
    generateRandomMatrix(B);

    // Display matrices A and B
    cout << "Matrix A:" << endl;
    displayMatrix(A);
    cout << endl << "Matrix B:" << endl;
    displayMatrix(B);

    // Start timing
    auto start = chrono::high_resolution_clock::now();

    // Create threads
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];
    int rowsPerThread = MATRIX_SIZE / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threadData[i].A = &A;
        threadData[i].B = &B;
        threadData[i].C = &C;
        threadData[i].startRow = i * rowsPerThread;
        threadData[i].endRow = (i == NUM_THREADS - 1) ? MATRIX_SIZE : (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, calculateDotProducts, &threadData[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    // Stop timing
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    // Output execution time
    cout << endl << "Time taken for matrix multiplication: " << duration.count() << " microseconds" << endl;

    // Write resultant matrix to a text file
    ofstream outFile("resultant_matrix.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < MATRIX_SIZE; ++i) {
            for (int j = 0; j < MATRIX_SIZE; ++j) {
                outFile << C[i][j] << " ";
            }
            outFile << endl;
        }
        outFile.close();
        cout << "Resultant matrix written to 'resultant_matrix.txt'" << endl;
    } else {
        cerr << "Unable to open file for writing!" << endl;
    }

    return 0;
}
