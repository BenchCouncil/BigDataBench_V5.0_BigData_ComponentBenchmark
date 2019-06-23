#!/bin/bash

#Run command: ./runSpark-bayes.sh <size>
#	size: the input data size, GB
  
a=$1

#--------------------------------------run-workload-----------------------#
$SPARK_HOME/bin/spark-submit --class cn.ac.ict.bigdatabench.NaiveBayesClassifier bigdatabench-spark_1.3.0-hadoop_1.0.4.jar /spark/Bayes/bayes-${a}G /spark/Bayes/model /spark/Bayes/output

echo "spark bayes end"
