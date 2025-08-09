// 多重背包 (每個物品mi個)
// w重量 v價值 m個數 最重取wm
// 攤成∑mi個元素的01背包，若mi*wi > wm，可退化成完全背包
/*
*/
// 二進制優化，將15個表示成1+2+4+8個
for(int i=1; i<=n; i++){
    cin >> vi >> wi >> mi;
    base = 1;
    while(mi - base >= 0){
        mi -= base;
        a.push_back({base * vi, base * wi});
        base <<= 1;
    }
    if(mi > 0){ // 若有剩餘
        a.push_back({mi * vi, mi * wi});
    }
}
/*
01背包
*/