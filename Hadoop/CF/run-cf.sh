#!/bin/bash

#Run command: ./run-cf.sh <size> <numFeatures> <numIterations> <lambda>
#	size: the input data size, GB
#	numFeatures: the number of features
#	numIterations: the number of features
#	lambda: regularization parameter

#-----------------running hadoop cf-------------
${MAHOUT_HOME}/bin/mahout parallelALS \
        -i /hadoop/cf/cf-${1}G \
        -o /hadoop/cf/cf-out \
        --numFeatures $2 \
        --numIterations $3 \
        --lambda $4 \
        --tempDir /hadoop/cf/temp

#-----------------killing monitor script--------------
echo "hadoop cf end"
