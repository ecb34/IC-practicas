EXECS=rectampi
MPICC?=mpic++ -std=c++11

rectampi: recta_regresion_mpi.cpp
	${MPICC} -o rectampi recta_regresion_mpi.cpp

clean:
	rm ${EXECS}
