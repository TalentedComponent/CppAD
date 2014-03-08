/* $Id$ */
# ifndef CPPAD_LOAD_OP_INCLUDED
# define CPPAD_LOAD_OP_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup load_op_hpp load_op.hpp
\{
\file load_op.hpp
Setting a variable so that it corresponds to current value of a VecAD element.
*/

/*!
Structure used to hold tape information about one vecad element.
Defining here uses fact that load_op.hpp is included before store_op.hpp.
*/
typedef struct { 
	/// is this a variable index
	bool is_var; 
	/// value of the index
	size_t index;
}  vecad_element;


/*!
Shared documentation for zero order forward mode implementation of 
op = LdpOp or LdvOp (not called).

The C++ source code corresponding to this operation is
\verbatim
	z = v[x]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = element_by_ind[ arg[0] + i_vec ].index
\endverbatim
where i_vec is defined under the heading arg[1] below:

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\n
arg[0]
is the offset of this VecAD vector relative to the beginning 
of the element_by_ind array.
\n
\n 
arg[1] 
\n
If this is the LdpOp operation (if x is a parameter),
i_vec is defined by
\verbatim
	i_vec = arg[1]
\endverbatim
If this is the LdvOp operation (if x is a variable), 
i_vec is defined by
\verbatim
	i_vec = floor( taylor[ arg[1] * nc_taylor + 0 ] )
\endverbatim
where floor(c) is the greatest integer less that or equal c.
\n
\n
arg[2]
Is the index of this vecad load instruction in the
element_by_load_op array.

\param num_par
is the total number of parameters on the tape
(only used for error checking).

\param parameter
If v[x] is a parameter, <code>parameter[ i_v_x ]</code> is its value.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\n
Input
\n
In LdvOp case, <code>taylor[ arg[1] * nc_taylor + 0 ]</code>
is used to compute the index in the definition of i_vec above.
If v[x] is a variable, <code>taylor[ i_v_x * nc_taylor + 0 ]</code>
is the zero order Taylor coefficient for v[x].
\n
\n
Output
\n
<code>taylor[ i_z * nc_taylor + 0 ]</code>
is set to the zero order Taylor coefficient for the variable z.

\param element_by_ind
\n
is_var
\n
If <code>element_by_ind[ arg[0] + i_vec ].is_var</code> is true,
v[x] is a variable.  Otherwise it is a parameter.
\n
\n
index
\n
<code>element_by_ind[ arg[0] - 1 ].index</code> 
is the number of elements in the user vector containing this element.

\param element_by_load_op
is a vector with size play->num_load_op_rec().
The input value of its elements does not matter.
Upon return,  it contains the variable index corresponding to each load 
instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\par Check User Errors
\li In the LdvOp case check that the index is with in range; i.e.
<code>i_vec < element_by_ind[ arg[0] - 1 ].index</code>. 
Note that, if x is a parameter, 
the corresponding vector index and it does not change.
In this case, the error above should be detected during tape recording.

\par Checked Assertions 
\li NumArg(LdpOp) == 3
\li NumRes(LdpOp) == 1
\li 0 <  arg[0]
\li i_vec < element_by_index[ arg[0] - 1 ].index 
\li if v[x] is a parameter, i_v_x < num_par
\li if v[x] is a variable, i_v_x < i_z
\li if x is a variable (LpvOp case), arg[1] < i_z
*/
template <class Base>
inline void forward_load_op_0(
	size_t         i_z         ,
	addr_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	pod_vector<vecad_element>& element_by_ind ,
	pod_vector<addr_t>&        element_by_load_op )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}
