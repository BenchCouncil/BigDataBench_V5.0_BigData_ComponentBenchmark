#!/bin/bash

# Running command: ./run-mpiLDA.sh <size>
#	size: the input data size, GB

rm -f ./model-out/lda_model.txt

#--------training--------
mpirun --allow-run-as-root -n 12 ./mpi_lda           \
  --num_topics 10 \
  --alpha 0.1    \
  --beta 0.01                                           \
  --training_data_file genData-mpiLDA/ldaData-$1"GB" \
  --model_file ./model-out/lda_model.txt    \
  --burn_in_iterations 10                              \
  --total_iterations 15
