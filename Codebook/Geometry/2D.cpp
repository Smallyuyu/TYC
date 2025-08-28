#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;
using vi = vector<int>;

using dbl = double;
const dbl EPS = 1e-9;
const dbl PI = acos(-1.0);

inline int sgn(dbl x){ if (x > EPS) return 1; if (x < -EPS) return -1; return 0; }
inline dbl sqr(dbl x){ return x*x; }

struct Point {
    dbl x,y;
    Point(): x(0), y(0) {}
    Point(dbl _x, dbl _y): x(_x), y(_y) {}
    Point operator + (const Point& b) const { return Point(x+b.x, y+b.y); }
    Point operator - (const Point& b) const { return Point(x-b.x, y-b.y); }
    Point operator * (dbl k) const { return Point(x*k, y*k); }
    Point operator / (dbl k) const { return Point(x/k, y/k); }
};
inline Point perp(const Point& p){ return Point(-p.y, p.x); }

inline dbl dot(const Point& a, const Point& b){ return a.x*b.x + a.y*b.y; }
inline dbl cross(const Point& a, const Point& b){ return a.x*b.y - a.y*b.x; }
inline dbl norm2(const Point& a){ return dot(a,a); }
inline dbl absP(const Point& a){ return sqrt(max((dbl)0.0, norm2(a))); }
inline dbl dist(const Point& a, const Point& b){ return absP(a-b); }

// 1) 兩點距離 -> dist(a,b)

// 2) 三點構成角度：角度 at b between ba and bc (返回弳度)
dbl angle_at(const Point& a, const Point& b, const Point& c){
    Point u = a - b, v = c - b;
    dbl du = absP(u), dv = absP(v);
    if (du < EPS || dv < EPS) return 0.0;
    dbl cosv = dot(u, v) / (du*dv);
    cosv = min<dbl>(1.0, max<dbl>(-1.0, cosv));
    return acos(cosv);
}

// 3) 點到直線距離 (線由 p--q 定義，返回距離)
dbl point_line_distance(const Point& p, const Point& a, const Point& b){
    // line a->b
    if (absP(b - a) < EPS) return dist(p, a);
    return fabs(cross(b - a, p - a)) / absP(b - a);
}

// 4) 判斷三點共線
bool collinear(const Point& a, const Point& b, const Point& c){
    return fabs(cross(b-a, c-a)) <= EPS * max((dbl)1.0, max(absP(b-a), absP(c-a)));
}

// 5) 判斷點是否在多邊形內部（含邊界）
// 回傳： 1 = inside, 0 = on boundary, -1 = outside
int point_in_polygon(const Point& pt, const vector<Point>& poly){
    // ray casting + boundary check
    int n = (int)poly.size();
    bool inside = false;
    for (int i = 0, j = n-1; i < n; j = i++){
        const Point &a = poly[j], &b = poly[i];
        // boundary: on segment
        dbl area2 = fabs(cross(b-a, pt-a));
        if (area2 <= EPS && dot(pt-a, pt-b) <= EPS) return 0;
        bool yi = (a.y > pt.y);
        bool yj = (b.y > pt.y);
        if (yi != yj){
            dbl xint = a.x + (b.x - a.x) * (pt.y - a.y) / (b.y - a.y);
            if (xint > pt.x) inside = !inside;
        }
    }
    return inside ? 1 : -1;
}

// 6) 求凸包（Andrew 簡單版，返回 ccw 凸包，若所有點 collinear 會 return endpoints）
// 輸入可包含重複點，輸出不含重複最後第一個點
vector<Point> convex_hull(vector<Point> pts){
    int n = pts.size();
    if (n <= 1) return pts;
    sort(pts.begin(), pts.end(), [](const Point& a, const Point& b){
        if (fabs(a.x - b.x) > EPS) return a.x < b.x;
        return a.y < b.y;
    });
    vector<Point> lo, hi;
    for (int i = 0; i < n; ++i){
        while (lo.size() >= 2 && cross(lo.back()-lo[lo.size()-2], pts[i]-lo.back()) <= EPS) lo.pop_back();
        lo.push_back(pts[i]);
    }
    for (int i = n-1; i >= 0; --i){
        while (hi.size() >= 2 && cross(hi.back()-hi[hi.size()-2], pts[i]-hi.back()) <= EPS) hi.pop_back();
        hi.push_back(pts[i]);
    }
    lo.pop_back(); hi.pop_back();
    lo.insert(lo.end(), hi.begin(), hi.end());
    if (lo.empty() && !pts.empty()) lo.push_back(pts[0]);
    return lo;
}