/*!
Shared documentation for sparsity operations corresponding to 
op = LdpOp or LdvOp (not called).

<!-- define sparse_load_op -->
The C++ source code corresponding to this operation is
\verbatim
	z = v[x]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 

\tparam Vector_set
is the type used for vectors of sets. It can be either
\c sparse_pack, \c sparse_set, or \c sparse_list.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param i_z
is the AD variable index corresponding to the variable z; i.e.,
the set with index \a i_z in \a var_sparsity is the sparsity pattern
correpsonding to z.

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the VecAD combined array.

\param num_combined
is the total number of elements in the VecAD combinded array.

\param combined
is the VecAD combined array.
\n
\n
\a combined[ \a arg[0] - 1 ]
is the index of the set corresponding to the vector v  in \a vecad_sparsity.
We use the notation i_v for this value; i.e.,
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set with index \a i_z in \a var_sparsity is the sparsity pattern for z.
This is an output for forward mode operations,
and an input for reverse mode operations.

\param vecad_sparsity
The set with index \a i_v is the sparsity pattern for the vector v.
This is an input for forward mode operations.
For reverse mode operations,
the sparsity pattern for z is added to the sparsity pattern for v.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li 0         <  \a arg[0]
\li \a arg[0] < \a num_combined
\li i_v       < \a vecad_sparsity.n_set()
<!-- end sparse_load_op -->
*/
template <class Vector_set>
inline void sparse_load_op(
	OpCode              op             ,
	size_t              i_z            ,
	const addr_t*        arg           , 
	size_t              num_combined   ,
	const size_t*       combined       ,
	Vector_set&         var_sparsity   ,
	Vector_set&         vecad_sparsity )
{
	// This routine is only for documentaiton, it should not be used
	CPPAD_ASSERT_UNKNOWN( false );
}


/*!
Zero order forward mode implementation of op = LdpOp.

\copydetails forward_load_op_0
*/
template <class Base>
inline void forward_load_p_op_0(
	size_t         i_z         ,
	addr_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	pod_vector<vecad_element>& element_by_ind     ,
	pod_vector<addr_t>&        element_by_load_op )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be
	// caught and reported to the user at an when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < element_by_ind[ arg[0] - 1 ].index );


	CPPAD_ASSERT_UNKNOWN( NumArg(LdpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdpOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );

	size_t i_v_x  = element_by_ind[ arg[0] + i_vec ].index;
	Base* z       = taylor + i_z * nc_taylor;
	if( element_by_ind[ arg[0] + i_vec ].is_var )
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < i_z );
		element_by_load_op[ arg[2] ] = i_v_x;
		Base* v_x = taylor + i_v_x * nc_taylor;
		z[0]      = v_x[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < num_par );
		element_by_load_op[ arg[2] ] = 0;
		Base v_x  = parameter[i_v_x];
		z[0]      = v_x;
	}
}

/*!
Zero order forward mode implementation of op = LdvOp.

\copydetails forward_load_op_0
*/
template <class Base>
inline void forward_load_v_op_0(
	size_t         i_z         ,
	addr_t*        arg         , 
	size_t         num_par     ,
	const Base*    parameter   ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	pod_vector<vecad_element>& element_by_ind     ,
	pod_vector<addr_t>&        element_by_load_op )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(LdvOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(LdvOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_z );

	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < element_by_ind[ arg[0] - 1 ].index ,
		"VecAD: index during zero order forward sweep is out of range"
	);

	size_t i_v_x  = element_by_ind[ arg[0] + i_vec ].index;	
	Base* z       = taylor + i_z * nc_taylor;
	if( element_by_ind[ arg[0] + i_vec ].is_var )
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < i_z );
		element_by_load_op[ arg[2] ] = i_v_x;
		Base* v_x = taylor + i_v_x * nc_taylor;
		z[0]      = v_x[0];
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( i_v_x < num_par );
		element_by_load_op[ arg[2] ] = 0;
		Base v_x  = parameter[i_v_x];
		z[0]      = v_x;
	}
}

