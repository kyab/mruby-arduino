	
#include "mruby.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/data.h"

#if (ARDUINO >= 100)
	#include <Arduino.h>
#else
	#include <WProgram.h>
#endif

#include <Servo/Servo.h>

mrb_value mrb_serial_available(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(Serial.available());
}

mrb_value mrb_serial_begin(mrb_state *mrb, mrb_value self){
  mrb_int speed = 0;
  int n = mrb_get_args(mrb,"i",&speed);
  Serial.begin(speed);
  return mrb_nil_value();
}


mrb_value mrb_serial_println(mrb_state *mrb, mrb_value self){
  mrb_value s;
  mrb_get_args(mrb,"S", &s);
  Serial.println(RSTRING_PTR(s));
  return mrb_nil_value();
}

void mrb_servo_free(mrb_state *mrb, void *ptr){
  delete (Servo *)ptr;
}

struct mrb_data_type mrb_servo_type = {"Servo", mrb_servo_free};

mrb_value mrb_servo_initialize(mrb_state *mrb, mrb_value self){
  Servo *newServo = new Servo();
  DATA_PTR(self) = newServo;  
  DATA_TYPE(self) = &mrb_servo_type;  
  return self;
}

mrb_value mrb_servo_attach(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  
  mrb_int pin = 0;
  int n = mrb_get_args(mrb, "i", &pin);
  servo->attach(pin);
  return mrb_nil_value();
}

mrb_value mrb_servo_write(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  mrb_int angle = 0;
  int n = mrb_get_args(mrb, "i", &angle);
  servo->write(angle);
  return mrb_nil_value();
}


mrb_value mrb_arduino_pinMode(mrb_state *mrb, mrb_value self){
  mrb_int pin, mode;
  int n = mrb_get_args(mrb, "ii", &pin, &mode);
  pinMode(pin, mode);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  int n = mrb_get_args(mrb, "ii", &pin, &value);
  digitalWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_digitalRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  int n = mrb_get_args(mrb, "i", &pin);
  int val = digitalRead(pin);
  return mrb_fixnum_value(val);
}

mrb_value mrb_arduino_analogReference(mrb_state *mrb, mrb_value self){
  mrb_int type;
  int n = mrb_get_args(mrb, "i", &type);
  analogReference(type);
  return mrb_nil_value();
}

mrb_value mrb_arduino_analogWrite(mrb_state *mrb, mrb_value self){
  mrb_int pin, value;
  int n = mrb_get_args(mrb, "ii", &pin, &value);
  analogWrite(pin, value);
  return mrb_nil_value();
}

mrb_value mrb_arduino_analogRead(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  int n = mrb_get_args(mrb, "i", &pin);
  int val = analogRead(pin);
  return mrb_fixnum_value(val);
}

mrb_value mrb_arduino_tone(mrb_state *mrb, mrb_value self){
  mrb_int pin, frequency, duration;
  int n = mrb_get_args(mrb, "ii|i", &pin, &frequency, &duration);
  if (n == 2){
    tone(pin, frequency);
  }else if(n == 3){
    tone(pin, frequency, duration);
  }
  return mrb_nil_value();
}

mrb_value mrb_arduino_noTone(mrb_state *mrb, mrb_value self){
  mrb_int pin;
  int n = mrb_get_args(mrb, "i", &pin);
  noTone(pin);
  return mrb_nil_value();
}

mrb_value mrb_arduino_shiftOut(mrb_state *mrb, mrb_value self){
  mrb_int dataPin, clockPin, bitOrder, value;
  int n = mrb_get_args(mrb, "iiii", &dataPin, &clockPin, &bitOrder, &value);
  shiftOut(dataPin, clockPin, bitOrder, (byte)value);
  return mrb_nil_value();
}

#if defined(MPIDE) && MPIDE <= 23
  //seems like MPIDE does not define shiftIn.
  extern uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);
#endif

mrb_value mrb_arduino_shiftIn(mrb_state *mrb, mrb_value self){
  mrb_int dataPin, clockPin, bitOrder;
  int n = mrb_get_args(mrb, "iii", &dataPin, &clockPin, &bitOrder);
  return mrb_fixnum_value(shiftIn(dataPin, clockPin, bitOrder));
}
 
mrb_value mrb_arduino_pulseIn(mrb_state *mrb, mrb_value self){
  mrb_int pin, value, timeout;
  int n = mrb_get_args(mrb, "ii|i", &pin, &value, &timeout);
  
  unsigned long ret = 0;
  if (n == 2){
    ret = pulseIn(pin, value);
  }else if (n == 3){
    ret = pulseIn(pin, value, timeout);
  }

  return mrb_fixnum_value(ret);
}

mrb_value mrb_arduino_millis(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(millis());
}

mrb_value mrb_arduino_micros(mrb_state *mrb, mrb_value self){
  return mrb_fixnum_value(micros());
}

mrb_value mrb_arduino_delay(mrb_state *mrb, mrb_value self){
  mrb_int ms;
  int n = mrb_get_args(mrb, "i", &ms);
  delay(ms);
  return mrb_nil_value();
}

mrb_value mrb_arduino_delayMicroseconds(mrb_state *mrb, mrb_value self){
  mrb_int us;
  int n = mrb_get_args(mrb, "i", &us);
  delayMicroseconds(us);
  return mrb_nil_value();
}


mrb_value mrb_arduino_map(mrb_state *mrb, mrb_value self){
  mrb_int value, fromLow, fromHigh, toLow, toHigh;
  int n = mrb_get_args(mrb, "iiiii", &value, &fromLow, &fromHigh, &toLow, &toHigh);
  mrb_int ret = map(value, fromLow, fromHigh, toLow, toHigh);
  return mrb_fixnum_value(ret);
}

