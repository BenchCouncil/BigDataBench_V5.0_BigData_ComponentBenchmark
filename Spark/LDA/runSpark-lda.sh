#!/bin/bash
  
# Running command: ./runSpark-lda.sh

sparkenv=`grep "SPARK_MASTER_IP=" $SPARK_HOME/conf/spark-env.sh | awk '{print $NF}'`
master=${sparkenv:16}
echo $master

#--------------------------------------run-workload-----------------------#
$SPARK_HOME/bin/spark-submit --class org.apache.spark.graph.algorithms.TopicModeling TopicModel/target/TopicModel-0.0.1-SNAPSHOT.jar spark://$master:7077 -tokens=hdfs://$master:9000/spark/lda/corpus -dictionary=dictionary/part-00000 -ntopics=10 -niter=5

echo "spark lda end"

rm -rf dictionary

