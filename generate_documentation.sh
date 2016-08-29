#!/bin/bash

if [ ! -e doxygen_config_file ]
then
        echo "Could not find configuration file for doxygen."
        echo "Creating default config file..."
        doxygen -g doxygen_config_file
fi

doxygen doxygen_config_file

