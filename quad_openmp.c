# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <time.h>
# include <omp.h>

int main ( int argc, char *argv[] );
double f ( double x );
double cpu_time ( void );
void timestamp ( void );

int main ( int argc, char *argv[] )
{
  double a = 0.0;
  double b = 10.0;
  double error;
  double exact = 0.49936338107645674464;
  int i;
  int n = 10000000;
  double total;
  double wtime;
  double x;

  timestamp ( );
  printf ( "\n" );
  printf ( "QUAD_OPENMP:\n" );
  printf ( "  Integral de f(x) = 50 / ( pi * ( 2500 * x * x + 1 ) ).\n" );
  printf ( "\n" );
  printf ( "  A(inicio)  = %f\n", a );
  printf ( "  B(fin)     = %f\n", b );
  printf ( "  N          = %d\n", n );
  printf ( "  Valor exacto = %24.16f\n", exact );

  wtime = omp_get_wtime ( );

  total = 0.0;

# pragma omp parallel shared ( a, b, n ) private ( i, x ) 
# pragma omp for reduction ( + : total )
  for ( i = 0; i < n; i++ ){
    x = ( ( double ) ( n - i - 1 ) * a + ( double ) ( i ) * b ) / ( double ) ( n - 1 );
    total = total + f ( x );
  }

  wtime = omp_get_wtime ( ) - wtime;

  total = ( b - a ) * total / ( double ) n;
  error = fabs ( total - exact );

  printf ( "\n" );
  printf ( " Valor estimado = %24.16f\n", total );
  printf ( "  Error    = %e\n", error );
  printf ( "  Tiempo     = %f\n", wtime );
  printf ( "\n" );
  timestamp ( );

  return 0;
}
double f(double x){
  double pi = 3.141592653589793;
  double value;
  value = 50.0 / ( pi * ( 2500.0 * x * x + 1.0 ) );
  return value;
}

double cpu_time (void){
  double value;
  value = ( double ) clock ( ) / ( double ) CLOCKS_PER_SEC;
  return value;
}
void timestamp ( void ){
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct tm *tm;
  time_t now;

  now = time ( NULL );
  tm = localtime ( &now );

  strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm );

  printf ( "%s\n", time_buffer );

  return;
# undef TIME_SIZE
}
