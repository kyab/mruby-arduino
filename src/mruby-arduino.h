#ifndef MRUBY_ARDUINO_H
#define MRUBY_ARDUINO_H

#if defined(MPIDE) || defined(__SAM3X8E__)
	#define MRUBY_ARDUINO_BOARD_CHIPKIT_OR_DUE
	/*#pragma message "Arduino Due or chipKITMax32 detected"*/
#elif defined(BOARD_discovery_f4)
	#define MRUBY_ARDUINO_BOARD_DISCOVERYF4
	/*#pragma message "STM32F4 Discovery detected"*/
#elif defined(ARDUINO) && (ARDUINO==153)
	#define MRUBY_ARDUINO_BOARD_GALILEO
	/*#pragma message "Intel Galileo detected"*/
#else
	/*Treat as chipKIT or ArduinoDue for unknown boards.*/
	#define MRUBY_ARDUINO_BOARD_CHIPKIT_OR_DUE
	#pragma message "Assume Arduino Due or chipKITMax32"
#endif


#endif