mrb_value mrb_arduino_randomSeed(mrb_state *mrb, mrb_value self){
  mrb_int seed;
  int n = mrb_get_args(mrb, "i", &seed);
  randomSeed(seed);
  return mrb_nil_value();
}

mrb_value mrb_arduino_random(mrb_state *mrb, mrb_value self){
  mrb_int arg1, arg2;
  int n = mrb_get_args(mrb, "i|i", &arg1, &arg2 );
  if (n == 1){
    return mrb_fixnum_value(random(arg1));
  }else{
    return mrb_fixnum_value(random(arg1, arg2));
  }
}

mrb_value mrb_arduino_interrupts(mrb_state *mrb, mrb_value self){
  interrupts();
  return mrb_nil_value();
}

mrb_value mrb_arduino_noInterrupts(mrb_state *mrb, mrb_value self){
  noInterrupts();
  return mrb_nil_value();
}

extern "C"
void
mrb_mruby_arduino_gem_init(mrb_state* mrb) {

  RClass *serialClass = mrb_define_class(mrb, "Serial", mrb->object_class);
  mrb_define_class_method(mrb, serialClass, "available", mrb_serial_available, ARGS_NONE());
  
  mrb_define_class_method(mrb, serialClass,"begin",mrb_serial_begin, ARGS_REQ(1));
  mrb_define_class_method(mrb, serialClass,"println", mrb_serial_println, ARGS_REQ(1));


  RClass *servoClass = mrb_define_class(mrb, "Servo", mrb->object_class);
  MRB_SET_INSTANCE_TT(servoClass, MRB_TT_DATA);
  mrb_define_method(mrb, servoClass,"initialize", mrb_servo_initialize, ARGS_NONE());
  mrb_define_method(mrb, servoClass,"attach", mrb_servo_attach, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass,"write", mrb_servo_write, ARGS_REQ(1));
  
  RClass *arduinoModule = mrb_define_module(mrb, "Arduino");
  mrb_define_module_function(mrb, arduinoModule, "pinMode", mrb_arduino_pinMode, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalWrite", mrb_arduino_digitalWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalRead", mrb_arduino_digitalRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogReference", mrb_arduino_analogReference, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogWrite", mrb_arduino_analogWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "analogRead", mrb_arduino_analogRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "tone", mrb_arduino_tone, ARGS_REQ(2) | ARGS_OPT(1));
  mrb_define_module_function(mrb, arduinoModule, "noTone", mrb_arduino_noTone, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "shiftOut", mrb_arduino_shiftOut, ARGS_REQ(4));
  mrb_define_module_function(mrb, arduinoModule, "shiftIn", mrb_arduino_shiftOut, ARGS_REQ(3));
  mrb_define_module_function(mrb, arduinoModule, "pulseIn", mrb_arduino_pulseIn, ARGS_REQ(2) | ARGS_OPT(1));
  mrb_define_module_function(mrb, arduinoModule, "millis", mrb_arduino_millis, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "micros", mrb_arduino_micros, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "delay", mrb_arduino_delay, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "delayMicroseconds", mrb_arduino_delayMicroseconds, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "map", mrb_arduino_map, ARGS_REQ(5));
  mrb_define_module_function(mrb, arduinoModule, "randomSeed", mrb_arduino_randomSeed, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "random", mrb_arduino_random, ARGS_REQ(1) | ARGS_OPT(1));
  mrb_define_module_function(mrb, arduinoModule, "interrupts", mrb_arduino_interrupts, ARGS_NONE());
  mrb_define_module_function(mrb, arduinoModule, "noInterrupts", mrb_arduino_noInterrupts, ARGS_NONE());


  mrb_define_const(mrb, arduinoModule, "HIGH", mrb_fixnum_value(HIGH));
  mrb_define_const(mrb, arduinoModule, "LOW", mrb_fixnum_value(LOW));
  mrb_define_const(mrb, arduinoModule, "INPUT", mrb_fixnum_value(INPUT));
  mrb_define_const(mrb, arduinoModule, "OUTPUT", mrb_fixnum_value(OUTPUT));

#ifdef INPUT_PULLUP
  mrb_define_const(mrb, arduinoModule, "INPUT_PULLUP", mrb_fixnum_value(INPUT_PULLUP));
#endif
  mrb_define_const(mrb, arduinoModule, "DEFAULT", mrb_fixnum_value(DEFAULT));
  mrb_define_const(mrb, arduinoModule, "INTERNAL", mrb_fixnum_value(INTERNAL));
  mrb_define_const(mrb, arduinoModule, "EXTERNAL", mrb_fixnum_value(EXTERNAL));
  //for chipKit, below are not defined.
#ifdef INTERNAL1V1
  mrb_define_const(mrb, arduinoModule, "INTERNAL1V1", mrb_fixnum_value(INTERNAL1V1));
#endif

#ifdef INTERNAL2V56
  mrb_define_const(mrb, arduinoModule, "INTERNAL2V56", mrb_fixnum_value(INTERNAL2V56));
#endif

  mrb_define_const(mrb, arduinoModule, "MSBFIRST", mrb_fixnum_value(MSBFIRST));
  mrb_define_const(mrb, arduinoModule, "LSBFIRST", mrb_fixnum_value(LSBFIRST));
  
}

extern "C"
void
mrb_mruby_arduino_gem_final(mrb_state* mrb) {

}