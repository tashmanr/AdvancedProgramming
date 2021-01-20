// 336423124	318879939

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <cmath>
#include "stdlib.h"
#include "anomaly_detection_util.h"

using namespace std;

class Circle {
public:
    Point center;
    float radius;

    Circle(Point c, float r) : center(c), radius(r) {}
};

//function to return the middle of two points
Point middle(const Point p1, const Point p2);

//Function to measure the distance between two points
float distance(const Point p1, const Point p2);

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
Circle threePointCircle(const Point p1, const Point p2, const Point p3);

//This function checks the size of r, and based on that knows how to make a circle
Circle circleFromPoints(vector<Point> r);

//Function checks if a given point lies within the circle (includes those on perimeter as well)
bool pointInCircle(const Point p, const Circle c);

//Recursive function, runs welzl algorithm
Circle recursiveCircleFinder(vector<Point> p, vector<Point> r);

//Function that receives initial array of points and size of array, returns final circle that should include all points
Circle findMinCircle(Point** points, size_t size);

#endif /* MINCIRCLE_H_ */
