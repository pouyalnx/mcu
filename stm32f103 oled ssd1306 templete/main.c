#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "cmsis_os.h"
#include <stdio.h>
//#include "Board_ADC.h"
#include "stm32f10x_adc.h"

extern void stdout_init(void);
extern void stdout_set(int page,int column);
extern void stdout_invert(int in);
extern void stdout_cmdmode(int in);




int main(){
  int gpv0;
	ADC_InitTypeDef adc;
	
	osKernelInitialize();

	//RCC->APB2ENR |=  (1U << 3);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,RCC_APB2ENR_IOPBEN);
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Pin=GPIO_Pin_12;
	gpio.GPIO_Speed=GPIO_Speed_10MHz;
	gpio.GPIO_Mode=GPIO_Mode_Out_OD;
	GPIO_Init(GPIOB,&gpio);
	 osDelay(100);
	
	stdout_init();
	
	puts("alita");
	puts("alita is an new\ncharacter");
	puts("i love alita ,becauseshe is an hero");
	//ADC_Initialize();
	
	
	
	
	
	adc.ADC_ContinuousConvMode=DISABLE;
	adc.ADC_DataAlign=ADC_DataAlign_Right;
	adc.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	adc.ADC_NbrOfChannel=1;
	adc.ADC_ScanConvMode=DISABLE;
	ADC_Init(ADC1,&adc);
	
	while(1){
	//	ADC_StartConversion();
	//	while(ADC_ConversionDone()==-1);
	//	stdout_set(6,0);
	//	printf("adc= %d  \n",ADC_GetValue());
		
		
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,1);
		osDelay(80);
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,0);
	  osDelay(80);		
	}
	
	
}