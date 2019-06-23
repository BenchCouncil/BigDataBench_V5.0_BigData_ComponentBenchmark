#!/bin/bash

# Generating command: ./genData-lda.sh <size> #GB
  
curdir=`pwd`
#------------generate-data---------------------
a=$1

cd ../../BigDataGeneratorSuite/Text_datagen

let L=a*2
rm -fr ./gen_data/wiki-$a"G"
./gen_text_data.sh lda_wiki1w $L 8000 10000 ./gen_data/wiki-$a"G"
${HADOOP_HOME}/bin/hadoop fs -rmr /spark/lda/*
${HADOOP_HOME}/bin/hadoop fs -mkdir -p /spark/lda/
${HADOOP_HOME}/bin/hadoop fs -put ./gen_data/wiki-$a"G" /spark/lda/

$SPARK_HOME/bin/spark-submit --class com.LDAInput.WikiToLDAInput lda-data-spark/target/LDAInput-0.0.1-SNAPSHOT.jar /spark/lda/wiki-$a"G" /spark/lda/dictionary /spark/lda/corpus
hadoop fs -get /spark/lda/dictionary $curdir/
rm -rf ./gen_data/wiki-$a"G"
