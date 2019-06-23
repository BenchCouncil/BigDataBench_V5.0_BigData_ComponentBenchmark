#!/bin/bash
##
# Use the WIKI dataset, trains and tests a cluster.
# Need HADOOP and MAHOUT
# To prepare and generate data:
# ./genData_Kmeans.sh
# To run:
# ./run_Kmeans.sh
##

echo "Preparing Kmeans data dir"
rm -fr data-Kmeans

echo "print data size GB :"
read GB
a=${GB}
let L=a*115000
./Generating Image_data/color100.txt $L > data-Kmeans

${HADOOP_HOME}/bin/hadoop fs -rmr /Flink-Kmeans
${HADOOP_HOME}/bin/hadoop fs -mkdir /Flink-Kmeans
${HADOOP_HOME}/bin/hadoop fs -copyFromLocal data-Kmeans /Flink-Kmeans
echo "Finished copyFromLocal"


