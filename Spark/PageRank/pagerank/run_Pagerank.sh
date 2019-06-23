#!/bin/bash
${HADOOP_HOME}/bin/hadoop fs -rmr /spark-pagerank-result
echo "Internation I:"
read I
spark-submit --class cn.ac.ict.bigdatabench.PageRank $JAR_FILE /data-PageRank/Google_genGraph_$I.txt $I /spark-pagerank-result
