// bottom-up
for(int l = 2 ; l <= n ; l++){ //長度
  for(int i = 0 ; i < n ; i++){ //左i 右j
    int j = i + l - 1;
    if(j >= n) break;
    for(int k = i ; k < j ; k++){ //切割點k
      dp[i][j] = max(dp[i][j],dp[i][k] + dp[k + 1][j] + pre[j + 1] - pre[i]);
      dp2[i][j] = min(dp2[i][j],dp2[i][k] + dp2[k + 1][j] + pre[j + 1] - pre[i]);
    }
  }
}
int mx = INT_MIN,mn = INT_MAX;
for(int i = 0 ; i <= n ; i++){ //如果環狀,陣列*2+特判
  mx = max(mx,dp[i][i + n - 1]);
  mn = min(mn,dp2[i][i + n - 1]);
}

// top-down
int Micnt(int L, int R) // 相鄰石子合併成一堆，最小花費
{
    if(dp[L][R] != -1){ // 記憶
        return dp[L][R];
    }
    if(L >= R){ // 邊界
        return dp[L][R] = 0;
    }
    if(L + 1 == R){ // 邊界
        return dp[L][R] = a[L] + a[R];
    }
    int lsum, rsum, msum, mi=1e9;
    for(int i=L; i<=R-1; i++){ // 分成[L, i]與[i+1, R]兩堆
        lsum = Micnt(L, i);
        rsum = Micnt(i+1, R);
        msum = (pre[i] - pre[L-1]) + (pre[R] - pre[i+1-1]);
        mi = min(mi, lsum + rsum + msum);
    }
    return dp[L][R] = mi;
}