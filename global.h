// Definition of the widely used functions

#ifndef global_h
#define global_h

#include "definitions.h"

extern void LOG_IT(std::string info);
extern void TEXTOUT(std::string info, vector<BYTE> v);
extern void TEXTOUT(std::string info);
extern void TEXTOUT(std::string info, BYTE number);
extern vector<BYTE> ERROR();

extern vector<BYTE> B2V(BYTE* data, int size);
extern void V2B(vector<BYTE> vdata, BYTE* output);

#endif
