/// \mainpage Sensorian C library for Raspberry Pi 
/// \section intro Introduction
/// The following is the documentation for the Sensorian shield userspace C library (Raspberry Pi edition). 
/// The API provides a number of library functions that allows the users to easily integrate the onboard sensors and peripherals  
/// on their own programs. 
///
/// The current firmware examples together with the code package can be downloaded from http://www.sensorian.com. 
/// Several examples programs demonstrating the breadth of the API are provided.
/// The programs were tested using Raspbian 2013 and 2014 editions on Raspberry Pi Verison1, models A, B, B+. 
///  
/// \par Sensors
/// The Sensorian board is divided in nine modules. Each module corresponds to a specific functionality.
/// The modules can be used together or in standlaone mode. Kernel modules for each driver will be provided in a future release.
///
/// \par Dependencies 
///
/// The Sensorian driver library is dependent on the BCM 2835 C library which can be found at http://www.airspayce.com/mikem/bcm2835/
/// To use the Sensorian C driver library follow the instructions below:
/// First, download the library from the above link and then open a terminal and issue the following commands: 
/// \code
/// # download the latest version of the library,  bcm2835-1.xx.tar.gz, then:
/// tar zxvf bcm2835-1.xx.tar.gz
/// cd bcm2835-1.xx
/// ./configure
/// make
/// sudo make check
/// sudo make install
/// \endcode
///
/// \par Sensorian Library Installation
/// Download the code packagege from the website and unzip it. 
/// Compile each example by entering 
/// \code 
/// make 
/// \endcode
/// and execute with :
/// \code 
/// sudo ./ProgramName 
/// \endcode
///
/// \par Pin Numbering
/// The onboard sensors and peripherals are mapped to the following pins:
/// \code
/// - Altimeter/ barometer 		 		MPL_PIN 		RPI_V2_GPIO_P1_07
/// - Ambient light 		 			LUX_PIN 		RPI_V2_GPIO_P1_11
/// - Accelerometer/ Magnetometer 		ACLM_PIN 		RPI_V2_GPIO_P1_12
/// - Capacitive controller				ALERT_PIN 		RPI_V2_GPIO_P1_13
///	- LED								LED_PIN 		RPI_V2_GPIO_P1_15
/// - RTCC 								MFP_PIN 		RPI_V2_GPIO_P1_18
/// \endcode
///
/// \par SPI Pins
/// The SPI bus controllers is used for the TFT and SPI flash module.
/// The Raspberry Pi GPIO pins used for the SPI peripherals are:
/// 
/// - P1-24 (CE0) - TFT display
/// - P1-26 (CE1) - SPI NOR flash module
///
/// \par I2C Pins
/// The Raspberry Pi GPIO pins used for I2C are:
///
/// - P1-03 (SDA)
/// - P1-05 (SLC)
///
/// \code
/// GPIO PIN    		Function    
///    12                    0            
///    13                    1            
///    18                    0            
///    19                    1            
///    40                    0            
///    41                    1            
///    45                    1            
///    52                    0            
///    53                    1            
/// \endcode
///
/// \par Licensing 
/// Sensorian code is licensed under GPL V2.
///
///   \section main_licence License
///   <ul>
///   <li>\ref License
///   </ul>
///
///  \section contactinfo Contact Information
///  For further information, visit <a href="http://www.sensorian.com/">Sensorian</a>.\n
///
///  Copyright (c) 2015 Sensorian. All rights reserved.
/// 
///  \page License
///  Sensorian code is licensed under GPL V2.
/// 
///  THIS SOFTWARE IS PROVIDED BY Sensorian "AS IS" AND ANY EXPRESS OR IMPLIED
///  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
///  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
///  EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
///  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
///  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
///  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
///  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
///  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
///  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
///  POSSIBILITY OF SUCH DAMAGE.
/// 
/// \par Acknowledgements
/// The Sensorian code uses the BCM2835 library by Mike McCauley
/// 
/// \par Revision History
///
/// \version 1.0 Initial release
///
/// \author  Dhimiter Qendri (admin@sensorian.com) DO NOT CONTACT THE AUTHOR DIRECTLY


/// @example accelerometer.c
/// Demonstrates the usage of the orientation capabilites of the IMU.

/// @example RotatingCube.c
/// Demonstrates the use of the graphic API on the TFT display.

/// @example Blinky.c
/// Demonstrates the usage of the GPIO API.

