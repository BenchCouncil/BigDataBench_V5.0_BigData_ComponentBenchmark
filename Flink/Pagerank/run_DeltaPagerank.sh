#!/bin/bash
. ../../config.sh
${HADOOP_HOME}/bin/hadoop fs -rmr /flink-pagerank-result
echo "Internation I:"
read I
#iterations
m=20
$FLINK_HOME/bin/flink run --class cn.ac.ict.bigdatabench.DeltaPageRank $JAR_FILE $HDFS/data-PageRank/Google_genGraph_$I.txt $m $HDFS/flink-pagerank-result
