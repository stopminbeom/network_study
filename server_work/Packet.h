#pragma once

enum class OP_TYPE : short
{
	ADD = 0,
	SUB = 1,
	Mul = 2,
	Div = 3,
};

enum class PACKET_ID : short
{
	CALCU_2_REQ = 21,
	CALCU_3_REQ = 22,

	CALCU_RES = 31,
};

#pragma pack(push, 1)
const int PACKET_HEADER_SIZE = 5;
struct PktHeader
{
	short TotalSize;
	short Id;
	unsigned char Reserve;
};

struct PktCalcu2Req : PktHeader
{
	int n1;
	short op1;
	int n2;	
};

struct PktCalcu3Req : PktHeader
{
	int n1;
	short op1;
	int n2;
	short op2;
	int n3;
};

struct PktCalcuRes : PktHeader
{
	int num;
};
#pragma pack(pop)