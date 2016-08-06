#include <p32xxxx.h>
#include <plib.h>
#define V_SOUND 17000 // 17000 cm/s
#define PBCLK   2500000 //2.5MHz
#define scale   256
//#pragma interrupt IC5_ISR ipl7 vector 21
//#pragma interrupt IC2_ISR ipl6 vector 9
#pragma interrupt TMR2_ISR ipl7 vector 8
int distance3 = 0;
int temp1 = 0;
int temp2 = 0;
int reduancy = 0;
int status3=0;
int predistance = 0;

void TMRconfig()
{
  T2CON = 0x70;// 1:256
  PR2 = 0xffff;
  T2CONSET = 0x8000;
  TMR2 = 0;
  IEC0SET = 0x100; 
  IFS0CLR = 0x100;
  IPC2SET = 0x1F;
}

void DelayUsec(int num) {
  IFS0CLR = 0x10;
  TMR1=0; 
  PR1=num; // base time is 3.2 us
  T1CON = 0x8010; // TMR2 on, prescale 1:8 = 0.3125 MHz
  while(IFS0bits.T1IF==0){
  }
  IFS0CLR = 0x10;
  T1CONCLR = 0x8000; // TMR2 off
}

void get_distance()
{
    PORTD = 0xc0;               //TRIGGER HIGH RD6, RD 7 high
    DelayUsec(10);               //10uS Delay
    PORTDCLR = 0xc0;               //TRIGGER LOW RD0, RD 1
}
void TMR2_ISR()
{
	IFS0CLR=0x100;
	get_distance();
}


void ICconfig(){
  INTCONbits.MVEC = 1;
  IC5CONbits.ICTMR = 1; // 
  IC5CONbits.ICM = 1; // capture every edge
  IC5CONbits.SIDL=1;
  IC5CONbits.FEDGE = 1; // capture rising edge first
  IC5CONSET = 0x8000;
  IFS0CLR = 0x200000;
} 
void UARTconfig(){  
  U1ABRG=65;
  U1AMODEbits.STSEL=1;
  U1AMODEbits.PDSEL=1;
  U1AMODEbits.BRGH=0;
  U1AMODEbits.UEN=0;
  U1AMODEbits.ON=1;
  U1ASTAbits.UTXEN=1;
  TRISF=0x0000;
}

main(){
  TRISD = 0x1c00; //RD9 as Input PIN (ECHO) 2 RD10 as Input PIN (ECHO) 1 
  ICconfig();
  PORTD = 0 ;
  UARTconfig();
  TMRconfig();
  get_distance();	
while(1)
{
	while(IFS0bits.IC5IF == 0){}
	if(status3 == 1)
	{
		temp2 = IC5BUF;
	 	distance3 = ((temp2 - temp1)* V_SOUND * scale )/(PBCLK);   
   		status3 = 0;
		if(predistance == 22){
			if(5 <= distance3 && distance3 <= 6)
				U1ATXREG =0b00100000; // do
  			 else if (distance3==8 )
				U1ATXREG = 0b00100001;
  			 else if (distance3==10)
				U1ATXREG = 0b00100010;
 			 else if (distance3 == 12)
				U1ATXREG = 0b00100011;
 			 else if (distance3==15)
				U1ATXREG = 0b00100100;
  			 else if (distance3==17)
				U1ATXREG = 0b00100101;
  			 else if (distance3==19)
				U1ATXREG = 0b00100110;
   			 else if (distance3==20)
				U1ATXREG = 0b00100111;
		}
		predistance = distance3;
	//	DelayUsec(65535);
	    get_distance();
		TMR2 = 0;	
	}
	else if(status3 == 0)
	{
	   temp1 = IC5BUF;
		status3 = 1;
	}
	while(IC5CONbits.ICBNE){
		reduancy = IC5BUF;
	}
   	IFS0bits.IC5IF= 0;
 }
	

}

