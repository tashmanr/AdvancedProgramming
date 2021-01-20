/*
 * anomaly_detection_util.h
 * Author: Rebecca Tashman 336423124
 * Created on 12/7/2020.
 */

#ifndef ADVANCED_PROGRAMMING_ANOMALY_DETECTION_UTIL_H
#define ADVANCED_PROGRAMMING_ANOMALY_DETECTION_UTIL_H

float avg(float* x, int size);

// returns the variance of X and Y
float var(float* x, int size);

// returns the covariance of X and Y
float cov(float* x,  float* y, int size);


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size);

class Line{
public:
    float a,b;
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
};

class Point{
public:
    float x,y;
    Point(float x, float y):x(x),y(y){}
};

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size);

// returns the deviation between point p and the line
float dev(Point p,Line l);

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size);

#endif //ADVANCED_PROGRAMMING_ANOMALY_DETECTION_UTIL_H