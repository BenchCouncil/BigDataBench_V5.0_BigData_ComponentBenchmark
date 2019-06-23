rm -f result
./get_perf_new.py result 1 &
mpirun -np 4 ./generator_test_mpi 20
killall -9 get_perf_new.py
./get_AI_new.py result
