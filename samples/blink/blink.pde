
#if defined(ARDUINO) && (ARDUINO==153)
#include <Wire.h>
#endif

#include <Servo.h>

#ifdef MPIDE
#else
#include <Arduino.h>
#endif

#include <stdio.h>

#include "mruby.h"
#include "mruby/class.h"
#include "mruby/value.h"
#include "mruby/irep.h"

//for chipKIT Max32
#ifdef MPIDE
//enhance heap
#define CHANGE_HEAP_SIZE(size) __asm__ volatile ("\t.globl _min_heap_size\n\t.equ _min_heap_size, " #size "\n")
CHANGE_HEAP_SIZE(110 * 1024);

#endif


mrb_state *mrb;
mrb_value blinker_obj;
int ai;

extern const uint8_t blinker[];

void printlnSize(size_t size){
	char str[15];
	sprintf(str,"%u", size);
	Serial.println(str);
}

// custom allocator to check heap shortage.
void *myallocf(mrb_state *mrb, void *p, size_t size, void *ud){
	if (size == 0){
		free(p);
		return NULL;
	}

	void *ret = realloc(p, size);
	if (!ret){
		Serial.print("memory allocation error. size:");
		printlnSize(size);
	}
}

void setup(){

	Serial.begin(9600);
	Serial.println("Hello Arduino");

	//Init mruby
	mrb = mrb_open_allocf(myallocf, NULL);

	//Load Blinker bytecode
	mrb_load_irep(mrb, blinker);

	Serial.println("mruby initialized");

	//Get Blinker class and create instance.
	//equivalent to ruby: blinker_obj = Blinker.new(13,1000)
	RClass *blinker_class = mrb_class_get(mrb, "Blinker");
	if (mrb->exc){
		Serial.println("failed to load class Blinker");
	}

	mrb_value args[2];
	args[0] = mrb_fixnum_value(13);		//pin Number
	args[1] = mrb_fixnum_value(1000);	//interval
	blinker_obj = mrb_class_new_instance(mrb, 2, args, blinker_class);

	//is exception occure?
	if (mrb->exc){
		Serial.println("failed to create Blinker instance");
		return;
	}

	ai = mrb_gc_arena_save(mrb);

}

void loop(){

	mrb_funcall(mrb, blinker_obj, "run", 0);

	//is exception occure?
	if (mrb->exc){
		Serial.println("failed to run!");
		mrb->exc = 0;
		delay(1000);
	}

	mrb_gc_arena_restore(mrb,ai);
}



//required to link with mruby-arduino
void __dummy(){
#if defined(ARDUINO) && (ARDUINO==153)
#else
  random(1,1);
#endif

#ifdef MPIDE
  tone(1,2,3);
#endif

#if defined(ARDUINO) && (ARDUINO==153)
#else
  pulseIn(1,2,3);
  shiftOut(1,2,3,4);
#endif
}
