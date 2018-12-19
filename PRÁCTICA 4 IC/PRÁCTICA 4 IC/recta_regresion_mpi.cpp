#include <chrono>
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <thread>
using namespace std;
using namespace std::chrono;

static int rango;
static int p;

//Se encarga de repartir las porciones de trabajo entre procesadores
void para_range(int n1, int n2, int &nprocs, int &irank, int &ista, int &iend) {
 int iwork1 = (n2 - n1 + 1) / nprocs;
 int iwork2 = ((n2 - n1 + 1) % nprocs);
 ista = irank * iwork1 + n1 + min(irank, iwork2);
 iend = iwork2 > irank ? ista + iwork1 : ista + iwork1 - 1;
}


float inline myFor(float *a1, float *a2, int n) {
  float resultado = 0;

  for (int i = 0; i <= n; i++) {
    resultado += a1[i] * a2[i]; // CALCULO DE MAYOR PESO!!
  }

  return resultado;
}

float inline Media(float array[], int n) {
  float resultado = 0;

  for (int i = 0; i <= n; i++) {
    resultado += array[i]; // CALCULO DE PESO GRANDE
  }

  return resultado/n;
}


float inline myForPar(float *a1, float *a2, int n) {
  float resultado = 0;
  float resultadoGlobal = 0;
  int i, ista, iend;
  
  para_range(1,n,p,rango,ista,iend);
  //Cada procesador calculará en resultado su porción de iteraciones
  for (i = ista; i <= iend; i++) {
    resultado += a1[i] * a2[i];
  }
  
  //Con MPI_Reduce sumamos resultado de cada rango en resultadoGlobal
  MPI_Reduce(&resultado,&resultadoGlobal,1,MPI_INTEGER,MPI_SUM,0,MPI_COMM_WORLD);

  return resultadoGlobal;
}

//MediaPar tiene repartido el trabajo igual que resultadoGlobal
float inline MediaPar(float array[], int n) {
  float resultado = 0;
  float resultadoGlobal = 0;
  int i, ista, iend;
  
  para_range(1,n,p,rango,ista,iend);
  //Cada procesador calculará en resultado su porción de iteraciones
  for (i = ista; i <= iend; i++) {
    resultado += array[i]; // CALCULO DE PESO GRANDE
  }

  MPI_Reduce(&resultado,&resultadoGlobal,1,MPI_INTEGER,MPI_SUM,0,MPI_COMM_WORLD);

  return resultadoGlobal/n;
}

void inline Recta_Regresion_X_sobre_Y(float *x, float *y, int n, float &m, float &t) {
  float mx, my, dataCov, dataVar;

  // Calculo de medias
  mx = MediaPar(x, n);
  my = MediaPar(y, n);

  // Calculo de datos para Covarianza y Varianza
  dataCov = myForPar(x, y, n);
  dataVar = myForPar(x, x, n);

  // Obtebemos los resultados deseados
  t = mx - my;
  // COVARIANZA/VARIANZA
  m = dataCov - (mx * my) / (dataVar - mx);
}

void inline Recta_Regresion_Y_sobre_X(float x[], float y[], int n, float &m, float &t) {
  float mx, my, dataCov, dataVar;

  mx = Media(x, n);
  my = Media(y, n);

  // Calculo de datos para Covarianza y Varianza
  dataCov = myFor(x, y, n);
  dataVar = myFor(y, y, n);

  // Obtebemos los resultados deseados
  t = my - mx;
  // COVARIANZA/VARIANZA
  m = dataCov - (mx * my) / (dataVar - my);
}

int main() {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);

 // RANGO DE PROCESOS
  MPI_Comm_rank(MPI_COMM_WORLD, &rango);

  // NUMERO DE PROCESOS
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  const int MAX = 1459938304;
  int n = 0;
  typedef std::chrono::duration<int> seconds_type;
  typedef std::chrono::duration<int, std::milli> milliseconds_type;

  // TOPE CON LA RAM PARA 12 GB (llega a 11 GB)
  // n = (MAX/1.5)*1.3;

  // TOPE CON LA RAM PARA 8 GB (llega a 6.6 GB)
  // n = (MAX/1.5)*0.7;

  // OTROS
  // n = MAX*0.95;
  // n = MAX*0.5;
  // n = MAX*0.1;

  // DOS GIGAS EN USO
  // TOPE CON LA RAM PARA 8 GB (llega a 3.4 GB) 2x3.4 => 7GB
  //n = MAX * 0.2;

   // TOPE CON LA RAM PARA 8 GB (llega a 2.95 GB) 2x2.95=> 5.9GB
   //n = MAX * 0.15;

  // TOPE CON LA RAM PARA 8 GB (llega a 2.4 GB) 2x2.4 => 4.8GB
  //n = MAX * 0.10;

  // TOPE CON LA RAM PARA 8 GB (llega a 1.9 GB) 2x1.9 => 3.8GB
  n = MAX * 0.05;

  // DESPRECIABLE
  // n = 200000;
  float *x = new float[n];
  float *y = new float[n];

  srand(n);
  for (int i = 0; i < n; i++) {
    x[i] = rand() % 50 + 1;
    y[i] = rand() % 50 + 1;
  }

  float Mxy = 0, Txy = 0, Myx = 0, Tyx = 0;
  float Mensxy = 0, Tensxy = 0, Mensyx = 0, Tensyx = 0;

  if(rango==0)
  cout << "INIT" << endl <<
          "Paralelo..." << endl;

  high_resolution_clock::time_point t1 = high_resolution_clock::now();
  Recta_Regresion_X_sobre_Y(x, y, n, Mxy, Txy);
  high_resolution_clock::time_point t2 = high_resolution_clock::now();
  duration<double> secsparal = duration_cast<duration<double>>(t2 - t1);


  if(rango==0)
  {
  cout << "Secuencial..." << endl;
  t1 = high_resolution_clock::now();
  Recta_Regresion_Y_sobre_X(x, y, n, Myx, Tyx);
  t2 = high_resolution_clock::now();
  duration<double> secsnormal = duration_cast<duration<double>>(t2 - t1);

  if (Tyx < 0) {
    cout << "X = " << Myx << "Y " << Tyx << endl;
  } else if (Tyx == 0) {
    cout << "X = " << Myx << "Y" << endl;
  } else {
    cout << "X = " << Myx << "Y + " << Tyx << endl;
  }

  if (Tyx < 0) {
    cout << "Y = " << Myx << "X " << Tyx << endl;
  } else if (Tyx == 0) {
    cout << "Y = " << Myx << "X" << endl;
  } else {
    cout << "Y = " << Myx << "X + " << Tyx << endl;
  }

  int procesadores = 4; // A CAMBIAR, MPI PROCESADORES
  float ganancia = secsnormal.count() / secsparal.count() - 1;
  float eficiencia = ganancia / procesadores;

  cout << "\n=====================================\n";
  cout << "==============RESULTADO==============\n";
  cout << "=====================================\n";
  cout << "Tiempo paralelo: " << secsparal.count() << " seconds." << endl;
  cout << "Tiempo normal: " << secsnormal.count() << " seconds." << endl;
  cout << "Ganancia: x" << ganancia << "  || Eficiencia: " << eficiencia
       << endl;
  cout << "=====================================\n";
  }

  MPI_Finalize();
  
  return 0;
}
