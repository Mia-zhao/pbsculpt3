

#include "PWMDriver.h"


PWMDriver::PWMDriver(uint8_t addr):Wire1(1) {
  _i2caddr = addr;
}

void PWMDriver::begin(void) {
 Wire1.begin(I2C_MASTER, 0x00, I2C_PINS_29_30, I2C_PULLUP_EXT, I2C_RATE_400);		
 reset();
}


void PWMDriver::reset(void) {

 write8(PCA9685_MODE1, 0x0);

}

void PWMDriver::setPWMFreq(float freq) {
  freq *= 0.9;  // Correct for overshoot in the frequency setting (see issue #11).
  float prescaleval = 25000000;
  prescaleval /= 4096;
  prescaleval /= freq;
  prescaleval -= 1;
  //Serial.print("Estimated pre-scale: "); Serial.println(prescaleval);
   uint8_t prescale = floor(prescaleval + 0.5);
  // Serial.print("Final pre-scale: "); Serial.println(prescale);  
  
  uint8_t oldmode = read8(PCA9685_MODE1);
  uint8_t newmode = (oldmode&0x7F) | 0x10; // sleep
  write8(PCA9685_MODE1, newmode); // go to sleep
  write8(PCA9685_PRESCALE, prescale); // set the prescaler
  write8(PCA9685_MODE1, oldmode);
  delay(5);
  write8(PCA9685_MODE1, oldmode | 0xa1);  //  This sets the MODE1 register to turn on auto increment.
}

void PWMDriver::setPWM(uint8_t num, uint16_t on, uint16_t off) {

  Wire1.beginTransmission(_i2caddr);
  Wire1.write(LED0_ON_L+4*num);
  Wire1.write(on);
  Wire1.write(on>>8);
  Wire1.write(off);
  Wire1.write(off>>8);
  Wire1.endTransmission(I2C_STOP, TIMEOUT);
}

// Sets pin without having to deal with on/off tick placement and properly handles
// a zero value as completely off.  Optional invert parameter supports inverting
// the pulse for sinking to ground.  Val should be a value from 0 to 4095 inclusive.
void PWMDriver::setPin(uint8_t num, uint16_t val, bool invert)
{
  // Clamp value between 0 and 4095 inclusive.
  val = min(val, 4095);
  if (invert) {
    if (val == 0) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 4095) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else {
      setPWM(num, 0, 4095-val);
    }
  }
  else {
    if (val == 4095) {
      // Special value for signal fully on.
      setPWM(num, 4096, 0);
    }
    else if (val == 0) {
      // Special value for signal fully off.
      setPWM(num, 0, 4096);
    }
    else {
      setPWM(num, 0, val);
    }
  }
}

void PWMDriver::setZeroDelay(uint8_t num) {

  Wire1.beginTransmission(_i2caddr);
  Wire1.write(LED0_ON_L+4*num);
  Wire1.write(0x0);
  Wire1.write(0x0);
  Wire1.endTransmission(I2C_STOP, TIMEOUT);
}

void PWMDriver::setPWMFast(uint8_t num, uint16_t off) {

  Wire1.beginTransmission(_i2caddr);
  Wire1.write(LED0_OFF_L+4*num);
  Wire1.write(off);
  Wire1.write(off>>8);
  Wire1.endTransmission(I2C_STOP, TIMEOUT);
}

uint8_t PWMDriver::read8(uint8_t addr) {
  Wire1.beginTransmission(_i2caddr);
  Wire1.write(addr);
  Wire1.endTransmission(I2C_STOP, TIMEOUT);

  Wire1.requestFrom((uint8_t)_i2caddr, (size_t)1, I2C_STOP, TIMEOUT);
  return Wire1.read();
}

void PWMDriver::write8(uint8_t addr, uint8_t d) {
  Wire1.beginTransmission(_i2caddr);
  Wire1.write(addr);
  Wire1.write(d);
  Wire1.endTransmission(I2C_STOP, TIMEOUT);
 
}
