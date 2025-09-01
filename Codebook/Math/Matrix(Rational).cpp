//高斯消元、轉置矩陣、逆矩陣、行列式
using ll = long long;

ll llgcd(ll a, ll b){
    if(a<0) a = -a;
    if(b<0) b = -b;
    if(b==0) return a;
    while(b){
        ll t = a % b;
        a = b;
        b = t;
    }
    return a;
}
struct Rational {
    ll num, den;
    Rational(): num(0), den(1) {}
    Rational(ll n): num(n), den(1) {}
    Rational(ll n, ll d){
        if(d == 0){ num = 1; den = 0; return; }
        if(d < 0) n = -n, d = -d;
        if(n == 0){ num = 0; den = 1; return; }
        ll g = llgcd(llabs(n), llabs(d));
        num = n / g; den = d / g;
    }
    void normalize(){
        if(den == 0) return;
        if(den < 0){ num = -num; den = -den; }
        if(num == 0){ den = 1; return; }
        ll g = llgcd(llabs(num), llabs(den));
        num /= g; den /= g;
    }
    bool isZero() const { return num == 0; }
    string str() const {
        if(den == 1) return to_string(num);
        return to_string(num) + "/" + to_string(den);
    }

    string abs_str() const {
        if(den == 1) return to_string(llabs(num));
        return to_string(llabs(num)) + "/" + to_string(den);
    }

    // operators
    Rational operator+(const Rational &o) const {
        __int128 a = (__int128)num * o.den + (__int128)o.num * den;
        __int128 b = (__int128)den * o.den;
        ll an = (ll)a;
        ll bn = (ll)b;
        ll g = llgcd(llabs(an), llabs(bn));
        an /= g; bn /= g;
        return Rational(an, bn);
    }
    Rational operator-(const Rational &o) const {
        __int128 a = (__int128)num * o.den - (__int128)o.num * den;
        __int128 b = (__int128)den * o.den;
        ll an = (ll)a;
        ll bn = (ll)b;
        ll g = llgcd(llabs(an), llabs(bn));
        an /= g; bn /= g;
        return Rational(an, bn);
    }
    Rational operator*(const Rational &o) const {
        ll g1 = llgcd(llabs(num), llabs(o.den));
        ll g2 = llgcd(llabs(o.num), llabs(den));
        __int128 a = (__int128)(num / g1) * (o.num / g2);
        __int128 b = (__int128)(den / g2) * (o.den / g1);
        return Rational((ll)a, (ll)b);
    }
    Rational operator/(const Rational &o) const {
        if(o.num == 0) return Rational(1,0); // invalid
        ll g1 = llgcd(llabs(num), llabs(o.num));
        ll g2 = llgcd(llabs(o.den), llabs(den));
        __int128 a = (__int128)(num / g1) * (o.den / g2);
        __int128 b = (__int128)(den / g2) * (o.num / g1);
        if(b < 0){ a = -a; b = -b; }
        return Rational((ll)a, (ll)b);
    }
    bool operator==(const Rational &o) const { return num == o.num && den == o.den; }
    bool operator!=(const Rational &o) const { return !(*this == o); }
};


template<typename T>
struct Matrix {
    int r,c;
    vector<vector<T>> mat;
    Matrix(){}
    Matrix(int n,int m, T x): r(n), c(m), mat(n, vector<T>(m, x)) {}
};


Rational parseR(const string &s){
    auto pos = s.find('/');
    if(pos == string::npos){
        ll p = stoll(s);
        return Rational(p, 1);
    } else {
        ll p = stoll(s.substr(0,pos));
        ll q = stoll(s.substr(pos+1));
        return Rational(p,q);
    }
}

/* ------------------ Gauss-Jordan ------------------ */
/*
 Return:
   status = 0 => unique solution
   status = 1 => no solution
   status = 2 => infinite many solutions
*/
pair<int, vector<Rational>> gauss_solve(Matrix<Rational> A, vector<Rational> b){
    int n = A.r;
    int m = A.c;
    vector<vector<Rational>> aug(n, vector<Rational>(m+1));
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++) aug[i][j] = A.mat[i][j];
        aug[i][m] = b[i];
    }
    int row = 0;
    vector<int> where(m, -1);
    for(int col = 0; col < m && row < n; ++col){
        int sel = -1;
        for(int i = row; i < n; ++i){
            if(!(aug[i][col] == Rational(0))){ sel = i; break; }
        }
        if(sel == -1) continue;
        swap(aug[sel], aug[row]);
        where[col] = row;
        // normalize pivot
        Rational pivot = aug[row][col];
        Rational inv_pivot = Rational(1) / pivot;
        for(int j = col; j <= m; ++j) aug[row][j] = aug[row][j] * inv_pivot;
        // eliminate
        for(int i = 0; i < n; ++i){
            if(i == row) continue;
            Rational factor = aug[i][col];
            if(!(factor == Rational(0))){
                for(int j = col; j <= m; ++j){
                    aug[i][j] = aug[i][j] - aug[row][j] * factor;
                }
            }
        }
        ++row;
    }
    vector<Rational> ans(m, Rational(0));
    for(int i=0;i<m;i++){
        if(where[i] != -1) ans[i] = aug[where[i]][m];
    }
    // check consistency
    for(int i=0;i<n;i++){
        Rational sum(0);
        for(int j=0;j<m;j++) sum = sum + A.mat[i][j] * ans[j];
        if(!(sum == b[i])) return {1, {}}; // no solution
    }
    for(int i=0;i<m;i++){
        if(where[i] == -1) return {2, {}}; // infinite
    }
    return {0, ans};
}

