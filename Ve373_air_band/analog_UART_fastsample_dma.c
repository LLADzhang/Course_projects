#include <plib.h>
#include <p32xxxx.h>

#pragma interrupt T3_ISR ipl7 vector 12
#pragma interrupt T4_ISR ipl7 vector 16
#pragma interrupt T5_ISR ipl7 vector 20
#pragma interrupt CN_ISR ipl7 vector 26
#pragma interrupt ADC_ISR ipl7 vector 27
#pragma interrupt ISR_INT1 ipl7 vector 7
#pragma interrupt IC4_ISR ipl7 vector 17


int x11=300; 
int x12=300; 
int x21=300; 
int x22=300; 
int y11=300; 
int y12=300; 
int y21=300; 
int y22=300; 
int z11=300; 
int z12=300; 
int z21=300; 
int z22=300;
int d11=0;
int d12=0;
int d13=0;
int d21=0;
int d22=0;
int d23=0;
int count=0;
int last1=0;
int last2=0;
int dang=0;
int i=0;
int re4=1;
char data=0;
char trans=0;

void IC4_ISR()
{		
		int ggg=IC4BUF;
		while(U1ASTAbits.UTXBF);
		U1ATXREG=0x60;
		PORTDbits.RD4=0;
		PR5=40000;
	    T5CON=0x8030;
	    TMR5=0;
		IFS0CLR=0x20000;
}



void ISR_INT1()
{		dang++;
		while(U1ASTAbits.UTXBF);
		U1ATXREG=0x40;
		PORTDbits.RD3=0;

		PR5=40000;
	    T5CON=0x8000;
	    TMR5=0;
	IFS0CLR=0X80;
}


void CN_ISR (void) 
{
	IEC1CLR = 0x0001;
if(PR5==50000)
{	

	if(PORTCbits.RC14==1)
	{

		dang++;
		while(U1ASTAbits.UTXBF);
		data=0x40;
		DCH0CONSET=0x80;
		DCH0ECONSET=0x00000080;
		PORTDbits.RD3=0;

		PR5=40000;
	    T5CON=0x8030;
	    TMR5=0;
		CNCON = 0x0000;
	}
	else if(PORTCbits.RC13==1)
	{
	
		while(U1ASTAbits.UTXBF);
		if (PORTCbits.RC13==1)
		data=0x60;
		DCH0CONSET=0x80;
		DCH0ECONSET=0x00000080;
		PORTDbits.RD4=0;
		PR5=40000;
	    T5CON=0x8030;
	    TMR5=0;
		CNCON = 0x0000;
	}
}	
	IFS1CLR = 0x0001;//clear interrupt flag
	IEC1SET = 0x0001;//

}



void T3_ISR()
{
	IFS0CLR = 0x00001000;
	if(PR3==50000)
	{
	PORTDbits.RD1=1;
	T3CON=0;
	TMR3=0;}
	else if(PR3==100)
	{
		count++;
		if(count==5000)
		{
			count=0;
			OC2CON=0;
			T3CON=0;
			TMR3=0;
		}
	}
}


void T4_ISR()
{
	IFS0CLR = 0x00010000;
	PORTDbits.RD2=1;
	T4CON=0;
	TMR4=0;
}


void T5_ISR()
{
	IFS0CLR = 0x00100000;
	if(PR5==40000)
	{
		CNCON = 0x8000;
		TMR5=0;
		T5CON=0x0;
		PR5=50000;
		T5CON=0x8050;
	}
	else if(PR5==50000)
	{PORTDbits.RD3=1;
	PORTDbits.RD4=1;
	T5CON=0;
	TMR5=0;}
}

void ADC_ISR()
{	
	 //int d11=d12=d13=d21=d22=d23=0;	
	int d1=0;
	int d2=0;
	
	 if(T4CONbits.ON==0)
	 {
	 	x11=ADC1BUF0;
	 	y11=ADC1BUF1;
	 	z11=ADC1BUF2;
		d11=x11-x12;
	 	d12=y11-y12;
	 	d13=z11-z12;
	    d1=d11*d11+d12*d12+d13*d13;
	 if ((d12>0)&&(d1>10000))	
	{	TMR4=0;
		//dang++;
		PORTDbits.RD2=0;
		T4CON=0x8050;
		while(U1ASTAbits.UTXBF);
		if(d1>0x10000){d1=0x10000;}
		if(PORTEbits.RE1==0)
		{//U1ATXREG=0x80+0x1F*d1/(0x10000);
		data=0x80+0x1F*d1/(0x10000);
		DCH0CONSET=0x80;
		DCH0ECONSET=0x00000080;
		}
		else {//U1ATXREG=0xE0+0x1F*d1/(0x10000);
		data=0xE0+0x1F*d1/(0x10000);
		DCH0CONSET=0x80;
		DCH0ECONSET=0x00000080;
		}
	    //i==0x80+(0x1F*d1/0x10000);
		
	}
	else{
	TMR4=0;
	PR4=50000;
	T4CON=0x8010;
		}	
	 }
	if(T3CONbits.ON==0)
	 {
		 x21=ADC1BUF4;
		 y21=ADC1BUF5;
		 z21=ADC1BUF6;
		 d21=x21-x22;
		 d22=y21-y22;
	 	 d23=z21-z22;
	     d2=d21*d21+d22*d22+d23*d23;	 	
		if ((d22>0)&&(d2>7000))	
		{
			if(d2>0x10000){d2=0x10000;}
			PR3=100;
			OC2CON=0x800E;
			OC2RS = 100*(0x10000-d2)/(0x10000);
			OC2R = 100*(0x10000-d2)/(0x10000);
			TMR3=0;
			//PORTDbits.RD1=1;
			T3CON=0x8010;
			while(U1ASTAbits.UTXBF);
			
			if(PORTEbits.RE2==0)
			{data=0xA0+0x1F*d2/(0x10000);
			DCH0CONSET=0x80;
			DCH0ECONSET=0x00000080;}
			else{data=0xC0+0x1F*d2/(0x10000);
			DCH0CONSET=0x80;
			DCH0ECONSET=0x00000080;}
		}
		else{
		TMR3=0;
		PR3=50000;
		T3CON=0x8000;
		}	
	 }
	 x12=x11;
	 y12=y11;
	 z12=z11;
	 x22=x21;
	 y22=y21;
Q		 z22=z21;	


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
	//TRISD=0x0000;
	//TRISB=0x0000;
} 

