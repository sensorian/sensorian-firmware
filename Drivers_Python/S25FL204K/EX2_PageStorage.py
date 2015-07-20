#!/usr/bin/python
import sys
import time  
import S25FL204K as NorMem

STAT_REG1 = 0x05           #status register number one
STAT_REG2 = 0x35           #status register number two
STAT_REG3 = 0x33           #status register mumber three

pattern = ['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g',
'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g']

Page0 = 0x000000
Page1 = 0x000100

def main():
	flashMem = NorMem.S25FL204K()
	flashMem.writeStatusRegister(0x00)                         #Remove protection from all blocks writes to SR1
	time.sleep(1)
	print ("Flash memory demo.\r\n")
	
	print ("Reading and Writing individual bytes.\r\n")
	print ("Writing byte...\r\n")
	flashMem.writeByte(0x34,Page0)
	one = flashMem.readByte(Page0)
	print ("Byte: %s" % int(one[0]))
	
	print ("Reading and Writing Pages. \r\n")
	
	print ("Erasing Sector 1...\r\n")
	flashMem.sectorErase(Page0)
	time.sleep(2)									#needs to erase any page if data has been written before on it, otherwise no dice
	
	print ("Writing list on Page 1... \r\n")
	flashMem.writeArray(Page1,pattern,256)
	print ("Data: ")
	data = flashMem.readArray(Page1,256)
	for item in data:
		print ("%s " % item)
	
	time.sleep(1)
	flashMem.close()
	print ("End of demo. \r\n")

if __name__=="__main__":
    main()

