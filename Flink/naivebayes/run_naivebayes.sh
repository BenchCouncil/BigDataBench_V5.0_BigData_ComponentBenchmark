${HADOOP_HOME}/bin/hadoop fs -rmr /flink-Bayes-result
$FLINK_HOME/bin/flink run --class cn.ac.ict.bigdatabench.Classifier ../JAR_FILE/bigdatabench-flink-0.10.0.jar $HDFS/Bayesclassifier/testdata $HDFS/flink-Bayes-result
