# csmc.c
An extension of the sleeping Barber Problem. It is a program simulating students comming to a coordinator, asking for help only for the coordinator to designate a seat and a tutor for them if they are available. Note: 

# Prerequisites
This program can only run on Linux. I ran program in Linux that was installed into a virtual machine.

# Running the tests
Type: "gcc -pthread  csmc.c -o main" to compile the file. Do not include the quotatation marks. "main" in the command could be any name you want but it is the name of the file you will execute. Type "./main 9 4 3 2" without the quotation marks to execute. The numbers specficied can be any numbers you want but in general the larger the numbers the longer the program is going to take to execute. It typically takes a few minutes to execute if the numbers are in the single digit range. I have included only what needs to be outputted. To see the extra statements I made you can set the DEBUG flags in the program to 1. You have to edit the flag within the code to do this. Keep in mind, when a student is waiting, it only stops waiting when the coordinator sets a chair free for other students to sit on. Not necessarily when a tutor is helping a student.

# Execution
{} ./main 2 3 2 1
Student 0 takes a seat, notifying coordinator its ready and waiting for help
Currently waiting students: 1
Tutor 1 helping student for 1 miliseconds
Currently waiting students: 1
Student 1 takes a seat, notifying coordinator its ready and waiting for help
Currently waiting students: 1
Tutor 1 helping student for 1 miliseconds
Currently waiting students: 0

