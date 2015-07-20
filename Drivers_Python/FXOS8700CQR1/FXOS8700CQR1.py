#!/usr/bin/python


__author__ = "D.Qendri"
__copyright__ = "Copyright 2015 Sensorian"
__license__ = "GPL V3"
__version__ = "1.0"


import math
import sys
import time
from ctypes import *

FXO = CDLL("./libFXO.so")

#########################################################################################################

FXOS8700CQR1_ADDRESS	=0x1E
FXOS8700CQ_WHOAMI_VAL 	=0xC7		# FXOS8700CQ WHOAMI production register value
FXOS8700CQ_READ_LEN 	=12			# 6 channels of two bytes = 12 bytes 
UINT14_MAX 				=16383		# For processing the accelerometer data to right-justified 2's complement

#######################################Memory Map########################################################

STATUS	= 0x00
OUT_X_MSB	= 0x01
OUT_X_LSB	= 0x02
OUT_Y_MSB	= 0x03
OUT_Y_LSB	= 0x04
OUT_Z_MSB 		= 0x05
OUT_Z_LSB 		= 0x05
F_SETUP 		= 0x09
TRIG_CFG 		= 0x0A
SYSMOD 			= 0x0B
INT_SOURCE 		= 0x0C
WHO_AM_I 		= 0x0D
XYZ_DATA_CFG 	= 0x0E
HP_FILTER_CUTOFF = 0x0F
PL_STATUS 		= 0x10
PL_CFG 			= 0x11
PL_COUNT 		= 0x12
PL_BF_ZCOMP 	= 0x13
PL_THS_REG 		= 0x14
A_FFMT_CFG		= 0x15
A_FFMT_SRC 		= 0x16
A_FFMT_THS 		= 0x17
A_FFMT_COUNT 	= 0x18
TRANSIENT_CFG 	= 0x1D
TRANSIENT_SRC 	= 0x1E
TRANSIENT_THS 	= 0x1F
TRANSIENT_COUNT = 0x20
PULSE_CFG 		= 0x21
PULSE_SRC 		= 0x22
PULSE_THSX 		= 0x23
PULSE_THSY 		= 0x24
PULSE_THSZ 		= 0x25
PULSE_TMLT 		= 0x26
PULSE_LTCY 		= 0x27
PULSE_WIND 		= 0x28
ASLP_COUNT 		= 0x29
CTRL_REG1 		= 0x2A
CTRL_REG2 		= 0x2B
CTRL_REG3 		= 0x2C
CTRL_REG4 		= 0x2D
CTRL_REG5 		= 0x2E
OFF_X 			= 0x2F
OFF_Y 			= 0x30
OFF_Z 			= 0x31
M_DR_STATUS 	= 0x32
M_OUT_X_MSB 	= 0x33
M_OUT_X_LSB 	= 0x34
M_OUT_Y_MSB 	= 0x35
M_OUT_Y_LSB 	= 0x36
M_OUT_Z_MSB	 	= 0x37
M_OUT_Z_LSB 	= 0x38
CMP_X_MSB 		= 0x39
CMP_X_LSB 		= 0x3A
CMP_Y_MSB 		= 0x3B
CMP_Y_LSB 		= 0x3C
CMP_Z_MSB 		= 0x3D
CMP_Z_LSB 		= 0x3E
M_OFF_X_MSB 	= 0x3F
M_OFF_X_LSB 	= 0x40
M_OFF_Y_MSB 	= 0x41
M_OFF_Y_LSB 	= 0x42
M_OFF_Z_MSB 	= 0x43
M_OFF_Z_LSB 	= 0x44
MAX_X_MSB 		= 0x45
MAX_X_LSB 		= 0x46
MAX_Y_MSB 		= 0x47
MAX_Y_LSB 		= 0x48
MAX_Z_MSB 		= 0x49
MAX_Z_LSB 		= 0x4A
MIN_X_MSB 		= 0x4B
MIN_X_LSB 		= 0x4C
MIN_Y_MSB		= 0x4D
MIN_Y_LSB 		= 0x4E
MIN_Z_MSB 		= 0x4F
MIN_Z_LSB 		= 0x50
TEMP 			= 0x51
M_THS_CFG 		= 0x52
M_THS_SRC 		= 0x53
M_THS_X_MSB 	= 0x54
M_THS_X_LSB 	= 0x55
M_THS_Y_MSB 	= 0x56
M_THS_Y_LSB 	= 0x57
M_THS_Z_MSB 	= 0x58
M_THS_Z_LSB	 	= 0x59
M_THS_COUNT 	= 0x5A
M_CTRL_REG1 	= 0x5B
M_CTRL_REG2 	= 0x5C
M_CTRL_REG3 	= 0x5D
M_INT_SRC 		= 0x5E
A_VECM_CFG 		= 0x5F
A_VECM_THS_MSB 	= 0x60
A_VECM_THS_LSB 	= 0x61
A_VECM_CNT 		= 0x62
A_VECM_INITX_MSB = 0x63
A_VECM_INITX_LSB = 0x64
A_VECM_INITY_MSB = 0x65
A_VECM_INITY_LSB = 0x66
A_VECM_INITZ_MSB = 0x67
A_VECM_INITZ_LSB = 0x68
M_VECM_CFG 		= 0x69
M_VECM_THS_MSB 	= 0x6A
M_VECM_THS_LSB 	= 0x6B
M_VECM_CNT 		= 0x6C
M_VECM_INITX_MSB = 0x6D
M_VECM_INITX_LSB = 0x6E
M_VECM_INITY_MSB = 0x6F
M_VECM_INITY_LSB = 0x70
M_VECM_INITZ_MSB = 0x71
M_VECM_INITZ_LSB = 0x72
A_FFMT_THS_X_MSB = 0x73
A_FFMT_THS_X_LSB = 0x74
A_FFMT_THS_Y_MSB = 0x75
A_FFMT_THS_Y_LSB = 0x76
A_FFMT_THS_Z_MSB = 0x77
A_FFMT_THS_Z_LSB = 0x78

