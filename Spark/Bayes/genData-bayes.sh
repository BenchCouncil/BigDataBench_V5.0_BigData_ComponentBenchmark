#!/bin/bash

##
#Generating command: ./genData-bayes.sh <size>
#	size: the data size of generated data, GB
##

curdir=`pwd`
#------------generate-data---------------------
a=$1
cd ../../BigDataGeneratorSuite/Text_datagen

rm -rf ./gen_data/data-naivebayes

    let L=a*2
    ./gen_text_data.sh amazonMR1 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR1
    ./gen_text_data.sh amazonMR2 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR2
    ./gen_text_data.sh amazonMR3 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR3
    ./gen_text_data.sh amazonMR4 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR4
    ./gen_text_data.sh amazonMR5 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR5

    cp $curdir/datareadyuce.py ./gen_data/data-naivebayes
    cd gen_data/data-naivebayes
    #python dataread.py $1 $2  you can change the number 
    #$1:A number of storage folder
    #$2:From the first few rows stored
    #$1 ad $2 based on amazonMR
    python datareadyuce.py 

    ${HADOOP_HOME}/bin/hadoop fs -rmr /spark/Bayes/*
    ${HADOOP_HOME}/bin/hadoop fs -mkdir -p /spark/Bayes
    ${HADOOP_HOME}/bin/hadoop fs -put $curdir/model /spark/Bayes/
    ${HADOOP_HOME}/bin/hadoop fs -put testdata /spark/Bayes/bayes-$a'G'
#rm -rf ../data-naivebayes 
