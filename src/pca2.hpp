#ifndef PCA2_HPP_
#define PCA2_HPP_

#define SIGN(a, b) ( (b) < 0 ? -fabs(a) : fabs(a) )

template <typename T>
T rescale(T min, T max, T ratio){
	return (T)(min+ ratio*(max-min));
}


template <typename T>
void caclucate_mean(const int n_points, const int dimensionality, const vector< vector<T> > &data, vector<T> &mean){

	for(int i=0; i<n_points; ++i)
		for(int j=0; j<dimensionality; ++j)
			mean[j]+=data[i][j];
#ifdef DEBUG
	cout <<"Means of column vectors:" <<endl;
#endif //DEBUG
	for(int j=0; j<dimensionality; ++j){
		mean[j]/=n_points;
#ifdef DEBUG
		cout<<mean[j]<<" ";
#endif //DEBUG
	}
#ifdef DEBUG
	cout << endl << endl;
#endif //DEBUG
}

template <typename T>
void calc_stddev(const int n_points, const int dimensionality, const vector< vector<T> > &data,  vector<T> &mean,  vector<T> &stddev){

	for (int i = 0; i < n_points; i++)
		for (int j = 0; j < dimensionality; j++)
			stddev[j] +=    (data[i][j]-mean[j])   *   (data[i][j]-mean[j])    ;


#ifdef DEBUG
	cout << "Standard deviations of columns:" << endl;
#endif //DEBUG
	for(int j=0; j<dimensionality; ++j){
		stddev[j]/=(float)n_points;
		stddev[j] = sqrt(stddev[j]);
		/* The following in an inelegant but usual way to handle
		        near-zero std. dev. values, which below would cause a zero-
		        divide. */
		if (stddev[j] <= EPS) stddev[j] = 1.0;
#ifdef DEBUG
		cout<<stddev[j]<<" ";
#endif //DEBUG
	}
	cout << endl << endl;
}

template <typename T>
void calc_matrices(const int n_points, const int dimensionality, vector< vector<T> > &data, vector< vector<T> > &correlation){
	/* Calculate the m * m correlation matrix. */
	int j1, j2, i, m=dimensionality, n=n_points;

	for (j1 = 0; j1 < m-1; j1++){
		correlation[j1][j1] = 1.0;
		for (j2 = j1+1; j2 < m; j2++){
			correlation[j1][j2] = 0.0;
			for (i = 0; i < n; i++){
				correlation[j1][j2] += ( data[i][j1] * data[i][j2]);
			}
			correlation[j2][j1] = correlation[j1][j2];
		}
	}
	correlation[m-1][m-1] = 1.0;
#ifdef DEBUG
	cout << "correlation" << endl;
#endif //DEBUG
	for (j1 = 0; j1 < m; j1++){
		for (j2 = 0; j2 < m; j2++){
#ifdef DEBUG
			cout << correlation[j2][j1] << " ";
#endif //DEBUG
		}
#ifdef DEBUG
		cout << endl;
#endif //DEBUG
	}
}




template <typename T>
void triangular_decomposition(vector< vector<T> > &a,int n, vector<T> &d, vector<T> &e){
	/* Householder reduction of matrix a to tridiagonal form.
	   Algorithm: Martin et al., Num. Math. 11, 181-195, 1968.
	   Ref: Smith et al., Matrix Eigensystem Routines -- EISPACK Guide
	        Springer-Verlag, 1976, pp. 489-494.
	        W H Press et al., Numerical Recipes in C, Cambridge U P,
	        1988, pp. 373-374.  */

	int l,k,j,i;
	float scale,hh,h,g,f;


	for (i=n-1;i>0;i--) {
		l=i-1;
		h=scale=0.0;
		if (l > 0) {
			for (k=0;k<l+1;k++)
				scale += fabs(a[i][k]);
			if (scale == 0.0)
				e[i]=a[i][l];
			else {
				for (k=0;k<l+1;k++) {
					a[i][k] /= scale;
					h += a[i][k]*a[i][k];
				}
				f=a[i][l];
				g=(f >= 0.0 ? -sqrt(h) : sqrt(h));
				e[i]=scale*g;
				h -= f*g;
				a[i][l]=f-g;
				f=0.0;
				for (j=0;j<l+1;j++) {
					// Next statement can be omitted if eigenvectors not wanted
					a[j][i]=a[i][j]/h;
					g=0.0;
					for (k=0;k<j+1;k++)
						g += a[j][k]*a[i][k];
					for (k=j+1;k<l+1;k++)
						g += a[k][j]*a[i][k];
					e[j]=g/h;
					f += e[j]*a[i][j];
				}
				hh=f/(h+h);
				for (j=0;j<l+1;j++) {
					f=a[i][j];
					e[j]=g=e[j]-hh*f;
					for (k=0;k<j+1;k++)
						a[j][k] -= (f*e[k]+g*a[i][k]);
				}
			}
		} else
			e[i]=a[i][l];
		d[i]=h;
	}
	// Next statement can be omitted if eigenvectors not wanted
	d[0]=0.0;
	e[0]=0.0;
	// Contents of this loop can be omitted if eigenvectors not
	//	wanted except for statement d[i]=a[i][i];
	for (i=0;i<n;i++) {
		l=i;
		if (d[i] != 0.0) {
			for (j=0;j<l;j++) {
				g=0.0;
				for (k=0;k<l;k++)
					g += a[i][k]*a[k][j];
				for (k=0;k<l;k++)
					a[k][j] -= g*a[k][i];
			}
		}
		d[i]=a[i][i];
		a[i][i]=1.0;
		for (j=0;j<l;j++) a[j][i]=a[i][j]=0.0;
	}

}



