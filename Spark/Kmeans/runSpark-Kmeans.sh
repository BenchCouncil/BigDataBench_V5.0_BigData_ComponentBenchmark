##!/bin/bash

# Running command: ./runSpark-Kmeans.sh <size> <centerNum> <iterNum>  

centerNum=$2
iterNum=$3


#---------------------------------------------------------------run--------------------------------------------#
hadoop dfs -rmr output 
$SPARK_HOME/bin/spark-submit --class cn.ac.ict.bigdatabench.KMeans cc-kmeans/JAR_FILE/bigdatabench-spark_1.3.0-hadoop_1.0.4.jar /spark/kmeans/kmeans-${1}G $centerNum $iterNum

echo "spark kmeans end"
