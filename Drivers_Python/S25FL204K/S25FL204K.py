#!/usr/bin/python


__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"


import RPi.GPIO as GPIO
import time
import spidev as spi

spi = spi.SpiDev()

##################******** Commands ************************************/
CMD_RDSR1 		= 0x05
CMD_RDSR2 		= 0x35
CMD_RDSR3 		= 0x33
CMD_WREN 		= 0x06
CMD_EWSR 		= 0x50           #write enable volatile
CMD_WRDI 		= 0x04
CMD_SETBURST 	= 0x77
PAGE_PROGRAM 	= 0x02
CMD_READ_DATA 	= 0x03
CMD_READ_HS 	= 0x0B
ERASE_SECTOR 	= 0x20
ERASE_BLOCK32 	= 0x52
ERASE_BLOCK64	= 0xD8
ERASE_CHIP 		= 0x60
CMD_DEEPSLP 	= 0xB9
CMD_RESDPD 		= 0xAB           #release from sleep
CMD_RDID 		= 0x90
JEDEC			= 0x9F
CMD_RDSFDP 		= 0x5A
CMD_RDSECREG 	= 0x48
CMD_ERSECREG 	= 0x44           #erase security reg
CMD_EHLD 		= 0xAA
S64MANID 		= 0x01
S64DEVID 		= 0x16	
CMD_RSID 		= 0x88
CMD_PSID		= 0xA5
CMD_LSID		= 0x85

#resume from deep sleep
CMD_WRSR  		= 0x01           #write status register
STAT_REG1 		= 0x05           #status register number one
STAT_REG2 		= 0x35           #status register number two
STAT_REG3 		= 0x33           #status register mumber three

##################Memory map#####################################

#sector 4 kbyte
#block 64 kbyte
SECTOR_SIZE 	= 4096       #0x000-0xfff
SECTOR_COUNT 	= 128
PAGE_NUM 		= 2048       #16 pages per sector
PAGE_INCR 		= 0x100      #256 bytes


