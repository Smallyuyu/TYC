using i128 = __int128;
 
istream &operator>>(istream &is, i128 &n) {
    string s;
    is >> s;
    n = 0;
    for (char c : s) {
        n = n * 10 + (c - '0');
    }
    return is;
}

ostream &operator<<(ostream &os, i128 n) {
    if (n == 0) {
        return os << 0;
    }
    string s;
    while (n > 0) {
        s += char('0' + n % 10);
        n /= 10;
    }
    reverse(s.begin(), s.end());
    return os << s;
}
 
i128 toi128(const std::string &s) {
    i128 n = 0;
    for (auto c : s) {
        n = n * 10 + (c - '0');
    }
    return n;
}
//1)
i128 ans;
cin>>ans;

//2)
string s;
cin>>s;
i128 ans = toi128(s);
