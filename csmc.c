// Binary  semaphore –integer value can range  only between 0 and 1 Same as a mutex lock
// https://www.tutorialspoint.com/cplusplus/cpp_multithreading.htm
//https://www.codeproject.com/Questions/49028/Sleeping-barber-problem
//http://pubs.opengroup.org/onlinepubs/7908799/xsh/sem_init.html
// 
// pthread t tid; /* the thread identifier */
// pthread attr t attr declaration represents the attributes for the thread
// set the attributes in the function call pthread attr init(&attr). Default attr used b/c attr not explicitly set
// pthread create() creates thread. pass the name of the function where the new thread will begin execution
// Last, we pass the integer parameter that was provided on the command line, argv[1]. argument must be a pointer

/*the parent thread
will wait for it to terminate by calling the pthread join() function. The
summation thread will terminate when it calls the function pthread exit().
Once the summation thread has returned, the parent thread will output the
value of the shared data sum.*/


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define DEBUG 0 // Debug flag. If this equals 1, debug statements will appear in program

void * theCoord (void * cId ); // Why do the data types have to be void * allow pass any data type? what value should pshare be = to
void * theTutor (void * tId );
void * theStudent (void * sId );
sem_t coordStu; // coordinator to talk to student
sem_t tutorReady;
sem_t studReady;
sem_t coordTut;
sem_t mutex; // mutex
// cond variables seem like semaphores that automatically unlock mutexes when waiting and used to control exe of prog when specfic conditions are met?

int numChairs; // has to be acceble to all functions hence global
int numAval; // number of chairs available
int help;
int studDone = 0;
int NumTutors;
int NumStudents;
int numHelp; // Count amount of times all the students helped. If 2 students need help 3 times. this = 6. Only for debugging purposes
int stuWaiting = 0; // Number of students waiting. Increments when a student takes a seat and decrements ehen a student is finished asking for help
int r; // Generate random number to find time tutor will help student or time student will program

