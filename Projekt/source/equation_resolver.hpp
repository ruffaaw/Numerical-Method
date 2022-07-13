#include <cmath>

#include "thomas_algorithm.hpp"
#include "decomposition_lu.hpp"
#include "calerf.h"

using namespace std;

const int b = 1;
const int D = 1;

const int t_min = 0;
const int t_max = 2;

const double a = b + 6 * std::sqrt(D*t_max);
const double x_max = a;
const double x_min = -a;

const double D_LAMBDA = 0.4;
const double I_LAMBDA = 1;

const double h = 0.07;

void initial_condition(double **matrix, int n, int m) {
    double x = x_min;

    for(int i = 0; i < m; ++i) {
        if( std::abs(x) < b ) {
            matrix[0][i] = 1.0;
        } else {
            matrix[0][i] = 0.0;
        }
        x += h;
    }
}

void boundary_conditions(double **matrix, int n, int m) {
    for(int i = 0; i < n; ++i) {
        matrix[i][0] = 0.0;
        matrix[i][m-1] = 0.0;

    }
}

double real_value(double x, double t) {
    return 0.5*ERFL( (x + b)/(2*std::sqrt(D*t) ) ) - 0.5*ERFL( (x - b)/(2*std::sqrt(D*t) ) );
}

double **get_analytical_solution(int n, int m, double h, double dt) {
    double **analytical = _alloc_matrix<double>(n, m);
    double x = x_min;
    double t = t_min;

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            analytical[i][j] = real_value(x, t);
            x += h;
        }
        x = x_min;
        t += dt;
    }
    return analytical;
}

void classical_direct_method(double **A, int n, int m) {
    for(int i = 1; i < n; ++i) {
        for(int j = 1; j < m - 1; ++j) {
            A[i][j]=A[i-1][j] + D_LAMBDA * (A[i-1][j-1] - (2 * A[i-1][j]) + A[i-1][j+1]);
        }
    }
}

double **get_classical_solution(int n, int m) {
    double **classic = _alloc_matrix<double>(n, m);

    initial_condition(classic, n, m);
    boundary_conditions(classic, n, m);

    classical_direct_method(classic, n, m);

    return classic;
}

void laasonen_indirect_thomas_method(double **A, int n, int m) {
    const double LAASONEN_LAMBDA = 1.0 + 2.0*I_LAMBDA;

    double *l = _alloc_vector<double>(m),
            *d = _alloc_vector<double>(m),
            *u = _alloc_vector<double>(m),
            *b = _alloc_vector<double>(m),
            *x = _alloc_vector<double>(m);

    for(int k = 1; k < n; ++k) {
        l[0] = 0.0;
        d[0] = 1.0;
        u[0] = 0.0;
        b[0] = A[k-1][0];
        for(int i = 1; i < m - 1; ++i) {
            l[i] = I_LAMBDA;
            d[i] = -LAASONEN_LAMBDA;
            u[i] = I_LAMBDA;
            b[i] = -A[k-1][i];
        }
        l[m-1] = 0.0;
        d[m-1] = 1.0;
        u[m-1] = 0.0;
        b[m-1] = 0.0;

        thomas_algorithm(l, d, u, b, x, m);
        for(int i = 1; i < m - 1; ++i) {
            A[k][i] = x[i];
        }
    }


    _dealloc_vector(l, m);
    _dealloc_vector(d, m);
    _dealloc_vector(u, m);
    _dealloc_vector(b, m);
    _dealloc_vector(x, m);
}

double **get_laasonen_solution_thomas(int n, int m) {
    double **laasonen_thomas = _alloc_matrix<double>(n, m);

    initial_condition(laasonen_thomas, n, m);
    boundary_conditions(laasonen_thomas, n, m);

    laasonen_indirect_thomas_method(laasonen_thomas, n, m);

    return laasonen_thomas;
}

void laasonen_indirect_lu_method(double** A, int n, int m){
    const double LAASONEN_LAMBDA = 1.0 + 2.0*I_LAMBDA;
    double *b = _alloc_vector<double>(m),
            *y = _alloc_vector<double>(m),
            *x = _alloc_vector<double>(m);
    int *vecIdx = new int[m];

    for(int i = 0; i < m; i++){
        vecIdx[i]=i;
    }

    for(int i = 0; i < m; i++){
        b[i] = 0.0;
        y[i] = 0.0;
        x[i] = 0.0;
    }

    double ** new_matrix_A = _alloc_matrix<double>(m,m);
    double ** L = _alloc_matrix<double>(m,m);

    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            new_matrix_A[i][j]=0.0;
        }
    }

    for(int k = 1; k < n; k++){
        new_matrix_A[0][0] = 1.0;
        b[0] = A[k-1][0];
        for(int i = 1; i < m - 1; i++){
            new_matrix_A[i][i] = -LAASONEN_LAMBDA;
            new_matrix_A[i][i+1] = I_LAMBDA;
            new_matrix_A[i][i-1] = I_LAMBDA;
            b[i] = -A[k-1][i];
        }

        b[m-1] = 0.0;
        new_matrix_A[m-1][m-1]=1.0;

        decomposition_lu(L,new_matrix_A,vecIdx,m,b,y,x);

        for(int i=1; i < m - 1; i++){
            A[k][i] = x[i];
        }
    }
    _dealloc_vector(b, m);
    _dealloc_vector(y,m);
    _dealloc_vector(x,m);
    _dealloc_matrix(new_matrix_A,m,m);
    _dealloc_matrix(L,m,m);

}

double **get_laasonen_solution_lu(int n, int m){
    double **laasonen_lu = _alloc_matrix<double>(n, m);

    initial_condition(laasonen_lu, n, m);
    boundary_conditions(laasonen_lu, n, m);

    laasonen_indirect_lu_method(laasonen_lu,n,m);

    return laasonen_lu;

}

double **compute_error(double **realSolution, double **numericalSolution, double **errors, int n, int m) {
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < m; ++j) {
            errors[i][j] = ( std::abs( numericalSolution[i][j] - realSolution[i][j] ) );
        }
    }
    return errors;
}

double **get_error(double **realSolution, double **numericalSolution, int n, int m) {
    double** errors = _alloc_matrix<double>(n, m);
    compute_error(realSolution, numericalSolution, errors, n, m);
    return errors;
}

double *max_error(double **errors, int n, int m) {
    double* max_error = _alloc_vector<double>(n);
    for(int i = 0; i < n; ++i) {
        max_error[i] = vector_max(errors[i], m);
    }
    return max_error;
}

double get_delta_t(double lambda, double h, double D) {
    return (lambda*h*h)/D;
}

double *get_space_steps(double dt, int n, int m) {
    double  *variables = _alloc_vector<double>(m);
    double x = x_min;

    for(int i = 0; i < m; ++i) {
        variables[i] = x;
        x += h;
    }
    return variables;
}

double *get_time_steps(double dt, int n, int m) {
    double  *variables = _alloc_vector<double>(n);
    double t = t_min;

    for(int i = 0; i < n; ++i) {
        variables[i] = t;
        t += dt;
    }
    return variables;
}

double get_time_step(double dt, int n) {
    double t = t_min;

    for(int i = 0; i < n; ++i) {
        t += dt;
    }
    return t;
}