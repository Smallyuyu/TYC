pair<unsigned long long,unsigned long long> fast_fib(unsigned long long n, unsigned long long mod) {
    if (n == 0) return {0, 1};
    auto p = fast_fib(n >> 1, mod);
    __uint128_t f = p.first, g = p.second;
    // F_{2k} = F_k * (2*F_{k+1} – F_k)
    unsigned long long c = (f * (((g<<1) + mod - f) % mod)) % mod;
    // F_{2k+1} = F_k^2 + F_{k+1}^2
    unsigned long long d = ( (f*f + g*g) % mod);
    if (n & 1) return {d, (c + d) % mod};
    return {c, d};
}

cout<< fast_fib(aa,10000000000).first;
