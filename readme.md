###Tuber Framework

A C++ Framework inspired by PHP Laravel Framework. This framework is written on top of GTK+3.

##Dependencies

Tuber is dependent on
1. GTK+3 - Graphics and widget library
2. rapidjson - json Parser library for C++

##Configuration

Config files goes inside 'config' directory where the compiled version of the application is set.
Main Config file is 'app.json' which is loaded during application 'Preboot'.

#Default configuration needed

All the configuration is set inside 'config' key. Default Configuration would be something like this.

{
	... : ...,
	"config" : {
		"database" : {
			"drivers" : {
				"sqlite" : {
					"database" : "test.db"
				}
			},
			"defaultDriver" : "sqlite"
		},
		"defaultController" : "MainController"
	}
} 

##Views

View files goes inside 'views' directory where the compiled version of the application is located. Views are basically '*.glade' files with the name of root element "root". 