struct Comb{
    int M = 1e9 + 7;
    vector<int> fact;
    vector<int> invF;

    Comb(){}
    Comb(int n){
        init(n);
    }

    int fast_pow(int x,int y,int M){
        if(y == 0) return 1;
        else if(x == 0) return 0;
        if(y % 2) return x * fast_pow(x * x % M,y / 2,M) % M;
        else return fast_pow(x * x % M,y / 2,M) % M;
    }
    int inv(int x){
        if(x == 1) return 1;
        if(x == 2) return 500000004;
        return fast_pow(x,M - 2,M);
    }
    void init(int n){
        fact.resize(n + 1);
        invF.resize(n + 1);
        //x!
        fact[0] = 1;
        for(int i = 1 ; i <= n ; i++){
            fact[i] = (fact[i - 1] * i) % M;
        }
        //x!的inv
        invF[n] = fast_pow(fact[n],M - 2,M);
        for(int i = n - 1 ; i >= 0 ; i--){
            invF[i] = (invF[i + 1] * (i + 1)) % M;
        }
    }

    int C(int x,int y){
        return fact[x] * invF[y] % M * invF[x - y] % M;
    }
    int P(int x,int y){
        return fact[x] * invF[x - y] % M;
    }
    int H(int x,int y){
        return C(x + y - 1,y);
    }
    //重複排列n!/(k1!*k2!*k3!...)
    int pr(int x,vector<int> p){
        int ans = fact[x] % M;
        for(auto u : p){
            ans = ans * invF[u] % M;
        }
        return ans;
    }
    //卡特蘭數Ct(n) = C(2n,n) / (n + 1),init記得開2n
    int Ct(int x){
        return C(2 * x,x) * inv(x + 1) % M;
    }
};
