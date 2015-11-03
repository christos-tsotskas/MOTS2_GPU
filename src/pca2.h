/*
 * pca2.h
 *
 *  Created on: 26 Jan 2013
 *      Author: Christos
 */

#ifndef PCA2_H_
#define PCA2_H_

#include <iostream>
#include <vector>
//#include <fstream>
//#include <cmath>
//#include <cstdlib>

#define SIGN(a, b) ( (b) < 0 ? -fabs(a) : fabs(a) )
#define EPS 0.005

static float sqrarg;
#define SQR(a) ((sqrarg=(a)) == 0.0 ? 0.0 : sqrarg*sqrarg)


using namespace std;


void erhand(const string err_msg);

float pythag(float a,float b);

template <typename T>
T rescale(T min, T max, T ratio);

template <typename T>
void caclucate_mean(const int n_points, const int dimensionality, const vector< vector<T> > &data, vector<T> &mean);

template <typename T>
void calc_stddev(const int n_points, const int dimensionality, const vector< vector<T> > &data,  vector<T> &mean,  vector<T> &stddev);

template <typename T>
void triangular_decomposition(vector< vector<T> > &a,int n, vector<T> &d, vector<T> &e);

template <typename T>
void tridiagonal_QLi(vector<T> &d, vector<T> &e, int n, vector< vector<T> > &z);

template <typename T>
void PCA(vector< vector<T> > &data, const int n, const int m, vector<T> &importance_vector);

#include "pca2.hpp"


//template <typename T>
//void PCA2(T &data);

#endif /* PCA2_H_ */
