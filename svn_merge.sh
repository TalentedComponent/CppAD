# ! /bin/bash
#
# Merge the changes that occurred in FromDirectory into the ToDirectory
#
DryRun="--dry-run"
if [ "$1" == "not--dry-run" ]
then
	DryRun=""
fi
echo "DryRun = $DryRun"
#
# script to help in execution of svn merge command
#
# Change into the directory corresponding to the entire repository; i.e.,
# http://www.coin-or.org/svn/cppad/ 
cd ..
#
# Name of the directory that will receive the changes
ToDirectory=trunk
#
# Name of the directory where the changes have been committed
FromDirectory=branches/copyright
#
# Version of the repository corresponding to FromDirectory just before changes
Start=462
# 
# Version of the repository corresponding to FromDirecrtory after the changes
End=529
#
# echo the svn merge command
echo "cd  .."
echo "svn merge $DryRun -r $Start:$End                    \\"
echo "	  http://www.coin-or.org/svn/cppad/$FromDirectory \\"
echo "	  $ToDirectory"
#
# execute the svn merge command
svn merge $DryRun -r $Start:$End                         \
    http://www.coin-or.org/svn/cppad/$FromDirectory      \
    $ToDirectory 
#
# put comment here so have newline at end of command above