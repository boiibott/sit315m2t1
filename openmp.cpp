#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstdlib> // For random number generation
#include <omp.h>   // OpenMP header

using namespace std;

const int MATRIX_SIZE = 300; // Define the size of the matrices

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

// Function to perform matrix multiplication
void multiplyMatrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
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

    // Perform matrix multiplication
    multiplyMatrices(A, B, C);

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
