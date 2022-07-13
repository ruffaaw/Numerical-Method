#include <fstream>
#include <cmath>

using namespace std;

#define nullptr NULL

template<typename T>
T** _alloc_matrix(int n, int m) {
    T** matrix = new T*[n];
    for(int i = 0; i < n; ++i) {
        matrix[i] = new T[m];
    }
    return matrix;
}

template<typename T>
T* _alloc_vector(int n) {
    T* _vector = new T[n];
    return _vector;
}

template<typename T>
void _dealloc_matrix(T** matrix, int n, int m) {;
    if(matrix) {
        for(int i = n-1; i >= 0; --i) {
            if(matrix[i]) {
                delete [] matrix[i];
                matrix[i] = nullptr;
            }
        }
        delete [] matrix;
        matrix = nullptr;
    }
}

template<typename T>
void _dealloc_vector(T* _vector, int n) {
    if(_vector) {
        delete [] _vector;
        _vector = nullptr;
    }
}

template<typename T>
void matrix_to_file(T** matrix, int n, int m, string filename) {
    fstream file(filename.c_str(), ios::out);
    if(file.is_open()) {
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                file << matrix[i][j] << ";";
            }
            file << "\n";
        }
    }
    file.close();
}

template<typename T>
void vector_to_file(T* _vector, int n, string filename) {
    fstream file(filename.c_str(), ios::out);
    if(file.is_open()) {
        for(int i = 0; i < n; ++i) {
            file << _vector[i] << endl;
        }
    }
    file.close();
}

template<typename T>
T vector_max(T* _vector, int n) {
    T max = std::abs(_vector[0]);
    for(int i = 1; i < n; ++i) {
        if( max < std::abs(_vector[i]) ) {
            max = _vector[i];
        }
    }
    return max;
}