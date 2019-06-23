rm -rf /mnt/sdh/PPM-100G.tar.gz

./genData_Kmeans.sh 150

hadoop fs -put /mnt/sdh/Kmeans-150G
