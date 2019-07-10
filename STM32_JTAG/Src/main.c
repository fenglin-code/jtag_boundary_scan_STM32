
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "Tap.h"
#include "TAP_LookUpTable.h"
#include "global.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
StateTable stateTable[16][16] = {
  // current state is TEST_LOGIC_RESET
  {{1, TEST_LOGIC_RESET}, {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE},
   {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE},
   {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE},
   {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE},
   {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE}, {0, RUN_TEST_IDLE},
   {0, RUN_TEST_IDLE}},

  // current state is RUN_TEST_IDLE
  {{1, SELECT_DR_SCAN}, {0, RUN_TEST_IDLE}, {1, SELECT_DR_SCAN},
   {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
   {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
   {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
   {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
   {1, SELECT_DR_SCAN}},

  // current state is SELECT_DR_SCAN
  {{1, SELECT_IR_SCAN}, {1, SELECT_IR_SCAN}, {X, X},
   {0, CAPTURE_DR}, {0, CAPTURE_DR}, {0, CAPTURE_DR},
   {0, CAPTURE_DR}, {0, CAPTURE_DR}, {0, CAPTURE_DR},
   {1, SELECT_IR_SCAN}, {1, SELECT_IR_SCAN}, {1, SELECT_IR_SCAN},
   {1, SELECT_IR_SCAN}, {1, SELECT_IR_SCAN}, {1, SELECT_IR_SCAN},
   {1, SELECT_IR_SCAN}},

  // current state is CAPTURE_DR
  {{1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {X, X}, {0, SHIFT_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}},

  // current state is SHIFT_DR
  {{1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {0, SHIFT_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
   {1, EXIT1_DR}},

  // current state is EXIT1_DR
  {{1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
   {1, UPDATE_DR}, {0, PAUSE_DR}, {X, X},
   {0, PAUSE_DR}, {0, PAUSE_DR}, {1, UPDATE_DR},
   {1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
   {1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
   {1, UPDATE_DR}},

  // current state is PAUSE_DR
  {{1, EXIT2_DR}, {1, EXIT2_DR}, {1, EXIT2_DR},
   {1, EXIT2_DR}, {1, EXIT2_DR}, {1, EXIT2_DR},
   {0, PAUSE_DR}, {1, EXIT2_DR}, {1, EXIT2_DR},
   {1, EXIT2_DR}, {1, EXIT2_DR}, {1, EXIT2_DR},
   {1, EXIT2_DR}, {1, EXIT2_DR}, {1, EXIT2_DR},
   {1, EXIT2_DR}},

   // current state is EXIT2_DR
   {{1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
    {1, UPDATE_DR}, {0, SHIFT_DR}, {0, SHIFT_DR},
    {0, SHIFT_DR}, {X, X}, {1, UPDATE_DR},
    {1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
    {1, UPDATE_DR}, {1, UPDATE_DR}, {1, UPDATE_DR},
    {1, UPDATE_DR}},

    // current state is UPDATE_DR
    {{1, SELECT_DR_SCAN}, {0, RUN_TEST_IDLE}, {1, SELECT_DR_SCAN},
     {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
     {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {X, X},
     {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
     {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
     {1, SELECT_DR_SCAN}},

     // current state is SELECT_IR_SCAN
     {{1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET},
      {1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET},
      {1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET}, {1, TEST_LOGIC_RESET},
      {X, X}, {0, CAPTURE_IR}, {0, CAPTURE_IR},
      {0, CAPTURE_IR}, {0, CAPTURE_IR}, {0, CAPTURE_IR},
      {0, CAPTURE_IR}},

     // current state is CAPTURE_IR
     {{1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_DR},
      {1, EXIT1_DR}, {1, EXIT1_DR}, {1, EXIT1_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}, {X, X}, {0, SHIFT_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}},

     // current state is SHIFT_IR
     {{1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {0, SHIFT_IR},
      {1, EXIT1_IR}, {1, EXIT1_IR}, {1, EXIT1_IR},
      {1, EXIT1_IR}},

     // current state is EXIT1_IR
     {{1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
      {1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
      {1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
      {1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
      {1, UPDATE_IR}, {0, PAUSE_IR}, {0, PAUSE_IR},
      {1, UPDATE_IR}},

     // current state is PAUSE_IR
     {{1, EXIT2_IR}, {1, EXIT2_IR}, {1, EXIT2_IR},
      {1, EXIT2_IR}, {1, EXIT2_IR}, {1, EXIT2_IR},
      {1, EXIT2_IR}, {1, EXIT2_IR}, {1, EXIT2_IR},
      {1, EXIT2_IR}, {1, EXIT2_IR}, {1, EXIT2_IR},
      {1, EXIT2_IR}, {0, PAUSE_IR}, {1, EXIT2_IR},
      {1, EXIT2_IR}},

      // current state is EXIT2_IR
      {{1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
       {1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
       {1, UPDATE_IR}, {1, UPDATE_IR}, {1, UPDATE_IR},
       {1, UPDATE_IR}, {1, UPDATE_IR}, {0, SHIFT_IR},
       {0, SHIFT_IR}, {0, SHIFT_IR}, {1, UPDATE_IR},
       {1, UPDATE_IR}},

       // current state is UPDATE_IR
       {{1, SELECT_DR_SCAN}, {0, RUN_TEST_IDLE}, {1, SELECT_DR_SCAN},
        {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
        {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
        {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
        {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN}, {1, SELECT_DR_SCAN},
        {X, X}},


};

int writeDR = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void writeTms(int tms){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, tms);
}

void setClock(int clk){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, clk);
}

void writeTdi(int data){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, data);
}

void clockGen(int ms){
	setClock(1);
	HAL_Delay(ms);
	setClock(0);
	HAL_Delay(ms);
}

void resetTapState(){
  int i = 0;

  while(i < 5){
	writeTms(1);
	HAL_Delay(10);
	setClock(1);
	HAL_Delay(50);
	setClock(0);
	HAL_Delay(50);
    i++;
  }
}

uint64_t readTapTdi(int length){
  uint64_t data = 0;
  int i = 0;

  while(length > 0){
	setClock(1);
	HAL_Delay(50);
	setClock(0);
    data |= HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) << i;
    HAL_Delay(50);
    length--;
    i++;
  }
  return data;
}


void jtagWriteBits(uint64_t data, int length){
  int dataMask = 1;
  int oneBitData = 0;
  int i = 0;

  // noted that last bit of data must be set at next tap state
  while(length > 0){
    oneBitData = dataMask & data;
    writeTms(0);
    HAL_Delay(5);
    setClock(1);
    writeTdi(oneBitData);
    HAL_Delay(50);
    setClock(0);
    HAL_Delay(50);
    length--;
    data = data >> 1;
    writeDR |= oneBitData << (1*i);
    i++;
  }
  oneBitData = dataMask & data;
  writeTms(1);
  HAL_Delay(5);
  setClock(1);
  writeTdi(oneBitData);
  HAL_Delay(50);
  setClock(0);
  HAL_Delay(50);
  writeDR |= oneBitData << (1*i);
}

void tapTravelFromTo(TapState start, TapState end){
    TapState currentState = start;
    int tmsRequired;
    int i = 0;


      while(currentState != end){
          tmsRequired = getTmsRequired(currentState, end);
          writeTms(tmsRequired);
          HAL_Delay(5);
          setClock(1);
          HAL_Delay(50);
          writeTms(0);
          setClock(0);
          HAL_Delay(50);

          currentState = stateTable[currentState][end].nextState;
          i++;
          }
}

void loadJtagIR(int data){
  resetTapState();
  tapTravelFromTo(TEST_LOGIC_RESET, SHIFT_IR);
  jtagWriteBits(data, 9);
  tapTravelFromTo(EXIT1_IR, RUN_TEST_IDLE);
}

void loadJtagDR(int data, int length){
  tapTravelFromTo(RUN_TEST_IDLE, SHIFT_DR);
  jtagWriteBits(data, length);
  tapTravelFromTo(EXIT1_DR, RUN_TEST_IDLE);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)

{
  /* USER CODE BEGIN 1 */
	volatile uint64_t val = 0;
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	loadJtagIR(IDCODE);
	loadJtagDR(0xffff,32);
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/