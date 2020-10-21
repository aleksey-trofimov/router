// CacheData.cpp: implementation of the CCacheData class.
//
//////////////////////////////////////////////////////////////////////
#include "MPSURecord.h"

CMPSURecord::CMPSURecord():
    ID		( 0 ),
    Device_ID	( 0xFF ),
    Ind		( 0xFF ),
    N		( 0xFF ),
    Code	( 0xFF ),
    Nchan	( 0xFF ),
    Byte	( 0xFF )
{
}

CMPSURecord::CMPSURecord( int _ID, int _Device_ID, BYTE _Ind, BYTE _N, BYTE _Code, BYTE _Nchan, BYTE _Byte):
    ID		( _ID ),
    Device_ID	( _Device_ID ),
    Ind		( _Ind ),
    N		( _N ),
    Code	( _Code ),
    Nchan	( _Nchan ),
    Byte	( _Byte )
{
}

CMPSURecord::CMPSURecord(int id)
{
    div_t x;
    
    if(id<=0 || id>=241)
    {
	//Incorrect id received
	cout<<"Incorrect id received. id="<<id<<"\n";
	ID		=id;
	Device_ID	=0xFF;
	Ind		=0xFF;
	N		=0xFF;
	Code		=0xFF;
	Nchan		=0xFF;
	Byte		=0xFF;	
    }
    
    //M201
    if(id>=1 && id<=24)
    {
	Ind = 0x03;
	Code = 'D';
	Nchan = 0x00;
	Device_ID = 0xFF;
	if(id%2 > 0)//nechet
	{
	    Byte = 0x00;
	    x = div(id,2);
	    N = x.quot;
	}
	else//chet
	{
	    Byte = 0x01;
	    x = div(id,2);
	    N = x.quot-1;
	}	
    }
    
    //M203
    if(id>=25 && id<=48)
    {
	Ind = 0x05;
	Code = 'C';
	Byte = 0x00;
	Device_ID = 0xFF;
	if(id%2 > 0)//nechet
	{
	    Nchan = 0x00;
	    x = div(id,2);
	    N = x.quot-12;//here we got 12 with id=25
	}
	else //chet
	{
	    Nchan = 0x01;
	    x = div(id,2);
	    N = x.quot-12-1;
	}	
    }
////////////////////
    //M204
    if(id>=49 && id<=144)
    {
	Ind = 0x06;
	Code = 'U';
	Byte = 0x00;
	Device_ID = 0xFF;
	
	int m204_index = id-49;// 0,1,2,3,4...95
	x = div(m204_index,8);
	N = x.quot;//Got N
        int chan_por = m204_index - (N*8);
	Nchan = 1<<chan_por;//Got Nchan	
    }

////////////////////

////////////////////
    //M210
    if(id>=145 && id<=240)
    {
	Ind = 0x0B;
	Code = 'V';
	Byte = 0x00;
	Device_ID = 0xFF;
	
	int m210_index = id-145;// 0,1,2,3,4...95
	x = div(m210_index,16);
	N = x.quot;//Got N
        Nchan = m210_index - (N*16);//Got Nchan
    }
////////////////////

    ID = id;
}

CMPSURecord::~CMPSURecord()
{
}

bool CMPSURecord::operator==(const CMPSURecord& cdata)
{
    if(    (ID		==cdata.ID)
    && (Device_ID	==cdata.Device_ID)
    && (Ind		==cdata.Ind)
    && (N		==cdata.N)
    && (Code		==cdata.Code)
    && (Nchan		==cdata.Nchan)
    && (Byte		==cdata.Byte))	    
        return true;	    
    else    
        return false;
}
void CMPSURecord::print(string comment)
{
    cout<<comment.c_str()
    <<" ID="<<ID
    <<" Device_ID="<<(int)Device_ID
    <<" Ind="<<(int)Ind
    <<" N="<<(int)N
    <<" Code="<<(int)Code
    <<" Nchan="<<(int)Nchan
    <<" Byte="<<(int)Byte
    <<"\n";
}
