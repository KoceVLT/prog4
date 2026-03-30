#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

using namespace std;

struct Point {
    double x, y;
};

struct Triangle {
    int v0, v1, v2;
};

bool inCircumcircle(const Point& p, const Triangle& tri, const vector<Point>& pts) {
    const Point& a = pts[tri.v0];
    const Point& b = pts[tri.v1];
    const Point& c = pts[tri.v2];

    double ax = a.x, ay = a.y;
    double bx = b.x, by = b.y;
    double cx = c.x, cy = c.y;
    double px = p.x, py = p.y;

    double d = 2.0 * (ax * (by - cy) + bx * (cy - ay) + cx * (ay - by));
    if (abs(d) < 1e-12) return false;

    double ux = ((ax*ax + ay*ay) * (by - cy) + (bx*bx + by*by) * (cy - ay) + (cx*cx + cy*cy) * (ay - by)) / d;
    double uy = ((ax*ax + ay*ay) * (cx - bx) + (bx*bx + by*by) * (ax - cx) + (cx*cx + cy*cy) * (bx - ax)) / d;

    double dx = ax - ux;
    double dy = ay - uy;
    double r2 = dx*dx + dy*dy;

    dx = px - ux;
    dy = py - uy;
    return (dx*dx + dy*dy) < r2;
}

int main() {
    vector<Point> points;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        double x, y;
        cin >> x >> y;
        points.push_back({x, y});
    }

    double minX = points[0].x, minY = points[0].y, maxX = points[0].x, maxY = points[0].y;
    for (size_t i = 0; i < points.size(); ++i) {
        if (points[i].x < minX) minX = points[i].x;
        if (points[i].y < minY) minY = points[i].y;
        if (points[i].x > maxX) maxX = points[i].x;
        if (points[i].y > maxY) maxY = points[i].y;
    }
    double dx = maxX - minX;
    double dy = maxY - minY;
    double delta = max(dx, dy) * 10.0;

    Point p1 = {minX - delta, minY - delta};
    Point p2 = {maxX + delta, minY - delta};
    Point p3 = {(minX + maxX) * 0.5, maxY + delta};

    points.push_back(p1);
    points.push_back(p2);
    points.push_back(p3);
    int superV0 = points.size() - 3;
    int superV1 = points.size() - 2;
    int superV2 = points.size() - 1;

    vector<Triangle> triangles;
    triangles.push_back(Triangle{superV0, superV1, superV2});

    for (size_t i = 0; i < points.size() - 3; ++i) {
        const Point& p = points[i];
        vector<int> badTriangles;
        for (size_t j = 0; j < triangles.size(); ++j) {
            if (inCircumcircle(p, triangles[j], points)) {
                badTriangles.push_back(j);
            }
        }

        vector<pair<int,int> > polygon;
        for (size_t idx = 0; idx < badTriangles.size(); ++idx) {
            const Triangle& t = triangles[badTriangles[idx]];
            pair<int,int> edges[3];
            edges[0] = make_pair(t.v0, t.v1);
            edges[1] = make_pair(t.v1, t.v2);
            edges[2] = make_pair(t.v2, t.v0);
            for (int k = 0; k < 3; ++k) {
                if (edges[k].first > edges[k].second) swap(edges[k].first, edges[k].second);
                vector<pair<int,int> >::iterator it = find(polygon.begin(), polygon.end(), edges[k]);
                if (it != polygon.end()) {
                    polygon.erase(it);
                } else {
                    polygon.push_back(edges[k]);
                }
            }
        }

        vector<Triangle> newTriangles;
        for (size_t j = 0; j < triangles.size(); ++j) {
            bool isBad = false;
            for (size_t k = 0; k < badTriangles.size(); ++k) {
                if ((int)j == badTriangles[k]) {
                    isBad = true;
                    break;
                }
            }
            if (!isBad) {
                newTriangles.push_back(triangles[j]);
            }
        }
        triangles.swap(newTriangles);

        for (size_t k = 0; k < polygon.size(); ++k) {
            triangles.push_back(Triangle{static_cast<int>(i), polygon[k].first, polygon[k].second});
        }
    }

    vector<Triangle> finalTriangles;
    for (size_t i = 0; i < triangles.size(); ++i) {
        const Triangle& t = triangles[i];
        if (t.v0 != superV0 && t.v0 != superV1 && t.v0 != superV2 &&
            t.v1 != superV0 && t.v1 != superV1 && t.v1 != superV2 &&
            t.v2 != superV0 && t.v2 != superV1 && t.v2 != superV2) {
            finalTriangles.push_back(t);
        }
    }

    for (size_t i = 0; i < finalTriangles.size(); ++i) {
        const Triangle& t = finalTriangles[i];
        cout << "Triangle: " << t.v0 << " " << t.v1 << " " << t.v2 << endl;
    }

    return 0;
}