int main(int argc, char *argv[])
{
srand(time(NULL));
   if (argc != 5) 
   {
      fprintf(stderr,"usage: a.out <integer value>\n");
      return -1;
   }
   
   if (atoi(argv[1]) < 0) 
   {
      fprintf(stderr,"%d must be >= 0\n",atoi(argv[1]));
      return -1;
   }
   
   if (atoi(argv[2]) < 0) 
   {
      fprintf(stderr,"%d must be >= 0\n",atoi(argv[2]));
      return -1;
   }
   
      if (atoi(argv[3]) < 0) 
   {
      fprintf(stderr,"%d must be >= 0\n",atoi(argv[3]));
      return -1;
   }
   
         if (atoi(argv[4]) < 0) 
   {
      fprintf(stderr,"%d must be >= 0\n",atoi(argv[4]));
      return -1;
   }
   
   sem_init(&coordStu, 0, 0);
   sem_init(&coordTut, 0, 0);
        sem_init(&tutorReady, 0, 0);    // .Declare coordReady semaphore
    sem_init(&studReady, 0, 0);    // Declare coordReady semaphore
    sem_init(&mutex, 0, 1);    // mutex = 1 since it a lock
   // m and n are arguments passed into the program via command line.
   //int NumTutors,NumStudents;
   int i; // Just a counter used to help give the tutors and students an argument to pass into the function
   NumTutors = atoi(argv[2]); // Represent number of tutors
   NumStudents = atoi(argv[1]); // Represent number of students
   help = atoi(argv[4]);
   
   numChairs = atoi(argv[3]); // number of chairs available. Its initial signals that all the chairs are available
                          // No chairs available when numChairs value equals 0
   numAval = numChairs;     
   
   int tutorNum[NumTutors];
   int studentNum[NumStudents];
   
   // Initilizaling Student arguments
   if(DEBUG)
   {
      printf("Initilizaling Student arguments\n");
   }
   for(i = 0; i < NumStudents; i++)
   {
      studentNum[i] = i;
   }
   
   // Initilizaling Student arguments
   if(DEBUG)
   {
      printf("Initilizaling tutor arguments\n");
   }   
   for(i = 0; i < NumTutors; i++)
   {
      tutorNum[i] = i;
   }
   
   pthread_t student[NumStudents]; /* the students identifiers */
   pthread_t tutor[NumTutors]; /* the tutors identifiers */
   pthread_t coordinator; /* the coordinator identifiers The coordinator wakes up an idle tutor and assign to a student  */ 
   
   pthread_attr_t attr; /* set of threads attributes. Will equal default value */
                     
   i = 0;
   
  // Creating coordinator thread
  if(DEBUG)
  {
      printf("Coordinator created\n");
  }    
   pthread_create(&coordinator, NULL, theCoord, (void *)i);
   
   // Creating tutor threads
   for (i = 0; i < NumTutors; i++) 
   {
        if(DEBUG)
        {
            printf("tutor %d created\n", tutorNum[i]);
        }    
        pthread_create(&tutor[i], NULL, theTutor, (void *)tutorNum[i]); 
   } 

   
   for (i = 0; i < NumStudents; i++) 
   {
        if(DEBUG)
        {
            printf("student %d created\n", studentNum[i]);
        }   
        pthread_create(&student[i], NULL, theStudent, (void *)studentNum[i]); 
        // Should you only create student threads that need help. If  n students need help
        // create n students provided that n <= number of students
   } 
   
   
    // Joining the threads
      for(i=0; i < NumStudents; i++)
   {
      pthread_join(student[i],NULL);
   }
   //studDone = 0; // If all students exit true
   
      for(i=0; i < NumTutors; i++)
   {
      pthread_join(tutor[i],NULL); // A thread can execute a thread join to wait until the other thread terminates
   }
   
    pthread_join(coordinator,NULL); //  blocks the calling thread until the specified threadid thread terminates.
    
    if(DEBUG)
    {
      printf("The students were helped and program is done \n");
    }  
   
   // What if all the students get help? How will the threads terminate?
   
   
   // student first program, sleep(5) cout << student i programming << endl;
   // if tutor is available get help or sit on a chair. If no chairs available student continues programing
   
   
   // Student threads will alternate between programming for a period of time and seeking help from the tutor
   // If the tutor is available, they will obtain help. Otherwise, they will either sit in a chair in the waiting area
   // or, if no chairs are available, will resume programming and seek help at a later time. 

return 0;
}

void * theStudent (void * sId )
{

//   If(help > 0)
// Mutex
// mutex
// While  numberOfFreeWRSeats > 0: # If there are any free seats:
// mutex
//       numberOfFreeWRSeats -= 1  #   sit down in a chair
// signal(mutex)     #   don't need to lock the chairs anymore mutex
//       signal(studReady)         #   notify the coord, who's waiting until there is a student
//       wait(CoordReady)         #   wait until theCoordinator is ready
//       # (Have hair cut here.)
//     else:                       # otherwise, there are no free seats; tough luck --
//       signal(mutex)
   int helpNum = help; // Each students needs a copy of help that they modify
   
   while(helpNum > 0) // Help is the amount of times each student needs help. Each student needs help
   {
                     // not changing value of helo here
         sem_wait(&mutex);
         r = ( rand() % 3 ) + 1;
         sem_post(&mutex);
         if(DEBUG)
         {
            printf("Student %d programming\n", (int)sId); // Students programming which takes time
         }
         sleep(r);                   
      
      if(numAval > 0) 
      {
         sem_wait(&mutex);
         numAval--; // sit on chair
         stuWaiting++;
         sem_post(&mutex); // Protect numAval, stuWaiting, r
         
          printf("Student %d takes a seat, notifying coordinator its ready and waiting for help\nCurrently waiting students: %d\n", (int)sId, stuWaiting);
         sem_post(&studReady); // notify the coord, who's waiting until there is a student
        
        // put student in an array. Space to save what students are want help and are waiting for help or not.
         sem_wait(&coordStu); // wait until theCoordinator is ready
         if(DEBUG)
         {
            printf("Student %d got help\n", (int)sId);
         }   
         
         sem_wait(&mutex);
         helpNum--;
         if(DEBUG)
         {
            printf("Student will wait for help %d more times.\n", helpNum); // This is debug statement
         }    
         sem_post(&mutex);
          
      }
     else
         //sem_post(&mutex);
         printf("No chairs are currently available for student %d. Student will program and ask for help later.\n", (int)sId);
         sem_wait(&mutex);
         r = ( rand() % 3 ) + 1;
         sem_post(&mutex);
         sleep(r);
         
      
   }
   if(DEBUG)
   {
      printf("Student %d is done\n", sId);
   }   
   sem_wait(&mutex);
   studDone++;
   sem_post(&mutex);
   //printf("StudDone %d is done\n", studDone); // Was a debug statement
 
   pthread_exit(0);
}

