
# inverting amplifier --> Gain = Rf/Rin
# non-inverting amplifier --> Gain = 1 + Rf/Rin

resolution = 65535

Vmin = float(input('Enter DAC min output voltage (after amplification): ') or "0")
Vmax = float(input('Enter DAC max output voltage (after amplification): ') or "5.82")
Vfloor = float(input('Enter voltage floor (before calibration): ') or "0.5")

# numBits = int(input('DAC bit resolution: '))



numNotes = 12
numOctaves = 6

bitVoltage = (Vmax - Vmin) / resolution

floor16bit = Vfloor / bitVoltage
range16Bit = 1 / bitVoltage

print('16 bit floor: ', floor16bit)
print('1 Volt Range (ie. 0.5v to 1.5v): ', range16Bit)


# take the floor + 1 volt range 16 bit numbers and map them to a 12 tone scale across 5 octaves

halfStep = range16Bit / numNotes
print('half step 16 bit value: ', halfStep)

VOLTAGE_MAP = []

for x in range(60):
    VOLTAGE_MAP.append(int(floor16bit + (halfStep*x)))

print(VOLTAGE_MAP) # 16,890.46