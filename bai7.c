#include "stm32f10x.h"
#include <stdio.h>

#define LED_PIN    GPIO_Pin_13
#define LED_PORT   GPIOC
#define CS_PIN     GPIO_Pin_4
#define CS_PORT    GPIOA

void GPIO_Config(void);
void USART_Config(void);
void SPI_Config(void);
void USART_SendString(USART_TypeDef* USARTx, char *str);
uint8_t SPI_TransmitReceive(uint8_t data);

int main(void)
{
    uint8_t received;
    char msg[50];

    GPIO_Config();
    USART_Config();
    SPI_Config();

    USART_SendString(USART1, "SPI Master Test Start\r\n");

    while (1)
    {
       
        GPIO_ResetBits(CS_PORT, CS_PIN);      
        received = SPI_TransmitReceive(0x55); 
        GPIO_SetBits(CS_PORT, CS_PIN);       

        sprintf(msg, "Sent: 0x55, Received: 0x%02X\r\n", received);
        USART_SendString(USART1, msg);

        for (volatile int i = 0; i < 1000000; i++); 
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
  
    gpio.GPIO_Pin = LED_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_PORT, &gpio);
    GPIO_SetBits(LED_PORT, LED_PIN);


    gpio.GPIO_Pin = CS_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(CS_PORT, &gpio);
    GPIO_SetBits(CS_PORT, CS_PIN); 
}

void USART_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef gpio;
    USART_InitTypeDef usart;


    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

   
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    usart.USART_BaudRate = 9600;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &usart);

    USART_Cmd(USART1, ENABLE);
}

void SPI_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;


    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);


    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; 
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &spi);
    SPI_Cmd(SPI1, ENABLE);
}

uint8_t SPI_TransmitReceive(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    return SPI_I2S_ReceiveData(SPI1);
}

void USART_SendString(USART_TypeDef* USARTx, char *str)
{
    while (*str)
    {
        USART_SendData(USARTx, *str++);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    }
}