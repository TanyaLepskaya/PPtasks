 /*   
 * 
 *  calcPiMPI.C
 *
 * Использование MPI для расчета значения Pi
 *
 * Использование:  mpirun -np N ./calcPiMPI <number of tosses> 
 * Где: N - количество потоков Б, <number of tosses> - количество бросков для всех процессов
 * 
 * Результат запуска: 
 * 
 * $ mpirun -np 32 ./calcPiMpi 10000000000
 * Elapsed time = 1.229934 seconds
 * Pi is approximately 3.1414150115793777, Error is 0.0001776420104154
 *
 */
 
#include <mpi.h>  
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Get_input(int argc, char* argv[], int myRank, long* totalNumTosses_p);
long Toss (long numProcessTosses, int myRank);

int main(int argc, char** argv) {
    
    // обьявление переменных
   int myRank, numProcs;
   long totalNumTosses, numProcessTosses, processNumberInCircle, totalNumberInCircle;
   double start, finish, loc_elapsed, elapsed, piEstimate;
   double PI25DT = 3.141592653589793238462643;         /* 25-значный-PI*/
   
   // Инициализация MPI
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
  
   // Считывание переменной totalNumTosses
   Get_input(argc, argv, myRank, &totalNumTosses); 
   
   numProcessTosses = totalNumTosses / numProcs; 
   
   MPI_Barrier(MPI_COMM_WORLD);

   // Время старта
   start = MPI_Wtime();
   // Выполнение задачи на каждом процессе. Каждый процесс возвращает колличество бросков, попавшик внутрь круга
   processNumberInCircle = Toss(numProcessTosses, myRank);
   // Время окончание
   finish = MPI_Wtime();

   loc_elapsed = finish-start;
   
   // Сбор максимального времени выполнения среди всех процессов
   MPI_Reduce(&loc_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD); 
 
    // Сумма всех точек, попавших внутрь круга
   MPI_Reduce(&processNumberInCircle, &totalNumberInCircle, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
   

   if (myRank == 0) {
	   piEstimate = (4*totalNumberInCircle)/((double) totalNumTosses);
	   printf("Elapsed time = %f seconds \n", elapsed);
	   printf("Pi is approximately %.16f, Error is %.16f\n", piEstimate, fabs(piEstimate - PI25DT));
   }

   MPI_Finalize(); 

   return 0;
}  

/* Функция считывает количество бросков для всех процессов (totalNumTosses) */
void Get_input(int argc, char* argv[], int myRank, long* totalNumTosses_p){
	if (myRank == 0) {
		if (argc!= 2){
		    fprintf(stderr, "usage: mpirun -np <N> %s <number of tosses> \n", argv[0]);
            fflush(stderr);
            *totalNumTosses_p = 0;
		} else {
			*totalNumTosses_p = atoi(argv[1]);
		}
	}
	// Рассылка totalNumTosses 
	MPI_Bcast(totalNumTosses_p, 1, MPI_LONG, 0, MPI_COMM_WORLD);
	
	// ЕслиtotalNumTosses == 0, завершение программы 
    if (*totalNumTosses_p == 0) {
        MPI_Finalize();
        exit(-1);
    }
}

/* Метод Монте Карло */
long Toss (long processTosses, int myRank){
	long toss, numberInCircle = 0;        
	double x,y;
	unsigned int seed = (unsigned) time(NULL);
	srand(seed + myRank);
	for (toss = 0; toss < processTosses; toss++) {
	   x = rand_r(&seed)/(double)RAND_MAX;
	   y = rand_r(&seed)/(double)RAND_MAX;
	   if((x*x+y*y) <= 1.0 ) numberInCircle++;
    }
    return numberInCircle;
}







































