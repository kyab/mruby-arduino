#include "mruby-arduino.h"
#ifdef MRUBY_ARDUINO_BOARD_DISCOVERYF4

#include <new>
	
#include "mruby.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/data.h"

#include <WProgram.h>

#include <Servo/Servo.h>

#define MRUBY_ARDUINO_DEFINE_SERIAL_METHODS(serial) \
  mrb_value mrb_##serial##_available(mrb_state *mrb, mrb_value self){ \
    return mrb_fixnum_value(serial.available()); \
  } \
 \
  mrb_value mrb_##serial##_begin(mrb_state *mrb, mrb_value self){ \
    mrb_int speed = 0;  \
    int n = mrb_get_args(mrb,"i",&speed); \
    serial.begin(speed);  \
    return mrb_nil_value(); \
  } \
 \
  mrb_value mrb_##serial##_println(mrb_state *mrb, mrb_value self){ \
    mrb_value s;  \
    mrb_get_args(mrb,"S", &s); \
    for (int i = 0; i < RSTRING_LEN(s); i++){ \
      serial.print( RSTRING_PTR(s)[i] ); \
    } \
    serial.println(""); \
    return mrb_nil_value();  \
  } \
  \
  mrb_value mrb_##serial##_print(mrb_state *mrb, mrb_value self){ \
    mrb_value s;  \
    mrb_get_args(mrb,"S", &s); \
    for (int i = 0; i < RSTRING_LEN(s); i++){ \
      serial.print( RSTRING_PTR(s)[i] ); \
    } \
    return mrb_nil_value(); \
  } \

MRUBY_ARDUINO_DEFINE_SERIAL_METHODS(Serial2)
MRUBY_ARDUINO_DEFINE_SERIAL_METHODS(Serial3)

void mrb_servo_free(mrb_state *mrb, void *ptr){
  Servo *servo = (Servo *)ptr;
  servo->~Servo();
  mrb_free(mrb, servo);
}

struct mrb_data_type mrb_servo_type = {"Servo", mrb_servo_free};

mrb_value mrb_servo_initialize(mrb_state *mrb, mrb_value self){
  void *p = mrb_malloc(mrb, sizeof(Servo));
  Servo *newServo = new(p) Servo();
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

mrb_value mrb_servo_detach(mrb_state *mrb, mrb_value self){
  Servo *servo = (Servo *)mrb_get_datatype(mrb, self, &mrb_servo_type);
  servo->detach();
  return mrb_nil_value();
}


mrb_value mrb_arduino_pinMode(mrb_state *mrb, mrb_value self){
  mrb_int pin, mode;
  int n = mrb_get_args(mrb, "ii", &pin, &mode);
  pinMode(pin, (WiringPinMode)mode);
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

mrb_value mrb_arduino_shiftOut(mrb_state *mrb, mrb_value self){
  mrb_int dataPin, clockPin, bitOrder, value;
  int n = mrb_get_args(mrb, "iiii", &dataPin, &clockPin, &bitOrder, &value);
  shiftOut(dataPin, clockPin, bitOrder, (byte)value);
  return mrb_nil_value();
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

#define MRUBY_ARDUINO_DEFINE_SERIAL(serial) \
  RClass * serial##Class = mrb_define_class(mrb, #serial, mrb->object_class); \
  mrb_define_class_method(mrb, serial##Class, "available", mrb_##serial##_available, ARGS_NONE()); \
  mrb_define_class_method(mrb, serial##Class, "begin",mrb_##serial##_begin, ARGS_REQ(1)); \
  mrb_define_class_method(mrb, serial##Class, "println", mrb_##serial##_println, ARGS_REQ(1)); \
  mrb_define_class_method(mrb, serial##Class, "print", mrb_##serial##_print, ARGS_REQ(1)); 

extern "C"
void
mruby_arduino_init_discoveryF4(mrb_state* mrb) {

  MRUBY_ARDUINO_DEFINE_SERIAL(Serial2);
  MRUBY_ARDUINO_DEFINE_SERIAL(Serial3);

  RClass *servoClass = mrb_define_class(mrb, "Servo", mrb->object_class);
  MRB_SET_INSTANCE_TT(servoClass, MRB_TT_DATA);
  mrb_define_method(mrb, servoClass, "initialize", mrb_servo_initialize, ARGS_NONE());
  mrb_define_method(mrb, servoClass, "attach", mrb_servo_attach, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass, "write", mrb_servo_write, ARGS_REQ(1));
  mrb_define_method(mrb, servoClass, "detach", mrb_servo_detach, ARGS_NONE());
  
  RClass *arduinoModule = mrb_define_module(mrb, "Arduino");
  mrb_define_module_function(mrb, arduinoModule, "pinMode", mrb_arduino_pinMode, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalWrite", mrb_arduino_digitalWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "digitalRead", mrb_arduino_digitalRead, ARGS_REQ(1));
  mrb_define_module_function(mrb, arduinoModule, "analogWrite", mrb_arduino_analogWrite, ARGS_REQ(2));
  mrb_define_module_function(mrb, arduinoModule, "analogRead", mrb_arduino_analogRead, ARGS_REQ(1));

  mrb_define_module_function(mrb, arduinoModule, "shiftOut", mrb_arduino_shiftOut, ARGS_REQ(4));
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
  mrb_define_const(mrb, arduinoModule, "OUTPUT_OPEN_DRAIN", mrb_fixnum_value(OUTPUT_OPEN_DRAIN));
  mrb_define_const(mrb, arduinoModule, "INPUT_ANALOG", mrb_fixnum_value(INPUT_ANALOG));
  mrb_define_const(mrb, arduinoModule, "INPUT_PULLUP", mrb_fixnum_value(INPUT_PULLUP));
  mrb_define_const(mrb, arduinoModule, "INPUT_PULLDOWN", mrb_fixnum_value(INPUT_PULLDOWN));
  mrb_define_const(mrb, arduinoModule, "INPUT_FLOATING", mrb_fixnum_value(INPUT_FLOATING));
  mrb_define_const(mrb, arduinoModule, "PWM", mrb_fixnum_value(PWM));
  mrb_define_const(mrb, arduinoModule, "PWM_OPEN_DRAIN", mrb_fixnum_value(PWM_OPEN_DRAIN));

}


#endif /*MRUBY_ARDUINO_DISCOVERYF4*/