int main()
{
		INTCONbits.MVEC = 1; // Enable multiple vector interrupt
		// Enable all interrupts
		asm ("di"); 

		UARTconfig();

		PR3=50000;
		PR4=50000;
		PR5=50000;
		TMR4=0;
		TMR5=0;
		TMR3=0;

 		IPC3SET = 0x0000001C; //IPC4SEet priority level = 7
		IFS0CLR = 0x00001000;
		IEC0SET = 0x00001000;

		IPC4SET = 0x0000001C; //Set priority level = 7
		IFS0CLR = 0x00010000;
		IEC0SET = 0x00010000;

		IPC5SET = 0x0000001C; //Set priority level = 7
		IFS0CLR = 0x00100000;
		IEC0SET = 0x00100000;

		IPC6SET = 0x1C000000;
		IFS1CLR = 0x00000002;
		IEC1SET = 0x00000002;
		
		//INTCON=0;
		//INTCONbits.INT1EP=1;
		//IPC1SET=0x1F000000;
		//IFS0CLR=0X80;
		//IEC0SET=0x80;

		//IC4CON =0x8083;
		//IPC4SET=0x00001C00;
		//IFS0CLR=0X20000;
		//IEC0SET=0x20000;
		//PR2=10000;
		//T2CON=8000;

		IEC1CLR=0x00010000; // disable DMA channel 0 interrupts
		IFS1CLR=0x00010000; // clear existing DMA channel 0 interrupt flag
		DMACONSET=0x00008000; // enable the DMA controller
		DCH0CON=0x3; // channel 0 disabled, priority 3, no chaining
		DCH0ECON=0; // no start or stop IRQ, no pattern match
		DCH0SSA=KVA_TO_PA(&data);; // transfer source physical address
		DCH0DSA=KVA_TO_PA(&U1ATXREG); // transfer destination physical address
		DCH0SSIZ=1; // source size 101 bytes
		DCH0DSIZ=1; // destination size 1 bytes
		DCH0CSIZ=1; // 1 bytes transferred per event
		DCH0INTCLR=0x00ff00ff; // clear existing events, disable all interrupts
		DCH0CONSET=0x80; // turn channel on


		//TRISB = 0x0000;
		TRISC = 0xFFFF;
		TRISD = 0xFF00;
		TRISE = 0xFFFF;
        //PORTBbits.RB5=1;
  		//PORTBbits.RB4=1;
		//PORTBbits.RB3=1;
		//PORTDbits.RD5=1;
		PORTDbits.RD4=1;
		PORTDbits.RD3=1;
		PORTDbits.RD2=1;
		PORTDbits.RD1=1;

		AD1PCFG = 0xFFC0;
		AD1CON1 = 0x00E4;
		AD1CON2 = 0x0414;
		AD1CON3 = 0x0000;
		AD1CSSL = 0x003F;

		
		CNCON = 0x8000;//Enable Change Notice module
		CNEN = 0x00000003;//Enable individual CN pins
		CNPUE = 0x00000003;//

		IPC6SET = 0x001C0000;//Set priority level = 7
		IFS1CLR = 0x0001;//Clear interrupt flag
		IEC1SET = 0x0001;//


		asm ("ei"); 
		
		AD1CON1SET = 0x8000;

	while(1)
{
if(PORTEbits.RE4==1){re4=1;}

int k=0;

if((PORTEbits.RE4==0)&&(re4==1))
{	
while(U1ASTAbits.UTXBF);
		U1ATXREG=0x60;
		PORTDbits.RD4=0;
		PR5=40000;
	    T5CON=0x8030;
	    TMR5=0;
		re4=0;
}
}

}
