#!/usr/bin/env python

# 
#/sys/bus/iio/devices/iio:device0/in_pressure_input
#/sys/bus/iio/devices/iio:device0/in_temp_input


class BMP180():
    def __init__(self, bus):
        self._pressure = None
        self._temperature = None

    def pressure(self):
        '''Returns a pressure value.  Returns None if no valid value is set
        yet.

        '''
        with open('/sys/bus/iio/devices/iio:device0/in_pressure_input', 'r') as press:
                value = float(press.read())
                self._pressure = value * 10.0
                return self._pressure

    def temperature(self):
        '''Returns a temperature value.  Returns None if no valid value is
        set yet.

        '''
        with open('/sys/bus/iio/devices/iio:device0/in_temp_input', 'r') as temp:
                value = float(temp.read())
                self._temperature = value / 1000.0
                return self._temperature

    def pressure_and_temperature(self):
        '''Returns pressure and temperature values as a tuple.  This call can
        save 1 transaction than getting a pressure and temperature
        values separetely.  Returns (None, None) if no valid values
        are set yet.

        '''
        self.pressure()
        self.temperature()
        return (self._pressure, self._temperature)


