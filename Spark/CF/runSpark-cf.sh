#!/bin/bash

##
#CF(Collaborate Filtering, ALS)
#       run:
#       spark-submit --class cn.ac.ict.bigdatabench.ALS $JAR_FILE <ratings_file> <rank> <iterations> [|       <splits>]
#
#               parameters:
#               <ratings_file>: path of input data file
#               <rank>: number of features to train the model
#               <iterations>: number of iterations to run the algorithm
#               [<splits>]: optional, level of parallelism to split computation into
#
#               input data:
#                       userID,productID,rating
#                       for example:
#               1,1,5
#               1,3,4
##

curdir=`pwd`
ratings_file=/spark/cf/cf-${1}G
rank=$2
iter=$3
#-----------------running hadoop cf-------------
$SPARK_HOME/bin/spark-submit --class cn.ac.ict.bigdatabench.ALS cf/pre-build/bigdatabench-spark_1.3.0-hadoop_1.0.4.jar $ratings_file $rank $iter 

echo "spark cf end"
