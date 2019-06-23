#!/bin/bash

##
# Parameter $I indicates that the vertex of the generated graph is 2^$I
# Running command: ./runSpark-PageRank.sh <log_vertex> 
##
  
curdir=`pwd`
I=$1

#--------------------------------------------run----------------------------#
${HADOOP_HOME}/bin/hadoop fs -rmr /spark/pagerank/output
$SPARK_HOME/bin/spark-submit --class cn.ac.ict.bigdatabench.PageRank pagerank/JAR_FILE/bigdatabench-spark_1.3.0-hadoop_1.0.4.jar /spark/pagerank/Google_genGraph_$I.txt $I /spark/pagerank/output

echo "spark pagerank end"
