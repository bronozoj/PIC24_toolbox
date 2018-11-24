/**
 * @file  eeprom.c
 * @brief This file contains function wrappers for EEPROM usage.
 * @author Jaime Bronozo
 * 
 * This is a library for accessing the functionality of an Electrically
 * Eraseable & Programmable Read-Only Memory (EEPROM) chip/s connected
 * through the Inter-IC Communication protocol (I2C). It abstracts most of
 * the work needed to set up and use the on-chip I2C peripheral. This
 * library works in conjunction with the settings found in the
 * configuration file toolbox_settings.h in order to set the proper flags
 * to the module.
 * 
 * @date November 19, 2018
 **************************************************************************/

/// @cond
#define __LIBEEPROM_I2C_SETTINGS

#include "toolbox_settings.h"
#include "eeprom.h"

#if __LIBEEPROM_I2C_DISABLE != 1
/// @endcond
#define _EEPROM_BRG __I2C_BRG(_I2C_NUM)
#define _EEPROM_STAT __I2C_STAT(_I2C_NUM)
#define _EEPROM_CON __I2C_CON(_I2C_NUM)
#define _EEPROM_TRN __I2C_TRN(_I2C_NUM)
#define _EEPROM_RCV __I2C_RCV(_I2C_NUM)

#define __BUSCOL I2C1STATbits.BCL
#define _ACK 0
#define _NACK 1

#define _START1 0x100
#define _START2 0x200
#define _RSTART 0x300
#define _SEND 0x400
#define _STOP 0x500
#define _INIT 0x600
#define _SENDACK 0x600
#define _WSEND1 0x1000
#define _WSEND2 0x2000
#define _WRITE_START 0x10000
#define _WRITE_CALL 0x20000
#define _WRITE_ADDR 0x30000
#define _WRITE_SEND 0x40000
#define _WRITE_STOP 0x50000
#define _READ_START 0x60000
#define _READ_CALL 0x70000
#define _READ_ADDR 0x80000
#define _READ_RSTART 0x90000
#define _READ_RADDR 0xA0000
#define _READ_NACK 0xB0000
#define _READ_END 0xC0000
#define _POLL_START 0xD0000
#define _POLL_SEND 0xE0000
#define _POLL_STOP 0xF0000


#define ADDR_READ(x) 0xa1 | ((x) << 1)
#define ADDR_WRITE(x) 0xa0 | ((x) << 1)

int __eeprom_bus_recover();

uint32_t eeprom_errno = 0;

short unsigned int EEPROM_error(){
    return eeprom_errno & 0xff;
}

short unsigned int EEPROM_error2(){
    return (eeprom_errno >> 16) & 0xff;
}

short unsigned int EEPROM_error3(){
    return (eeprom_errno >> 8) & 0xff;
}

int __eeprom_start(){
    if(__BUSCOL){
        if(__eeprom_bus_recover() < 0){
            eeprom_errno = _EEPROM_FATAL_ERROR;
            return -1;
        }
        __BUSCOL = 0;
    }
    _EEPROM_CON.SEN = 1;
    Nop();
    do{
        if(__BUSCOL){
            if(__eeprom_bus_recover() < 0){
                eeprom_errno = _EEPROM_FATAL_ERROR | _START1;
                return -1;
            }
            _EEPROM_CON.SEN = 0;
            __BUSCOL = 0;
            continue;
        }
        else if(_EEPROM_STAT.IWCOL){
            eeprom_errno = _EEPROM_WRITE_BUF_COLLISION | _START1;
            return -1;
        }
        while(_EEPROM_CON.SEN);
    }while(0);
    if(__BUSCOL){
        eeprom_errno = _EEPROM_BUS_COLLISION | _START2;
        return -1;
    }
    return 0;
}

int __eeprom_restart(){
    __BUSCOL = 0;
    _EEPROM_CON.RSEN = 1;
    Nop();
    if(__BUSCOL){
        eeprom_errno = _EEPROM_BUS_COLLISION | _RSTART;
        return -1;
    }
    while(_EEPROM_CON.RSEN);
    return 0;
}

