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
rm -fr kmeans-${1}G

a=$1
let L=a*115000
./Generating Image_data/color100.txt $L > kmeans-${1}G

echo "Finished copyFromLocal"


