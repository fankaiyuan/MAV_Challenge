from smbus import SMBus
from time import sleep

x = 1
while True:
 try:
  i2cbus = SMBus(1)
  i2cbus.write_byte(0x70, 0x51)
  sleep(0.12)
  val = i2cbus.read_word_data(0x70, 0xe1)

  print (val >> 8) & 0xff | (val>>8 & 0xff), 'cm'
