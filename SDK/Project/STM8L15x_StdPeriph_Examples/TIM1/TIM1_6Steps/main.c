/**
  ******************************************************************************
  * @file    TIM1/TIM1_6Steps/main.c
  * @author  MCD Application Team
  * @version V1.5.2
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"

/** @addtogroup STM8L15x_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM1_6Steps
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TIM1_PERIOD                  4095
#define TIM1_PRESCALER                  0
#define TIM1_REPTETION_COUNTER          0

#define CCR1_VAL                     2047
#define CCR2_VAL                     1023
#define CCR3_VAL                      511

#define DEADTIME                        1
#define TIM4_PERIOD                   255

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void CLK_Config(void);
static void GPIO_Config(void);
static void TIM1_Config(void);
static void TIM4_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
   /* CLK configuration -------------------------------------------*/
  CLK_Config(); 

   /* GPIO configuration -------------------------------------------*/
  GPIO_Config(); 

  /*-----------------------------------------------------------------------------
  The STM8L15x TIM1 peripheral offers the possibility to program in advance the 
  configuration for the next TIM1 outputs behaviour (step) and change the configuration
  of all the channels at the same time. This operation is possible when the COM 
  (commutation) event is used.
  The COM event can be generated by software by setting the COM bit in the TIM1_EGR
  register or by hardware.
  In this example, a software COM event is generated each 180 ms: using the TIM4 Update 
  interrupt.
  The TIM1 is configured in Timing Mode, each time a COM event occurs, 
  a new TIM1 configuration will be set in advance.
  The following Table  describes the TIM1 Channels states:
              -----------------------------------------------
             | Step1 | Step2 | Step3 | Step4 | Step5 | Step6 |
   ----------------------------------------------------------
  |Channel1  |  PWM  |   0   |   0   |   0   |   0   |  PWM  |
   ----------------------------------------------------------
  |Channel1N |   0   |   0   |  PWM  |  PWM  |   0   |   0   |
   ----------------------------------------------------------
  |Channel2  |   0   |   0   |   0   |  PWM  |  PWM  |   0   |
   ----------------------------------------------------------
  |Channel2N |  PWM  |  PWM  |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3  |   0   |  PWM  |  PWM  |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3N |   0   |   0   |   0   |   0   |  PWM  |  PWM  |
   ----------------------------------------------------------
  -----------------------------------------------------------------------------*/

   /* TIM1 configuration -------------------------------------------*/
  TIM1_Config(); 

   /* TIM4 configuration -------------------------------------------*/
  TIM4_Config(); 
    
  /* Enable general interrupts */
  enableInterrupts();

  while (1)
  {}
}

/**
  * @brief  Configure peripherals Clock   
  * @param  None
  * @retval None
  */
static void CLK_Config(void)
{
  /* Enable TIM1 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
  /* Enable TIM4 clock */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
}

/**
  * @brief  Configure TIM Channels GPIO 
  * @param  None
  * @retval None
  */
static void GPIO_Config(void)
{
  /* TIM1 Channels 1, 2, 3 and 1N configuration: PD2, PD4, PD5 and PD7 */
  GPIO_Init(GPIOD, GPIO_Pin_2 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);

  /* TIM1 Channels 2N and 3N configuration: PE1 and PE2 */
  GPIO_Init(GPIOE, GPIO_Pin_1 | GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);

  /* TIM1 Break input pin configuration */
  GPIO_Init(GPIOD, GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);

}

/**
  * @brief  Configure TIM1 peripheral 
  * @param  None
  * @retval None
  */
static void TIM1_Config(void)
{
  /* Time Base configuration */
  TIM1_TimeBaseInit(TIM1_PRESCALER, TIM1_CounterMode_Up, TIM1_PERIOD, TIM1_REPTETION_COUNTER);

  /* Channels 1, 2 and 3 Configuration in TIMING mode */
  TIM1_OC1Init(TIM1_OCMode_Timing, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR1_VAL,
               TIM1_OCPolarity_High, TIM1_OCNPolarity_High, TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);
  TIM1_OC2Init(TIM1_OCMode_Timing, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR2_VAL,
               TIM1_OCPolarity_High, TIM1_OCNPolarity_High, TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);
  TIM1_OC3Init(TIM1_OCMode_Timing, TIM1_OutputState_Enable, TIM1_OutputNState_Enable, CCR3_VAL,
               TIM1_OCPolarity_High, TIM1_OCNPolarity_High, TIM1_OCIdleState_Reset, TIM1_OCNIdleState_Reset);

  /* Automatic Output Enable, Break, dead time and lock configuration*/
  TIM1_BDTRConfig(TIM1_OSSIState_Enable, TIM1_LockLevel_Off, DEADTIME,
                  TIM1_BreakState_Enable, TIM1_BreakPolarity_Low, TIM1_AutomaticOutput_Disable);

  TIM1_CCPreloadControl(ENABLE);
  TIM1_ITConfig(TIM1_IT_COM, ENABLE);
  /* Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);
}

/**
  * @brief  Configure TIM4 peripheral 
  * @param  None
  * @retval None
  */
static void TIM4_Config(void)
{
  /* TIM4 Peripheral Configuration */
  /* Time Base configuration */
  TIM4_TimeBaseInit(TIM4_Prescaler_128, TIM4_PERIOD);
  /* TIM4 update interrupt enable */
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  /* TIM4 counter enable */
  TIM4_Cmd(ENABLE);
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
