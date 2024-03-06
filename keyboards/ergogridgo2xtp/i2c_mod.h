
#pragma once
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "apidef.h"
#include "i2c.h"


static inline uint8_t i2c_writeReg16(uint8_t devaddr, uint16_t regaddr, const uint8_t* data, uint16_t length, uint16_t timeout)
{
  uint8_t reg_and_data[length + 2];
  reg_and_data[0] = regaddr >> 8;
  reg_and_data[1] = regaddr & 0xFF;
  
  uint8_t i2c_temporary_buffer[I2C_MAX_DATA_LEN];
  uint8_t *p_send;

  if (data <= (uint8_t*)0xFFFFF)
  {
    // copy data on ROM to RAM
    if(sizeof(i2c_temporary_buffer)/sizeof(uint8_t) < length) return 1;
    memcpy(i2c_temporary_buffer, data, length);
    for (uint16_t i = 0; i < length; i++) {
      reg_and_data[i+2] = i2c_temporary_buffer[i];
    }
    p_send = reg_and_data;
  }
  else
  {
    for (uint16_t i = 0; i < length; i++) {
      reg_and_data[i+2] = data[i];
    }
    p_send = (uint8_t*)reg_and_data;
  }

  uint8_t status = i2c_transmit(devaddr, p_send, sizeof(p_send), timeout);
  return status;
}

static inline uint8_t i2c_readReg16(uint8_t devaddr, uint16_t regaddr, uint8_t* data, uint16_t length, uint16_t timeout) {
  uint8_t reg_split[2];
  reg_split[0] = regaddr >> 8;
  reg_split[1] = regaddr & 0x00FF;
  BMPAPI->i2cm.transmit(devaddr >> 1, reg_split, sizeof(reg_split));
  return BMPAPI->i2cm.receive(devaddr >> 1, data, length);
}