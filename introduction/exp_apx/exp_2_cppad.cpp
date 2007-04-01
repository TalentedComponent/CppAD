/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_2_cppad.cpp$$
$spell
	cppad.hpp
	cmath
	fabs
	bool
	exp_2_cppad
	du
	dv
	dw
	endl
	hpp
	http
	org
	std
	www
	CppAD
	apx
$$

$section exp_2: CppAD First Order Forward and Reverse$$.

$head Purpose$$
Use CppAD forward and reverse modes to compute the
partial derivative with respect to $latex x$$,
at the point $latex x = .5$$,
of the function 
$syntax%
	exp_2(%x%)
%$$
as defined by the $cref/exp_2.hpp/$$ include file.

$head Exercises$$
$list number$$
Create and tests a modified version of the routine below that computes
partial derivative with respect to $latex x$$,
at the point $latex x = .1$$ 
of the function 
$syntax%
	exp_2(%x%)
%$$
$lnext
Create routine called
$syntax%
	exp_3(%x%)
%$$ 
that evaluates the function
$latex \[
	f(x) = 1 + x^2 / 2 + x^3 / 6
\] $$
Test a modified version of the routine below that computes
the derivative of $latex f(x)$$
at the point $latex x = .5$$.
$lend
$codep */

# include <cppad/cppad.hpp>  // http://www.coin-or.org/CppAD/ 
# include "exp_2.hpp"        // second order exponential approximation
bool exp_2_cppad(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;    // can use any simple vector template class
	using CppAD::NearEqual; // checks if values are nearly equal

	// domain space vector
	size_t n = 1; // dimension of the domain space
	vector< AD<double> > U(n);
	U[0] = .5;    // value of x for this operation sequence

	// declare independent variables and start recording operation sequence
	CppAD::Independent(U);

	// evaluate our exponential approximation
	AD<double> x   = U[0];
	AD<double> apx = exp_2(x);  

	// range space vector
	size_t m = 1;  // dimension of the range space
	vector< AD<double> > V(m);
	V[0] = apx;    // variable that represents only range space component

	// Create f: U -> V corresponding to this operation sequence
	// and stop recording. This also executes a zero order forward 
	// mode sweep using values in U for x and e.
	CppAD::ADFun<double> f(U, V);

	// first order forward mode sweep that computes partial w.r.t x
	vector<double> du(n);  // differential in domain space
	vector<double> dv(m);  // differential in range space
	du[0] = 1.;  // x direction in domain space
	// partial of exp_2(x) with respect to x
	dv    = f.Forward(1, du);
	double check = 1.5;
	ok   &= NearEqual(dv[0], check, 1e-10, 1e-10);

	// first order reverse mode sweep that computes the derivative
	vector<double>  w(m);   // weights for components of the range
	vector<double> dw(n);   // derivative of the weighted function
	w[0] = 1.;   // only one weight and it is one
	// derivative of w[0] * exp_2(x)
	dw   = f.Reverse(1, w);
	check = 1.5;  // partial of exp_2(x) with respect to x
	ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);

	return ok;
}
/* $$
$end
*/