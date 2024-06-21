# NYCU_2023_ECE_Embedded Operating Systems
Course Overview :
1. Introduction to Embedded Systems, Embedded OS and Real-time OS
- Kernel objects and RTOS services
2. Multitasking
- Task, process, thread, scheduler
3. Communication & Synchronization
- Semaphores, mutex, message queues and pipeline
4. Interrupt
- Signal, timer and timer services
5. I/O & Memory
- Socket, I/O Subsystem, Memory


Environment: ubuntu-22.04.0  
Linux version : 6.1.77  


## LAB
### LAB3
Write a driver and use GPIO pins to implement a scrolling LED display of your student ID on a Raspberry Pi.  

Goal:Implementing communication between user space and kernel space.  

### LAB4


### LAB5
Please write a socket program on a VM. First, start the server program on the VM, which will wait for the client program to connect. Once the connection is established, the server will transmit ASCII art of a train to the client via the socket. As a result, on the client-side terminal, you will see a train rushing from right to left.  

Goal:Learning how to create a socket server and utilize fork() to spawn multiple processes for achieving multi-client connections.  

### LAB6
Please write a socket program on a VM. Multiple clients will simultaneously connect to perform deposit and withdrawal operations on the same account. The server-side code should handle race conditions effectively to prevent financial losses for the clients due to concurrent access. 

Goal:Learning how to protect resources using semaphores.  

### LAB7

Please write a Mastermind game program on a VM. The game consists of two parts: the first program uses a timer to prompt the user to guess a number every second. It sends a signal to the second program, which reads the guessed number and responds with "correct", "too high", or "too low". Both programs communicate using shared memory for data transfer.

Goal: Learning how to transfer data using shared memory and synchronize processes using signals.  


## Homework
### HW1
Please write a delivery system on RPi.  
Goal:Implementing communication between user space and kernel space.  

### HW2
Continuing from HW1, modify the original standalone version of the delivery system to a networked version using a socket server.  
Goal: Implementing socket server useing  C.  

### HW3
Continuing from HW2, modify the server to a multi-client version using processes or threads, and ensure it correctly handles race conditions.  
Goal: use threads,Semaphores and Timer to implement a multi-client version of a socket server.  

