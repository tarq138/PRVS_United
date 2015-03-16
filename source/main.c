/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
	
}
