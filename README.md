# PIC General Usage Toolbox

This is a collection of libraries that can be used for PIC microcontroller programming.
The library is designed to be compatible for the MPLab X IDE using the XC16 compiler.
Simply include the header [PIC24_toolbox.h](PIC24_toolbox.h) to use various libraries available to it such as:

- *delay_us()*, *delay_ms()*, and *delay_s()* which take delay values in microseconds, milliseconds, and seconds respectively.
- various function for interfacing with generic 16x2 character lcd modules

## Configuration

Certain macros need to be configured for certain parts of the library to work.
All macros to be configured lies in the header file [toolbox_settings.h](utilities/toolbox_settings.h).
This header file is included by other C code in the library to use their respective configuration blocks.
It also uses and includes **xc.h** and **libpic30.h** libraries from the XC16 compiler to improve compatibility with all 16-bit PIC microcontrollers.

### Delay and libpic30.h

Regardless of which you use from it, the instruction frequency FCY must be defined to use the delay functions and any other library that relies on timed delays.
This value is used by **libpic30.h** which is then used by the other libraries in this package.

```C
#define FCY 4000000 //Instruction frequency in Hz
```

The functions *delay_us()* and *delay_ms()* are the same functions from **libpic30.h** and has the same limitations from it.
The function *delay_s()* works similarly to the other delay functions but in seconds.
Beware that this simply converts the unit time delay to a 32 bit cycle count delay and may be prone to overflow.

### Generic 16x2 LCD

Only 4-bit mode operation is supported for now.
It also waits for a set maximum processing time specific for every command rather than checking if the lcd is busy.
Configuring the macro for the pins connected to the lcd will allow you to use the lcd specific functions.
To initialize the lcd, *lcd_begin()* must be called before using other functions.
Check [lcd_generic.c](utilities/lcd_generic.c) for an in depth description of each function available.
This library is to be expanded more and is a work in progress.