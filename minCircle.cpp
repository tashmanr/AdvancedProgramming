// 336423124	318879939

#include "minCircle.h"

using namespace std;

//function to return the middle of two points
Point middle(const Point p1, const Point p2) {
    return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}

//Function to measure the distance between two points
float distance(const Point p1, const Point p2) {
    float distanceX, distanceY;
    distanceX = (p1.x - p2.x);
    distanceY = (p1.y - p2.y);
    return sqrt((distanceX * distanceX) + (distanceY * distanceY));
}

/*
 * The algorithm for this function is based on the function to find a circle (x^2+y^2+2gx+2fy+c= 0),
 * where the radius is equal to the square root of (x�h)^2 + (y -k)^2 when center point = (h,k).
 * If we create a system of equations for three of the points, we can solve for g,f,c and then
 * from that we'll be able to calculate the radius and center point.
 * Basically we can isolate f (or g, in a similar way) by calculating:
 * f=(((x1)^2�(x3)^2)(x1�x2)+((y1)^2�(y3)^2)(x1�x2)+((x2)^2�(x1)^2)(x1�x3)+((y2)^2�(y1)^2)(x1�x3)) /
 * 2(y3�y1)(x1�x2)�(y2�y1)(x1�x3)
 * Once we have the values for f and g, we can find c, and therefore we have our circle formula.
 * */
Circle threePointCircle(const Point p1, const Point p2, const Point p3) {
    float p2minusP1x, p2minusP1y, p3minusP1x, p3minusP1y;
    p2minusP1x = p2.x - p1.x;
    p2minusP1y = p2.y - p1.y;
    p3minusP1x = p3.x - p1.x;
    p3minusP1y = p3.y - p1.y;
    float f1 = (p2minusP1x * p2minusP1x) + (p2minusP1y * p2minusP1y);
    float f2 = (p3minusP1x * p3minusP1x) + (p3minusP1y * p3minusP1y);
    float f3 = (p2minusP1x * p3minusP1y) - (p2minusP1y * p3minusP1x);
    Point center(
            (((p3minusP1y * f1 - p2minusP1y * f2) / (2 * f3)) + p1.x),
            (((p2minusP1x * f2 - p3minusP1x * f1) / (2 * f3)) + p1.y));
    return Circle(center, distance(center, p1));
}

//This function checks the size of r, and based on that knows how to make a circle
Circle circleFromPoints(vector<Point> r) {
    if (r.size() == 3) {//if there are 3 points then we will send to threePointCircle function
        return threePointCircle(r[0], r[1], r[2]);
    }
    else if (r.empty()) {//if size is 0 then we will make a circle at point (0,0) that has radius of 0
        return Circle(Point(0, 0), 0);
    }
    else {//we have either 1 or 2 points (1 point will create trivial circle with radius of 0)
        const Point p = *r.end();
        Point center = middle(r[0], p);
        return Circle(center, distance(center, r[0]));
    }
}

//Function checks if a given point lies within the circle (includes those on perimeter as well)
bool pointInCircle(const Point p, const Circle c) {
    return (distance(p, c.center) <= c.radius);
}

//Recursive function, runs welzl algorithm
Circle recursiveCircleFinder(vector<Point> p, vector<Point> r) {
    while (1) {
        if (p.empty() || (r.size() == 3)) {
            return circleFromPoints(r);
        }
        const int i = (rand() % p.size());
        Point point = p[i];
        p.erase(p.begin() + i);
        Circle d = recursiveCircleFinder(p, r);
        if (pointInCircle(point, d)) {
            return d;
        }
        r.push_back(point);
    }
    //return recursiveCircleFinder(p, r);
}

//Function that receives initial array of points and size of array, returns final circle that should include all points
Circle findMinCircle(Point** points, size_t size) {
    vector<Point> p;
    for (int i = 0; i < size; i++) {
        p.push_back(*points[i]);
    }
    vector<Point> r;
    return recursiveCircleFinder(p, r);//call to recursive function to run welzl algorithm
}