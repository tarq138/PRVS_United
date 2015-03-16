/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_adc.h"
#include "MDR32F9Qx_rst_clk.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef port;
ADC_InitTypeDef sADC;
ADCx_InitTypeDef sADCx;

/* Private function prototypes -----------------------------------------------*/

int main() {
	
	// Настройка тактирования ядра
	RST_CLK_DeInit();														// Set RST_CLK to default
	RST_CLK_HSEconfig(RST_CLK_HSE_ON);										// Enable HSE clock oscillator
	RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul7);	// Select HSE clock as CPU_PLL input clock source and set PLL multiplier to 7
	RST_CLK_CPU_PLLcmd(ENABLE);												// Enable CPU_PLL
	RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);							// Set CPU_C3_prescaler to 2
	RST_CLK_CPU_PLLuse(ENABLE);												// Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock
	RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);							// Select CPU_C3 clock on the CPU clock MUX
	// ----------------------
	
	// Настройка тактирования периферии
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);							// Enables the RTCHSE clock on PORTD
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);								// Enables the RTCHSE clock on ADC
	RST_CLK_PCLKcmd(RST_CLK_PCLK_DMA, ENABLE);								// Enables the RTCHSE clock on DMA
	// --------------------------------
	
	// Настройка портов
	PORT_StructInit(&port);
	port.PORT_Pin 				= PORT_Pin_10;
	port.PORT_OE 				= PORT_OE_OUT;
	port.PORT_FUNC 				= PORT_FUNC_PORT;
	port.PORT_MODE 				= PORT_MODE_DIGITAL;
	port.PORT_SPEED 			= PORT_SPEED_MAXFAST;
	PORT_Init(MDR_PORTD, &port);
	// ----------------
	
	// Настройка АЦП
	ADC_DeInit();				// Reset all ADC settings
	ADC_StructInit(&sADC);
	
	sADC.ADC_SynchronousMode      = ADC_SyncMode_Synchronous;
	sADC.ADC_StartDelay           = 0;
	sADC.ADC_TempSensor           = ADC_TEMP_SENSOR_Enable;
	sADC.ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Enable;
	sADC.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Enable;
	sADC.ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Disable;
	sADC.ADC_IntVRefTrimming      = 1;
	
	ADC_Init(&sADC);
	
	// Настройка АЦП1
	ADCx_StructInit(&sADCx);
	
	sADCx.ADC_ClockSource      = ADC_CLOCK_SOURCE_CPU;
	sADCx.ADC_SamplingMode     = ADC_SAMPLING_MODE_CICLIC_CONV;
	// sADCx.ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV;
	sADCx.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
	sADCx.ADC_ChannelNumber    = ADC_CH_TEMP_SENSOR;
	sADCx.ADC_Channels         = 0;
	sADCx.ADC_LevelControl     = ADC_LEVEL_CONTROL_Disable;
	sADCx.ADC_LowLevel         = 0;
	sADCx.ADC_HighLevel        = 0;
	sADCx.ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL;
	sADCx.ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_INEXACT;
	sADCx.ADC_Prescaler        = ADC_CLK_div_4;
	sADCx.ADC_DelayGo          = 0;
	
	ADC1_Init(&sADCx);
	// -------------
	
	ADC1_Cmd(ENABLE);
	ADC1_ITConfig(ADCx_IT_END_OF_CONVERSION, ENABLE);						// Enable ADC1 EOCIF interupts 
	NVIC->ISER[0] = (1<<ADC_IRQn);											// Enable ADC interrupt
	
	ADC1_Start();
	
	while(1) {
		
	}
}

void ADC_IRQHandler(void) {
	uint16_t tmp;
	
	if (ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) == SET) {
		MDR_PORTD->RXTX ^= PORT_Pin_10;
		
		tmp = MDR_ADC->ADC1_RESULT;
		
		MDR_ADC->ADC1_STATUS &= ~ADCx_IT_END_OF_CONVERSION;
	}
	
}
