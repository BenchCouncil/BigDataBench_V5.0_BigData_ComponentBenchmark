#!/bin/bash

mpirun -f ../nodefile -n 12 ./mpiLDA-data test outfile
