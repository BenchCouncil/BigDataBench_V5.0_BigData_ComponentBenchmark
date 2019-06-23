#!/bin/bash
source /etc/profile
cd $BigdataBench_HomeHP/BigDataGeneratorSuite/Text_datagen/
HaHa
rm -fr ./gen_data/data-naivebayes
let L=a*2
./gen_text_data.sh amazonMR1 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR1
./gen_text_data.sh amazonMR2 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR2
./gen_text_data.sh amazonMR3 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR3
./gen_text_data.sh amazonMR4 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR4
./gen_text_data.sh amazonMR5 $L 1900 11500 ./gen_data/data-naivebayes/amazonMR5
cd ./gen_data/data-naivebayes/
rename amazon `hostname` amazon*
cd -
${HADOOP_HOME}/bin/hadoop fs -rmr /hadoop/Bayes
${HADOOP_HOME}/bin/hadoop fs -mkdir -p /hadoop/Bayes
${HADOOP_HOME}/bin/hadoop fs -put ./gen_data/data-naivebayes /hadoop/Bayes