// 轉置矩陣
template<typename T>
Matrix<T> transpose(const Matrix<T> &A){
    Matrix<T> B(A.c, A.r, T());
    for(int i=0;i<A.r;i++) for(int j=0;j<A.c;j++) B.mat[j][i] = A.mat[i][j];
    return B;
}

// 行列式
template<typename T>
T determinant(Matrix<T> A){
    if(A.r != A.c) return T(0);
    int n = A.r;
    T det = T(1);
    int sign = 1;
    for(int i=0;i<n;i++){
        int sel = -1;
        for(int j=i;j<n;j++){
            if(!(A.mat[j][i] == T(0))){ sel = j; break; }
        }
        if(sel == -1) return T(0);
        if(sel != i){
            swap(A.mat[sel], A.mat[i]);
            sign = -sign;
        }
        T pivot = A.mat[i][i];
        det = det * pivot;
        for(int r = i+1; r<n; ++r){
            if(!(A.mat[r][i] == T(0))){
                T factor = A.mat[r][i] / pivot;
                for(int c = i; c<n; ++c){
                    A.mat[r][c] = A.mat[r][c] - A.mat[i][c] * factor;
                }
            }
        }
    }
    if(sign == -1) det = det * T(-1);
    return det;
}

//逆矩陣
template<typename T>
bool inverse(const Matrix<T> &A, Matrix<T> &Inv){
    if(A.r != A.c) return false;
    int n = A.r;
    vector<vector<T>> aug(n, vector<T>(2*n));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) aug[i][j] = A.mat[i][j];
        for(int j=0;j<n;j++) aug[i][n+j] = (i==j? T(1) : T(0));
    }
    for(int col = 0, row = 0; col < n && row < n; ++col){
        int sel = -1;
        for(int i = row; i < n; ++i) if(!(aug[i][col] == T(0))){ sel = i; break; }
        if(sel == -1) continue;
        swap(aug[sel], aug[row]);
        T pivot = aug[row][col];
        T inv_pivot = T(1) / pivot;
        for(int j = col; j < 2*n; ++j) aug[row][j] = aug[row][j] * inv_pivot;
        for(int i=0;i<n;i++){
            if(i==row) continue;
            T factor = aug[i][col];
            if(!(factor == T(0))){
                for(int j = col; j < 2*n; ++j){
                    aug[i][j] = aug[i][j] - aug[row][j] * factor;
                }
            }
        }
        ++row;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j){
                if(!(aug[i][j] == T(1))) return false;
            } else {
                if(!(aug[i][j] == T(0))) return false;
            }
        }
    }
    Inv = Matrix<T>(n, n, T(0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++) Inv.mat[i][j] = aug[i][n+j];
    }
    return true;
}


string coeff_with_sign(const Rational &r){
    if(r.num < 0){
        return "-" + r.abs_str();
    }
    else{
        return "+" + r.abs_str();
    }
}

int main(){
    while(true){
        int n;
        cin >> n;
        if(n == 0) return 0;

        Matrix<Rational> A(n, n, Rational(0));
        vector<Rational> b(n, Rational(0));
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                string tok;
                cin >> tok;
                A.mat[i][j] = parseR(tok); //讀分數(eg. -1/2)
            }
            string tok;
            cin >> tok;
            b[i] = parseR(tok);
        }


        cout << "n = " << n << "\n";

        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                cout << coeff_with_sign(A.mat[i][j]) << " " << "X" << (j+1);
                if(j != n-1) cout << " ";
            }
            cout << " = ";
            cout << b[i].str() << "\n";
        }

        auto res = gauss_solve(A, b);
        if(res.first == 1) cout << "no solutions\n";
        else if(res.first == 2) cout << "infinite many solutions\n";
        else{
            auto sol = res.second;
            for(int i=0;i<n;i++){
                cout << "X" << (i+1) << " = " << sol[i].str() << "\n";
            }
        }
        // Rational det = determinant(A);
        // Matrix<Rational> Inv;
        // bool ok = inverse(A, Inv);
        // Matrix<Rational> At = transpose(A);
    }
    return 0;
}
