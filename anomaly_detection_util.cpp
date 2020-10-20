/*
 * anomaly_detection_util.cpp
 *
 * Author: Rebecca Tashman 336423124
 */

#include <math.h>
#include "anomaly_detection_util.h"

// returns average of a set of numbers
float avg(float *x, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum / size;
}

// returns the variance of X and Y
float var(float *x, int size) {
    float sum1 = 0;
    float sum2 = 0;
    for (int i = 0; i < size; i++) {
        sum1 += pow(x[i], 2);
        sum2 += x[i];
    }
    float avg = sum2 / size;
    return (sum1 / size) - pow(avg, 2);
}

// returns the covariance of X and Y
float cov(float *x, float *y, int size) {
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += (x[i] - avg(x, size)) * (y[i] - avg(y, size));
    }
    return (sum / size);
}

// returns the Pearson correlation coefficient of X and Y
float pearson(float *x, float *y, int size) {
    return cov(x, y, size) / (sqrt(var(x, size)) * sqrt(var(y, size)));
}

// performs a linear regression and return s the line equation
Line linear_reg(Point **points, int size) {
    float x[size], y[size], a, b;
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    a = cov(x, y, size) / var(x, size);
    b = avg(y, size) - (a * avg(x, size));
    return Line(a, b);
}

// returns the deviation between point p and the line
float dev(Point p, Line l) {
    return fabs(l.f(p.x) - p.y);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p, Point **points, int size) {
    return dev(p, linear_reg(points, size));
}