tool
extends Node

#list with common baudrates
var baud_list= [300,600,1200,2400,4800,9600,14400,19200,28800,38400,57600,115200]

#the endline character, can be modified
export (String) var endline= "\n"

#byte size, to be used with open function (argument #4) and is optional
enum  bytesz{
    SER_BYTESZ_8 = 0,
    SER_BYTESZ_7,
    SER_BYTESZ_6,
    SER_BYTESZ_5
}

#parity, to be used with open function (argument #5) and is optional
enum parity{
    SER_PAR_NONE,
    SER_PAR_ODD,
    SER_PAR_EVEN,
    SER_PAR_MARK,
    SER_PAR_SPACE}

#stopbyte, to be used with open function (argument #6) and is optional
enum stopbyte{
    SER_STOPB_ONE, #1
    SER_STOPB_ONE5,#1.5
    SER_STOPB_TWO} #2

#queue, to be used with flush function 
#if not added, the function defaults to SER_QUEUE_IN
enum queue{
    SER_QUEUE_IN,
    SER_QUEUE_OUT,
    SER_QUEUE_ALL}


#a readline function, just add the current Port (based on sercomm)
#and it will return a line, since sercomm always use a timeout, it should not lag

func readline(port): 
	if !port.has_method("read"): #to avoid problems
		return "NOT A PORT"
	var cho=""
	var chango=""
	while(cho!=endline):
		cho=port.read()
		if typeof(cho)==TYPE_STRING:
			if cho!=endline:
				chango+=cho
		else:
			chango="FAILED"
			break
	return chango