######################################################################################################################
FXOS8700CQ_ADDRESS  = 0x1E


#############*STATUS Register################/

ZYXOW_MASK     	=   0x80
ZOW_MASK       	=   0x40
YOW_MASK        =   0x20
XOW_MASK        =   0x10
ZYXDR_MASK      =   0x08
ZDR_MASK        =   0x04
YDR_MASK        =   0x02
XDR_MASK        =   0x01

##############STATUS Register#################
F_OVF_MASK          =  0x80
F_WMRK_FLAG_MASK    =   0x40
F_CNT5_MASK         =   0x20
F_CNT4_MASK         =   0x10
F_CNT3_MASK         =   0x08
F_CNT2_MASK         =   0x04
F_CNT1_MASK         =   0x02
F_CNT0_MASK         =   0x01
F_CNT_MASK          =   0x3F

##############STATUS Register################/
OUT_X_MSB_REG    =    0x01
OUT_X_LSB_REG    =    0x02
OUT_Y_MSB_REG    =    0x03
OUT_Y_LSB_REG    =    0x04
OUT_Z_MSB_REG    =    0x05
OUT_Z_LSB_REG    =    0x06

#############*FIFO Register################/
F_MODE1_MASK     =    0x80
F_MODE0_MASK     =    0x40
F_WMRK5_MASK     =    0x20
F_WMRK4_MASK     =    0x10
F_WMRK3_MASK     =    0x08
F_WMRK2_MASK     =    0x04
F_WMRK1_MASK     =    0x02
F_WMRK0_MASK     =    0x01
F_MODE_MASK      =    0xC0
F_WMRK_MASK      =    0x3F

F_MODE_DISABLED    =  0x00
F_MODE_CIRCULAR    =  (F_MODE0_MASK)
F_MODE_FILL        =  (F_MODE1_MASK)
F_MODE_TRIGGER     =  (F_MODE1_MASK+F_MODE0_MASK)

#############*TRIG_CFG Register################/
TRIG_TRANS_MASK    =   0x20
TRIG_LNDPRT_MASK   =   0x10
TRIG_PULSE_MASK    =   0x08
TRIG_FF_MT_MASK    =   0x04

#############*SYSMOD Register################/
FGERR_MASK       =    0x80    
FGT_4_MASK       =    0x40    
FGT_3_MASK       =    0x20    
FGT_2_MASK       =    0x10    
FGT_1_MASK       =    0x08    
FGT_0_MASK       =    0x04    
FGT_MASK         =    0x7C    
SYSMOD1_MASK     =    0x02
SYSMOD0_MASK     =    0x01
SYSMOD_MASK      =    0x03

SYSMOD_STANDBY    =   0x00
SYSMOD_WAKE       =   (SYSMOD0_MASK)
SYSMOD_SLEEP      =   (SYSMOD1_MASK)

#############*INT_SOURCE Register################/
SRC_ASLP_MASK      =  0x80
SRC_FIFO_MASK      =  0x40    
SRC_TRANS_MASK     =  0x20
SRC_LNDPRT_MASK    =  0x10
SRC_PULSE_MASK     =  0x08
SRC_FF_MT_MASK     =  0x04
SRC_DRDY_MASK      =  0x01

############WHO_AM_I Device ID Register##############/
FXOS8700CQ       =    0xC7
MXOS8700CQ       =    0xC4

########*XYZ_DATA_CFG Sensor Data Configuration Register########
HPF_OUT_MASK      =   0x10    # MMA8451 and MMA8452 only
FS1_MASK          =   0x02
FS0_MASK          =   0x01
FS_MASK            =  0x03

FULL_SCALE_2G     =    0x00
FULL_SCALE_4G     =    (FS0_MASK)
FULL_SCALE_8G     =    (FS1_MASK)


