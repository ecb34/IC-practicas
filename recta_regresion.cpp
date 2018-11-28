#include<iostream>
using namespace std;

float Media(float array[], int n)
{
  float resultado = 0;
  for (int i = 0; i < n; i++)
  {
    resultado += array[i];
  }
  resultado = resultado / n;
  return resultado;
}

float Varianza(float array[], int n)
{
  float resultado = 0;
  for (int i = 0; i < n; i++)
  {
    resultado += (array[i] * array[i]);
  }
  resultado = (resultado / n) - Media(array, n);
  return resultado;
}

float Covarianza(float x[],float y[], int n)
{
  float resultado = 0;

  for (int i = 0; i < n; i++)
  {
    resultado += (x[i] * y[i]);
  }
  resultado = (resultado / n) - (Media(x, n) * Media(y, n));
  return resultado;
}

void Recta_Regresion_X_sobre_Y(float x[], float y[],int n, float &m, float &t)
{
  m = Covarianza(x, y, n)/Varianza(x,n);
  t = (Media(x, n) - Media(y, n));
}

void Recta_Regresion_Y_sobre_X(float x[], float y[], int n, float &m, float &t)
{
  m = Covarianza(x, y, n) / Varianza(y,n);
  t = (Media(y, n) - Media(x, n));
}

int main()
{
 int n = 0;
 clock_t tiempo_inicialC, tiempo_finalC;
 clock_t tiempo_finalens, tiempo_inicialens;
 double secs = 0;

 n = 10000;

 float *x = new float[n];
 float*y = new float[n];

 srand(time(NULL));
 for (int i = 0; i < n; i++) {
  x[i] = rand() % 50+1;
  y[i] = rand() % 50+1;
 }

 float Mxy = 0, Txy = 0, Myx = 0, Tyx = 0;
 float Mensxy = 0, Tensxy = 0, Mensyx = 0, Tensyx = 0;

 tiempo_inicialC = clock();
 Recta_Regresion_X_sobre_Y(x, y, n, Mxy, Txy);
 tiempo_finalC = clock();
 secs = (double)(tiempo_finalC - tiempo_inicialC);

 Recta_Regresion_Y_sobre_X(x, y, n, Myx, Tyx);

 if (Txy < 0)
 {
  cout << "X = " << Mxy << "Y " << Txy << endl;
 }
 else if(Txy == 0)
 {
  cout << "X = " << Mxy << "Y" << endl;
 }
 else
 {
  cout << "X = " << Mxy << "Y + " << Txy << endl;
 }

 if (Tyx < 0)
 {
  cout << "Y = " << Myx << "X " << Tyx << endl;
 }
 else if (Tyx == 0)
 {
  cout << "Y = " << Myx << "X" << endl;
 }
 else
 {
  cout << "Y = " << Myx << "X + " << Tyx << endl;
 }
 cout << "tiempo (ms) : " << secs << endl;

 return 0;
}
