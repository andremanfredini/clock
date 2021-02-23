#include <Wire.h>
#include <TM1637.h>

#define CLK 6
#define DIO 7
#define brightness 2
#define DS3231_I2C_ADDRESS 0x68

TM1637 tm1637(CLK, DIO);


volatile boolean flag;
//////
// Converter números decimais normais em decimais codificados em binário
byte decToBcd(byte val) {
  return ( (val / 10 * 16) + (val % 10) );
}
// Converte decimais codificados em binários em números decimais normais
byte bcdToDec(byte val) {
  return ( (val / 16 * 10) + (val % 16) );
}
//////////

void setDateDs3231(byte second,
                   byte minute, // 0-59
                   byte hour, // 1-23
                   byte dayOfWeek, // 1-7
                   byte dayOfMonth, // 1-28/29/30/31
                   byte month, // 1-12
                   byte year) // 0-99
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.write(decToBcd(second));
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));
  Wire.write(decToBcd(dayOfWeek));
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void getDateDs3231(byte *second,
                   byte *minute,
                   byte *hour,
                   byte *dayOfWeek,
                   byte *dayOfMonth,
                   byte *month,
                   byte *year)
{

  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0);
  Wire.endTransmission();

  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);

  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void setINT() {
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0x0E);
  Wire.write(0x0);
  Wire.endTransmission();
}


boolean alterna;

void setup() {

  Wire.begin();

  tm1637.init();
  tm1637.set(brightness);

}


void loop() {

  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  getDateDs3231(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);

  int8_t TimeDisp[4];

  TimeDisp[0] = hour / 10;
  TimeDisp[1] = hour % 10;
  TimeDisp[2] = minute / 10;
  TimeDisp[3] = minute % 10;

  tm1637.display(TimeDisp);

  if (alterna)
  {
    tm1637.point(POINT_OFF);
    alterna = false;
  }
  else
  {
    tm1637.point(POINT_ON);
    alterna = true;
  }

  delay(500);

}
