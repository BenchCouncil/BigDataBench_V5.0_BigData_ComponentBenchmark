##!/bin/bash

##
# Parameter $I indicates that the vertex of the generated graph is 2^$I
# Generating command: ./genData-kmeans.sh <log_vertex> 
##
 
I=$1
cd ../../BigDataGeneratorSuite/Graph_datagen

rm -fr ./gen_data/Facebook_genGragh_$I.txt
./gen_kronecker_graph  -o:./gen_data/Facebook_genGragh_$I.txt -m:"0.9999 0.5887; 0.6254 0.3676" -i:$I
head -4 ./gen_data/Facebook_genGragh_$I.txt > ./gen_data/Facebook_parameters_$I
sed 1,4d ./gen_data/Facebook_genGragh_$I.txt > ./gen_data/Facebook_genGragh_$I.tmp
mv ./gen_data/Facebook_genGragh_$I.tmp ./gen_data/Facebook_genGragh_$I.txt
sed 's/[[:space:]][[:space:]]*/ /g' ./gen_data/Facebook_genGragh_$I.txt >./gen_data/testdata

${HADOOP_HOME}/bin/hadoop fs -rmr testdata
${HADOOP_HOME}/bin/hadoop fs -put ./gen_data/testdata

