
#define BASE_ADDR 0XA0000000
#define TFT_X 480
#define TFT_Y 270


/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/
unsigned short *lcd;

void lcd_setup(void){
	unsigned int gpv0;

//CLOCK AND POWER CONTROL
LPC_SC->PCONP|=1;	
LPC_SC->LCD_CFG=1;	
LPC_LCD->CTRL=0;	
	//PIN ENABLING
	
//pincon	RED
LPC_IOCON->P2_12=5;	
LPC_IOCON->P2_6=7;	
LPC_IOCON->P2_7=7;	
LPC_IOCON->P2_8=7;	
LPC_IOCON->P2_9=7;	

//PINCON GREEN
LPC_IOCON->P1_20=7;	
LPC_IOCON->P1_21=7;
LPC_IOCON->P1_22=7;
LPC_IOCON->P1_23=7;
LPC_IOCON->P1_24=7;
LPC_IOCON->P1_25=7;

//PINCON BLUE
LPC_IOCON->P2_13=7;
LPC_IOCON->P1_26=7;
LPC_IOCON->P1_27=7;
LPC_IOCON->P1_28=7;
LPC_IOCON->P1_29=7;

//INSTALLING SIGNALS
LPC_IOCON->P2_2=7;
LPC_IOCON->P2_0=7;
LPC_IOCON->P2_5=7;
LPC_IOCON->P2_3=7;
LPC_IOCON->P2_4=7;	

LPC_GPIO2->DIR=2;
LPC_GPIO2->CLR=2;


for(gpv0=0;gpv0<128;gpv0++){
	LPC_LCD->PAL[gpv0]=0;
}
	

#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */

LPC_LCD->PAL[0]=Black|Navy<<16;
LPC_LCD->PAL[1]=DarkGreen|DarkCyan<<16;
LPC_LCD->PAL[2]=Maroon|Purple<<16;
LPC_LCD->PAL[3]=Olive|LightGrey<<16;
LPC_LCD->PAL[4]=DarkGrey|Blue<<16;
LPC_LCD->PAL[5]=Green|Cyan<<16;
LPC_LCD->PAL[6]=Red|Magenta<<16;
LPC_LCD->PAL[7]=Yellow|White<<16;
LPC_LCD->PAL[8]=54622|34543<<16;
LPC_LCD->PAL[9]=54164|887<<16;
LPC_LCD->PAL[10]=556|55721<<16;
LPC_LCD->PAL[11]=260|41531<<16;
LPC_LCD->PAL[12]=36063|34833<<16;
LPC_LCD->PAL[13]=326|424<<16;
LPC_LCD->PAL[14]=23406|25552<<16;
LPC_LCD->PAL[15]=33970|38164<<16;
LPC_LCD->PAL[16]=61274|65468<<16;
LPC_LCD->PAL[17]=46144|51778<<16;
LPC_LCD->PAL[18]=55685|53680<<16;
LPC_LCD->PAL[19]=27544|9334<<16;
LPC_LCD->PAL[20]=11539|33984<<16;
LPC_LCD->PAL[21]=33984;

//SYSTEM ENABLE WORKING	
LPC_LCD->CTRL=6<<1|1<<5|0<<8;
LPC_LCD->POL=1<<11|1<<12|1<<13|(TFT_X-1)<<16|8;
LPC_LCD->TIMH=((TFT_X/16)-1)<<2|19<<8|1<<16|1<<24;
LPC_LCD->TIMV=(TFT_Y-1)|14<<10|2<<16|2<<24;
LPC_LCD->LPBASE=BASE_ADDR ;
LPC_LCD->UPBASE=BASE_ADDR ;

lcd=(unsigned short*)BASE_ADDR;
LPC_LCD->CTRL|=1|1<<11;	

}
///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
  

  void sdram_setup(void){
	volatile unsigned int gpv0,i;
	
	//PROGRAMMMING POWER BLOCK
	
	LPC_SC->PCONP|=1<<11;
	
	LPC_SC->EMCCLKSEL=0;
	
	LPC_SC->SCS|=0X3;

	LPC_SC->EMCDLYCTL |= (8<<0);
	/*Set data read delay*/
	LPC_SC->EMCDLYCTL |=(8<<8);
	LPC_SC->EMCDLYCTL |= (0x08 <<16);
	
	//PROGRAMMING PINS
	//DATA PINS
	LPC_IOCON->P3_0=1|1<<9;
	LPC_IOCON->P3_1=1|1<<9;
	LPC_IOCON->P3_2=1|1<<9;
	LPC_IOCON->P3_3=1|1<<9;
	LPC_IOCON->P3_4=1|1<<9;
	LPC_IOCON->P3_5=1|1<<9;
	LPC_IOCON->P3_6=1|1<<9;
	LPC_IOCON->P3_7=1|1<<9;
	LPC_IOCON->P3_8=1|1<<9;
	LPC_IOCON->P3_9=1|1<<9;
	LPC_IOCON->P3_10=1|1<<9;
	LPC_IOCON->P3_11=1|1<<9;
	LPC_IOCON->P3_12=1|1<<9;
	LPC_IOCON->P3_13=1|1<<9;
	LPC_IOCON->P3_14=1|1<<9;
	LPC_IOCON->P3_15=1|1<<9;
	LPC_IOCON->P3_16=1|1<<9;
	LPC_IOCON->P3_17=1|1<<9;
	LPC_IOCON->P3_18=1|1<<9;
	LPC_IOCON->P3_19=1|1<<9;
	LPC_IOCON->P3_20=1|1<<9;
	LPC_IOCON->P3_21=1|1<<9;
	LPC_IOCON->P3_22=1|1<<9;
	LPC_IOCON->P3_23=1|1<<9;
	LPC_IOCON->P3_24=1|1<<9;
	LPC_IOCON->P3_25=1|1<<9;
	LPC_IOCON->P3_26=1|1<<9;
	LPC_IOCON->P3_27=1|1<<9;
	LPC_IOCON->P3_28=1|1<<9;
	LPC_IOCON->P3_29=1|1<<9;
	LPC_IOCON->P3_30=1|1<<9;
	LPC_IOCON->P3_31=1|1<<9;
//ADDRESS PIN
 	LPC_IOCON->P4_0=1|1<<9;
	LPC_IOCON->P4_1=1|1<<9;
	LPC_IOCON->P4_2=1|1<<9;
	LPC_IOCON->P4_3=1|1<<9;
	LPC_IOCON->P4_4=1|1<<9;
	LPC_IOCON->P4_5=1|1<<9;
	LPC_IOCON->P4_6=1|1<<9;
	LPC_IOCON->P4_7=1|1<<9;
	LPC_IOCON->P4_8=1|1<<9;
	LPC_IOCON->P4_9=1|1<<9;
	LPC_IOCON->P4_10=1|1<<9;
	LPC_IOCON->P4_11=1|1<<9;
	LPC_IOCON->P4_12=1|1<<9;
	LPC_IOCON->P4_13=1|1<<9;
	LPC_IOCON->P4_14=1|1<<9;
//CONTOL SIGNAL PINS
	LPC_IOCON->P2_16=1|1<<9;
	LPC_IOCON->P2_17=1|1<<9;
	LPC_IOCON->P2_18=1|1<<9;
	LPC_IOCON->P2_19=1|1<<9;
	LPC_IOCON->P2_20=1|1<<9;
	LPC_IOCON->P2_24=1|1<<9;
	LPC_IOCON->P4_25=1|1<<9;	
	LPC_IOCON->P2_28=1|1<<9;
	LPC_IOCON->P2_29=1|1<<9;
	LPC_IOCON->P2_30=1|1<<9;
	LPC_IOCON->P2_31=1|1<<9;
	
//TIME REGISTERS FOR EMC DYNAMIC

LPC_EMC->Control=1;
LPC_EMC->Config=0;
LPC_EMC->DynamicControl|=3;
LPC_EMC->DynamicControl&=0XFD;


LPC_EMC->DynamicRefresh=2700;
LPC_EMC->DynamicReadConfig=1;
LPC_EMC->DynamicRP=4;
LPC_EMC->DynamicRAS=9;
LPC_EMC->DynamicSREX=1;
LPC_EMC->DynamicAPR=1;
LPC_EMC->DynamicDAL=6;
LPC_EMC->DynamicWR=2;
LPC_EMC->DynamicRC=13;
LPC_EMC->DynamicRFC=13;
LPC_EMC->DynamicXSR=1;
LPC_EMC->DynamicRRD=4;
LPC_EMC->DynamicMRD=2;


LPC_EMC->DynamicRasCas0=3|3<<8;

//LPC_EMC->DynamicConfig0 = 0x0004680;
LPC_EMC->DynamicConfig0 =  0x0004680;
	// NOP
	LPC_EMC->DynamicControl = 0x0183;
	for(i= 200*30; i;i--);
	// PALL
	LPC_EMC->DynamicControl = 0x0103;
	LPC_EMC->DynamicRefresh = 8;
	for(i= 256; i; --i); // > 128 clk
	LPC_EMC->DynamicRefresh = 2700;
	// COMM

  LPC_EMC->DynamicControl    = 0x00000083; /* Issue MODE command */
	gpv0 = *((volatile uint32_t *)(0xa0000000 | (0x32<<13)));
	// NORM
	LPC_EMC->DynamicControl = 0x0000;
	LPC_EMC->DynamicConfig0 |=(1<<19);
	for(i = 100000; i;i--);
}


void inti_base(void){
	sdram_setup();
	lcd_setup();
}
/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/
