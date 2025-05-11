#include "key.h"
#include "gpio.h"
uint8_t key_press(uint8_t mode){
	static uint8_t m=1;
	
	if(mode) m=1;
	if(m&&(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)==0||HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)==0||HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1)){
		HAL_Delay(100);
		m=0;
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)==0) return 1;
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)==0) return 2;
		else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==0) return 3;
	}
		else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_1)==1&&HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_2)==1&&HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0)==1){
		m=1;
			return 0;
		}				//无按键按下
		
	
}
			
