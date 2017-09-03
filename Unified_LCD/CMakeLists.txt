#
# Copyright (C) Dougie Lawson 2015-2017, all rights reserved.
#

cmake_minimum_required(VERSION 3.0)

project(unified_lcd)

add_executable(i2clcd main.c i2cLcd.c ipLcd.c commonLcd.c)
add_executable(i2cCG charGenLCD.c i2cLcd.c commonLcd.c)
add_executable(spilcd main.c spiLcd.c ipLcd.c commonLcd.c)
add_executable(spiCG charGenLCD.c spiLcd.c commonLcd.c)
add_executable(gpiolcd main.c gpioLcd.c ipLcd.c commonLcd.c)
add_executable(gpioCG charGenLCD.c gpioLcd.c commonLcd.c)
add_executable(gpio31855 gpio31855.c gpioLcd.c get31855.c commonLcd.c)
add_executable(gpioboth gpioboth.c gpioLcd.c get6675.c get31855.c commonLcd.c)
add_executable(gpio6675 gpio6675.c gpioLcd.c get6675.c commonLcd.c)
add_executable(pcflcd main.c pcfLcd.c ipLcd.c commonLcd.c)
add_executable(pcfCG charGenLCD.c pcfLcd.c commonLcd.c)
add_executable(pcf31855 pcf31855.c pcfLcd.c get31855.c commonLcd.c)
add_executable(pcf6675 pcf6675.c pcfLcd.c get6675.c commonLcd.c)
add_executable(pcfboth pcfboth.c pcfLcd.c get6675.c get31855.c commonLcd.c)
add_executable(iptest ipTest.c)

set_target_properties(i2clcd i2cCG PROPERTIES COMPILE_FLAGS -Di2c)
set_target_properties(spilcd spiCG PROPERTIES COMPILE_FLAGS -DSPI)
set_target_properties(gpiolcd gpioCG gpio31855 gpio6675 gpioboth PROPERTIES COMPILE_FLAGS -Dgpio)
set_target_properties(pcflcd pcfCG pcf31855 pcf6675 pcfboth PROPERTIES COMPILE_FLAGS -Dpcf)

find_library(wiringPi_LIB wiringPi)

target_link_libraries(gpiolcd ${wiringPi_LIB})
target_link_libraries(gpioCG ${wiringPi_LIB})
target_link_libraries(gpio31855 ${wiringPi_LIB})
target_link_libraries(gpio6675 ${wiringPi_LIB})
target_link_libraries(gpioboth ${wiringPi_LIB})
