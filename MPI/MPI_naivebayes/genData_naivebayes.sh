#/bin/bash
##
# Use the amazon naivebayes dataset, trains and tests a classifier.
# Need HADOOP and MAHOUT
# To prepare and generate data:
# ./genData_naivebayes.sh
# To run:
# ./run_naivebayes.sh
##

echo "Preparing naivebayes-naivebayes data dir"

#read -p "Please Enter WORK_DIR: " WORK_DIR
WORK_DIR=`pwd`
echo "WORK_DIR=$WORK_DIR data will be generated in $WORK_DIR/data-naivebayes"
#rm -rf ${WORK_DIR}/data-naivebayes
#mkdir ${WORK_DIR}/data-naivebayes

echo "Preparing naivebayes-naivebayes data dir"
mkdir -p ${WORK_DIR}/data-naivebayes/amazonMR1
mkdir -p ${WORK_DIR}/data-naivebayes/amazonMR2
mkdir -p ${WORK_DIR}/data-naivebayes/amazonMR3
mkdir -p ${WORK_DIR}/data-naivebayes/amazonMR4
mkdir -p ${WORK_DIR}/data-naivebayes/amazonMR5

cd ../../BigDataGeneratorSuite/Text_datagen/

echo "print data size GB :"
read GB
a=${GB}
let L=a*2
./gen_text_data.sh amazonMR1 $L 1900 11500 ${WORK_DIR}/data-naivebayes/amazonMR1
./gen_text_data.sh amazonMR2 $L 1900 11500 ${WORK_DIR}/data-naivebayes/amazonMR2
./gen_text_data.sh amazonMR3 $L 1900 11500 ${WORK_DIR}/data-naivebayes/amazonMR3
./gen_text_data.sh amazonMR4 $L 1900 11500 ${WORK_DIR}/data-naivebayes/amazonMR4
./gen_text_data.sh amazonMR5 $L 1900 11500 ${WORK_DIR}/data-naivebayes/amazonMR5

cd ${WORK_DIR}/data-naivebayes
python dataread.py $L 47

mv testdata ../bayes-data$a"G"

echo "Finished copyFromLocal"