int __eeprom_bus_recover(){
    int i;

    _EEPROM_CON.RCEN = 0;
    _EEPROM_STAT.IWCOL = 0;
    _EEPROM_STAT.BCL = 0;

    _EEPROM_CON.I2CEN = 0;
    __LATx(_I2C_SDA) = 1;
    __LATx(_I2C_SCL) = 1;

    delay_us(10);
    if(__PORTx(_I2C_SCL) == 0){
        return -1;
    }

    for(i = 10; i > 0; i--){
        if(__PORTx(_I2C_SDA)){
            break;
        }\

        __LATx(_I2C_SCL) = 0;
        delay_us(10);
        __LATx(_I2C_SCL) = 1;
        delay_us(10);
    }
    if(!__LATx(_I2C_SCL) || !__LATx(_I2C_SDA)){
        return -2;
    }

    __LATx(_I2C_SDA) = 0;
    delay_us(10);
    __LATx(_I2C_SDA) = 1;
    delay_us(10);
    _EEPROM_CON.I2CEN = 1;
    return 0;
}

int __eeprom_byte_send(char data){
    __BUSCOL = 0;
    _EEPROM_TRN = data;
    while(_EEPROM_STAT.TRSTAT);
    if(__BUSCOL){
        eeprom_errno = _EEPROM_BUS_COLLISION | _SEND;
        return -1;
    }
    return _EEPROM_STAT.ACKSTAT;
}

int __eeprom_word_send(int16_t data){
    int ack1 = 0, ack2 = 0;
    ack1 = __eeprom_byte_send(data >> 8);
    if(ack1 == -1){
        eeprom_errno |= _WSEND1;
        return -1;
    }
    ack2 = __eeprom_byte_send(data & 0xff);
    if(ack2 == -1){
        eeprom_errno |= _WSEND2;
        return -1;
    }
    return (ack1 << 1) | ack2;
}

int __eeprom_byte_receive(){
    int count = 0;
    _EEPROM_CON.RCEN = 1;
    while(!_EEPROM_STAT.RBF);/*{
        if(count++ == 0xffff){
            eeprom_errno = _EEPROM_READ_TIMEOUT;
            return -1;
        }
    }*/
    return _EEPROM_RCV;
}

int __eeprom_read_ack(char ack){
    _EEPROM_CON.ACKDT = ack;
    _EEPROM_CON.ACKEN = 1;

    if(_EEPROM_STAT.I2COV){
        eeprom_errno = _EEPROM_READ_BUF_OVERFLOW | _SENDACK;
        return -1;
    }

    while(_EEPROM_CON.ACKEN);
    return 0;
}

int __eeprom_stop(){
    __BUSCOL = 0;
    _EEPROM_CON.PEN = 1;
    Nop();
    if(__BUSCOL){
        eeprom_errno = _EEPROM_BUS_COLLISION | _STOP;
        //return -1;
    }
    while(_EEPROM_CON.PEN);
    return 0;
}

void EEPROM_begin(){
    _EEPROM_BRG = _CLOCK_RATE;

    _EEPROM_CON.I2CEN = 1;
    _EEPROM_STAT.BCL = 0;
}

int EEPROM_write(char data, uint16_t mem_address, char dev_address){
    int fval;
    if(__eeprom_start()){
        eeprom_errno |= _WRITE_START;
        return -1;
    }

    fval = __eeprom_byte_send(ADDR_WRITE(dev_address));
    if(fval == -1){
        eeprom_errno |= _WRITE_CALL;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_CALL;
        return -1;
    }

    fval = __eeprom_word_send((int16_t) mem_address);
    if(fval == -1){
        eeprom_errno |= _WRITE_ADDR;
        return -1;
    }
    else if(fval > 0){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_ADDR;
        return -1;
    }

    fval = __eeprom_byte_send(data);
    if(fval == -1){
        eeprom_errno |= _WRITE_SEND;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_SEND;
        return -1;
    }

    if(__eeprom_stop()){
        eeprom_errno |= _WRITE_STOP;
        return -1;
    }
    return 0;
}

