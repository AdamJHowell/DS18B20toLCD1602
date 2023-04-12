cmake_minimum_required( VERSION 3.24 )
project( DS18B20toLCD1602 )

set( CMAKE_CXX_STANDARD 17 )

add_executable( DS18B20toLCD1602
		DS18B20toLCD1602.ino )

set_target_properties( DS18B20toLCD1602 PROPERTIES LINKER_LANGUAGE CXX )