// 7) 旋轉卡殼：求凸包直徑 (最遠點對距離) 與最小寬度
// 7a: 直徑 (Antipodal pairs)
pair<dbl, pair<int,int>> rotating_calipers_diameter(const vector<Point>& ch){
    // ch must be convex hull in CCW, no repeated last point
    int n = ch.size();
    if (n == 0) return {0.0, {-1,-1}};
    if (n == 1) return {0.0, {0,0}};
    if (n == 2) return {dist(ch[0], ch[1]), {0,1}};
    int j = 1;
    dbl best = 0;
    pair<int,int> idx = {0,1};
    for (int i = 0; i < n; ++i){
        int ni = (i+1)%n;
        // advance j while area increases
        while (fabs(cross(ch[ni] - ch[i], ch[(j+1)%n] - ch[i])) > fabs(cross(ch[ni] - ch[i], ch[j] - ch[i]) ) + EPS){
            j = (j+1) % n;
        }
        // check distance candidates
        dbl d1 = dist(ch[i], ch[j]);
        if (d1 > best + EPS){ best = d1; idx = {i,j}; }
        dbl d2 = dist(ch[ni], ch[j]);
        if (d2 > best + EPS){ best = d2; idx = {ni,j}; }
    }
    return {best, idx};
}

// 7b: 最小寬度 (minimum distance between pair of parallel supporting lines)
// For convex polygon ch (ccw), compute min over edges of (max distance to edge)/|edge|
dbl rotating_calipers_min_width(const vector<Point>& ch){
    int n = ch.size();
    if (n <= 2) return 0;
    int j = 1;
    dbl minWidth = 1e300;
    for (int i = 0; i < n; ++i){
        int ni = (i+1)%n;
        Point edge = ch[ni] - ch[i];
        // find farthest vertex from line i->ni (antipodal)
        while (fabs(cross(edge, ch[(j+1)%n] - ch[i])) > fabs(cross(edge, ch[j] - ch[i])) + EPS){
            j = (j+1)%n;
        }
        dbl maxDist = fabs(cross(edge, ch[j] - ch[i])) / absP(edge);
        minWidth = min(minWidth, maxDist);
    }
    return minWidth;
}

// 8) 最近點對 (Closest Pair, O(n log n))
dbl closest_pair_rec(vector<Point>& a, int l, int r, vector<Point>& tmp){
    if (r - l <= 3){
        dbl best = 1e300;
        for (int i = l; i < r; ++i)
            for (int j = i+1; j < r; ++j)
                best = min(best, dist(a[i], a[j]));
        sort(a.begin()+l, a.begin()+r, [](const Point& A, const Point& B){ return A.y < B.y; });
        return best;
    }
    int m = (l + r) >> 1;
    dbl midx = a[m].x;
    dbl dl = closest_pair_rec(a, l, m, tmp);
    dbl dr = closest_pair_rec(a, m, r, tmp);
    dbl d = min(dl, dr);
    // merge by y
    inplace_merge(a.begin()+l, a.begin()+m, a.begin()+r, [](const Point& A, const Point& B){ return A.y < B.y; });
    int tsz = 0;
    for (int i = l; i < r; ++i){
        if (fabs(a[i].x - midx) < d + EPS){
            for (int j = tsz - 1; j >= 0 && (a[i].y - tmp[j].y) < d + EPS; --j){
                d = min(d, dist(a[i], tmp[j]));
            }
            tmp[tsz++] = a[i];
        }
    }
    return d;
}
dbl closest_pair(vector<Point> pts){
    int n = pts.size();
    if (n < 2) return 1e300;
    sort(pts.begin(), pts.end(), [](const Point& A, const Point& B){ if (fabs(A.x - B.x) > EPS) return A.x < B.x; return A.y < B.y; });
    vector<Point> tmp(max(0,n));
    return closest_pair_rec(pts, 0, n, tmp);
}

