#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
if [ ! -e "bin/search.sh" ]
then
	echo "bin/search.sh: must be executed from its parent directory"
	exit 1
fi
if [ "$1" == "" ]
then
	echo 'usage: bin/search.sh pattern'
	echo '       Search for pattern in the cppad source code'
	exit 1
fi
pattern="$1"
extensions='.ac .am .c .cmake .cpp .h .hpp .in .omh .pc .py .sed .sh .txt'
list='
	bin
	cppad 
	cppad_ipopt
	example 
	introduction
	multi_thread
	omh
	pkgconfig
	print_for
	speed
	test_more 
'
dir_list=''
for dir in $list
do
	sub_list=`find $dir -type d | sed -e '/\/new$/d'`
	dir_list="$dir_list $sub_list"
done
#
find_files.sh "$pattern" "$extensions" "$dir_list" | \
	 sed -e '/\/makefile.in/d'
