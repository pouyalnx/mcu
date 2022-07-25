#include "spi_user_template.h"
#include "driver_spi.h"
#include "gpio_lpc17xx.h"
#include "lpc17xx.h"
#include "cmsis_os.h"

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/



extern ARM_DRIVER_SPI Driver_SPI0;
ARM_DRIVER_SPI *nrf;

struct{
	unsigned char tx_addr[5];
	unsigned char rx_addr_p0[5];
	unsigned char rx_addr_p1[5];
	
	unsigned ask_mode:1;
	unsigned rf_mode:1;
	unsigned reseved:6;
}NRF_profile;



int NRF_transfer_res;
/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/
void nrf_send_call_back(uint32_t events){
	
}

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/

int nrf_write(unsigned char reg,unsigned char *data,int n){
	nrf->Control(ARM_SPI_CONTROL_SS,1);
	nrf->Send(&reg,1);
	while(nrf->GetStatus().busy);
	if(n){
	nrf->Send(data,n);
	while(nrf->GetStatus().busy);	
	}
	nrf->Control(ARM_SPI_CONTROL_SS,0);
	return 1;
}

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/

int nrf_read(unsigned char reg,unsigned char *data,int n){
	nrf->Control(ARM_SPI_CONTROL_SS,1);
	nrf->Send(&reg,1);
	while(nrf->GetStatus().busy);
	nrf->Receive(data,n);
	while(nrf->GetStatus().busy);	
	nrf->Control(ARM_SPI_CONTROL_SS,0);	
	return 1;
}

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/

int nrf_read_stat(void){
	unsigned char stat;
	nrf_read(NRF_R_REGISTER|NRF_CMD_STATUS,&stat,1);
	return stat;
}

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/

int nrf_write_stat(unsigned char stat){
	nrf_write(NRF_W_REGISTER|NRF_CMD_STATUS,&stat,1);
	return stat;
}

/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/

int nrf_set_addr(unsigned char *addr){
	if(NRF_profile.tx_addr[0]!=addr[0]||NRF_profile.tx_addr[1]!=addr[1]||NRF_profile.tx_addr[2]!=addr[2]
		||NRF_profile.tx_addr[3]!=addr[3]||NRF_profile.tx_addr[4]!=addr[4]){
	   nrf_write(NRF_W_REGISTER|NRF_CMD_RX_ADDR_P0,addr,5);
	  nrf_write(NRF_W_REGISTER|NRF_CMD_TX_ADDR,addr,5);	
		}
	return 1;
}


/*********************************************************************//**
 * @brief            
 * @param[in]    
 * @return         
 **********************************************************************/
#define NRF_MODE_PTX 0
#define NRF_MODE_PRX 1

void nrf_set_mode(int mode){
	unsigned char arg;
	if(mode) arg=0xf;
	else     arg=0xe;
  nrf_write(NRF_W_REGISTER|NRF_CMD_CONFIG,&arg,1);	
}

void nrf_enable_rf(int mode){
	if(mode) GPIO_PinWrite(0,1,1);
	else     GPIO_PinWrite(0,1,0);
}

void nrf_enable_irq(int irq_req){
	LPC_GPIOINT->IO0IntClr=1;
	LPC_GPIOINT->IO0IntEnF=1;
	if(irq_req)
	NVIC_EnableIRQ(EINT3_IRQn);
}

int nrf_irq_stat(void){
	return LPC_GPIOINT->IO0IntStatF & 1;
}

void EINT3_IRQHandler(void){
	NRF_transfer_res=1;
	nrf_enable_rf(0);
	NVIC_DisableIRQ(EINT3_IRQn);
}



