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
