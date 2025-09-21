#include "stm32f10x.h"
#include <stdint.h>

#define ST7735_CS_PIN      GPIO_Pin_4
#define ST7735_CS_PORT     GPIOA

#define ST7735_DC_PIN      GPIO_Pin_0
#define ST7735_DC_PORT     GPIOB

#define ST7735_RST_PIN     GPIO_Pin_1
#define ST7735_RST_PORT    GPIOB


#define CS_LOW()   GPIO_ResetBits(ST7735_CS_PORT, ST7735_CS_PIN)
#define CS_HIGH()  GPIO_SetBits(ST7735_CS_PORT, ST7735_CS_PIN)

#define DC_COMMAND() GPIO_ResetBits(ST7735_DC_PORT, ST7735_DC_PIN)
#define DC_DATA()    GPIO_SetBits(ST7735_DC_PORT, ST7735_DC_PIN)

#define RST_LOW()   GPIO_ResetBits(ST7735_RST_PORT, ST7735_RST_PIN)
#define RST_HIGH()  GPIO_SetBits(ST7735_RST_PORT, ST7735_RST_PIN)

void GPIO_Config(void);
void SPI_Config(void);
void ST7735_SendCommand(uint8_t cmd);
void ST7735_SendData(uint8_t data);
void ST7735_Init(void);
void ST7735_FillScreen(uint16_t color);
void DelayMs(uint32_t ms);

int main(void)
{
    GPIO_Config();
    SPI_Config();

    ST7735_Init();
    ST7735_FillScreen(0xF800); 

    while (1)
    {
        ST7735_FillScreen(0x07E0); 
        DelayMs(1000);
        ST7735_FillScreen(0x001F); 
        DelayMs(1000);
        ST7735_FillScreen(0xFFFF); 
        DelayMs(1000);
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef gpio;

    
    gpio.GPIO_Pin = ST7735_CS_PIN;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ST7735_CS_PORT, &gpio);
    CS_HIGH();

    
    gpio.GPIO_Pin = ST7735_DC_PIN;
    GPIO_Init(ST7735_DC_PORT, &gpio);
    DC_DATA();

    
    gpio.GPIO_Pin = ST7735_RST_PIN;
    GPIO_Init(ST7735_RST_PORT, &gpio);
    RST_HIGH();
}

void SPI_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitTypeDef gpio;
    SPI_InitTypeDef spi;

    
    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);

    
    spi.SPI_Direction = SPI_Direction_1Line_Tx; 
    spi.SPI_Mode = SPI_Mode_Master;
    spi.SPI_DataSize = SPI_DataSize_8b;
    spi.SPI_CPOL = SPI_CPOL_Low;
    spi.SPI_CPHA = SPI_CPHA_1Edge;
    spi.SPI_NSS = SPI_NSS_Soft;
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 
    spi.SPI_FirstBit = SPI_FirstBit_MSB;
    spi.SPI_CRCPolynomial = 7;

    SPI_Init(SPI1, &spi);
    SPI_Cmd(SPI1, ENABLE);
}

void SPI_Write(uint8_t data)
{
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
}

void ST7735_SendCommand(uint8_t cmd)
{
    CS_LOW();
    DC_COMMAND();
    SPI_Write(cmd);
    CS_HIGH();
}

void ST7735_SendData(uint8_t data)
{
    CS_LOW();
    DC_DATA();
    SPI_Write(data);
    CS_HIGH();
}

void ST7735_Init(void)
{
    
    RST_LOW();
    DelayMs(50);
    RST_HIGH();
    DelayMs(50);

    
    ST7735_SendCommand(0x01); /
    DelayMs(150);
    ST7735_SendCommand(0x11); 
    DelayMs(150);
    ST7735_SendCommand(0x29); 
}

void ST7735_FillScreen(uint16_t color)
{
    uint16_t x, y;

    
    ST7735_SendCommand(0x2A); 
    ST7735_SendData(0x00);
    ST7735_SendData(0x00);
    ST7735_SendData(0x00);
    ST7735_SendData(0x7F); 

    ST7735_SendCommand(0x2B);
    ST7735_SendData(0x00);
    ST7735_SendData(0x00);
    ST7735_SendData(0x00);
    ST7735_SendData(0x9F);

    ST7735_SendCommand(0x2C); 

    DC_DATA();
    CS_LOW();
    for (y = 0; y < 160; y++)
    {
        for (x = 0; x < 128; x++)
        {
            SPI_Write(color >> 8);
            SPI_Write(color & 0xFF);
        }
    }
    CS_HIGH();
}

void DelayMs(uint32_t ms)
{
    for (uint32_t i = 0; i < ms * 8000; i++)
        __NOP();
}
