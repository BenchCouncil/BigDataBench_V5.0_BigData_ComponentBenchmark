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
rm -fr Kmeans-$1G

echo "print data size GB :"
read GB
a=${GB}
let L=$a*115000
./Generating Image_data/color100.txt $L > Kmeans-$a"G"

echo "Finished copyFromLocal"