int EEPROM_write_page(char *data, int size, uint16_t mem_address, char dev_address){
    int fval, count;
    if(__eeprom_start()){
        eeprom_errno |= _WRITE_START;
        return -1;
    }

    fval = __eeprom_byte_send(ADDR_WRITE(dev_address));
    if(fval == -1){
        eeprom_errno |= _WRITE_CALL;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_CALL;
        return -1;
    }

    fval = __eeprom_word_send((int16_t) mem_address);
    if(fval == -1){
        eeprom_errno |= _WRITE_ADDR;
        return -1;
    }
    else if(fval > 0){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_ADDR;
        return -1;
    }

    for(count = 0; count < size; count++){
       fval = __eeprom_byte_send(data[count]);
        if(fval == -1){
            eeprom_errno |= _WRITE_SEND;
            return -1;
        }
        else if(fval == _NACK){
            eeprom_errno = _EEPROM_NOT_RESPONDING | _WRITE_SEND;
            return -1;
        } 
    }

    if(__eeprom_stop()){
        eeprom_errno |= _WRITE_STOP;
        return -1;
    }
    return count;
}

int EEPROM_read(uint16_t mem_address, char dev_address){
    int fval;
    char byte_out;
    if(__eeprom_start()){
        eeprom_errno |= _READ_START;
        return -1;
    }

    fval = __eeprom_byte_send(ADDR_WRITE(dev_address));
    if(fval == -1){
        eeprom_errno |= _READ_CALL;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_CALL;
        return -1;
    }

    fval = __eeprom_word_send((int16_t) mem_address);
    if(fval == -1){
        eeprom_errno |= _READ_ADDR;
        return -1;
    }
    else if(fval > 0){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_ADDR;
        return -1;
    }

    if(__eeprom_restart()){
        eeprom_errno |= _READ_RSTART;
        return -1;
    }

    fval = __eeprom_byte_send(ADDR_READ(dev_address));
    if(fval == -1){
        eeprom_errno |= _READ_RADDR;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_RADDR;
        return -1;
    }

    fval = __eeprom_byte_receive();
    if(fval == -1 || __eeprom_read_ack(_NACK)){
        eeprom_errno |= _READ_NACK;
        return -1;
    }

    if(__eeprom_stop()){
        eeprom_errno |= _READ_END;
        return -1;
    }

    byte_out = fval;

    return byte_out;
}

int EEPROM_read_delim(char *buf, int size, char *delim, uint16_t mem_address, char dev_address){
    int fval, count = 0, count2;
    if(__eeprom_start()){
        eeprom_errno |= _READ_START;
        return -1;
    }

    fval = __eeprom_byte_send(ADDR_WRITE(dev_address));
    if(fval == -1){
        eeprom_errno |= _READ_CALL;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_CALL;
        return -1;
    }

    fval = __eeprom_word_send((int16_t) mem_address);
    if(fval == -1){
        eeprom_errno |= _READ_ADDR;
        return -1;
    }
    else if(fval > 0){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_ADDR;
        return -1;
    }

    if(__eeprom_restart()){
        if(__eeprom_stop()){
            eeprom_errno |= _READ_RSTART;
            return -1;
        }
        if(__eeprom_start()){
            eeprom_errno |= _READ_RSTART;
            return -1;
        }
    }

    fval = __eeprom_byte_send(ADDR_READ(dev_address));
    if(fval == -1){
        eeprom_errno |= _READ_RADDR;
        return -1;
    }
    else if(fval == _NACK){
        eeprom_errno = _EEPROM_NOT_RESPONDING | _READ_RADDR;
        return -1;
    }

    while(count < size){
        fval = __eeprom_byte_receive();
        if(fval == -1){
            _READ_NACK;
            return -1;
        }
        buf[count] = fval;

        for(count2 = 0; delim[count2] != 0; count2++){
            if(delim[count2] == buf[count]){
                buf[count] = 0;
                break;
            }
        }
        count++;
        if(__eeprom_read_ack((count < size && buf[count - 1] != 0) ? _ACK : _NACK)){
        //if(__eeprom_read_ack(_ACK)){
            eeprom_errno |= _READ_NACK;
            return -1;
        }
        if(!buf[count-1]){
            count--;
            break;
        }
    }

    if(__eeprom_stop()){
        eeprom_errno |= _READ_END;
        return -1;
    }

    return count;
}

int EEPROM_isPresent(char dev_address){
    char retval; 
    _EEPROM_STAT.BCL = 0;
    if(__eeprom_start()){
        eeprom_errno |= _POLL_START;
        return -1;
    }

    retval = __eeprom_byte_send(ADDR_WRITE(dev_address));
    if(retval == -1){
        eeprom_errno |= _POLL_SEND;
        return -1;
    }
    
    if(__eeprom_stop()){
        eeprom_errno |= _POLL_STOP;
        return -1;
    }
    
    return !retval;
}

#endif