// 9) 多邊形面積（signed area，若為 CCW 則為正）
dbl polygon_area_signed(const vector<Point>& poly){
    int n = poly.size();
    if (n < 3) return 0;
    dbl A = 0;
    for (int i = 0; i < n; ++i){
        int j = (i+1)%n;
        A += cross(poly[i], poly[j]);
    }
    return A / 2.0;
}
dbl polygon_area(const vector<Point>& poly){
    return fabs(polygon_area_signed(poly));
}

// 10) 最小覆蓋圓 (Welzl's algorithm)
struct Circle { Point c; dbl r; Circle(): c(Point(0,0)), r(0) {} Circle(Point _c, dbl _r): c(_c), r(_r) {} };
Circle circle_from_2points(const Point& a, const Point& b){
    Point cen = (a + b) / 2.0;
    dbl r = dist(a,b) / 2.0;
    return Circle(cen, r);
}
Circle circle_from_3points(const Point& a, const Point& b, const Point& c){
    // circumcenter
    dbl A = b.x - a.x, B = b.y - a.y;
    dbl C = c.x - a.x, D = c.y - a.y;
    dbl E = A*(a.x + b.x) + B*(a.y + b.y);
    dbl F = C*(a.x + c.x) + D*(a.y + c.y);
    dbl G = 2.0*(A*(c.y - b.y) - B*(c.x - b.x));
    if (fabs(G) < EPS){
        // collinear -> return big circle
        Point cen(0,0);
        return Circle(cen, -1);
    }
    dbl cx = (D*E - B*F) / G;
    dbl cy = (A*F - C*E) / G;
    Point cen(cx, cy);
    return Circle(cen, dist(cen, a));
}
bool in_circle(const Circle& Cc, const Point& p){
    if (Cc.r < 0) return false;
    return dist(Cc.c, p) <= Cc.r + EPS;
}
Circle mec(vector<Point> pts){
    random_shuffle(pts.begin(), pts.end());
    Circle C = Circle(Point(0,0), -1);
    int n = pts.size();
    for (int i = 0; i < n; ++i){
        if (C.r >= 0 && in_circle(C, pts[i])) continue;
        C = Circle(pts[i], 0);
        for (int j = 0; j < i; ++j){
            if (in_circle(C, pts[j])) continue;
            C = circle_from_2points(pts[i], pts[j]);
            for (int k = 0; k < j; ++k){
                if (in_circle(C, pts[k])) continue;
                C = circle_from_3points(pts[i], pts[j], pts[k]);
            }
        }
    }
    return C;
}

// 11) 求凸包周長
dbl polygon_perimeter(const vector<Point>& poly){
    dbl per = 0;
    int n = poly.size();
    for (int i = 0; i < n; ++i){
        per += dist(poly[i], poly[(i+1)%n]);
    }
    return per;
}

// 12) 求凸包面積 -> use polygon_area on hull
// (we already have polygon_area and convex_hull)

