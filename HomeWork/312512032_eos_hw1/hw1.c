#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h> 
#include <unistd.h> 
#include <ctype.h>
#include <termios.h>



uint32_t price=0;
uint32_t count=0;
char write_buf[100]={};
char read_buf[1] = {};

void clear(void){
    char ch;
    while((ch=getchar())!='\n'&&ch!=EOF){
    	
    }

}


void mypause(void){
    printf("Press any key to continue!! \n\n\n");
    struct termios oldt , newt;
    char ch;
    tcgetattr(STDIN_FILENO,&oldt);
    newt=oldt;
    newt.c_lflag&=~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO,TCSANOW,&newt);
    ch=getchar();
    tcsetattr(STDIN_FILENO,TCSANOW,&oldt);
    //clear();
}







   
char menu(void){
    char ch;
	printf("1. shop list\n");
    printf("2. order\n\n");	
	ch=getchar();
	clear();
	return ch ;
}
void shop_list(void){
    printf("\nDessert shop: 3km\n");
    printf("Beverage shop: 5km\n");	
    printf("Diner: 8km\n\n");
    mypause();
  
}
char order(void){
	char ch;
	printf("\nPlease choose from 1~3\n");
	printf("1. Dessert shop\n");
	printf("2. Beverage Shop\n");
	printf("3. Diner\n\n");
	ch=getchar();
	clear();
	return ch ;
	
}

uint32_t Dessert_shop(void){
	uint32_t number;
	char ch;
	while(1){
		printf("\nPlease choose from 1~4\n");
		printf("1. cookie: $60\n");
		printf("2. cake: $80\n");
		printf("3. confirm\n");
		printf("4. cancel\n\n");
		ch=getchar();
		clear();	
		if(ch=='1'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+60*number;
		}else if (ch=='2'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+80*number;
		}else if (ch=='3'){
			break;
		}else if (ch=='4'){
			price=0;
			break;
		}
		clear();
		
	}
    return price ;
}
uint32_t Beverag_shop(void){
	uint32_t number;
	char ch;
	while(1){
		printf("\nPlease choose from 1~4\n");
		printf("1. tea: $40\n");
		printf("2. boba: $70\n");
		printf("3. confirm\n");
		printf("4. cancel\n\n");
		ch=getchar();
		clear();	
		if(ch=='1'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+40*number;
		}else if (ch=='2'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+70*number;
		}else if (ch=='3'){
			break;
		}else if (ch=='4'){
			price=0;
			break;
		}
		clear();
		
	}
    return price ;
}
uint32_t Diner(void){
	uint32_t number;
	char ch;
	while(1){
		printf("\nPlease choose from 1~4\n");
		printf("1. fried rice: $120\n");
		printf("2. egg-drop soup: $50\n");
		printf("3. confirm\n");
		printf("4. cancel\n\n");
		ch=getchar();
		clear();
			
		if(ch=='1'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+120*number;
		}else if (ch=='2'){
			printf("\nHow many?\n");
			scanf("%u", &number);
			price=price+50*number;
		}else if (ch=='3'){
			break;
		}else if (ch=='4'){
			price=0;
			break;
		}
		clear();
		
	}
    return price ;
}


int main(int argc, char *argv[]){   
	char  ch;
    char  distance;
    int fd;

    

    while(1){
    
    fd = open("/dev/order", O_RDWR);
    if(fd < 0)
        perror("open");
    
    
    	
    	ch=menu();

    	if (ch=='1'){
    		shop_list();	
		}else if(ch=='2'){

		 	switch(ch=order()) {
		 	
			case '1':
				price=Dessert_shop();
                		distance='3';
                		
				break;
			case '2':
				price=Beverag_shop();
                distance='5';
				break;
			case '3':
				price=Diner();
                distance='8';
				break;
			 }	

		}
    		
    	if(price !=0){
    		count= sprintf(write_buf, "%c", distance);
    		count+=sprintf(write_buf+count, "%d", price);
			//printf("\n%c:%u\n",ch,price);
		printf("distance:%c km\n",distance);
    		printf("price:%d \n",price);
    		printf("\nPlease wait for a few minutes...\n"); 
    		write(fd, write_buf, count);

			
			do{
		    	read(fd, read_buf, 1);
			}while(read_buf[0]!='1');
			printf("\nplease pick up your meal\n"); 
			mypause();;
		}
		
		
	memset(write_buf, '0', count);	
	memset(read_buf, '0', 1);
	count=0;	
	price=0;    
	if(fd > 0)
        close(fd);
	
	
	}

	return 0;
}
