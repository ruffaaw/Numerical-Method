void thomas_algorithm(double *l, double *d, double *u, double *b, double *x, int m) {
    for (int i = 2; i < m; ++i) {
        d[i] = d[i] - (l[i - 1] / d[i - 1])*u[i - 1];
        b[i] = b[i] - (l[i - 1] / d[i - 1])*b[i - 1];
    }

    x[m-1] = b[m-1] / d[m-1];

    for (int i = m-2; i >= 0; --i) {
        x[i] = (b[i] - u[i] * x[i+1]) / d[i];
    }

}