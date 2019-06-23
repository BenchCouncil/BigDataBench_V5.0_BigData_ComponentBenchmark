#!/bin/bash
echo "number of centers K:"
read K
echo "max_iterations m:"
read m
$FLINK_HOME/bin/flink run --class cn.ac.ict.bigdatabench.BasicKMeans ../JAR_FILE/bigdatabench-flink-0.10.0.jar $HDFS/Flink-Kmeans/data-Kmeans ${K} ${m}