void NRF_init(void){
	unsigned char arg[5];
	nrf=&Driver_SPI0;
	nrf->Initialize(nrf_send_call_back);
	nrf->PowerControl(ARM_POWER_FULL);
  nrf->Control(ARM_SPI_MODE_MASTER|
	             ARM_SPI_DATA_BITS(8)|
	             ARM_SPI_SS_MASTER_SW,8000000);

	
	arg[0]=3;
	nrf_write(NRF_W_REGISTER|NRF_CMD_SETUP_AW,arg,1);
	
	arg[0]=3;
	nrf_write(NRF_W_REGISTER|NRF_CMD_EN_RXADDR,arg,1);	
	
	arg[0]=0xf|1<<4;
	nrf_write(NRF_W_REGISTER|NRF_CMD_SETUP_RETR,arg,1);	

	arg[0]=0;
	nrf_write(NRF_W_REGISTER|NRF_CMD_RF_CH,arg,1);	

	arg[0]=0xf;
	nrf_write(NRF_W_REGISTER|NRF_CMD_RF_SETUP,arg,1);	

	arg[0]=4;
	nrf_write(NRF_W_REGISTER|NRF_CMD_FEATURE,arg,1);
	
	arg[0]=3;
	nrf_write(NRF_W_REGISTER|NRF_CMD_DYNPL,arg,1);		

  arg[0]=0XF1;
	arg[1]=0XE6;
	arg[2]=0X6E;
	arg[3]=0X1F;
	arg[4]=0XAA;
  nrf_set_addr(arg);

  arg[0]=0xf;
	nrf_write(NRF_W_REGISTER|NRF_CMD_CONFIG,arg,1);	
	
	GPIO_SetDir(0,1,GPIO_DIR_OUTPUT);

 NRF_profile.ask_mode=1;
 NRF_profile.rf_mode=1;
}



/*********************************************************************//**
 * @brief        new group api for transmite better than old api    
 * @param[in]    and able to use rtx function for delay
 * @return         effiecent for real timee applications
 **********************************************************************/

void nrf_read_data(unsigned char *data,int size){
	nrf_read(NRF_R_RX_PAYLOAD,data,size);
}

void nrf_write_data(unsigned char *data,int size){
	nrf_write(NRF_W_TX_PAYLOAD,data,size);
}


/*********************************************************************//**
 * @brief        last arg has  two diffrent action    
 * @param[in]    1 -if auto ack =1 used as time out timer
 * @return       2 -if auto ack =0 used as send timer wait 
 **********************************************************************/




int nrf_translate_en(int auto_ack_mode,unsigned char *address,int send_time_ms_time_out){
	unsigned char gpv0;
	
	nrf_write_stat(0x70);
	nrf_set_addr(address);
  nrf_enable_irq(1);
	
	
	if(auto_ack_mode!=NRF_profile.ask_mode){
  if(auto_ack_mode){
	gpv0=1;                           
	NRF_profile.ask_mode=1;
	}
	else{
	gpv0=0;                           
	NRF_profile.ask_mode=0;
	}		
	nrf_write(NRF_W_REGISTER|NRF_CMD_EN_AA,&gpv0,0);		
	}                

	if(NRF_profile.rf_mode==1){
	nrf_set_mode(NRF_MODE_PTX);
	NRF_profile.rf_mode=0;
	}
	

	

	
	if(auto_ack_mode){
		nrf_enable_rf(1);
		for(gpv0=0;gpv0<send_time_ms_time_out;gpv0++){
			if(nrf_irq_stat()) break;
			osDelay(1);                      //you can clear stat in here and continue translate
		}
	}
	else{
	  nrf_write(NRF_REUSE_TX_PL,&gpv0,0);
	  nrf_enable_rf(1);		
		osDelay(send_time_ms_time_out);
	}
	
	nrf_enable_rf(0);
  gpv0=nrf_read_stat();
	if(gpv0 & 0x20) return 1;
	return 0;
}







void nrf_download_en(int auto_ack_mode,unsigned char *address,int pipe){
  unsigned char gpv0;

	nrf_write_stat(0x70);
	nrf_set_addr(address);
  nrf_enable_irq(0);
	
	if(auto_ack_mode!=NRF_profile.ask_mode){
  if(auto_ack_mode){
	gpv0=1;                           
	NRF_profile.ask_mode=1;
	}
	else{
	gpv0=0; //designed for pipe zero                           
	NRF_profile.ask_mode=0;
	}		
	nrf_write(NRF_W_REGISTER|NRF_CMD_EN_AA,&gpv0,0);		
	}                

	if(NRF_profile.rf_mode==0){
	nrf_set_mode(NRF_MODE_PRX);
	NRF_profile.rf_mode=1;
	}
	nrf_enable_rf(1);	
	
}



int nrf_download_end_check(void){
	if(nrf_read_stat() & 0x40){
	nrf_enable_rf(0);	
	return 1;
	}
  return 0;	
}


