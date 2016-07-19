/**
 * @file SensorsInterface.h
 * @author Dylan Kauling
 * @date 24 June 2016
 * @brief Header for the Sensorian SensorsInterface file, which is used to easily call basic sensor functions
 */

#ifndef C_SENSORSINTERFACE_H
#define C_SENSORSINTERFACE_H

int setupSensorian(void);
float getAmbientLight(void);
void pollMPL(void);
int getTemperature(void);
int getAltitude(void);
int getBarometricPressure(void);
void pollFXOS(void);
int getMagX(void);
int getMagY(void);
int getMagZ(void);
int getAccelX(void);
int getAccelY(void);
int getAccelZ(void);
void poll_rtcc(void);
int get_rtcc_year(void);
int get_rtcc_month(void);
int get_rtcc_date(void);
int get_rtcc_hour(void);
int get_rtcc_minute(void);
int get_rtcc_second(void);
void set_rtcc_datetime(int year, int month, int date, int w_day, int hour, int minute, int second);
void set_rtcc_alarm(int year, int month, int date, int w_day, int hour, int minute, int second, int match_mode);
int poll_rtcc_alarm(void);
void reset_alarm(void);
void orange_led_on(void);
void orange_led_off(void);

#endif //C_SENSORSINTERFACE_H
