# BastionBuds Chat App

### How to run the server

1. `cd` to the base directory which contains the Makefile
2. run `make`
3. run  `./main` 

### How to run the client

1. Make sure that Python is installed with `sudo apt-get install --upgrade python`
2. Install  npyscreen with `pip3 install npyscreen`
3. run `python3 client/client.py`

To use the client, type in the input box, press Tab to shift focus to the "Send" button, and press Enter to send your message. Shift + Tab will shift focus back to the input box if needed.

##### Optional parameters

*  `--ip` to specify the IP address,
* `--port` to specify the port, and
* `--script` to specify the filename of a script to run

A command using all these optional parameters might look like `python3 client/client.py --ip 192.168.0.1 --port 1337 --script test_script_1`

#### How to write a script

Each line in the script corresponds to a command that the client will send. The client sleeps for 1 second in between each command. You may find example scripts are in the base directory.