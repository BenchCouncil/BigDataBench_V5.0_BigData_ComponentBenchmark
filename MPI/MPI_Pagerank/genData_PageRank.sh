#!/bin/bash
##

##

echo "Generate PageRank data"

WORK_DIR=`pwd`       

mkdir $WORK_DIR/data-PageRank
read -p "Please Enter The Iterations of GenGragh: " I

echo "WORK_DIR=$WORK_DIR data will be generated in $WORK_DIR/data-PageRank"

#/////////////////////////////////////////////////////////////////////////////
#Parameters:
# -o:Output graph file name (default:'graph.txt')
# -m:Matrix (in Maltab notation) (default:'0.9 0.5; 0.5 0.1')
# -i:Iterations of Kronecker product (default:5)
# -s:Random seed (0 - time seed) (default:0)
#/////////////////////////////////////////////////////////////////////////////

../../BigDataGeneratorSuite/Graph_datagen/gen_kronecker_graph  -o:$WORK_DIR/data-PageRank/Google_genGraph_$I.txt -m:"0.8305 0.5573; 0.4638 0.3021" -i:$I





