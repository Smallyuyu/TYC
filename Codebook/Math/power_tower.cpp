//Sol (a^b^c^d)%M
int mod_pow(int x, int e, int mod) {
    x %= mod;
    int res = 1;
    while (e > 0) {
        if (e & 1) res = (__int128)res * x % mod;
        x = (__int128)x * x % mod;
        e >>= 1;
    }
    return res;
}

int euler_phi(int m) {
    int res = m;
    for (int p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res = res / p * (p - 1);
        }
    }
    if (m > 1) res = res / m * (m - 1);
    return res;
}

int tower_pow(const vector<int>& a, int idx, int mod) {
    if (idx + 1 == (int)a.size() || mod == 1) {
        return a[idx] % mod;
    }
    int phi = euler_phi(mod);
    int exp = tower_pow(a, idx + 1, phi);
    if (a[idx+1] >= phi) {
        exp += phi;
    }
    return mod_pow(a[idx], exp, mod);
}

//find (7^7^7^x) % Period
vector<int> a = {7,7,7,0};
a[3] = x;
int aa = tower_pow(a, 0, PERIOD) % PERIOD;
