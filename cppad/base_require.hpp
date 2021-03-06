// $Id$
# ifndef CPPAD_BASE_REQUIRE_INCLUDED
# define CPPAD_BASE_REQUIRE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin base_require$$
$spell
	ostream
	alloc
	eps
	std
	Lt
	Le
	Eq
	Ge
	Gt
	cppad.hpp
	namespace
	optimizations
	bool
	const
	CppAD
	enum
	Lt
	Le
	Eq
	Ge
	Gt
	inline
	Op
	std
	CondExp
$$

$section AD<Base> Requirements for a CppAD Base Type$$

$head Syntax$$
$code # include <cppad/base_require.hpp>$$

$head Purpose$$
This section lists the requirements for the type
$icode Base$$ so that the type $codei%AD<%Base%>%$$ can be used.

$head Warning$$
Defining a CppAD $icode Base$$ type is an advanced use of CppAD.
This part of the CppAD API changes with time. The most common change
is adding more requirements.
Search for mention `base type' in the
current $cref whats_new$$ section for these changes.

$head Standard Base Types$$
In the case where $icode Base$$ is
$code float$$,
$code double$$,
$code std::complex<float>$$,
$code std::complex<double>$$,
or $codei%AD<%Other%>%$$,
these requirements are provided by including the file
$code cppad/cppad.hpp$$.

$head Include Order$$
If you are linking a non-standard base type to CppAD,
you must first include the file $code cppad/base_require.hpp$$,
then provide the specifications below,
and then include the file $code cppad/cppad.hpp$$.

$head Numeric Type$$
The type $icode Base$$ must support all the operations for a
$cref NumericType$$.

$head Output Operator$$
The type $icode Base$$ must support the syntax
$codei%
	%os% << %x%
%$$
where $icode os$$ is an $code std::ostream&$$
and $icode x$$ is a $code const base_alloc&$$.
For example, see
$cref/base_alloc/base_alloc.hpp/Output Operator/$$.

$head Integer$$
The type $icode Base$$ must support the syntax
$codei%
	%i% = CppAD::Integer(%x%)
%$$
which converts $icode x$$ to an $code int$$.
The argument $icode x$$ has prototype
$codei%
	const %Base%& %x%
%$$
and the return value $icode i$$ has prototype
$codei%
	int %i%
%$$

$subhead Suggestion$$
In many cases, the $icode Base$$ version of the $code Integer$$ function
can be defined by
$codei%
namespace CppAD {
	inline int Integer(const %Base%& x)
	{	return static_cast<int>(x); }
}
%$$
For example, see
$cref/base_float/base_float.hpp/Integer/$$ and
$cref/base_alloc/base_alloc.hpp/Integer/$$.

$head Absolute Zero$$
If this base type will be used with
$cref/multiple levels of AD/mul_level/$$;
e.g. $codei%AD< AD<%Base%> >%$$,
and $cref/reverse mode/reverse/$$ calculations will be recorded,
the type $icode Base$$ must have an
$cref/absolute zero/zdouble/Absolute Zero/$$.

$subhead Nan$$
If the type $icode Base$$ has an absolute zero,
the CppAD $cref nan$$ template function must be specialized
because it assumes that zero divided by zero is $code nan$$.
For example, here is the specialization defined by $cref zdouble$$
$codei%
namespace CppAD {%$$
$code
$verbatim%cppad/local/zdouble.hpp%4%// BEGIN nan%// END nan%$$
$$
$codei }$$

$childtable%
	omh/base_require/base_member.omh%
	cppad/local/base_cond_exp.hpp%
	omh/base_require/base_identical.omh%
	omh/base_require/base_ordered.omh%
	cppad/local/base_std_math.hpp%
	omh/base_require/base_example.omh
%$$

$end
*/

// definitions that must come before base implementations
# include <cppad/error_handler.hpp>
# include <cppad/local/define.hpp>
# include <cppad/local/cppad_assert.hpp>
# include <cppad/local/declare_ad.hpp>

// grouping documentation by feature
# include <cppad/local/base_cond_exp.hpp>
# include <cppad/local/base_std_math.hpp>

// must define template class numeric_limits before the base cases
# include <cppad/local/limits.hpp>
# include <cppad/local/epsilon.hpp> // deprecated

// base cases that come with CppAD
# include <cppad/local/base_float.hpp>
# include <cppad/local/base_double.hpp>
# include <cppad/local/base_complex.hpp>

# endif