######*HP_FILTER_CUTOFF High Pass Filter Register ###########/
PULSE_HPF_BYP_MASK   =  0x20
PULSE_LPF_EN_MASK    =  0x10
SEL1_MASK           =   0x02
SEL0_MASK           =   0x01
SEL_MASK            =   0x03

#######PL_STATUS Portrait/Landscape Status Register ##########
NEWLP_MASK         =   0x80
LO_MASK           =    0x40
LAPO1_MASK         =   0x04
LAPO0_MASK         =   0x02
BAFRO_MASK         =   0x01
LAPO_MASK           =  0x06


#######* PL_CFG Portrait/Landscape Configuration Register########
DBCNTM_MASK         =  0x80
PL_EN_MASK          =  0x40

###PL_BF_ZCOMP Back/Front and Z Compensation Register############

BKFR1_MASK         =   0x80
BKFR0_MASK         =   0x40
ZLOCK2_MASK        =   0x04
ZLOCK1_MASK        =   0x02
ZLOCK0_MASK        =   0x01
BKFR_MASK          =   0xC0
ZLOCK_MASK         =   0x07

######*PL_P_L_THS Portrait to Landscape Threshold Register########
PL_P_L_THS_REG      =  0x14
P_L_THS4_MASK       =  0x80
P_L_THS3_MASK       =  0x40
P_L_THS2_MASK       =  0x20
P_L_THS1_MASK       =  0x10
P_L_THS0_MASK       =  0x08
HYS2_MASK           =  0x04
HYS1_MASK           =  0x02
HYS0_MASK           =  0x01
P_L_THS_MASK        =  0xF8
HYS_MASK            =  0x07

##########*FF_MT_CFG Freefall and Motion Configuration Register###/
FF_MT_CFG_REG       =  0x15
ELE_MASK            =  0x80
OAE_MASK            =  0x40
ZEFE_MASK           =  0x20
YEFE_MASK           =  0x10
XEFE_MASK           =  0x08

#####*FF_MT_SRC Freefall and Motion Source Registers###########/
FF_MT_SRC_REG       =  0x16
EA_MASK            =   0x80
ZHE_MASK           =   0x20
ZHP_MASK           =   0x10
YHE_MASK           =   0x08
YHP_MASK           =   0x04
XHE_MASK           =   0x02
XHP_MASK           =   0x01

###FF_MT_THS Freefall and Motion Threshold Registers###########
FT_MT_THS_REG        = 0x17
TRANSIENT_THS_REG    = 0x1F
DBCNTM_MASK          = 0x80
THS6_MASK            = 0x40
THS5_MASK            = 0x20
THS4_MASK            = 0x10
THS3_MASK            = 0x08
THS2_MASK            = 0x04
TXS1_MASK            = 0x02
THS0_MASK            = 0x01
THS_MASK             = 0x7F


####*FF_MT_COUNT Freefall Motion Count Registers############/
FF_MT_COUNT_REG     =  0x18

########*TRANSIENT_CFG Transient Configuration Register########/

TELE_MASK       =	   0x10
ZTEFE_MASK      =      0x08
YTEFE_MASK      =      0x04
XTEFE_MASK      =      0x02
HPF_BYP_MASK    =      0x01


######TRANSIENT_SRC Transient Source Register##############/

TEA_MASK           =   0x40
ZTRANSE_MASK       =   0x20
Z_TRANS_POL_MASK   =   0x10
YTRANSE_MASK       =   0x08
Y_TRANS_POL_MASK   =   0x04
XTRANSE_MASK       =   0x02
X_TRANS_POL_MASK   =   0x01


#############*PULSE_CFG Register##############/

DPA_MASK           =  0x80
PELE_MASK          =   0x40
ZDPEFE_MASK        =   0x20
ZSPEFE_MASK        =   0x10
YDPEFE_MASK        =   0x08
YSPEFE_MASK        =   0x04
XDPEFE_MASK        =   0x02
XSPEFE_MASK        =   0x01


#############*PULSE_SRC Register################/

PEA_MASK           =   0x80
AXZ_MASK           =   0x40
AXY_MASK           =   0x20
AXX_MASK           =   0x10
DPE_MASK           =   0x08
POLZ_MASK          =   0x04
POLY_MASK          =   0x02
POLX_MASK          =   0x01
PTHS_MASK          =   0x7F

########CTRL_REG1 System Control 1 Register##############/
ASLP_RATE1_MASK    =   0x80
ASLP_RATE0_MASK    =   0x40
DR2_MASK           =   0x20
DR1_MASK           =   0x10
DR0_MASK           =   0x08
LNOISE_MASK        =   0x04
FREAD_MASK         =   0x02
ACTIVE_MASK        =   0x01
ASLP_RATE_MASK     =   0xC0
DR_MASK            =   0x38

