#include "mruby.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/data.h"

#include "mruby-arduino.h"

#if defined(MRUBY_ARDUINO_BOARD_CHIPKIT_OR_DUE)
	extern void mruby_arduino_init_chipKIT_or_Due(mrb_state *mrb);
#elif defined(MRUBY_ARDUINO_BOARD_DISCOVERYF4)
	extern void mruby_arduino_init_discoveryF4(mrb_state *mrb);
#elif defined(MRUBY_ARDUINO_BOARD_GALILEO)
	extern void mruby_arduino_init_galileo(mrb_state *mrb);
#endif


void
mrb_mruby_arduino_gem_init(mrb_state* mrb) 
{

#if defined(MRUBY_ARDUINO_BOARD_CHIPKIT_OR_DUE)
	mruby_arduino_init_chipKIT_or_Due(mrb);
#elif defined(MRUBY_ARDUINO_BOARD_DISCOVERYF4)
	mruby_arduino_init_discoveryF4(mrb);
#elif defined(MRUBY_ARDUINO_BOARD_GALILEO)
	mruby_arduino_init_galileo(mrb);
#endif
}

void
mrb_mruby_arduino_gem_final(mrb_state* mrb) {

}