class S25FL204K(object):
	"""Class object for 4Mbit NOR flash memory chip"""
	
	def __init__(self):
		"""Configures the SPI bus and the chip select pin."""
		spi.open(0,1)             		#will open bus 0, CE1. 
		spi.max_speed_hz = 8000000
		GPIO.setwarnings(False)
		GPIO.setmode(GPIO.BOARD)
		self.CE_OUTPUT()
		self.CE_DESELECT()	
	
	def CE_OUTPUT(self):
		"""
		Configure Chip Enable pin as output.
		
		
		:param none: 
		:returns: none
		"""
		GPIO.setup(26, GPIO.OUT)	#Set GPIO26 as output
			
	def CE_SELECT(self):
		"""
		Select flash memory chip.
		
		
		:param none: 
		:returns: none
		"""
		GPIO.output(26, 0)
			
	def CE_DESELECT(self):
		"""
		Deselect flash memory.
		
		
		:param none: 
		:returns: none
		"""
		GPIO.output(26, 1)

	def writeByte(self,data,address):
		"""
		Write one byte at a specific address in the memory chip.
		
		
		:param data: 
		:param address: 
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                  #Send WREN command
		self.CE_DESELECT()
		
		self.CE_SELECT()
		spi.writebytes([PAGE_PROGRAM])
		spi.writebytes([(address&0xFF0000)>>16,(address&0xFF00)>>8,address&0xFF,0])
		spi.writebytes([data])  					#Program page, and send 3 address bytes and data
		self.CE_DESELECT()
		
		# while ((self.readStatusRegister(CMD_RDSR1) and 0x01) == 0x01):#Waste time until not busy
			# pass

	def readByte(self,address):
		"""
		Read one byte from the given memory address.
		
		
		:param address: Address of data in memory.
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_READ_DATA])
		spi.writebytes([(address&0xFF0000)>>16,(address&0xFF00)>>8,address&0xFF,0])	#Read command,Send 3 address bytes
		mybyte = spi.readbytes(1)
		self.CE_DESELECT()
		
		return mybyte	#Return one byte read

	def writeArray(self,address, pData, arrayLength):
		"""
		Write an array to the memory chip.
		
		
		:param address: Address of data in memory.
		:param pData: 
		:param arrayLength:
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                   #Send WREN command
		self.CE_DESELECT()

		self.CE_SELECT()
		spi.writebytes([PAGE_PROGRAM,(address&0xFF0000)>>16,(address&0xFF00)>>8,address&0xFF,0])               #Send Byte 
		
		for item in range(len(pData)):
			data = ord(pData[item])			#this is a hack ,convert char data to hex and write bytes individually, slow
			spi.writebytes([data])
			
		self.CE_DESELECT()		

	def readArray(self,address, arrayLength):
		"""
		Flash memory chip class.
		
		
		:param address: Starting address of array
		:param arrayLength: Array length in bytes
		:returns: none
		"""
		pData = []
		self.CE_SELECT()
		spi.writebytes([CMD_READ_DATA,(address&0xFF0000)>>16,(address&0xFF00)>>8,address&0xFF,0])                       #Read command
		pData = spi.readbytes(arrayLength)
		self.CE_DESELECT()
		
		return pData

	def jedec_ID_Read(self):
		"""
		Flash memory chip class.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([JEDEC])                           #Send JEDEC ID command (9Fh)    
		jedec = spi.readbytes(3)
		self.CE_DESELECT()
		jedid = int((jedec[0] << 16) | (jedec[1] << 8) | jedec[2])
		return jedid

	def	readID(self):
		"""
		Flash memory chip class.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_RDID])                #Send read ID command (90h or ABh)
		spi.writebytes([0x00])                    #Send dummy byte
		spi.writebytes([0x00])                    #Send dummy byte
		spi.writebytes([0x00])                    #Send 0x00
		id = spi.readbytes(2)                #Send either manufacturer ID or device ID
		self.CE_DESELECT()
		return ((id[1] << 8) | id[0])

	def chipErase(self):
		"""
		Flash memory chip class.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                   #Send WREN command
		self.CE_DESELECT()
		
		self.CE_SELECT()
		spi.writebytes([ERASE_CHIP])                 #Send Sector Erase command
		self.CE_DESELECT()


	def sectorErase(self,address):
		"""
		Erase a sector from the flash memory chip.
		
		
		:param address: Sector address
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                    #Send WREN command
		self.CE_DESELECT()
		
		self.CE_SELECT()
		spi.writebytes([ERASE_SECTOR,(address&0xFF0000)>>16,(address&0xFF00)>>8,address&0xFF,0])                #Send Sector Erase command
		self.CE_DESELECT()
		

	def blockErase(self,address):
		"""
		Erase a block from the flash memory chip.
		
		
		:param address:  Block initial address
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                      #Send WREN command
		self.CE_DESELECT()
		data = [hex(address >> i & 0xff) for i in (16,8,0)]
		self.CE_SELECT()
		spi.writebytes([ERASE_BLOCK64,data])                 #Send Sector Erase command
		self.CE_DESELECT()


	def isWriteBusy(self):
		"""
		Check if a write is in process.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_RDSR1])                   #Send RDSR command
		temp = spi.read(1)
		self.CE_DESELECT()
		
		if (temp and 0x01):
			return 1
		else: 
			return 0

	def readStatusRegister(self,statReg):
		"""
		Read the contents of the status register.
		
		
		:param statReg:  Address of one of the status registers
		:returns: stat - Status register contents
		"""
		self.CE_SELECT()
		spi.writebytes([statReg])                      # send RDSR command
		byte = spi.readbytes(1)                      # receive byte
		self.CE_DESELECT()
		
		return byte

	def writeStatusRegister(self,statReg):
		"""
		Write a new value to the contents of the status register
		
		:param statReg:  Address of one of the status registers
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_WREN])                    #Send WREN command
		self.CE_DESELECT()
		
		self.CE_SELECT()
		spi.writebytes([CMD_WRSR])                    # select write to status register
		spi.writebytes([statReg])                     # data that will change the status of BPx or BPL (only bits 2,3,4,5,7 can be written)
		self.CE_DESELECT()


	def deepSleep(self):
		"""
		Put the memory in deep sleep mode.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()                           #enable device
		spi.writebytes([CMD_DEEPSLP])             #  select write to status register
		self.CE_DESELECT()                        # disable the device
		return 0

	def wakeUp(self):
		"""
		Wake up from deep sleep mode.
		
		
		:param none: 
		:returns: none
		"""
		self.CE_SELECT()		                    # enable device
		spi.writebytes([CMD_RESDPD])                # select write to status register
		self.CE_DESELECT()                        # disable the device
		return 0
		
	def setBlockProtection(self,prot):
		"""
		Set the contents of a specific block as protected.
		
		
		:param prot: 	Specific block
		:returns: none
		"""
		self.CE_SELECT()
		spi.writebytes([CMD_EWSR])		#enable write to volatile register
		self.CE_DESELECT()

		self.CE_SELECT()
		spi.writebytes([CMD_WRSR,(prot & 0x0f)<<2])		#write status register
		self.CE_DESELECT()

	def close(self):
		"""
		Close the SPI interface for the flash memory chip.
		
		
		:param none: 
		:returns: none
		"""
		spi.close()

