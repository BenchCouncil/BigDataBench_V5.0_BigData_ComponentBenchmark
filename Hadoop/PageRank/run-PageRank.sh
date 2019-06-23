#!/bin/bash

##
# Parameter $I indicates that the vertex of the generated graph is 2^$I
# Running command: ./run-pagerank.sh <log_vertex> 
##
  
I=$1

${HADOOP_HOME}/bin/hadoop fs -rmr /hadoop/pagerank/prtemp /hadoop/pagerank/output pr_distr pr_minmax pr_vector
${HADOOP_HOME}/bin/hadoop jar pegasus-2.0.jar pegasus.PagerankNaive /hadoop/pagerank/Google_genGraph_$I.txt /hadoop/pagerank/prtemp /hadoop/pagerank/output 450 4 3 nosym new

echo "hadoop pagerank end"
