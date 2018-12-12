# IC-practica-4

1.generar la clave con ssh-keygen -t rsa en la maquina maestra. 

2.pasar la clave publica /.ssh/id_rsa.pub a todas las maquinas esclavas por pendrive

3.cambiar el nombre de id_rsa.pub a authorized_keys y copiarlo a .ssh

4.tener en todas las máquinas el programa recta_rpi

5.compilar con el makefile en todas las maquinas en la misma carpeta (comando usado mpic++ exec recta_rpi.cpp)

6.crear el archivo hosts que contenga el nombre/ip de todas las máquinas.

7.ejecutar en la maquina maestra mpirun -mca plm_rsh_no_tree_spawn 1 -hostfile hosts -n 4 ./exec
