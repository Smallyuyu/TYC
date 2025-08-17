const int M = 1e9 + 7;
template <typename T>
struct Matrix{
    int r,c;
    vector<vector<T>> mat;
    
    Matrix(){}
    Matrix(int n,int m,T x){
        r = n,c = m;
        mat = vector<vector<T>>(n,vector<T>(m,x));
    }
    Matrix(vector<vector<T>> arr){
        r = arr.size();
        c = arr[0].size();
        mat = arr;
    }
    Matrix(vector<T> arr){
        r = 1;
        c = arr.size();
        mat.push_back(arr);
    }
    void print(){
    	for(int i = 0 ; i < r ; i++){
    		for(int j = 0 ; j < c ; j++){
    			cout<<mat[i][j]<<" ";
    		}
    		cout<<'\n';
    	}
    }
};
template <typename T>
Matrix<T> operator*(const Matrix<T> &a,const Matrix<T> &b){
    int n = a.r;
    int m = b.c;
    int p = b.mat[0].size();
    vector<vector<T>> c(n,vector<T>(m));
    for(int i = 0 ; i < n ; i++){
        for(int j = 0 ; j < m ; j++){
            for(int k = 0 ; k < p ; k++){
                c[i][j] = (c[i][j] + ((a.mat[i][k] * b.mat[k][j]) % M)) % M;
            }
        }
    }
    return Matrix<T>(c);
}
template <typename T>
Matrix<T> operator*(const Matrix<T> &a,int b){
    b = b % M;
    vector<vector<T>> c(a.r,vector<T>(a.c));
    for(int i = 0 ; i < a.r ; i++){
        for(int j = 0 ; j < a.c ; j++){
            c[i][j] = (a.mat[i][j] * b) % M;
        }
    }
    return Matrix<T>(c);
}
template <typename T>
Matrix<T> operator*(int b,const Matrix<T> &a){
    return a * b;
}
template <typename T>
Matrix<T> operator+(const Matrix<T> &a,const Matrix<T> &b){
    vector<vector<T>> c(a.r,vector<T>(a.c));
    for(int i = 0 ; i < a.r ; i++){
        for(int j = 0 ; j < a.c ; j++){
            c[i][j] = (a.mat[i][j] + b.mat[i][j]) % M;
        }
    }
    return Matrix<T>(c);
}
template <typename T>
Matrix<T> operator-(const Matrix<T> &a,const Matrix<T> &b){
    vector<vector<T>> c(a.r,vector<T>(a.c));
    for(int i = 0 ; i < a.r ; i++){
        for(int j = 0 ; j < a.c ; j++){
            c[i][j] = a.mat[i][j] - b.mat[i][j];
            if(a.mat[i][j] > b.mat[i][j]) c[i][j] %= M;
        }
    }
    return Matrix<T>(c);
}
template <typename T>
Matrix<T> mat_fast_pow(Matrix<T> base, long long exp){
    int n = base.r;
    Matrix<T> res(n,n,0);
    for(int i=0;i<n;i++) res.mat[i][i] = 1;

    while(exp > 0){
        if(exp & 1) res = res * base;
        base = base * base;
        exp >>= 1;
    }
    return res;
}

Matrix<int> ini(n,n,0);
for(int i = 0 ; i < n ; i++){
	for(int j = 0 ; j < n ; j++){
		cin>>ini.mat[i][j];
	}
}
Matrix<int> ans = mat_fast_pow(ini,k);
ans.print();
