/******************************************************************************
 * * Расчет Pi методом Монте Карло.
 * *
 * * Компиляция:
 * *   gcc -lm -pthread pthreadTimed.c
 * * Запуск:
 * *    ./a.out
 * ******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#define NUM_THREADS 16          // 16 потоков для параллельно запуска
#define TOT_COUNT 10000000      // Общее кол-во итераций
#define BILLION  1E9; 

/**
Генератор случайных числе от 0 до 1
**/
float randNumGen(){
   int random_value = rand(); 
   return random_value / (float) RAND_MAX; // делаем число от 0 до 1
}

/**
Задача выполняется на каждом потоке
**/
void *doCalcs(void *threadid)
{
   long longTid;
   longTid = (long)threadid;

   int tid = (int)longTid;     

   float *in_count = (float *)malloc(sizeof(float));
   *in_count=0;
   
   // кол-во итераций для одного потока
   float tot_iterations= TOT_COUNT/NUM_THREADS;
   
   int counter=0;
   
   // расчет
   for(counter=0;counter<tot_iterations;counter++){
      float x = randNumGen();
      float y = randNumGen();
      
      float result = sqrt((x*x) + (y*y));
      
      if(result<1){
         *in_count+=1;
      }
      
   }
   
   // отдельный раччет для нулевого потока
   if(tid==0){
      float remainder = TOT_COUNT%NUM_THREADS;
      
      for(counter=0;counter<remainder;counter++){
      float x = randNumGen();
      float y = randNumGen();
      
      float result = sqrt((x*x) + (y*y));
      
      if(result<1){
         *in_count+=1;         //check if the generated value is inside a unit circle
      }
      
   }
   }
   
   pthread_exit((void *)in_count);     //return the in count
}

int main(int argc, char *argv[])
{
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   void *status;
   float tot_in=0;

   // Расчет затраченного времени
  struct timespec requestStart, requestEnd;
  clock_gettime(CLOCK_REALTIME, &requestStart);
   
   for(t=0;t<NUM_THREADS;t++){
     rc = pthread_create(&threads[t], NULL, doCalcs, (void *)t);
     if (rc){
       printf("ERROR; return code from pthread_create() is %d\n", rc);
       exit(-1);
       }
     }

   //join 
   for(t=0;t<NUM_THREADS;t++){
           
      pthread_join(threads[t], &status);
      tot_in+=*(float*)status;         
     
     }
     
     clock_gettime(CLOCK_REALTIME, &requestEnd);
     // Расчет времени
     double accum = ( requestEnd.tv_sec - requestStart.tv_sec )
       + ( requestEnd.tv_nsec - requestStart.tv_nsec )
       / BILLION;
     printf( "Time taken: %lf\n", accum );
     printf("Value for Pi: %f \n",1, 4*(tot_in/TOT_COUNT));
   
   pthread_exit(NULL);
}