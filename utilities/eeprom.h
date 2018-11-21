/** 
 * @file  eeprom.h
 * @brief This file contains function wrappers for EEPROM usage.
 * @author Jaime Bronozo
 * 
 * This is a header file for eeprom.c which must be included to any source
 * files that require usage of analog reading related functions. This
 * library is dynamically included in the main header PIC24_toolbox.h
 * 
 * @date November 8, 2018
 **************************************************************************/

#ifndef __EEPROM_I2C_TOOLBOX_H__
#define __EEPROM_I2C_TOOLBOX_H__

#define _EEPROM_BUS_COLLISION 1
#define _EEPROM_WRITE_BUF_COLLISION 2
#define _EEPROM_NOT_RESPONDING 3
#define _EEPROM_READ_BUF_OVERFLOW 4
#define _EEPROM_READ_TIMEOUT 5

void EEPROM_begin();
short unsigned int EEPROM_error();
int EEPROM_isPresent(char dev_address);
int EEPROM_write_page(char *data, int size, uint16_t mem_address, char dev_address);
int EEPROM_read(uint16_t mem_address, char dev_address);
int EEPROM_read_delim(char *buf, int size, char *delim, uint16_t mem_address, char dev_address);

#define EEPROM_read_string(buf, size, mem_address, dev_address) EEPROM_read_delim(buf, size, "", mem_address, dev_address)

#endif