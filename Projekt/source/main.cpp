#include "vector_helper.hpp"
#include "equation_resolver.hpp"

using namespace std;

int main() {

    {
        double dt = get_delta_t(D_LAMBDA, h, D);
        int n = ((t_max - t_min) / dt);
        int m = ((x_max - x_min) / h);

        double **analytic = get_analytical_solution(n, m, h, dt);
        matrix_to_file<double>(analytic, n, m, "analytical_1.txt");

        double **classic = get_classical_solution(n, m);
        matrix_to_file<double>(classic, n, m, "indirect_solution.txt");

        double **errors = get_error(analytic, classic, n, m);
        double *max_er  = max_error(errors, n, m);

        vector_to_file<double>(max_er, n, "max_errors_indirect.txt");
        matrix_to_file<double>(errors, n, m, "errors_indirect.txt");

        double *_x = get_time_steps(dt, n, m);
        vector_to_file<double>(_x, n, "variables_indirect.txt");

        _dealloc_matrix(analytic, n, m);
        _dealloc_matrix(classic, n, m);
        _dealloc_vector(max_er, n);
        _dealloc_matrix(errors, n, m);
        _dealloc_vector(_x, m);
    }

    {
        double dt = get_delta_t(I_LAMBDA, h, D);
        int n = ((t_max - t_min) / dt);
        int m = ((x_max - x_min) / h);

        double **analytic = get_analytical_solution(n, m, h, dt);
        matrix_to_file<double>(analytic, n, m, "analytical_2.txt");

        double **laasonen_thomas = get_laasonen_solution_thomas(n, m);
        matrix_to_file<double>(laasonen_thomas, n, m, "laasonen_solution_thomas.txt");

        double **errors = get_error(analytic, laasonen_thomas, n, m);
        double *max_er  = max_error(errors, n, m);

        vector_to_file<double>(max_er, n, "max_errors_lassonen_thomas.txt");
        matrix_to_file<double>(errors, n, m, "errors_laasonen_thomas.txt");

        double *_x = get_time_steps(dt, n, m);
        vector_to_file<double>(_x, n, "variables_lassonen_thomas.txt");

        _dealloc_matrix(analytic, n, m);
        _dealloc_matrix(laasonen_thomas, n, m);
        _dealloc_vector(max_er, n);
        _dealloc_matrix(errors, n, m);
        _dealloc_vector(_x, m);
    }

    {
        double dt = get_delta_t(I_LAMBDA, h, D);
        int n = ((t_max - t_min) / dt);
        int m = ((x_max - x_min) / h);

        double **analytic = get_analytical_solution(n, m, h, dt);
        matrix_to_file<double>(analytic, n, m, "analytical_3.txt");

        double **laasonen_lu = get_laasonen_solution_lu(n, m);
        matrix_to_file<double>(laasonen_lu, n, m, "laasonen_solution_lu.txt");

        double **errors = get_error(analytic, laasonen_lu, n, m);
        double *max_er = max_error(errors, n, m);

        vector_to_file<double>(max_er, n, "max_errors_lassonen_lu.txt");
        matrix_to_file<double>(errors, n, m, "errors_laasonen_lu.txt");

        //double* _x = get_space_steps(dt, n, m);
        double *_x = get_time_steps(dt, n, m);
        vector_to_file<double>(_x, n, "variables_lassonen_lu.txt");

        _dealloc_matrix(analytic, n, m);
        _dealloc_matrix(laasonen_lu, n, m);
        _dealloc_vector(max_er, n);
        _dealloc_matrix(errors, n, m);
        _dealloc_vector(_x, m);
    }

    return 0;
}