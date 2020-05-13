# `RPi tool`

Raspberry Pi tool to read and write GPIO. Includes an implementation of IPC communication. It was created to remotely controll writing data from an SD card to the testbench. Rebooting with the fresh data was necessary  in the process of testing the software. 

* read_write.c - programm was run on the RPi as daemon. It reads and writes GPIO pins according to received instructions. 
* hello.cpp - the program can be run remotely from the LAN network and passes received data to a separate process-read_write.c This structure is necessary since it is not possible co controll GPIO pins with a program that is run from a remote destination (insuficient provilages). The read_write.c run as daemon has superuser privilages and can controll the pins.
* GPIORPi.py - is used to send the requests to hello.cpp. Data is passed in json format
