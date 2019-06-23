#!/bin/bash

# Generating command: ./genData-mpiLDA.sh <size>
#	size: the input data size, GB

#----------------------------genenrate-data----------------------------#
curdir=`pwd`
a=$1
let L=a*2
cd ../../BigDataGeneratorSuite/Text_datagen/
rm -fr $curdir/genData-mpiLDA/wiki-$a"GB"
./gen_text_data.sh lda_wiki1w $L 8000 10000 $curdir/genData-mpiLDA/wiki-$a"GB"
cd $curdir/genData-mpiLDA/wiki-$a"GB"
n=`ls -l | grep "^-" | wc -l`
cd ..
rm -f lda-tmp
for ((i=1; i<=$n; i ++))
do
cat wiki-$a"GB"/lda_wiki1w_$i >> lda-tmp
done
make
echo "Generating the lda data input..."
mpirun --allow-run-as-root -n 12 ./mpiLDA-data lda-tmp ldaData-$a"GB" 
rm -f lda-tmp wiki-$a"GB"
echo "Generating Complete, the data is genData-mpiLDA/ldaData-$a'GB'."
