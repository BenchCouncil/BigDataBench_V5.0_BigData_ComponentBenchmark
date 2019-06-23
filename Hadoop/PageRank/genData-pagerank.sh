#!/bin/bash

##
# Parameter $I indicates that the vertex of the generated graph is 2^$I
# Generating command: ./genData-pagerank.sh <log_vertex> 
##
  
#----------------------------genenrate-data----------------------------#
I=$1
cd ../../BigDataGeneratorSuite/Graph_datagen
dir=/hadoop/pagerank

rm -fr ./gen_data/Google_genGraph_$I.txt
./gen_kronecker_graph  -o:./gen_data/Google_genGraph_$I.txt -m:"0.8305 0.5573; 0.4638 0.3021" -i:$I
head ./gen_data/Google_genGraph_$I.txt > ./gen_data/Google_parameters_$I
sed 1,4d ./gen_data/Google_genGraph_$I.txt > ./gen_data/Google_genGraph_$I.tmp

mv ./gen_data/Google_genGraph_$I.tmp ./gen_data/Google_genGraph_$I.txt

${HADOOP_HOME}/bin/hadoop fs -rmr /hadoop/pagerank/Google_genGraph_$I.txt
${HADOOP_HOME}/bin/hadoop fs -mkdir -p /hadoop/pagerank
${HADOOP_HOME}/bin/hadoop fs -put ./gen_data/Google_genGraph_$I.txt /hadoop/pagerank

