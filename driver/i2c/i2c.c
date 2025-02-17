#include "i2c.h"

static void i2c_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    gpio_init_structure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF;
    gpio_init_structure.GPIO_Speed = GPIO_Speed_100MHz;
    gpio_init_structure.GPIO_OType = GPIO_OType_OD;
    gpio_init_structure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &gpio_init_structure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1);
}

static void i2c_base_init(void)
{
    I2C_InitTypeDef i2c_init_structure;
    i2c_init_structure.I2C_Mode = I2C_Mode_I2C;
    i2c_init_structure.I2C_DutyCycle = I2C_DutyCycle_2;
    i2c_init_structure.I2C_OwnAddress1 = 0x00;
    i2c_init_structure.I2C_Ack = I2C_Ack_Enable;
    i2c_init_structure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &i2c_init_structure);
    
}

void i2c_init(void)
{
    i2c_gpio_init();
    i2c_base_init();
}

void i2c_start(void)
{
    I2C_GenerateSTART(I2C1, ENABLE);
}

void i2c_stop(void)
{
    I2C_GenerateSTOP(I2C1, ENABLE);
}

void i2c_send(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length)
{
    // Generate start condition
    i2c_start();
    
    // Wait for the start condition to be generated
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB));

    // Send the address
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    
    // Wait for the address to be sent
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));
    
    // Clear the ADDR flag
    I2C_ClearFlag(I2C1, I2C_FLAG_ADDR);

    // Send the register address
    I2C_SendData(I2C1, reg);
    
    // Wait for the data register to be empty
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE));

    // Send the data
    for (uint16_t i = 0; i < length; i++)
    {
        I2C_SendData(I2C1, data[i]);
        
        // Wait for the data register to be empty
        while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE));
    }

    // Generate stop condition
    i2c_stop();
}

void i2c_recv(uint8_t addr, uint8_t reg, uint8_t* data, uint16_t length)
{
    // Check if length is zero
    if (length == 0) 
        return;

    // Generate start condition
    i2c_start();
    
    // Wait for the start condition to be generated
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_SB));
    
    // Send the address
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);

    // Wait for the address to be sent
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));

    // Clear the ADDR flag
    I2C_ClearFlag(I2C1, I2C_FLAG_ADDR);

    // Send the register address
    I2C_SendData(I2C1, reg);

    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    // Wait for the address to be sent
    while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR));
    for (uint16_t i = 0; i < length; i++)
    {
        data[i] = I2C_ReceiveData(I2C1);
        // Wait for the data register to be empty
        while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE));
    }
    i2c_stop();

}