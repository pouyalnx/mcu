/*-----------------------------------------------------------------------------
 * Name:    stdin_user.c
 * Purpose: STDIN User Template
 * Rev.:    1.0.0
 *-----------------------------------------------------------------------------*/
 
/* Copyright (c) 2013 - 2015 ARM LIMITED
 
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/
/*
extern void stdin_init(void);
extern int mpr121_read_touch(void);


*/
#include "driver_i2c.h"
#include "mpr121_reg.h"


#define mpr121_address 0X5A 



extern ARM_DRIVER_I2C Driver_I2C1;
ARM_DRIVER_I2C *mpr121;

void mpr121_send(unsigned char *in,int size,int continue_req){
	mpr121->MasterTransmit(mpr121_address,in,size,continue_req);
	while(mpr121->GetStatus().busy);
}

void mpr121_read(unsigned char *in,int size,int continue_req){
	mpr121->MasterReceive(mpr121_address,in,size,continue_req);
	while(mpr121->GetStatus().busy);
}


void stdin_init(void){
	/*	
	//for test porposes
	#pragma pack(1)		
	struct afe1{
			unsigned CDC:6;
			unsigned FF1:2;
		}afe1;
		#pragma pack(1)	
		struct afe2{
		  unsigned ESI:3;
		  unsigned SFI:2;
			unsigned CDT:3;
		}afe2;
///notic progma is so imprtant
		*/
		
	unsigned int gpv0;
	unsigned char send_buffer[4];
	mpr121=&Driver_I2C1;
	
	mpr121->Initialize(NULL);
  mpr121->PowerControl(ARM_POWER_FULL);	
  mpr121->Control(ARM_I2C_BUS_SPEED,ARM_I2C_BUS_SPEED_STANDARD);	
	mpr121->Control(ARM_I2C_BUS_CLEAR, 0);	
	
  #define write_register(x,y) send_buffer[0]=x;send_buffer[1]=y;mpr121_send(send_buffer,2,false);
	
#define NUM_OF_ELECTRODES	13



/* turn off all electrodes to stop */
		write_register(ECR, 0x00); 

		/* write register with initial values */
		write_register(MHDR, 0x01);
		write_register(NHDR, 0x01);
		write_register(NCLR, 0x10);
		write_register(FDLR, 0x20);
		write_register(MHDF, 0x01);
		write_register(NHDF, 0x01);
		write_register(NCLF, 0x10);
		write_register(FDLF, 0x20);
		write_register(NHDT, 0x01);
		write_register(NCLT, 0x10);
		write_register(FDLT, 0xFF);
		write_register(MHDPROXR, 0x0F);
		write_register(NHDPROXR, 0x0F);
		write_register(NCLPROXR, 0x00);
		write_register(FDLPROXR, 0x00);
		write_register(MHDPROXF, 0x01);
		write_register(NHDPROXF, 0x01);
		write_register(NCLPROXF, 0xFF);
		write_register(FDLPROXF, 0xFF);
		write_register(NHDPROXT, 0x00);
		write_register(NCLPROXT, 0x00);
		write_register(FDLPROXT, 0x00);
	  write_register(DTR, 0x33);
		
		/*
		afe1.CDC=0x3f;
		afe1.FF1=3;
		
		afe2.CDT=4;
		afe2.ESI=4;		
		afe2.SFI=2;
		*/
		write_register(AFE1,0x20);//*((unsigned char*)&afe1));  
		write_register(AFE2,0x10);//*((unsigned char*)&afe2));
		
		write_register(ACCR0, 0x00);
		write_register(ACCR1, 0x00);
		write_register(USL, 0x00); 
		write_register(LSL, 0x00); 
		write_register(TL, 0x00); 
		

		/* apply next setting for all electrodes */
		for (gpv0 = 0; gpv0 < NUM_OF_ELECTRODES; gpv0++) {
			write_register((E0TTH + (gpv0<<1)), 40); 
			write_register((E0RTH + (gpv0<<1)), 40); 
		}

   		write_register(ECR, 0xcf);

}


int mpr121_read_touch(void){
	unsigned char buffer[4];
	buffer[0]=0;
	buffer[1]=0;
	mpr121_send(buffer,1,true);
	mpr121_read(buffer,2,false);
	return buffer[1]<<8|buffer[0];
}


int last_char;
int time_out;

int stdin_getchar (void) {
  int stat;
	while(1){
	stat=mpr121_read_touch();

	if(stat & 1 && last_char!='0'){
  last_char='0';
	return '0';
	}
	
	if(stat & 2 && last_char!='1'){
  last_char='1';
	return '1';
	}
	
	if(stat & 4 && last_char!='2'){
  last_char='2';
	return '2';
	}
	
	if(stat & 8 && last_char!='3'){
  last_char='3';
	return '3';
	}
	
	if(stat & 16 && last_char!='4'){
  last_char='4';
	return '4';
	}
	
	if(stat & 32 && last_char!='5'){
  last_char='5';
	return '5';
	}
	
	if(stat & 64 && last_char!='6'){
  last_char='6';
	return '6';
	}	
	
	if(stat & 128 && last_char!='7'){
  last_char='7';
	return '7';
	}

	if(stat & 256 && last_char!='8'){
  last_char='8';
	return '8';
	}

	if(stat & 512 && last_char!='9'){
  last_char='9';
	return '9';
	}	
	
	if(stat & 1024 && last_char!=' '){
  last_char=' ';
	return ' ';
	}
	
	if(stat & 2048 && last_char!='\n'){
  last_char='\n';
	return '\n';
	}	
  
	time_out++;
	if(time_out>8000){ 
		time_out=0;
	  last_char=0;
	}
	}
	
}
 