/*!
Forward mode, except for zero order, for op = LdpOp or op = LdvOp

The C++ source code corresponding to this operation is
\verbatim
	z = v[x]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> or Base index. 

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param p
is the lowest order of the Taylor coefficient that we are computing.

\param q
is the highest order of the Taylor coefficient that we are computing.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
arg[2]
Is the index of this vecad load instruction in the
element_by_load_op array.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients.

\param taylor
\n
Input
\n
If v[x] is a variable, <code>taylor[ arg[2] * nc_taylor + k ]</code>
for k = 0 , ... , q,
is the k-order Taylor coefficient corresponding to v[x].
\n
\n
Output
\n
<code>taylor[ i_z * nc_taylor + d ]</code>
for k = p , ... , q,
is set to the k-order Taylor coefficient for the variable z.

\param element_by_load_op
is a vector with size play->num_load_op_rec().
It contains the variable index corresponding to each load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li q < nc_taylor
\li 0 < p <= q 
\li size_t(arg[2]) < i_z
*/
template <class Base>
inline void forward_load_op(
	OpCode         op          ,
	size_t         p           ,
	size_t         q           ,
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         nc_taylor   ,
	Base*          taylor      ,
	const pod_vector<addr_t>&  element_by_load_op )
{	size_t i_load = size_t( element_by_load_op[ arg[2] ] );

	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( q < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( 0 < p && p <= q );
	CPPAD_ASSERT_UNKNOWN( i_load < i_z );

	Base* z      = taylor + i_z * nc_taylor;
	if( i_load > 0 )
	{	Base* v_x = taylor + i_load * nc_taylor;
		for(size_t d = p; d <= q; d++)
			z[d] = v_x[d];
	}
	else
	{	for(size_t d = p; d <= q; d++)
			z[d] = Base(0);
	}
}

/*!
Reverse mode for op = LdpOp or LdvOp.

The C++ source code corresponding to this operation is
\verbatim
	z = y[x]
\endverbatim
where y is a VecAD<Base> vector and x is an AD<Base> or Base index. 

This routine is given the partial derivatives of a function 
G(z , y[x] , w , u ... )
and it uses them to compute the partial derivatives of 
\verbatim
	H( y[x] , w , u , ... ) = G[ z( y[x] ) , y[x] , w , u , ... ]
\endverbatim

\tparam Base
base type for the operator; i.e., this operation was recorded
using AD< \a Base > and computations by this routine are done using type 
\a Base.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param d
highest order the Taylor coefficient that we are computing the partial
derivative with respect to.

\param i_z
is the AD variable index corresponding to the variable z.

\param arg
\a arg[2]
Is the index of this vecad load instruction in the
element_by_load_op array.

\param nc_taylor
number of columns in the matrix containing the Taylor coefficients
(not used).

\param taylor
matrix of Taylor coefficients (not used).

\param nc_partial
number of colums in the matrix containing all the partial derivatives
(not used if \a arg[2] is zero).

\param partial
If \a arg[2] is zero, y[x] is a parameter
and no values need to be modified; i.e., \a partial is not used.
Otherwise, y[x] is a variable and:
\n
\n
\a partial [ \a i_z * \a nc_partial + k ] 
for k = 0 , ... , \a d
is the partial derivative of G
with respect to the k-th order Taylor coefficient for z.
\n
\n
If \a arg[2] is not zero,
\a partial [ \a arg[2] * \a nc_partial + k ]
for k = 0 , ... , \a d
is the partial derivative with respect to 
the k-th order Taylor coefficient for x.
On input, it corresponds to the function G,
and on output it corresponds to the the function H. 

\param element_by_load_op
is a vector with size play->num_load_op_rec().
It contains the variable index corresponding to each load instruction.
In the case where the index is zero,
the instruction corresponds to a parameter (not variable).

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li d < nc_taylor
\li size_t(arg[2]) < i_z
*/
template <class Base>
inline void reverse_load_op(
	OpCode         op          ,
	size_t         d           ,
	size_t         i_z         ,
	const addr_t*  arg         , 
	size_t         nc_taylor   ,
	const Base*    taylor      ,
	size_t         nc_partial  ,
	Base*          partial     ,
	const pod_vector<addr_t>& element_by_load_op )
{	size_t i_load = size_t( element_by_load_op[ arg[2] ] );

	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( d < nc_taylor );
	CPPAD_ASSERT_UNKNOWN( i_load < i_z );

	if( i_load > 0 )
	{
		Base* pz   = partial + i_z    * nc_partial;
		Base* py_x = partial + i_load * nc_partial;
		size_t j = d + 1;
		while(j--)
			py_x[j]   += pz[j];
	}
}


/*!
Forward mode sparsity operations for LdpOp and LdvOp

\copydetails sparse_load_op
*/
template <class Vector_set>
inline void forward_sparse_load_op(
	OpCode             op             ,
	size_t             i_z            ,
	const addr_t*      arg            , 
	size_t             num_combined   ,
	const size_t*      combined       ,
	Vector_set&        var_sparsity   ,
	Vector_set&        vecad_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

	var_sparsity.assignment(i_z, i_v, vecad_sparsity);

	return;
}


/*!
Reverse mode Jacobian sparsity operations for LdpOp and LdvOp

\copydetails sparse_load_op
*/
template <class Vector_set>
inline void reverse_sparse_jacobian_load_op(
	OpCode             op             ,
	size_t             i_z            ,
	const addr_t*      arg            , 
	size_t             num_combined   ,
	const size_t*      combined       ,
	Vector_set&        var_sparsity   ,
	Vector_set&        vecad_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

	vecad_sparsity.binary_union(i_v, i_v, i_z, var_sparsity);

	return;
}


/*!
Reverse mode Hessian sparsity operations for LdpOp and LdvOp

This routine is given the sparsity patterns for 
G(z , v[x] , w , u ... )
and it uses them to compute the sparsity patterns for
\verbatim
	H( v[x] , w , u , ... ) = G[ z( v[x] ) , v[x] , w , u , ... ]
\endverbatim

<!-- replace sparse_load_op -->
The C++ source code corresponding to this operation is
\verbatim
	z = v[x]
\endverbatim
where v is a VecAD<Base> vector and x is an AD<Base> index. 

\tparam Vector_set
is the type used for vectors of sets. It can be either
\c sparse_pack, \c sparse_set, or \c sparse_list.

\param op
is the code corresponding to this operator; i.e., LdpOp or LdvOp
(only used for error checking).

\param i_z
is the AD variable index corresponding to the variable z; i.e.,
the set with index \a i_z in \a var_sparsity is the sparsity pattern
correpsonding to z.

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the VecAD combined array.

\param num_combined
is the total number of elements in the VecAD combinded array.

\param combined
is the VecAD combined array.
\n
\n
\a combined[ \a arg[0] - 1 ]
is the index of the set corresponding to the vector v  in \a vecad_sparsity.
We use the notation i_v for this value; i.e.,
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param var_sparsity
The set with index \a i_z in \a var_sparsity is the sparsity pattern for z.
This is an output for forward mode operations,
and an input for reverse mode operations.

\param vecad_sparsity
The set with index \a i_v is the sparsity pattern for the vector v.
This is an input for forward mode operations.
For reverse mode operations,
the sparsity pattern for z is added to the sparsity pattern for v.

\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 1
\li 0         <  \a arg[0]
\li \a arg[0] < \a num_combined
\li i_v       < \a vecad_sparsity.n_set()
<!-- end sparse_load_op -->

\param var_jacobian
\a var_jacobian[i_z] 
is false (true) if the Jacobian of G with respect to z is always zero 
(many be non-zero).

\param vecad_jacobian
\a vecad_jacobian[i_v] 
is false (true) if the Jacobian with respect to x is always zero 
(may be non-zero).
On input, it corresponds to the function G,
and on output it corresponds to the function H.

*/
template <class Vector_set>
inline void reverse_sparse_hessian_load_op(
	OpCode             op             ,
	size_t             i_z            ,
	const addr_t*      arg            , 
	size_t             num_combined   ,
	const size_t*      combined       ,
	Vector_set&        var_sparsity   ,
	Vector_set&        vecad_sparsity ,
	bool*              var_jacobian   ,
	bool*              vecad_jacobian )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 1 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < vecad_sparsity.n_set() );

	vecad_sparsity.binary_union(i_v, i_v, i_z, var_sparsity);

	vecad_jacobian[i_v] |= var_jacobian[i_z];

	return;
}


/*! \} */
} // END_CPPAD_NAMESPACE
# endif
