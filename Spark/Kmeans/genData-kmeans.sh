##!/bin/bash

#
#Generating command: ./genData-kmeans.sh <size> #GB
#

curdir=`pwd`

cd cc-kmeans/Kmeans
make
rm -f kmeans-${1}G
chmod a+x genData_Kmeans.sh
./genData_Kmeans.sh $1

${HADOOP_HOME}/bin/hadoop fs -rmr /spark/kmeans/kmeans-${1}G
${HADOOP_HOME}/bin/hadoop fs -mkdir -p /spark/kmeans/
${HADOOP_HOME}/bin/hadoop fs -put kmeans-${1}G /spark/kmeans/
rm -f kmeans-${1}G
