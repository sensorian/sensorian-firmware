#!/usr/bin/python
import sys
import smbus
import time
import EEPROM as ee

DEL = 0.5

def main(argv):
	print "=== Real Time Clock and Calendar  ===" 
	print "*************************************" 
	rtc_eeprom = ee.EEPROM()			#create RTCC object
	
	wr_buff1 = [0x01,0x02,0x03,0x04,0x05,0x06,0x7,0x08]
	wr_buff2 = [0xa1,0xb2,0xc3,0xd4,0xe5,0xf6,0xa7,0x0a8]
	rd_buffer = [0,0,0,0,0,0,0,0]
	i = 0
	
	val = rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_SREG)
	print "Status reg: %X. \r\n" % int(val)
	
	rtc_eeprom.EEPROM_Write(rtc_eeprom.EEPROM_START,0xae)
	time.sleep(DEL)
	rtc_eeprom.EEPROM_Write(0x01,0x99)
	time.sleep(DEL);
	rtc_eeprom.EEPROM_Write(0x02,0xa9);
	time.sleep(DEL);

	print ("Reading individual bytes. \r\n")
	val = rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_START)
	print "EE0 0x%02X. \r\n" % val
	print ("EE1 0x%02X. \r\n" % rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_START+1))
	print ("EE2 0x%02X. \r\n" % rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_START+2))
	print ("EE3 0x%02X. \r\n" % rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_START+3))
	print ("EE4 0x%02X. \r\n" % rtc_eeprom.EEPROM_Read(rtc_eeprom.EEPROM_START+4))
	
	print ("Writing individual pages. \r\n") 
	rtc_eeprom.EEPROM_WritePage(rtc_eeprom.EEPROM_START,wr_buff1) 
	time.sleep(DEL) 
	print ("Reading individual pages. \r\n") 
	
	
	rd_buffer = rtc_eeprom.EEPROM_ReadPage(rtc_eeprom.EEPROM_START)
	for i in range (0 ,8):
		print ("EE%d 0x%02X. \r\n" % (i,rd_buffer[i]))
	
	
	print ("Writing individual pages. \r\n")
	rtc_eeprom.EEPROM_WritePage(rtc_eeprom.EEPROM_START+8,wr_buff2)
	time.sleep(DEL)
	print ("Reading individual pages. \r\n")
	rd_buffer = rtc_eeprom.EEPROM_ReadPage(rtc_eeprom.EEPROM_START+8)
	
	for i in range (0 ,8):
		print ("EE%d 0x%02X. \r\n" % (i,rd_buffer[i]))
	

if __name__ == '__main__':
    try:
        main(sys.argv[1:])
    except KeyboardInterrupt:
        print("")
		
