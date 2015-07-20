#!/usr/bin/python

import time  
import S25FL204K as NorMem

STAT_REG1 = 0x05           #Status register number one
STAT_REG2 = 0x35           #Status register number two
STAT_REG3 = 0x33           #Status register mumber three

def main():
	flashMem = NorMem.S25FL204K()                              #Create a Flash memory object
	flashMem.writeStatusRegister(0x00)                         #Remove protection from all blocks writes to SR1
	time.sleep(1)
	
	print ("NOR Flash memory demo.\r\n")
	print ("This example will read the chip factory ID ")
	print ("and display the contents of the status registers.")

	id = flashMem.readID()                                     #Read id - 0x01 ox16
	print "Manufacturer id: 0x%02x \n" % id
	a = flashMem.jedec_ID_Read()                               #0x01 0x40 0x17
	print "Jedec ID: %06x\n" % a
	
	time.sleep(1)
	cell = flashMem.readStatusRegister(STAT_REG1)
	print "Status register one: 0x%02X \n" % cell[0]
	cell = flashMem.readStatusRegister(STAT_REG2)
	print "Status register two: 0x%02X \n" % cell[0]
	cell = flashMem.readStatusRegister(STAT_REG3)
	print "Status register three: 0x%02X \n" % cell[0]
	time.sleep(1)
	
	flashMem.close()
	print ("End of demo.")

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print("")