ASLP_RATE_20MS      =  0x00
ASLP_RATE_80MS      =  (ASLP_RATE0_MASK)
ASLP_RATE_160MS     =  (ASLP_RATE1_MASK)
ASLP_RATE_640MS     =  (ASLP_RATE1_MASK+ASLP_RATE0_MASK)

ASLP_RATE_50HZ       = (ASLP_RATE_20MS)
ASLP_RATE_12_5HZ     = (ASLP_RATE_80MS)
ASLP_RATE_6_25HZ     = (ASLP_RATE_160MS)
ASLP_RATE_1_56HZ     = (ASLP_RATE_640MS)

HYB_ASLP_RATE_25HZ      =  (ASLP_RATE_20MS)
HYB_ASLP_RATE_6_25HZ    =  (ASLP_RATE_80MS)
HYB_ASLP_RATE_1_56HZ    =  (ASLP_RATE_160MS)
HYB_ASLP_RATE_0_8HZ     = (ASLP_RATE_640MS)

DATA_RATE_1250US    =  0x00
DATA_RATE_2500US    =  (DR0_MASK)
DATA_RATE_5MS       =  (DR1_MASK)
DATA_RATE_10MS      =  (DR1_MASK+DR0_MASK)
DATA_RATE_20MS      =  (DR2_MASK)
DATA_RATE_80MS      =  (DR2_MASK+DR0_MASK)
DATA_RATE_160MS     =  (DR2_MASK+DR1_MASK)
DATA_RATE_640MS     =  (DR2_MASK+DR1_MASK+DR0_MASK)

DATA_RATE_800HZ     =  (DATA_RATE_1250US)
DATA_RATE_400HZ     =  (DATA_RATE_2500US)
DATA_RATE_200HZ     =  (DATA_RATE_5MS)
DATA_RATE_100HZ     =  (DATA_RATE_10MS)
DATA_RATE_50HZ      =  (DATA_RATE_20MS)
DATA_RATE_12_5HZ    =  (DATA_RATE_80MS)
DATA_RATE_6_25HZ    =  (DATA_RATE_160MS)
DATA_RATE_1_56HZ    =  (DATA_RATE_640MS)

HYB_DATA_RATE_400HZ   =(DATA_RATE_1250US)
HYB_DATA_RATE_200HZ   =(DATA_RATE_2500US)
HYB_DATA_RATE_100HZ   =(DATA_RATE_5MS)
HYB_DATA_RATE_50HZ    =(DATA_RATE_10MS)
HYB_DATA_RATE_25HZ    =(DATA_RATE_20MS)
HYB_DATA_RATE_6_25HZ  =(DATA_RATE_80MS)
HYB_DATA_RATE_3_15HZ  =(DATA_RATE_160MS)
HYB_DATA_RATE_0_8HZ   =(DATA_RATE_640MS)

ACTIVE              =  (ACTIVE_MASK)
STANDBY             =  0x00


########CTRL_REG2 System Control 2 Register##############/
CTRL_REG2        	=  0x2B
ST_MASK             =  0x80
RST_MASK            =  0x40
SMODS1_MASK         =  0x10
SMODS0_MASK         =  0x08
SLPE_MASK           =  0x04
MODS1_MASK          =  0x02
MODS0_MASK          =  0x01
SMODS_MASK          =  0x18
MODS_MASK           =  0x03

SMOD_NORMAL         =  0x00
SMOD_LOW_NOISE      =  (SMODS0_MASK)
SMOD_HIGH_RES       =  (SMODS1_MASK)
SMOD_LOW_POWER      =  (SMODS1_MASK+SMODS0_MASK)

MOD_NORMAL          =  0x00
MOD_LOW_NOISE       =  (MODS0_MASK)
MOD_HIGH_RES        =  (MODS1_MASK)
MOD_LOW_POWER       =  (MODS1_MASK+MODS0_MASK)


########CTRL_REG3 System Control 3 Register##############/
FIFO_GATE_MASK       = 0x80    
WAKE_TRANS_MASK      = 0x40
WAKE_LNDPRT_MASK     = 0x20
WAKE_PULSE_MASK      = 0x10
WAKE_FF_MT_MASK      = 0x08
IPOL_MASK            = 0x02
PP_OD_MASK           = 0x01

########CTRL_REG4 System Control 4 Register##############/
INT_EN_ASLP_MASK      =0x80
INT_EN_FIFO_MASK      =0x40  
INT_EN_TRANS_MASK     =0x20
INT_EN_LNDPRT_MASK    =0x10
INT_EN_PULSE_MASK     =0x08
INT_EN_FF_MT_MASK     =0x04
INT_EN_DRDY_MASK      =0x01

########CTRL_REG5 System Control 5 Register##############/
INT_CFG_ASLP_MASK     =0x80
INT_CFG_FIFO_MASK     =0x40
INT_CFG_TRANS_MASK    =0x20
INT_CFG_LNDPRT_MASK   =0x10
INT_CFG_PULSE_MASK    =0x08
INT_CFG_FF_MT_MASK    =0x04
INT_CFG_DRDY_MASK     =0x01

