#!/bin/bash
##prepare 
source conf.properties
##prepare txt
cd $BigdataBench_Home/BigDataGeneratorSuite/Text_datagen
tar -zxvf gsl-1.15.tar.gz 
cd gsl-1.15  
./configure
make
make install
 
