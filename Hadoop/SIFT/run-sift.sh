#!/bin/bash

##
#Running command: ./run-sift.sh <imgsize>
#	imgsize: the total size of the images, GB
##

#----------------check whether opencv is installed--------#  
isopencv=`pkg-config --modversion opencv`
strB="Package opencv was not found"
result=$(echo $isopencv | grep "${strB}")
echo $result
if [[ $result != "" ]];then
echo "no opencv"
exit 1
fi

if [[ $1 -ge 10 ]];then
a=10
else
a=1
fi

#-----------------generating input data---------------
$HADOOP_HOME/bin/hadoop dfs -rmr /hadoop/sift/data/image${a}G.*
$HADOOP_HOME/bin/hadoop fs -mkdir -p /hadoop/sift/data/
$HADOOP_HOME/bin/hadoop dfs -rmr /hadoop/sift/sift-out
$HADOOP_HOME/bin/hadoop fs -put hadoop-SIFT/data/image${a}G.* /hadoop/sift/data/



#-----------------running hadoop sift-------------
$HADOOP_HOME/bin/hadoop jar hadoop-SIFT/hipi-SIFT/tools/sift/build/libs/sift.jar /hadoop/sift/data/image${a}G.hib /hadoop/sift/sift-out

echo "hadoop sift end"