#
#  XYZ Offset Correction Registers
#
OFF_X_REG           =  0x2F
OFF_Y_REG           =  0x30
OFF_Z_REG           =  0x31

#
#  MAG CTRL_REG1 System Control 1 Register
#
M_ACAL_MASK          = 0x80
M_RST_MASK           = 0x40
M_OST_MASK           = 0x20
M_OSR2_MASK          = 0x10
M_OSR1_MASK          = 0x08
M_OSR0_MASK          = 0x04
M_HMS1_MASK          = 0x02
M_HMS0_MASK          = 0x01
M_OSR_MASK           = 0x1C
M_HMS_MASK           = 0x03

#OSR Selections
M_OSR_1_56_HZ      =   0x00
M_OSR_6_25_HZ      =   M_OSR0_MASK
M_OSR_12_5_HZ      =   M_OSR1_MASK
M_OSR_50_HZ        =   M_OSR1_MASK+M_OSR0_MASK
M_OSR_100_HZ       =   M_OSR2_MASK
M_OSR_200_HZ       =   M_OSR2_MASK+M_OSR0_MASK
M_OSR_400_HZ       =   M_OSR2_MASK+M_OSR1_MASK
M_OSR_800_HZ       =   M_OSR2_MASK+M_OSR1_MASK+M_OSR0_MASK

#Hybrid Mode Selection
ACCEL_ACTIVE       =   0x00
MAG_ACTIVE         =   M_HMS0_MASK
HYBRID_ACTIVE      =   (M_HMS1_MASK | M_HMS0_MASK)


#
#  MAG CTRL_REG2 System Control 2 Register
#

M_HYB_AUTOINC_MASK   	= 0x20
M_MAXMIN_DIS_MASK     	= 0x10
M_MAXMIN_DIS_THS_MASK  	= 0x08
M_MAXMIN_RST_MASK    	= 0x04
M_RST_CNT1_MASK      	= 0x02
M_RST_CNT0_MASK      	= 0x01

#Mag Auto-Reset De-Gauss Frequency
RST_ODR_CYCLE        = 0x00
RST_16_ODR_CYCLE     = M_RST_CNT0_MASK
RST_512_ODR_CYCLE    = M_RST_CNT1_MASK
RST_DISABLED         = M_RST_CNT1_MASK+M_RST_CNT0_MASK

#
#  MAG CTRL_REG3 System Control 3 Register
#

M_RAW_MASK          =  0x80
M_ASLP_OS_2_MASK    =  0x40
M_ASLP_OS_1_MASK    =  0x20
M_ASLP_OS_0_MASK    =  0x10
M_THS_XYZ_MASK      =  0x08
M_ST_Z_MASK         =  0x04
M_ST_XY1_MASK       =  0x02
M_ST_XY0_MASK       =  0x01
M_ASLP_OSR_MASK     =  0x70
M_ST_XY_MASK        =  0x03

#OSR Selections
M_ASLP_OSR_1_56_HZ   = 0x00
M_ASLP_OSR_6_25_HZ   = M_ASLP_OS_0_MASK
M_ASLP_OSR_12_5_HZ   = M_ASLP_OS_1_MASK
M_ASLP_OSR_50_HZ     = M_ASLP_OS_1_MASK+M_ASLP_OS_0_MASK
M_ASLP_OSR_100_HZ    = M_ASLP_OS_2_MASK
M_ASLP_OSR_200_HZ    = M_ASLP_OS_2_MASK+M_ASLP_OS_0_MASK
M_ASLP_OSR_400_HZ    = M_ASLP_OS_2_MASK+M_ASLP_OS_1_MASK
M_ASLP_OSR_800_HZ    = M_ASLP_OS_2_MASK+M_ASLP_OS_1_MASK+M_ASLP_OS_0_MASK

#
#  MAG INT SOURCE Register
#
M_INT_SOURCE       =   0x5E

SRC_M_DRDY_MASK     =  0x04
SRC_M_VECM_MASK     =  0x02
SRC_M_THS_MASK     =   0x01

#  ACCEL VECTOR CONFIG Register

A_VECM_INIT_CFG_MASK = 0x40
A_VECM_INIT_EN_MASK  = 0x20
A_VECM_WAKE_EN_MASK  = 0x10
A_VECM_EN_MASK       = 0x08
A_VECM_UPDM_MASK     = 0x04
A_VECM_INITM_MASK    = 0x02
A_VECM_ELE_MASK      = 0x01

#  ACCEL VECTOR THS MSB AND LSB Register

A_VECM_DBCNTM_MASK  =  0x80

#  MAG VECTOR CONFIG Register

M_VECM_INIT_CFG_MASK = 0x40
M_VECM_INIT_EN_MASK  = 0x20
M_VECM_WAKE_EN_MASK  = 0x10
M_VECM_EN_MASK       = 0x08
M_VECM_UPDM_MASK     = 0x04
M_VECM_INITM_MASK    = 0x02
M_VECM_ELE_MASK      = 0x01