// 13) 兩線段距離 (segment a-b 和 c-d)
dbl segment_segment_distance(const Point& a, const Point& b, const Point& c, const Point& d){
    // if segments intersect -> distance 0
    auto on_segment = [&](const Point& p, const Point& u, const Point& v){
        if (fabs(cross(v - u, p - u)) > EPS) return false;
        return dot(p - u, p - v) <= EPS;
    };
    auto seg_intersect = [&](const Point& a, const Point& b, const Point& c, const Point& d)->bool{
        auto ori = [&](const Point& p, const Point& q, const Point& r){ return sgn(cross(q-p, r-p)); };
        int o1 = ori(a,b,c), o2 = ori(a,b,d), o3 = ori(c,d,a), o4 = ori(c,d,b);
        if (o1 != o2 && o3 != o4) return true;
        if (o1==0 && on_segment(c, a, b)) return true;
        if (o2==0 && on_segment(d, a, b)) return true;
        if (o3==0 && on_segment(a, c, d)) return true;
        if (o4==0 && on_segment(b, c, d)) return true;
        return false;
    };
    if (seg_intersect(a,b,c,d)) return 0.0;
    // else min of point-line distances to segments endpoints
    auto point_segment_dist = [&](const Point& p, const Point& u, const Point& v){
        Point uv = v - u, up = p - u;
        dbl t = dot(up, uv) / norm2(uv);
        if (t < 0) return dist(p, u);
        if (t > 1) return dist(p, v);
        Point proj = u + uv * t;
        return dist(p, proj);
    };
    dbl ret = min( min(point_segment_dist(a,c,d), point_segment_dist(b,c,d)),
                   min(point_segment_dist(c,a,b), point_segment_dist(d,a,b)) );
    return ret;
}

// 14) 線段旋轉：rotate segment (a,b) around point o by angle rad -> returns pair new endpoints
pair<Point,Point> rotate_segment(const Point& a, const Point& b, const Point& o, dbl rad){
    auto rotate_about = [&](const Point& p, const Point& o, dbl rad){
        dbl c = cos(rad), s = sin(rad);
        Point v = p - o;
        return Point(v.x*c - v.y*s + o.x, v.x*s + v.y*c + o.y);
    };
    return { rotate_about(a, o, rad), rotate_about(b, o, rad) };
}

// 15) 求某點向某角度延伸距離 L 的點位置 (角度 rad)
Point point_from_angle_and_length(const Point& p, dbl angle_rad, dbl L){
    return p + Point(cos(angle_rad), sin(angle_rad)) * L;
}

// 16) 求兩直線交點 (每條線以兩點表示): return pair<bool, Point> (bool = have intersection unique)
pair<bool, Point> line_intersection(const Point& p1, const Point& p2, const Point& q1, const Point& q2){
    // Solve p1 + t*(p2-p1) = q1 + s*(q2-q1)
    Point r = p2 - p1, s = q2 - q1;
    dbl rxs = cross(r, s);
    dbl qpxr = cross(q1 - p1, r);
    if (fabs(rxs) < EPS){
        // parallel
        return {false, Point(0,0)};
    }
    dbl t = cross(q1 - p1, s) / rxs;
    Point inter = p1 + r * t;
    return {true, inter};
}

// === Usage example in sol() (可移除/修改) ===
void sol(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // 範例：建立一些點並示範函式
    vector<Point> pts = { Point(0,0), Point(1,0), Point(0,1), Point(1,1), Point(2,2) };

    // convex hull
    auto ch = convex_hull(pts);
    cerr << "Convex hull size: " << ch.size() << "\n";

    // distance between two points
    cerr << "dist(0,1) = " << dist(pts[0], pts[1]) << "\n";

    // angle at pts[0] between pts[1] and pts[2]
    cerr << "angle = " << angle_at(pts[1], pts[0], pts[2]) << " rad\n";

    // point in polygon
    vector<Point> poly = { {0,0}, {2,0}, {2,2}, {0,2} }; // ccw square
    Point test(1,1);
    cerr << "point_in_polygon = " << point_in_polygon(test, poly) << "\n"; // 1

    // closest pair
    cerr << "closest pair = " << closest_pair(pts) << "\n";

    // minimum enclosing circle
    auto C = mec(pts);
    cerr << "MEC center: " << C.c.x << "," << C.c.y << " r=" << C.r << "\n";

    // rotating calipers diameter
    auto diam = rotating_calipers_diameter(ch);
    cerr << "diam = " << diam.first << " between " << diam.second.first << "," << diam.second.second << "\n";

    // polygon area & perimeter
    cerr << "area = " << polygon_area(poly) << " per = " << polygon_perimeter(poly) << "\n";

    // two lines intersection
    auto res = line_intersection(Point(0,0), Point(1,1), Point(0,1), Point(1,0));
    if (res.first) cerr << "intersect at " << res.second.x << "," << res.second.y << "\n";
}

int main(){
    sol();
    return 0;
}
