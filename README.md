# NYCU_2023_ECE_Embedded Operating Systems

Hardware : Raspberry Pi 3 Model B+   
Environment : ubuntu-22.04.0  
Linux version : 6.1.77  


Course Overview :
1. Introduction to Embedded Systems,Embedded OS and Real-time OS  
- Kernel objects and RTOS services  
2. Multitasking  
- Task, process, thread, scheduler  
3. Communication & Synchronization  
- Semaphores, mutex, message queues and pipeline  
4. Interrupt  
- Signal, timer and timer services  
5. I/O & Memory  
- Socket, I/O Subsystem, Memory  



## LAB  
### LAB3  
Write a driver and use GPIO pins to implement a scrolling LED display of your student ID on a Raspberry Pi.    
 
Goal:Implementing communication between user space and kernel space.  

### LAB4
Please write a name marquee program on Raspberry Pi. Using a writer program on Raspberry Pi, write English letters into a driver. A reader program will read these letters from the driver. Finally, transmit the letters via a socket to a program named seg.py running on a VM, which will display the letters on a sixteen-segment display (GUI).  

Goal:Implementing communication between user space and kernel space.  

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

## Final Project : Smart parking management system

### Contributor 
School : National Yang Ming Chiao Tung University   
Department : Institute of Electrical and Control Engineering  
Student ID : 312511068、312512013、312512032、312512039  

###  Introduce
  Simulate a smart parking management system and use what you have learned in this course to realize intelligent and automated management of parking lots, improve parking efficiency, reduce traffic congestion, and improve the urban traffic environment.  

The vehicle part will be divided into fuel vehicles and electric vehicles. There will be different parking areas and charging standards according to different vehicles, and the following functions will be implemented:  
1. Parking space guidance.  
2. Check the parking location, time and amount according to the license plate.  
3. Confirm whether the car left the venue within a specific time after payment.  
4. Record license plate, parking space, entry time, payment time, departure time, payment amount, and total assets.    

[Watch the video on YouTube](https://youtu.be/_s5z6sf62Iw)  


### Hardware
Led : Parking space display (light: parked, dark: free).  
WebCam : For license plate reading at exits and entrances on both sides.  
Raspberry Pi 3 Model B+ : As a server, it handles license plate recognition, amount calculation, I/O control, parking space status display, etc. 

### What we used that learned in this course
Semaphore :  To avoid race conditions, use semaphores to protect these shared resources.  
Signal : Use signals to release processes upon completion to avoid the creation of zombie processes. Also, ensure that when a process ends, it releases the occupied shared memory and semaphores.    
Share memory :  Allowing multiple processes to share information(data).  
File descriptor : Communicate with the kernel space.  
Muti-Process : simultaneously handle data passed through multiple I/O .  
Socket : Communicate between Client and Server.    

### Program

#### driver.c 
Using File Descriptors for Communication Between User Space and Kernel Space to Control an LED Simulating Parking Space Status


#### server.c 
Use multi-process handling to process the multiple vehicle behaviors (entering, exiting, paying) from client.py through sockets, and based on the situation, respond to client.py via socket or use file descriptors to transmit parking space information to driver.c. Additionally, use shared memory to access vehicle data and parking space data, allowing multiple processes to share this information. To avoid race conditions, use semaphores to protect these shared resources. Use signals to release processes upon completion to avoid the creation of zombie processes. Also, ensure that when a process ends, it releases the occupied shared memory and semaphores.

#### client.c 
Use multi-processing to simultaneously read information from multiple cameras, identify Regions of Interest (ROI) to reduce computation, and use Tesseract to recognize license plate numbers. Simulate vehicle entry, exit, and payment, and communicate vehicle information with server.c using sockets.

### 



