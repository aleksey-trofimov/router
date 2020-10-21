#include "MPSUSerialDriver.h"

CMPSUSerialDriver::CMPSUSerialDriver(int port)
{
    	SetRange(SERIAL_PORT);
	SetDevice(port);
	sprintf(_pcPort_path,"/dev/ttyS%i", GetDevice());
	
	//Save current port settings
	tcgetattr(_iPort_descriptor,&_tSave_tio);
	
	InitPort();

}
void CMPSUSerialDriver::InitPort()
{
	INFO("CMPSUSerialDriver InitPort executed");
	struct termios newtio;
        
	_iPort_descriptor = open(_pcPort_path, O_RDWR | O_NOCTTY ); 
	if (_iPort_descriptor <0)
	{
	    perror(_pcPort_path);
	    exit(-1);
	}
//Delete current port settings
        bzero(&newtio, sizeof(newtio));
	
///////////////////////Port settings////////////////////////////

//Speed setting
	cfsetispeed(&newtio, MPSUBAUDRATE);
	cfsetospeed(&newtio, MPSUBAUDRATE);

//Disable hardware flow control and parity
	newtio.c_cflag &= ~(CRTSCTS | PARENB | CSIZE);
	
//Enable 8 character size, ignore modem control lines, enable receiver,
//set 2 stop bits
        newtio.c_cflag |= (CS8 | CLOCAL | CREAD);
	newtio.c_cflag |= CSTOPB;	
	
//Apply changes
	tcsetattr(_iPort_descriptor,TCSANOW,&newtio);
	
//Setting DTR & RTS to 1
	int status;
	ioctl(_iPort_descriptor, TIOCMGET, &status);
	status |= TIOCM_DTR;
	status &= ~TIOCM_RTS;
	ioctl(_iPort_descriptor, TIOCMSET, &status);

//Cleaning port	
	tcflush(_iPort_descriptor, TCIOFLUSH);

//---------------------------------------------------    

    cout<<"(in MPSUSerialDriver InitPort) Port opened - "<<_pcPort_path<<"\n";    
}
CMPSUSerialDriver::~CMPSUSerialDriver()
{
}

//---------------------------------MOUSE---------------------------------------
CMOUSESerialDriver::CMOUSESerialDriver(int port)
{
    	SetRange(SERIAL_PORT);
	SetDevice(port);
	sprintf(_pcPort_path,"/dev/ttyS%i", GetDevice());
	
	//Save current port settings
	tcgetattr(_iPort_descriptor,&_tSave_tio);
	
	InitPort();

}
void CMOUSESerialDriver::InitPort()
{
	INFO("CMOUSESerialDriver InitPort executed");
	struct termios newtio;
        
	_iPort_descriptor = open(_pcPort_path, O_RDWR | O_NOCTTY ); 
	if (_iPort_descriptor <0)
	{
	    perror(_pcPort_path);	    
	    exit(-1);
	}
        
//Delete current port settings
        bzero(&newtio, sizeof(newtio));
	
///////////////////////Port settings////////////////////////////

//Speed setting
	cfsetispeed(&newtio, MPSUBAUDRATE);
	cfsetospeed(&newtio, MPSUBAUDRATE);

//Disable hardware flow control and parity
	newtio.c_cflag &= ~(CRTSCTS | PARENB | CSIZE);
	
//Enable 8 character size, ignore modem control lines, enable receiver,
//set 2 stop bits
        newtio.c_cflag |= (CS8 | CLOCAL | CREAD);
	newtio.c_cflag |= CSTOPB;	
	
//Apply changes
	tcsetattr(_iPort_descriptor,TCSANOW,&newtio);	

//Cleaning port	
	tcflush(_iPort_descriptor, TCIOFLUSH);

//---------------------------------------------------    
    cout<<"(in MOUSESerialDriver InitPort) Port opened - "<<_pcPort_path<<"\n";    
}

CMOUSESerialDriver::~CMOUSESerialDriver()
{
}

//---------------------------------CONFIG---------------------------------------
CCONFIGSerialDriver::CCONFIGSerialDriver(int port)
{
    	SetRange(SERIAL_PORT);
	SetDevice(port);
	sprintf(_pcPort_path,"/dev/ttyS%i", GetDevice());
	
	//Save current port settings
	tcgetattr(_iPort_descriptor,&_tSave_tio);
	
	InitPort();

}
void CCONFIGSerialDriver::InitPort()
{
	INFO("CCONFIGSerialDriver InitPort executed");
	struct termios newtio;
        
	_iPort_descriptor = open(_pcPort_path, O_RDWR | O_NOCTTY ); 
	if (_iPort_descriptor <0)
	{
	    perror(_pcPort_path);	    
	    exit(-1);
	}
        
//Delete current port settings
        bzero(&newtio, sizeof(newtio));
	
///////////////////////Port settings////////////////////////////

//Speed setting
	cfsetispeed(&newtio, MPSUBAUDRATE);
	cfsetospeed(&newtio, MPSUBAUDRATE);

//Disable hardware flow control and parity
	newtio.c_cflag &= ~(CRTSCTS | PARENB | CSIZE);
	
//Enable 8 character size, ignore modem control lines, enable receiver,
//set 2 stop bits
        newtio.c_cflag |= (CS8 | CLOCAL | CREAD);
	newtio.c_cflag |= CSTOPB;	
	
//Apply changes
	tcsetattr(_iPort_descriptor,TCSANOW,&newtio);	

//Cleaning port	
	tcflush(_iPort_descriptor, TCIOFLUSH);

//---------------------------------------------------    

    cout<<"(in CONFIGSerialDriver InitPort) Port opened - "<<_pcPort_path<<"\n";    
}

CCONFIGSerialDriver::~CCONFIGSerialDriver()
{
}



