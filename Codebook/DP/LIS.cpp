// 最長嚴格遞增
for(auto e:v){
    auto it = lower_bound(lis.begin(), lis.end(), e);
    if(it == lis.end()){
        lis.push_back(e);
    } 
    else{
        *it = e;
    }
}
/*
最長嚴格遞增 (LIS)  a<b  auto it = lower_bound(g.begin(), g.end(), e);
最長非遞減 (LNDS)   a≤b  auto it = upper_bound(g.begin(), g.end(), e);
最長嚴格遞減 (LSDS)	a>b  auto it = lower_bound(g.begin(), g.end(), e, greater<int>());
最長非遞增 (LNIS)	a≥b  auto it = upper_bound(g.begin(), g.end(), e, greater<int>());
*/