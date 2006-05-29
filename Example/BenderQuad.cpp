/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin BenderQuad.cpp$$
$spell
	argmin
$$

$section BenderQuad: Example and Test$$

$index BenderQuad, example$$
$index example, BenderQuad$$
$index test, BenderQuad$$

Define 
$latex F : \R \times \R \rightarrow \R$$ by
$latex \[
F(x, y) 
= 
\frac{1}{2} \sum_{i=1}^N [ y * \sin ( x * t_i ) - z_i ]^2
\] $$ 
where $latex z \in \R^N$$ is a fixed vector.
It follows that
$latex \[
\begin{array}{rcl}
\partial_y F(x, y) 
& = & 
\sum_{i=1}^N [ y * \sin ( x * t_i ) - z_i ] \sin( x * t_i )
\\
\partial_y \partial_y F(x, y)
& = & 
\sum_{i=1}^N \sin ( x t_i )^2
\end{array}
\] $$
Furthermore if we define $latex Y(x)$$ 
as the argmin of $latex F(x, y)$$ with respect to $latex y$$,
$latex \[
\begin{array}{rcl}
Y(x) 
& = &
y - [ \partial_y \partial_y F(x, y) ]^{-1} \partial_y F[x,  y] 
\\
& = &
\left. 
	\sum_{i=1}^N z_i \sin ( x t_i ) 
		\right/ 
			\sum_{i=1}^N z_i \sin ( x * t_i )^2 
\end{array}
\] $$



$code
$verbatim%Example/BenderQuad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace {
	// make one copy of t and z visible in all the functions below
	CppADvector<double> t;
	CppADvector<double> z;

	class F_type {
	public:
		// constructor
		F_type(void)
		{ }
		// f = F(x, y)
		template <class Vector>
		Vector operator()(const Vector &x, const Vector &y)
		{	// determine type of vector elements
        		typedef typename Vector::value_type Type;	

			size_t i;
			size_t N = z.size();

			Vector f(1);
			f[0] = Type(0);

			Type residual;
			for(i = 0; i < N; i++)
			{	residual = y[0] * sin( x[0] * t[i] ) - z[i];
				f[0]    += residual * residual;
			}
			return f;
		}

	};
	class Fy_type {
	public:
		// constructor
		Fy_type(void)
		{ }
		// fy = F_y (x, y)
		template <class Vector>
		Vector operator()(const Vector &x, const Vector &y)
		{	// determine type of vector elements
        		typedef typename Vector::value_type Type;	

			size_t i;
			size_t N = z.size();

			Vector fy(1);
			fy[0] = Type(0);

			Type residual;
			for(i = 0; i < N; i++)
			{	residual = y[0] * sin( x[0] * t[i] ) - z[i];
				fy[0]   += residual * sin( x[0] * t[i] );
			}

			return fy;
		}
	};
	class Newton_type {
	public:
		// constructor
		Newton_type(void)
		{ }
		// dy = - F_yy (x, y)^{-1} * fy
		template <class Vector>
		Vector operator()(
			const Vector &x , 
			const Vector &y , 
			const Vector &fy)
		{	// determine type of vector elements
        		typedef typename Vector::value_type Type;	

			size_t i;
			size_t N = z.size();

			Vector dy(1);
			Type fyy = Type(0);

			for(i = 0; i < N; i++)
			{	fyy += sin( x[0] * t[i] ) * sin( x[0] * t[i] );
			}
			dy[0] = - fy[0] / fyy;

			return dy;
		}
	};
	class Y_type {
	public:
		// constructor
		Y_type(void)
		{ }
		// y = Y(x) 
		template <class Vector>
		Vector operator()(const Vector &x )
		{	// determine type of vector elements
        		typedef typename Vector::value_type Type;	

			size_t i;
			size_t N = z.size();

			Vector y(1);
			Type num = Type(0);
			Type den = Type(0);

			for(i = 0; i < N; i++)
			{	num += z[i] * sin( x[0] * t[i] );
				den += sin( x[0] * t[i] ) * sin( x[0] * t[i] );
			}
			y[0] = num / den;

			return y;
		}
	};
}

bool BenderQuad(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// temporary indices
	size_t i;

	// x space vector
	size_t n = 1;
	CppADvector<double> x(n);
	x[0] = 2. * 3.141592653;

	// y space vector
	size_t m = 1;
	CppADvector<double> y(m);
	y[0] = 1.;

	// t and z vectors
	size_t N = 10;
	t.resize(N);
	z.resize(N);
	for(i = 0; i < N; i++)
	{	t[i] = double(i) / double(N);       // time or measurement
		z[i] = y[0] * sin( x[0] * t[i] );   // data without noise
	}

	// construct the function objects
	F_type      F;
	Fy_type     Fy;
	Newton_type Newton;

	// evaluate the G(x), G'(x) and G''(x)
	CppADvector<double> g(1), gx(n), gxx(n * n);
	BenderQuad(x, y, F, Fy, Newton, g, gx, gxx);

	// Evaluate G(x) at nearby points
	Y_type      Y;
	double              step(1e-5);
	CppADvector<double> g0 = F(x, Y(x) );
	x[0] = x[0] + 1. * step;
	CppADvector<double> gp = F(x, Y(x) );
	x[0] = x[0] - 2. * step;
	CppADvector<double> gm = F(x, Y(x) );

	// check function value
	double check = g0[0];
	ok          &= NearEqual(check, g[0], 1e-10, 1e-10);

	// check derivative value
	check        = (gp[0] - gm[0]) / (2. * step);
	ok          &= NearEqual(check, gx[0], 1e-10, 1e-10);

	check        = (gp[0] - 2. * g0[0] + gm[0]) / (step * step);
	ok          &= NearEqual(check, gxx[0], 1e-10, 1e-10);

	// resize global vectors to zero to avoid memory leak report
	t.resize(0);
	z.resize(0);

	return ok;
}

// END PROGRAM