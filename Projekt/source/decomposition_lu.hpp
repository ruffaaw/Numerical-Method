void swap_rows(double** A, double** L, int* vector_idx, int idx,int m);
void gaussian_eliminate(double** A, double** L, int indx,int m);

void decomposition_lu(double** L, double** A, int* vector_idx, int m, double* b, double* y, double* x){
    for(int i = 0; i < m; i++){
        L[i][i] = 1.0;
    }

    for(int i = 0; i < m; i++){
        if(abs(A[i][i]) < 1e-8) {
            swap_rows(A, L, vector_idx, i, m);
        }
        gaussian_eliminate(A,L,i,m);
    }
    int temp;
    double tempVector[m];
    for(int i = 0; i < m; i++)
    {
        tempVector[i] = b[i];
    }
    for(int i = 0; i < m; i++){
        temp = vector_idx[i];
        b[i] = tempVector[temp];
    }

    for (int i = 0; i < m; i++){
        double sum = 0.0;
        for (int j = 0; j <= i - 1; j++){
            sum += L[i][j] * y[j];
        }
        y[i] = (b[i] - sum);
    }

    for (int i = m - 1; i >= 0; i--){
        double sum = 0.0;
        for (int j = i+1; j < m; j++){
            sum += A[i][j] * x[j];
        }
        x[i] = (y[i] - sum) / A[i][i];
    }


}

void swap_rows(double** A, double** L, int* vector_idx, int idx,int m){
    double tempRow[m];
    int temp;
    int tempVectorIdx;
    double tempValue = 0;
    for(int j = idx + 1; j < m; j++){
        if(abs(A[j][idx]) > tempValue){
            temp = j;
            tempValue = abs(A[j][idx]);
        }
    }
    for(int j = 0; j < m; j++){
        tempRow[j] = A[temp][j];
        A[temp][j] = A[idx][j];
        A[idx][j] = tempRow[j];
    }
    tempVectorIdx = vector_idx[idx];
    vector_idx[idx] = vector_idx[temp];
    vector_idx[temp] = tempVectorIdx;
    for(int k = 0; k < idx; k++){
        tempRow[k] = L[temp][k];
        L[temp][k] = L[idx][k];
        L[idx][k] = tempRow[k];
    }
}

void gaussian_eliminate(double** A, double** L, int indx,int m){
    double temp;
    for(int k = indx + 1; k < m; k++){
        if(A[k][indx] == 0){
            L[k][indx] = 0;
        }else{
            temp = A[k][indx] / A[indx][indx];
            for(int j = indx; j < m; j++){
                A[k][j] = A[k][j] - temp * A[indx][j];
            }
            L[k][indx] = temp;
        }
    }
}