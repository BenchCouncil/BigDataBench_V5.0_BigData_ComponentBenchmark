##!/bin/bash

##
# Kmeans running command:
# ./run-Kmeans.sh <t1> <t2> <cd> <x>
# 	t1: T1 threshold value (0-1), such as 0.4
#	t2: T2 threshold value (0-1), such as 0.1
#	cd: The convergence delta value (0-1), such as 0.1
#	x: The max iteration number
##
  
#---------------------------------------------------------------run--------------------------------------------#
$HADOOP_HOME/bin/hadoop dfs -rmr output 
${MAHOUT_HOME}/bin/mahout org.apache.mahout.clustering.syntheticcontrol.kmeans.Job \
        -i testdata \
        -o output \
        -dm org.apache.mahout.common.distance.CosineDistanceMeasure \
        -t1 $1 \
        -t2 $2 \
        -cd $3 \
        -x $4 \


#-----------------killing monitor script--------------
echo "hadoop kmeans end"
