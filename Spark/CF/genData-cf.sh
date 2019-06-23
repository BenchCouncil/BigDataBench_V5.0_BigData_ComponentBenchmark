#!/bin/bash
  
#Command for generating data:
#       ./genData-cf.sh <size> #GB
#

a=`expr $1 \* 1024`

#-----------------generating input data---------------
rm -rf genData-CF/als_input.txt
$HADOOP_HOME/bin/hadoop dfs -rmr /spark/cf/cf-${1}G
cd genData-CF
make
./ALS-DataGen $a
$HADOOP_HOME/bin/hadoop fs -mkdir -p /spark/cf/
$HADOOP_HOME/bin/hadoop fs -put als_input.txt /spark/cf/cf-${1}G
$HADOOP_HOME/bin/hadoop dfs -rmr /spark/cf/cf-out /spark/cf/temp
rm -rf als_input.txt