void * theTutor (void * tId )
{
   // Signal(tutorReady) // signal tutor is ready
//    Wait(CoordReady) // wait for coord to give them a student
//    // Help student

   while(studDone < NumStudents ) // If student not done continue to help. Tutors work till all students done
   {
      if(DEBUG)
      {
         printf("Tutor %d is signalling to the coordinator that it is ready to help students\n", tId);
      }
      sem_post(&tutorReady); // says to coord it is ready.
      while((sem_trywait(&coordTut)) < 0) // continue to happen in till does get semaphore
       // Wait until student finsished 
       {
          if(studDone >= NumStudents)
          {
            if(DEBUG)
            {
               printf("The tutors have finished helping the students\n");
               printf("Tutor %d is done helping\n", tId);
            }   
            
            pthread_exit(0);
            
          }
       }

      
      //sem_wait(&coordTut); // Wait for coordinator when coordinator is ready
        sem_wait(&mutex);
        r = ( rand() % 3 ) + 1;
        sem_post(&mutex);
      printf("Tutor %d helping student for %d miliseconds\nCurrently waiting students: %d\n", tId, r, stuWaiting);
      sem_wait(&mutex);
      sleep(r);  
      sem_post(&mutex);
      
      sem_wait(&mutex);
      numHelp++;
      if(DEBUG)
      {
         printf("tutors helped %d times\n", numHelp); // Tells how many times the tutors in total helped.
      } 
      sem_post(&mutex);   
   }
if(DEBUG)
{
   printf("tutor %d is done helping\n", tId);
}  
pthread_exit(0);
   
}

void * theCoord (void * cId )
{
   
   while(studDone < NumStudents) // Help is the amount of times each student needs help. Each student needs help
   {
                     // not changing vaalue of helo here
      if(DEBUG)
      {   
         printf("Coordinator is waiting for a student to assign a tutor to\n");
      }   
      
      while((sem_trywait(&studReady)) < 0) // continue to happen in till does hget semaphore
       // Wait for student to be ready and ask for help
       {
          if(studDone >= NumStudents)
          {
            if(DEBUG)
            {
               printf("All students are finished and received help\n");
               printf("coordinator is done helping\n");
            }   
            pthread_exit(0);
            
          }
       }
      
      if(DEBUG)
      {
         printf("Coordinator is waiting for a tutor to be available\n");
      }   
      //sem_post(&coordReady); // Signal 
      sem_wait(&tutorReady); // signal tutor to be available to help student
      
      sem_post(&coordStu); // Coordinator is ready to asign a tutor to a student
       sem_post(&coordTut);
      if(DEBUG)
      {
         printf("Coordinator has found and assigned an available tutor to a student\n"); 
      }    
   
      sem_wait(&mutex);
      numAval++; // Coordinator setting a chair free for another student to sit on
      stuWaiting--;
      //help--;
      sem_post(&mutex);
     
      }
   if(DEBUG)
   {
      printf("coordinator %d is done helping\n");
   }   
   
   pthread_exit(0);
}

