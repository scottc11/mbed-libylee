# via https://nickfever.com/Music/Blog/bpm-to-ms#:~:text=BPM%20to%20MS%20conversion%20example,3%2C000MS%20or%203%20seconds.


bpm = int(input('Enter BPM: '))
ppqn = int(input('Enter PPQN (pulses per quarter note): '))

secondsPerMinute = 60
millisecondsPerMinute = 60000      # 60s per minute, 1,000ms per second, so 60 * 1000 = 60,000ms
microsecondsPerMinute = 60000000   # 1000us per 1ms, so 60,000 * 1000 = 60,000,000

print('pulse length in seconds: ', (secondsPerMinute / bpm) / ppqn)
print('pulse length in milliseconds (ms): ', (millisecondsPerMinute / bpm) / ppqn)
print('pulse length in microseconds (us): ', (microsecondsPerMinute / bpm) / ppqn)

microArray = []
for x in range(40, 190):
    value = (microsecondsPerMinute / x) / ppqn
    microArray.append(int(value))

print(microArray)