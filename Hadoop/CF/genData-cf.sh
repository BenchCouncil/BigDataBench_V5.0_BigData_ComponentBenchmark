#!/bin/bash

#Command for generating data:
# 	./genData-cf.sh <size> #GB
#
  
a=`expr $1 \* 1024`

#-----------------generating input data---------------
rm -rf genData-CF/als_input.txt
$HADOOP_HOME/bin/hadoop dfs -rmr /hadoop/cf/cf-${1}G
cd genData-CF
make
./ALS-DataGen $a
$HADOOP_HOME/bin/hadoop fs -mkdir -p /hadoop/cf/
$HADOOP_HOME/bin/hadoop fs -put als_input.txt /hadoop/cf/cf-${1}G
$HADOOP_HOME/bin/hadoop dfs -rmr /hadoop/cf/cf-out /hadoop/cf/temp