#  MAG VECTOR THS MSB AND LSB Register
M_VECM_DBCNTM_MASK   = 0x80

#  ACCEL FFMT THS X MSB AND LSB Register

A_FFMT_THS_XYZ_EN_MASK = 	0x80
A_FFMT_THS_X_LSB_MASK  =	0xFC

#  ACCEL FFMT THS Y MSB AND LSB Register

A_FFMT_THS_Y_EN_MASK 	= 0x80
A_FFMT_THS_Y_LSB_MASK 	= 0xFC

#  ACCEL FFMT THS Z MSB AND LSB Register

A_FFMT_THS_Z_EN_MASK    =    0x80
A_FFMT_THS_Z_LSB_MASK   =    0xFC
#########################################################################################################


class FXOS8700CQR1(object):

	"""Representation of an FXOS8700CQ accelerometer/ magnetometer object."""
	def __init__(self):
		"""
		Initializes the accelerometer and magnetometer chip in hybrid mode. Both Accel and mangnetometer functionality are available.
		
		
		:param none: 
		:returns: none
		"""
		self._address = FXOS8700CQR1_ADDRESS
		FXO.I2C_Initialize()
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG2, RST_MASK)					#Reset sensor, and wait for reboot to complete
		FXO.bcm2835_delay(2)											#Wait at least 1ms after issuing a reset before attempting communications.
		FXO.FXOS8700CQ_StandbyMode()
		while ((FXO.FXOS8700CQ_ReadByte(self._address,CTRL_REG2) & RST_MASK) == True):
			pass
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK))	#OSR=max, Hybrid Mode 
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG2, M_HYB_AUTOINC_MASK)							#Enable Hyb Mode Auto Increments  in order to read all data
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG4, INT_EN_DRDY_MASK )								#Enable interrupts for DRDY (TO, Aug 2012)
		FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, FULL_SCALE_2G)								#Full Scale of +/-2g
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG1,(HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ))			#System Output Data Rate of 200Hz (5ms), Sleep Mode Poll Rate of 50Hz (20ms)
		
	def	getID (self):
		"""
		Read the chip ID.
		
		
		:param none: 
		:returns: chip id
		"""
		id = FXO.FXOS8700CQ_ReadByte(self._address,WHO_AM_I)
		return id
	
	def	readStatusReg(self):
		""" 
		Read the Status register.
		
		
		:param none: 
		:returns: status register
		"""
		return FXO.FXOS8700CQ_ReadByte(self._address,STATUS)
		
	def activeMode(self):
		"""
		Put the chip in Active mode.
		
		
		:param none: 
		:returns: none
		"""
		reg1 = FXO.FXOS8700CQ_ReadByte(self._address,CTRL_REG1);
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG1, (reg1 | ACTIVE_MASK))			#Set the Active bit in System Control 1 Register.
		
	def	standbyMode(self):
		""" 
		Put the chip in Standby mode.
		
		
		:param none: 
		:returns: none
		"""
		n = FXO.FXOS8700CQ_ReadByte(self._address,CTRL_REG1)
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG1, n & (~ACTIVE_MASK))
		return (n & ~ACTIVE_MASK)
	  
	def	configureAccelerometer(self):
		"""
		Configure the Accelerometer with default settings +/- 2g.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_StandbyMode()
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG4, INT_EN_DRDY_MASK )          		# Enable interrupts for DRDY (TO, Aug 2012)
		FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, FULL_SCALE_2G)           		# Set FSR of accel to +/-2g
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG1, (HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ))     # Set ODRs
		FXO.FXOS8700CQ_ActiveMode()
	  
	def configureMagnetometer(self):
		"""
		Configure the magnetometer with default settigs.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_StandbyMode()
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK))    # OSR=max, hybrid mode (TO, Aug 2012)
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG2, M_HYB_AUTOINC_MASK)       #enable hybrid autoinc
		FXO.FXOS8700CQ_ActiveMode()

	def hybridMode(self):
		"""
		Configure the chip in hybrid mode with default settigs 50 Hz ouput data rate.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_StandbyMode()
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG1, (HYBRID_ACTIVE|M_OSR2_MASK|M_OSR1_MASK|M_OSR0_MASK))     #OSR=max, hybrid mode (TO, Aug 2012)
		FXO.FXOS8700CQ_WriteByte(self._address,M_CTRL_REG2, M_HYB_AUTOINC_MASK)       				# enable hybrid autoinc
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG4, INT_EN_DRDY_MASK )           				# Enable interrupts for DRDY (TO, Aug 2012)
		FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, FULL_SCALE_2G)            				 # Set FSR of accel to +/-2g
		FXO.FXOS8700CQ_WriteByte(self._address,CTRL_REG1, (HYB_ASLP_RATE_25HZ|HYB_DATA_RATE_50HZ))     			#Set ODRs
		FXO.FXOS8700CQ_ActiveMode()

	def pollAccelerometer (self):
		"""
		Get accelerometer data , these are left justified.
		
		
		:param none: 
		:returns: acceleration data (ax, ay, az)
		"""
		raw0 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_X_MSB)   
		raw1 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_X_LSB)
		raw2 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_Y_MSB)
		raw3 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_Y_LSB)
		raw4 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_Z_MSB)
		raw5 = FXO.FXOS8700CQ_ReadByte(self._address,OUT_Z_LSB)
		ax = (raw0 << 8) | raw1 		# Pull out 16-bit, 2's complement magnetometer data
		ay = (raw2 << 8) | raw3 
		az = (raw4 << 8) | raw5 
		
		if(ax > UINT14_MAX/2):	 
			ax -= UINT14_MAX 
		if(ay > UINT14_MAX/2):	 
			ay -= UINT14_MAX 
		if(az > UINT14_MAX/2):	 
			az -= UINT14_MAX 
			
		return (ax, ay, az)

	def	FIFOMode(self,mode):
		"""
		Put the sensor in FIFO mode.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_WriteByte(self._address,F_SETUP,6<<mode)
		 
	def	highPassFilter(self,status):
		"""
		Enable the highpass filter.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG,status)
		
	def	fullScaleRange(self,range):
		"""
		Configure full scale range.
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG,range)
		
	def	setAccelerometerDynamicRange(self,range):
		"""
		Configure the sensor accelerometer dynamic range.
		
		
		:param range:  Accelerometer dynamic range
		:returns: none
		"""
		FXO.FXOS8700CQ_StandbyMode() 	
		if range == SCALE2G:
				FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(XYZ_DATA_CFG) & ~FS_MASK))		#Write the 2g dynamic range value into register = 0x0E		 
		elif range == SCALE4G:
				FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(XYZ_DATA_CFG) & ~FS_MASK)) 		#Write the 4g dynamic range value into register = 0x0E
				FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(XYZ_DATA_CFG) | FULL_SCALE_4G)) 		 
		elif range ==SCALE8G:
				FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(XYZ_DATA_CFG) & ~FS_MASK)) 		#Write the 8g dynamic range value into register = 0x0E
				FXO.FXOS8700CQ_WriteByte(self._address,XYZ_DATA_CFG, (FXOS8700CQ_ReadByte(XYZ_DATA_CFG) | FULL_SCALE_8G)) 
		FXO.FXOS8700CQ_ActiveMode() 

	def	magnetometerStatus(self):
		"""
		Configure the accelerometer status.
		
		
		:param none: 
		:returns: none
		"""
		stat = FXO.FXOS8700CQ_ReadByte(self._address,M_DR_STATUS)
		return stat

	def	pollMagnetometer (self):
		"""
		Reads the magnetometer data.
		
		
		:param none: 
		:returns: magnetometer data (mx, my, mz)
		"""
		raw0 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_X_MSB)
		raw1 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_X_LSB)
		raw2 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_Y_MSB)
		raw3 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_Y_MSB)
		raw4 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_Z_MSB)
		raw5 = FXO.FXOS8700CQ_ReadByte(self._address,M_OUT_Z_MSB)	
		mx = (raw0 << 8) | raw1			# Pull out 16-bit, 2's complement magnetometer data
		my = (raw2 << 8) | raw3
		mz = (raw4 << 8) | raw5
		return (mx, my, mz)
		
	def getHeading(self):
		"""
		Get magnetometer heading .
		
		
		:param none: 
		:returns: magnetometer heading
		"""
		(x, y, z) = self.pollAccelerometer()		
		heading = (math.atan2((y), (x)) * 180)/math.pi
  		return heading
		
	def	setODR (self,DataRateValue):
		"""
		Set the output data rate.
		
		
		:param DataRateValue:  Output data rate 
		:returns: none
		"""
		FXO.FXOS8700CQ_SetODR(DataRateValue)
		
	def	getODR (self):
		"""
		Get the output data rate .
		
		
		:param none: 
		:returns: none
		"""
		FXO.FXOS8700CQ_GetODR()
		
	def getOrientation(self):
		"""
		Returns current orientation of the sensor.
		
		
		:param none: 
		:returns: current orientation of the Sensorian
		"""
		orientation = FXO.FXOS8700CQ_ReadByte(self._address,PL_STATUS)
		return orientation
		
	def configureOrientation(self):
		"""
		Configure Orientation mode.
		
		
		:param none: 
		:returns: none
		"""
		CTRL_REG1_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x2A)			#read contents of register 
		CTRL_REG1_Data &= 0xFE									#Set last bit to 0. 
		FXO.FXOS8700CQ_WriteByte(self._address,0x2A, CTRL_REG1_Data)			 

		CTRL_REG1_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x2A) 			#Note: Can combine this step with above 
		CTRL_REG1_Data &= 0xC7								#Clear the sample rate bits 
		CTRL_REG1_Data |= 0x20								#Set the sample rate bits to 50 Hz 
		FXO.FXOS8700CQ_WriteByte(self._address,0x2A, CTRL_REG1_Data) 		#Write updated value into the register. 

		#Set the PL_EN bit in Register 0x11 PL_CFG. This will enable the orientation detection. 
		PLCFG_Data = FXO.FXOS8700CQ_ReadByte (self._address,0x11)
		PLCFG_Data |= 0x40 
		FXO.FXOS8700CQ_WriteByte(self._address,0x11, PLCFG_Data)

		#Set the Back/Front Angle trip points in register 0x13 following the table in the data sheet.

		PL_BF_ZCOMP_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x13)
		PL_BF_ZCOMP_Data &= 0x3F 							#Clear bit 7 and 6 
		#Select one of the following to set the B/F angle value: 
		PL_BF_ZCOMP_Data |= 0x00						#This does nothing additional and keeps bits [7:6] = 00 
		PL_BF_ZCOMP_Data |= 0x40						# Sets bits[7:6] = 01
		PL_BF_ZCOMP_Data |= 0x80						# Sets bits[7:6] = 02
		PL_BF_ZCOMP_Data |= 0xC0						# Sets bits[7:6] = 03 
		FXO.FXOS8700CQ_WriteByte(self._address,0x13, PL_BF_ZCOMP_Data)	#Write in the updated Back/Front Angle
		PL_BF_ZCOMP_Data = FXO.FXOS8700CQ_ReadByte(0x1C) 	#Read out contents of the register (can be read by all 
		PL_BF_ZCOMP_Data &= 0xF8 						#Clear the last three bits of the register 
		PL_BF_ZCOMP_Data |= 0x00 
		
		FXO.FXOS8700CQ_WriteByte(self._address,0x13, PL_BF_ZCOMP_Data)		#Write in the updated Z-lockout angle
		P_L_THS_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x14)					
		P_L_THS_Data &= 0x07		#Clear the Threshold values 
		P_L_THS_Data |= (0x07)<<3
		FXO.FXOS8700CQ_WriteByte(self._address,0x14,P_L_THS_Data)
		#Set the Hysteresis Angle 
		P_L_THS_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x14) 
		P_L_THS_Data &= 0xF8		#Clear the Hysteresis values
		P_L_THS_Data |= 0x02
		FXO.FXOS8700CQ_WriteByte(self._address,0x14,P_L_THS_Data)
		
		CTRL_REG4_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x2D)	#Read out the contents of the register
		CTRL_REG4_Data |= 0x10							#Set bit 4 
		FXO.FXOS8700CQ_WriteByte(self._address,0x2D, CTRL_REG4_Data)		#Set the bit and write into CTRL_REG4
		
		CTRL_REG5_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x2E)	 	
		CTRL_REG5_Data &= 0xEF							#Clear bit 4 to choose the interrupt to route to INT2 
		CTRL_REG5_Data |= 0x10							#Set bit 4 to choose the interrupt to route to INT1 
		FXO.FXOS8700CQ_WriteByte(self._address,0x2E, CTRL_REG5_Data)		#Write in the interrupt routing selection 
		
		FXO.FXOS8700CQ_WriteByte(self._address,0x12, 0x05)				#This sets the debounce counter to 100 ms at 50 Hz 
		CTRL_REG1_Data = FXO.FXOS8700CQ_ReadByte(self._address,0x2A)		#Read out the contents of the register 
		CTRL_REG1_Data |= 0x01							#Change the value in the register to Active Mode. 
		FXO.FXOS8700CQ_WriteByte(self._address,0x2A, CTRL_REG1_Data)		#Write in the updated value to put the device in Active Mode 

	def getChipMode(self):
		"""
		Get the current mode of the chip.
		
		
		:param none: 
		:returns: none
		"""
		mode = FXO.FXOS8700CQ_ReadByte(self._address,SYSMOD)
		return mode
	
	def	getTemperature(self):
		"""
		Reads the sensor temperature.
		
		
		:param none: 
		:returns: none
		"""
		temp = FXO.FXOS8700CQ_ReadByte(self._address,TEMP)    
		return temp
		
	def readStatusReg(self):
		"""
		Read contents of status register.
		
		
		:param none: 
		:returns: none
		"""
		return FXO.FXOS8700CQ_ReadByte(self._address,STATUS)

	def writeByte(self,address,reg, value):
		"""
		Write a data byte on the given register.
		
		:param address: Sensor address
		:param reg: Register address 
		:param value: Register value 
		:returns: none
		"""
		FXO.I2C_WriteByteRegister(address,reg,value)		#Write value to register

	def readByte(self,address,reg):
		"""
		Read a data byte from the given register.
		
		
		:param address: Sensor address
		:param reg: Register address
		:returns: none
		"""
		return FXO.I2C_ReadByteRegister(address,reg)	#Read register current value

