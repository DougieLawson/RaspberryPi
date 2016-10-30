cmake_minimum_required(VERSION 3.0)

project(unified_lcd)

add_executable(i2clcd i2cmain.c i2cLcd.c ipLcd.c commonLcd.c)
add_executable(spilcd spimain.c spiLcd.c ipLcd.c commonLcd.c)
add_executable(gpiolcd gpiomain.c gpioLcd.c ipLcd.c commonLcd.c)
add_executable(gpioCG charGenLCD.c gpioLcd.c commonLcd.c)
add_executable(i2cCG charGenLCD.c i2cLcd.c commonLcd.c)
add_executable(spiCG charGenLCD.c spiLcd.c commonLcd.c)

find_library(wiringPi_LIB wiringPi)

target_link_libraries(gpiolcd ${wiringPi_LIB})
target_link_libraries(gpioCG ${wiringPi_LIB})