template <typename T>
void tridiagonal_QLi(vector<T> &d, vector<T> &e, int n, vector< vector<T> > &z){
	int m, l, iter, i, k;
	float s, r, p, g, f, dd, c, b;

	for (i=1;i<n;i++) e[i-1]=e[i];
	e[n-1]=0.0;
	for (l=0;l<n;l++) {
		iter=0;
		do {
			for (m=l;m<n-1;m++) {
				dd=fabs(d[m])+fabs(d[m+1]);
				if (fabs(e[m])+dd == dd) break;
			}
			if (m != l) {
				if (iter++ == 30) erhand("Too many iterations in tqli");
				g=(d[l+1]-d[l])/(2.0*e[l]);
				r=pythag(g,1.0);
				g=d[m]-d[l]+e[l]/(g+SIGN(r,g));
				s=c=1.0;
				p=0.0;
				for (i=m-1;i>=l;i--) {
					f=s*e[i];
					b=c*e[i];
					e[i+1]=(r=pythag(f,g));
					if (r == 0.0) {
						d[i+1] -= p;
						e[m]=0.0;
						break;
					}
					s=f/r;
					c=g/r;
					g=d[i+1]-p;
					r=(d[i]-g)*s+2.0*c*b;
					d[i+1]=g+(p=s*r);
					g=c*r-b;
					// Next loop can be omitted if eigenvectors not wanted
					for (k=0;k<n;k++) {
						f=z[k][i+1];
						z[k][i+1]=s*z[k][i]+c*f;
						z[k][i]=c*z[k][i]-s*f;
					}
				}
				if (r == 0.0 && i >= l) continue;
				d[l] -= p;
				e[l]=g;
				e[m]=0.0;
			}
		} while (m != l);
	}

}

template <typename T>
void PCA(vector< vector<T> > &data, const int n, const int m, vector<T> &importance_vector){

	// PRINCIPA COMPONENT ANALYSIS
	//data - input data
	//n - number of points
	//m - dimensionality

	vector<T> mean(m, 0.0);
	vector<T> stddev(m, 0.0);


	vector< vector<T> > correlation(m, vector<T>(m,-1) ); /* Allocation of correlation (etc.) matrix */

	vector<T> evals(m,0.0);     /* Storage alloc. for vector of eigenvalues */
	vector<T>  interm(m,0.0);    /* Storage alloc. for 'intermediate' vector */


	caclucate_mean(n, m, data, mean);
	calc_stddev(n, m, data, mean, stddev);



	for (int i = 0; i < n; i++)	{
		for (int j = 0; j < m; j++)		{
			data[i][j] -= mean[j];
			data[i][j] /= ( sqrt((float)n) * stddev[j]);
		}
	}

	calc_matrices(n, m, data, correlation);

	triangular_decomposition(correlation, m, evals, interm);  /* Triangular decomposition */

	tridiagonal_QLi(evals, interm, m, correlation);   /* Reduction of sym. trid. matrix */


#ifdef DEBUG
	printf("\nEigenvalues:\n");
#endif //DEBUG

	float tot=0.0;
	for (int j = m-1; j > 1; j--) {
		//		printf("%18.5f\n", evals[j]);
#ifdef DEBUG
		cout << evals[j] << endl;
#endif //DEBUG
		tot+=evals[j];

	}
#ifdef DEBUG
	cout <<"\n(Eigenvalues should be strictly positive; limited"<< endl;
	cout <<"precision machine arithmetic may affect this."<< endl;
	cout <<"Eigenvalues are often expressed as cumulative"<< endl;
	cout <<"percentages, representing the 'percentage variance"<< endl;
	cout <<"explained' by the associated axis or principal component.)"<< endl;
#endif //DEBUG

#ifdef DEBUG
	cout << "importance%" << endl;
	for (int j = m-1; j >= 0; --j) {
		cout << evals[j]*100/tot <<" ";
	}
	cout << endl;
#endif //DEBUG


	for(int i=0; i<m; ++i){
		importance_vector[i]=evals[m-1-i]*100/tot;
		if(importance_vector[i] <EPS ) importance_vector[i]=0.0;
#ifdef DEBUG
		cout << importance_vector[i] <<" ";
#endif //DEBUG
	}

	cout << endl;

	return;
}




#endif /* PCA2_HPP_ */
