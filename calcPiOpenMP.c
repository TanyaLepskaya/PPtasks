 /*   
 * 
 *  calcPiOpenMP.c
 *
 * Использование OpenMP для расчета значения Pi
 *
 * Использование:  
 * gcc calcPiOpenMP.c -fopenmp
 * ./a.out
 * 
 * Результат запуска: 
 * $ ./a.out
 * Pi is approximately 3.1211308000000000, Error is 0.0204618535897931
 * time to compute = 3.80073 seconds
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "timer.h" 
#include <math.h>
 
int main(int argc, char* argv[])
{
        int numthreads = 16;                    // количество тредов
        long totalNumIter = 100000000;          // общее количество итераций
        long niter = totalNumIter/numthreads;   // количество интераций для одного потока     

    double x,y;                     
    int i;                          
        int count=0;                
    double z;                      
    double pi;                      
    double start_time, end_time;
    double PI25DT = 3.141592653589793238462643;         /* 25-значный-PI*/
 
    #pragma omp parallel firstprivate(x, y, z, i) shared(count) num_threads(numthreads)
    {
        /* Запись времени запуска */
    	start_time = get_time();

        srandom((int)time(NULL) ^ omp_get_thread_num());   

        // Подсчет количество точек внутри окружности
        for (i=0; i<niter; ++i)              
        {
            x = (double)random()/RAND_MAX;      
            y = (double)random()/RAND_MAX;      
            z = (x*x)+(y*y);          
        if (z<=1)
        {
                ++count;           
        }
    }
    }
        /* Запись времени окончания */
	    end_time = get_time();
        
        pi = ((double)count/(double)(niter*numthreads))*4.0;
        printf("Pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        printf("time to compute = %g seconds\n", end_time - start_time);
 
    return 0;
}
