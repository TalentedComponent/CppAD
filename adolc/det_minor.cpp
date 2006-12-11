/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin DetMinorAdolc.cpp$$
$spell
	adolc
	Cpp
$$

$section Adolc & CppAD Gradient of Determinant by Minors$$


$index Adolc, speed$$
$index speed, Adolc$$
$index determinant, Adolc$$
$index Adolc, determinant$$
$index Minors, Adolc expand$$
$index Adolc, Minor expand$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%adolc/det_minor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include "../example/det_by_minor.hpp"

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

void DetMinorCp(size_t size, size_t repeat, CppADvector<double> &u)
{
	using namespace CppAD;

	size_t i;
	size_t j;


	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	DetByMinor< AD<double> > Det(size);

	CppADvector< AD<double> >            detA(1);
	CppADvector< AD<double> >   A( size * size );
	for( i = 0; i < size; i++)
	{	A[ i ] = 1.;
		for(j = 1; j < size; j++)
			A[i + j * size] = double(i) * A[ i + (j-1) * size ];
	}
	
	// vectors for reverse mode input 
	CppADvector<double> v(1);
	v[0] = 1.;

	// ------------------------------------------------------

	while(repeat--)
	{	// declare independent variables
		Independent(A);

		// compute the determinant
		detA[0] = Det(A);

		// create function object f : A -> detA
		ADFun<double> f(A, detA);

		// evalute and return gradient using reverse mode
		u = f.Reverse(1, v);
	}
}

void DetMinorAd(size_t size, size_t repeat, double *u)
{
	size_t i;
	size_t j;

	double  deta;

	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	CppAD::DetByMinor<adouble> Det(size);

	adouble                           detA;
	CppADvector<adouble>  A( size * size );
	CppADvector<double>   a( size * size );
	for( i = 0; i < size; i++)
	{	a[ i ] = 1.;
		for(j = 1; j < size; j++)
			a[i + j * size] = double(i) * a[ i + (j-1) * size ];
	}

	// vectors for reverse mode input 
	double v[1];
	v[0] = 1.;

	// tag and keep flags
	int tag  = 1;
	int keep = 1;
	int d    = 0; 

	// number of elements in A
	size_t  length = size * size;

	// ------------------------------------------------------

	while(repeat--)
	{	// declare independent variables
		trace_on(tag, keep);
		for(i = 0; i < length; i++)
			A[i] <<= a[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= deta;
		trace_off();

		// evalute and return gradient using reverse mode
		reverse(tag, 1, length, d, v, u);
	}
}

bool DetMinor(void)
{	bool ok = true;

	size_t size   = 4;
	size_t repeat = 1;

	double *uAd = new double [size * size];
	CppADvector<double> uCp(size * size);

	DetMinorCp(size, repeat, uCp);
	DetMinorAd(size, repeat, uAd);


	size_t i;
	for(i = 0; i < size * size; i++)
		ok &= uCp[i] == uAd[i];

	delete [] uAd;
	return ok;
}
std::string DetMinorCp(size_t size, size_t repeat)
{	CppADvector<double> u(size * size);

	DetMinorCp(size, repeat, u);
	return "CppAD: gradient of Determinant by Minors";
}
std::string DetMinorAd(size_t size, size_t repeat)
{
	double *u = new double [size * size];

	DetMinorAd(size, repeat, u);

	delete [] u;
	return "Adolc: gradient of Determinant by Minors";
}

// END PROGRAM