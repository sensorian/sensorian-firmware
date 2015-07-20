/****************************************************************************
 * Copyright (C) 2015 Sensorian
 *                                                                          *
 * This file is part of Sensorian.                                          *
 *                                                                          *
 *   Sensorian is free software: you can redistribute it and/or modify it   *
 *   under the terms of the GNU Lesser General Public License as published  *
 *   by the Free Software Foundation, either version 3 of the License, or   *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   Sensorian is distributed in the hope that it will be useful,           *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU Lesser General Public License for more details.                    *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with Sensorian.                                          *
 *   If not, see <http://www.gnu.org/licenses/>.                            *
 ****************************************************************************/
 
 /**
 * @file FXOS8700CQR1/Example1/MemoryMap.h
 * @author D.Qendri
 * @date 30 May 2015
 * @brief Register memory map header.
 *
 */
 
#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

#define STATUS 				0x00
#define OUT_X_MSB 			0x01
#define OUT_X_LSB			0x02
#define OUT_Y_MSB			0x03
#define OUT_Y_LSB			0x04
#define OUT_Z_MSB			0x05
#define OUT_Z_LSB			0x05
#define F_SETUP				0x09
#define TRIG_CFG			0x0A
#define SYSMOD				0x0B
#define INT_SOURCE			0x0C
#define WHO_AM_I			0x0D
#define XYZ_DATA_CFG		0x0E
#define HP_FILTER_CUTOFF	0x0F
#define PL_STATUS			0x10
#define PL_CFG				0x11
#define PL_COUNT			0x12
#define PL_BF_ZCOMP			0x13
#define PL_THS_REG			0x14
#define A_FFMT_CFG			0x15
#define A_FFMT_SRC			0x16
#define A_FFMT_THS			0x17
#define A_FFMT_COUNT		0x18
#define TRANSIENT_CFG		0x1D
#define TRANSIENT_SRC		0x1E
#define TRANSIENT_THS		0x1F
#define TRANSIENT_COUNT		0x20
#define PULSE_CFG			0x21
#define PULSE_SRC			0x22
#define PULSE_THSX			0x23
#define PULSE_THSY			0x24
#define PULSE_THSZ			0x25
#define PULSE_TMLT			0x26
#define PULSE_LTCY			0x27
#define PULSE_WIND			0x28
#define ASLP_COUNT			0x29
#define CTRL_REG1			0x2A
#define CTRL_REG2			0x2B
#define CTRL_REG3			0x2C
#define CTRL_REG4			0x2D
#define CTRL_REG5			0x2E
#define OFF_X				0x2F
#define OFF_Y				0x30
#define OFF_Z				0x31
#define M_DR_STATUS			0x32
#define M_OUT_X_MSB			0x33
#define M_OUT_X_LSB			0x34
#define M_OUT_Y_MSB			0x35
#define M_OUT_Y_LSB			0x36
#define M_OUT_Z_MSB			0x37
#define M_OUT_Z_LSB			0x38
#define CMP_X_MSB			0x39
#define CMP_X_LSB			0x3A
#define CMP_Y_MSB			0x3B
#define CMP_Y_LSB			0x3C
#define CMP_Z_MSB			0x3D
#define CMP_Z_LSB			0x3E
#define M_OFF_X_MSB			0x3F
#define M_OFF_X_LSB			0x40
#define M_OFF_Y_MSB			0x41
#define M_OFF_Y_LSB			0x42
#define M_OFF_Z_MSB			0x43
#define M_OFF_Z_LSB			0x44
#define MAX_X_MSB			0x45
#define MAX_X_LSB			0x46
#define MAX_Y_MSB			0x47
#define MAX_Y_LSB			0x48
#define MAX_Z_MSB			0x49
#define MAX_Z_LSB			0x4A
#define MIN_X_MSB			0x4B
#define MIN_X_LSB			0x4C
#define MIN_Y_MSB			0x4D
#define MIN_Y_LSB			0x4E
#define MIN_Z_MSB			0x4F
#define MIN_Z_LSB			0x50
#define TEMP				0x51
#define M_THS_CFG			0x52
#define M_THS_SRC			0x53
#define M_THS_X_MSB			0x54
#define M_THS_X_LSB			0x55
#define M_THS_Y_MSB			0x56
#define M_THS_Y_LSB			0x57
#define M_THS_Z_MSB			0x58
#define M_THS_Z_LSB			0x59
#define M_THS_COUNT			0x5A
#define M_CTRL_REG1			0x5B
#define M_CTRL_REG2			0x5C
#define M_CTRL_REG3			0x5D
#define M_INT_SRC			0x5E
#define A_VECM_CFG			0x5F
#define A_VECM_THS_MSB		0x60
#define A_VECM_THS_LSB		0x61
#define A_VECM_CNT			0x62
#define A_VECM_INITX_MSB	0x63
#define A_VECM_INITX_LSB	0x64
#define A_VECM_INITY_MSB 	0x65
#define A_VECM_INITY_LSB	0x66
#define A_VECM_INITZ_MSB	0x67
#define A_VECM_INITZ_LSB	0x68
#define M_VECM_CFG			0x69
#define M_VECM_THS_MSB		0x6A
#define M_VECM_THS_LSB		0x6B
#define M_VECM_CNT			0x6C
#define M_VECM_INITX_MSB	0x6D
#define M_VECM_INITX_LSB	0x6E
#define M_VECM_INITY_MSB	0x6F
#define M_VECM_INITY_LSB	0x70
#define M_VECM_INITZ_MSB	0x71
#define M_VECM_INITZ_LSB	0x72
#define A_FFMT_THS_X_MSB	0x73
#define A_FFMT_THS_X_LSB	0x74
#define A_FFMT_THS_Y_MSB	0x75
#define A_FFMT_THS_Y_LSB	0x76
#define A_FFMT_THS_Z_MSB	0x77
#define A_FFMT_THS_Z_LSB	0x78


#endif
