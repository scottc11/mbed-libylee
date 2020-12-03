
# inverting amplifier --> Gain = Rf/Rin
# non-inverting amplifier --> Gain = 1 + Rf/Rin

resolution = 65535

Vmin = float(input('Enter min DAC output voltage (after amplification): ') or "0")
Vmax = float(input('Enter max DAC output voltage (after amplification): ') or "5.82")
Vfloor = float(input('Enter voltage floor (before calibration): ') or "0.5")

numNotes = 12
numOctaves = 6

voltPerBit = (Vmax - Vmin) / resolution #  the change in voltage from ex. 22 to 23 == 0.00008880750744 volts

floor16bit = Vfloor / voltPerBit
range16Bit = 1 / voltPerBit
print('16 bit floor: ', floor16bit)
print('1 Volt Range (ie. 0.5v to 1.5v): ', range16Bit)


halfStep = range16Bit / numNotes
print('half step 16 bit value: ', halfStep)

VOLTAGE_MAP = []

for x in range( numNotes * numOctaves ):
    VOLTAGE_MAP.append(int(floor16bit + (halfStep*x)))

print(VOLTAGE_MAP) # 16,890.46