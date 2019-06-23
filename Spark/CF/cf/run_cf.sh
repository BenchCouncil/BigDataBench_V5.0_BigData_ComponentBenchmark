#!/bin/bash
##
#CF(Collaborate Filtering, ALS)
#	run:
#	spark-submit --class cn.ac.ict.bigdatabench.ALS $JAR_FILE <ratings_file> <rank> <iterations> [|       <splits>]
#
#		parameters:
#		<ratings_file>: path of input data file
#		<rank>: number of features to train the model
#		<iterations>: number of iterations to run the algorithm
#		[<splits>]: optional, level of parallelism to split computation into
#
#		input data:
#			userID,productID,rating
#			for example:
#	        1,1,5
#           1,3,4
#           1,5,1
#           2,1,4
#           2,5,5
##

JAR_FILE=pre-build/bigdatabench-spark_1.3.0-hadoop_1.0.4.jar
RATE_FILE=data-generator/als_input.txt
rank=$1
iter=$2
spark-submit --class cn.ac.ict.bigdatabench.ALS $JAR_FILE $RATE_FILE $rank $iter
  

