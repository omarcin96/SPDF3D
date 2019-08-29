# cmake minimal version required to work.
cmake_minimum_required(VERSION 3.14.4)

function(FIND_LIBS PREF LIBS_LIST LIBS_DIR LIBS_OUT)
		MESSAGE("LOAD: ""${${PREF}}" "${${LIB_LIST}}")
		set(LIBS_OUT_INTERNAL "")
		
	FOREACH(LIB ${${LIBS_LIST}})
		FIND_LIBRARY(${${PREF}}_LIB_${LIB} ${LIB} PATH ${${LIBS_DIR}})
		LIST(APPEND LIBS_OUT_INTERNAL ${${${PREF}}_LIB_${LIB}})
		MESSAGE("Lib: ${LIB}")
		MESSAGE("Found Lib: ${${PREF}}_LIB_${LIB}")
	ENDFOREACH(LIB)
	
	#message("List libraries: " ${LIBS_OUT_INTERNAL})
	set(${LIBS_OUT} ${LIBS_OUT_INTERNAL} PARENT_SCOPE) 
	
endfunction()