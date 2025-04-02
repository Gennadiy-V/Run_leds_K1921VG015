/*==============================================================================
 * Пример работы GPIO для K1921VG015
 *------------------------------------------------------------------------------
 * НИИЭТ, Александр Дыхно <dykhno@niiet.ru>
 *==============================================================================
 * ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО
 * ГАРАНТИЙ, ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ
 * ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ
 * НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ
 * ПРЕДНАЗНАЧЕНО ДЛЯ ОЗНАКОМИТЕЛЬНЫХ ЦЕЛЕЙ И НАПРАВЛЕНО ТОЛЬКО НА
 * ПРЕДОСТАВЛЕНИЕ ДОПОЛНИТЕЛЬНОЙ ИНФОРМАЦИИ О ПРОДУКТЕ, С ЦЕЛЬЮ СОХРАНИТЬ ВРЕМЯ
 * ПОТРЕБИТЕЛЮ. НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ
 * ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА ПРЯМОЙ ИЛИ КОСВЕННЫЙ УЩЕРБ, ИЛИ
 * ПО ИНЫМ ТРЕБОВАНИЯМ, ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ
 * ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.
 *
 *                              2024 АО "НИИЭТ"
 *==============================================================================
 */

//-- Includes ------------------------------------------------------------------
#include <K1921VG015.h>
#include <stdint.h>
#include <stdio.h>
#include <system_k1921vg015.h>
#include "retarget.h"



//-- Defines -------------------------------------------------------------------
#define GPIOA_ALL_Msk	0xFFFF
#define GPIOB_ALL_Msk	0xFFFF

#define LEDS_MSK	0xF0F0
#define LED0_MSK	(1 << 4)
#define LED1_MSK	(1 << 5)
#define LED2_MSK	(1 << 6)
#define LED3_MSK	(1 << 7)
#define LED4_MSK	(1 << 12)
#define LED5_MSK	(1 << 13)
#define LED6_MSK	(1 << 14)
#define LED7_MSK	(1 << 15)

char buff[120];

void TMR32_IRQHandler();

void delay (uint32_t val)
{
	// 10000 =    600 uS  при нормальной тактовой

	for(;val > 0; val--)
		asm("NOP");
}

void Send_buff(char *a)
{
  uint8_t i=0;
  while ((i<120) && (a[i]!=0))
  {
	  retarget_put_char(a[i]);
    i++;
  }
}

void BSP_led_init()
{
	//Разрешаем тактирование GPIOA
	RCU->CGCFGAHB_bit.GPIOAEN = 1;
	RCU->CGCFGAHB_bit.GPIOBEN = 1;

	//Включаем  GPIOA
	RCU->RSTDISAHB_bit.GPIOAEN = 1;
    GPIOA->OUTENSET = LEDS_MSK;
	GPIOA->DATAOUTSET = LEDS_MSK;

	//Включаем  GPIOB
	RCU->RSTDISAHB_bit.GPIOBEN = 1;
    GPIOB->OUTENSET = LEDS_MSK;
	GPIOB->DATAOUTSET = LEDS_MSK;
}

void TMR32_init(uint32_t period)
{
  RCU->CGCFGAPB_bit.TMR32EN = 1;
  RCU->RSTDISAPB_bit.TMR32EN = 1;

  //Записываем значение периода в CAPCOM[0]
  TMR32->CAPCOM[0].VAL = period-1;
  //Выбираем режим счета от 0 до значения CAPCOM[0]
  TMR32->CTRL_bit.MODE = 1;

  //Разрешаем прерывание по совпадению значения счетчика и CAPCOM[0]
  TMR32->IM = 2;

  // Настраиваем обработчик прерывания для TMR32
  PLIC_SetIrqHandler (Plic_Mach_Target, IsrVect_IRQ_TMR32, TMR32_IRQHandler);
  PLIC_SetPriority   (IsrVect_IRQ_TMR32, 0x1);
  PLIC_IntEnable     (Plic_Mach_Target, IsrVect_IRQ_TMR32);
}

//-- Peripheral init functions -------------------------------------------------
void periph_init()
{
	BSP_led_init();
	SystemInit();
	SystemCoreClockUpdate();
	BSP_led_init();
	retarget_init();

	delay (10000);

	//printf("K1921VG015 SYSCLK = %d MHz\r\n",(int)(SystemCoreClock / 1E6));

	printf("K1921VG015 SYSCLK = %d Hz\r\n",(int) SystemPll0Clock);
	printf("  UID[0] = 0x%X  UID[1] = 0x%X  UID[2] = 0x%X  UID[3] = 0x%X\r\n",(unsigned int)PMUSYS->UID[0], (unsigned int)PMUSYS->UID[1], (unsigned int)PMUSYS->UID[2], (unsigned int)PMUSYS->UID[3]);
    printf("  Start MY_Debug V1.0 by GGV \r\n");
}

//--- USER FUNCTIONS ----------------------------------------------------------------------


volatile uint32_t led_shift;

//******************************************************************************
//-- Main ----------------------------------------------------------------------
//******************************************************************************

int main(void)
{
  periph_init();
  TMR32_init(SystemCoreClock>>4);
  led_shift = LED4_MSK;
  InterruptEnable();

  while(1)
  {

	  delay (10000);// 400 uS
	  GPIOB->DATAOUTTGL = LED0_MSK;

  }

  return 0;
}


//-- IRQ INTERRUPT HANDLERS ---------------------------------------------------------------
void TMR32_IRQHandler()
{
	//GPIOA->DATAOUTTGL = led_shift;
	GPIOB->DATAOUTTGL = led_shift;  // toggle
	led_shift = led_shift << 1;
    if(led_shift > LED7_MSK) led_shift = LED4_MSK;
    //Сбрасываем флаг прерывания таймера
    TMR32->IC = 3;
}
