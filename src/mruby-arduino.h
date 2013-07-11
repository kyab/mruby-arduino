#ifndef MRUBY_ARDUINO_H
#define MRUBY_ARDUINO_H

#if defined(MPIDE) || defined(__SAM3X8E__)
	#define MRUBY_ARDUINO_BOARD_CHIPKIT_OR_DUE
#elif defined(BOARD_discovery_f4)
	#define MRUBY_ARDUINO_BOARD_DISCOVERYF4
#else
	/*Treat as chipKIT or ArduinoDue for unknown boards.*/
	#define MRUBY_ARDUINO_CHIPKIT_OR_DUE
#endif


#endif
