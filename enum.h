#ifndef _ENUMS_H
#define _ENUMS_H

/*************Operations***************/
enum Ops 
{
//		0			1			2			3			4			5			6			7
//		8			9			A			B			C			D			E			F
/*0*/	NOP,		LD_BC_nn,	LD_BC_A,	INC_BC,		INC_B,		DEC_B,		LD_B_n,		RLC_A,
		LD_nn_SP,	ADD_HL_BC,	LD_A_BC,	DEC_BC,		INC_C,		DEC_C,		LD_C_n,		RRC_A,
		
/*1*/   STOP,		LD_DE_nn,	LD_DE_A,	INC_DE,		INC_D,		DEC_D,		LD_D_n,		RL_A,
		JR_n,		ADD_HL_DE,	LD_A_DE,	DEC_DE,		INC_E,		DEC_E,		LD_E_n,		RR_A,
  
/*2*/   JR_NZ_n,	LD_HL_nn,	LDI_HL_A,	INC_HL2,	INC_H,		DEC_H,		LD_H_n,		DAA_A,
		JR_Z_n,		ADD_HL_HL,	LDI_A_HL,	DEC_HL2,	INC_L,		DEC_L,		LD_L_n,		CPL_A,
  
/*3*/   JR_NC_n,	LD_SP_nn,	LDD_HL_A,	INC_SP,		INC_HL,		DEC_HL,		LD_HL_n,	SCF,
		JR_C_n,		ADD_HL_SP,	LDD_A_HL,	DEC_SP,		INC_A,		DEC_A,		LD_A_n,		CCF,

/*4*/	LD_B_B,		LD_B_C,		LD_B_D,		LD_B_E,		LD_B_H,		LD_B_L,		LD_B_HL,	LD_B_A,
		LD_C_B,		LD_C_C,		LD_C_D,		LD_C_E,		LD_C_H,		LD_C_L,		LD_C_HL,	LD_C_A,
  
/*5*/   LD_D_B,		LD_D_C,		LD_D_D,		LD_D_E,		LD_D_H,		LD_D_L,		LD_D_HL,	LD_D_A,
		LD_E_B,		LD_E_C,		LD_E_D,		LD_E_E,		LD_E_H,		LD_E_L,		LD_E_HL,	LD_E_A,
  
/*6*/   LD_H_B,		LD_H_C,		LD_H_D,		LD_H_E,		LD_H_H,		LD_H_L,		LD_H_HL,	LD_H_A,
		LD_L_B,		LD_L_C,		LD_L_D,		LD_L_E,		LD_L_H,		LD_L_L,		LD_L_HL,	LD_L_A,

/*7*/	LD_HL_B,	LD_HL_C,	LD_HL_D,	LD_HL_E,	LD_HL_H,	LD_HL_L,	HALT,		LD_HL_A,
		LD_A_B,		LD_A_C,		LD_A_D,		LD_A_E,		LD_A_H,		LD_A_L,		LD_A_HL,	LD_A_A,

/*8*/	ADD_A_B,	ADD_A_C,	ADD_A_D,	ADD_A_E,	ADD_A_H,	ADD_A_L,	ADD_A_HL,	ADD_A_A,
		ADC_A_B,	ADC_A_C,	ADC_A_D,	ADC_A_E,	ADC_A_H,	ADC_A_L,	ADC_A_HL,	ADC_A_A,

/*9*/	SUB_A_B,	SUB_A_C,	SUB_A_D,	SUB_A_E,	SUB_A_H,	SUB_A_L,	SUB_A_HL,	SUB_A_A,
		SBC_A_B,	SBC_A_C,	SBC_A_D,	SBC_A_E,	SBC_A_H,	SBC_A_L,	SBC_A_HL,	SBC_A_A,

/*A*/	AND_A_B,	AND_A_C,	AND_A_D,	AND_A_E,	AND_A_H,	AND_A_L,	AND_A_HL,	AND_A_A,
		XOR_A_B,	XOR_A_C,	XOR_A_D,	XOR_A_E,	XOR_A_H,	XOR_A_L,	XOR_A_HL,	XOR_A_A,

/*B*/	OR_A_B,		OR_A_C,		OR_A_D,		OR_A_E,		OR_A_H,		OR_A_L,		OR_A_HL,	OR_A_A,
		CP_A_B,		CP_A_C,		CP_A_D,		CP_A_E,		CP_A_H,		CP_A_L,		CP_A_HL,	CP_A_A,

/*C*/	RET_NZ,		POP_BC,		JP_NZ_nn,	JP_nn,		CALL_NZ_nn,	PUSH_BC,	ADD_A_n,	RST_00,
		RET_Z,		RET,		JP_Z_nn,	CB,			CALL_Z_nn,	CALL_nn,	ADC_A_n,	RST_08,

/*D*/	RET_NC,		POP_DE,		JP_NC_nn,	xx0,		CALL_NC_nn,	PUSH_DE,	SUB_A_n,	RST_10,
		RET_C,		RETI,		JP_C_nn,	xx1,		CALL_C_nn,	xx2,		SBC_A_n,	RST_18,

/*E*/	LD_FFn_A,	POP_HL,		LD_FFC_A,	xx3,		xx4,		PUSH_HL,	AND_A_n,	RST_20,
		ADD_SP_n,	JP_HL,		LD_nn_A,	xx5,		xx6,		xx7,		XOR_A_n,	RST_28,

/*F*/	LD_A_FFn,	POP_AF,		LD_A_FFC,	DI,			xx8,		PUSH_AF,	OR_A_n,		RST_30,
		LD_HL_SPn,	LD_SP_HL,	LD_A_nn,	EI,			xx9,		xx10,		CP_A_n,		RST_38
};

enum OpsCB
{
//		0			1			2			3			4			5			6			7
//		8			9			A			B			C			D			E			F
/*0*/  	RLC_B,		RLC_C,		RLC_D,		RLC_E,		RLC_H,		RLC_L,		RLC_HL,		RLC_A2,
		RRC_B,		RRC_C,		RRC_D,		RRC_E,		RRC_H,		RRC_L,		RRC_HL,		RRC_A2,

/*1*/	RL_B,		RL_C,		RL_D,		RL_E,		RL_H,		RL_L,		RL_HL,		RL_A2,
		RR_B,		RR_C,		RR_D,		RR_E,		RR_H,		RR_L,		RR_HL,		RR_A2,

/*2*/	SLA_B,		SLA_C,		SLA_D,		SLA_E,		SLA_H,		SLA_L,		SLA_HL,		SLA_A,
		SRA_B,		SRA_C,		SRA_D,		SRA_E,		SRA_H,		SRA_L,		SRA_HL,		SRA_A,

/*3*/	SWAP_B,		SWAP_C,		SWAP_D,		SWAP_E,		SWAP_H,		SWAP_L,		SWAP_HL,	SWAP_A,
		SRL_B,		SRL_C,		SRL_D,		SRL_E,		SRL_H,		SRL_L,		SRL_HL,		SRL_A,

/*4*/	BIT0_B,		BIT0_C,		BIT0_D,		BIT0_E,		BIT0_H,		BIT0_L,		BIT0_HL,	BIT0_A,
		BIT1_B,		BIT1_C,		BIT1_D,		BIT1_E,		BIT1_H,		BIT1_L,		BIT1_HL,	BIT1_A,

/*5*/   BIT2_B,		BIT2_C,		BIT2_D,		BIT2_E,		BIT2_H,		BIT2_L,		BIT2_HL,	BIT2_A,
		BIT3_B,		BIT3_C,		BIT3_D,		BIT3_E,		BIT3_H,		BIT3_L,		BIT3_HL,	BIT3_A,

/*6*/	BIT4_B,		BIT4_C,		BIT4_D,		BIT4_E,		BIT4_H,		BIT4_L,		BIT4_HL,	BIT4_A,
		BIT5_B,		BIT5_C,		BIT5_D,		BIT5_E,		BIT5_H,		BIT5_L,		BIT5_HL,	BIT5_A,
  
/*7*/  	BIT6_B,		BIT6_C,		BIT6_D,		BIT6_E,		BIT6_H,		BIT6_L,		BIT6_HL,	BIT6_A,
		BIT7_B,		BIT7_C,		BIT7_D,		BIT7_E,		BIT7_H,		BIT7_L,		BIT7_HL,	BIT7_A,

/*8*/	RES0_B,		RES0_C,		RES0_D,		RES0_E,		RES0_H,		RES0_L,		RES0_HL,	RES0_A,
		RES1_B,		RES1_C,		RES1_D,		RES1_E,		RES1_H,		RES1_L,		RES1_HL,	RES1_A,

/*9*/	RES2_B,		RES2_C,		RES2_D,		RES2_E,		RES2_H,		RES2_L,		RES2_HL,	RES2_A,
		RES3_B,		RES3_C,		RES3_D,		RES3_E,		RES3_H,		RES3_L,		RES3_HL,	RES3_A,

/*A*/	RES4_B,		RES4_C,		RES4_D,		RES4_E,		RES4_H,		RES4_L,		RES4_HL,	RES4_A,
		RES5_B,		RES5_C,		RES5_D,		RES5_E,		RES5_H,		RES5_L,		RES5_HL,	RES5_A,

/*B*/	RES6_B,		RES6_C,		RES6_D,		RES6_E,		RES6_H,		RES6_L,		RES6_HL,	RES6_A,
		RES7_B,		RES7_C,		RES7_D,		RES7_E,		RES7_H,		RES7_L,		RES7_HL,	RES7_A,  

/*C*/	SET0_B,		SET0_C,		SET0_D,		SET0_E,		SET0_H,		SET0_L,		SET0_HL,	SET0_A,
		SET1_B,		SET1_C,		SET1_D,		SET1_E,		SET1_H,		SET1_L,		SET1_HL,	SET1_A,

/*D*/	SET2_B,		SET2_C,		SET2_D,		SET2_E,		SET2_H,		SET2_L,		SET2_HL,	SET2_A,
		SET3_B,		SET3_C,		SET3_D,		SET3_E,		SET3_H,		SET3_L,		SET3_HL,	SET3_A,

/*E*/	SET4_B,		SET4_C,		SET4_D,		SET4_E,		SET4_H,		SET4_L,		SET4_HL,	SET4_A,
		SET5_B,		SET5_C,		SET5_D,		SET5_E,		SET5_H,		SET5_L,		SET5_HL,	SET5_A,

/*F*/	SET6_B,		SET6_C,		SET6_D,		SET6_E,		SET6_H,		SET6_L,		SET6_HL,	SET6_A,
		SET7_B,		SET7_C,		SET7_D,		SET7_E,		SET7_H,		SET7_L,		SET7_HL,	SET7_A
};
/****************************************/



/************Hram I/O Map*******************/
enum IO_Map
{
	P1 		= 	0x00,
	SB		=	0x01,
	SC		=	0x02,
	DIV		=	0x04,
	TIMA 	= 	0x05,
	TMA		=	0x06,
	TAC		=	0x07,
	IF		=	0x0F,
	NR10	= 	0x10,
	NR11	=	0x11,
	NR12	=	0x12,
	NR13	=	0x13,
	NR14	=	0x14,
	NR21	=	0x16,
	NR22	=	0x17,
	NR23	=	0x18,
	NR24	=	0x19,
	NR30	=	0x1A,
	NR31	=	0x1B,
	NR32	=	0x1C,
	NR33	=	0x1D,
	NR34	= 	0x1E,
	NR41	=	0x20,
	NR42	=	0x21,
	NR43	=	0x22,
	NR44	=	0x23,
	NR50	=	0x24,
	NR51	=	0x25,
	NR52	=	0x26,
	LCDC	=	0x40,
	STAT	= 	0x41,
	SCY		=	0x42,
	SCX		=	0x43,
	LY		=	0x44,
	LYC		=	0x45,
	DMA		=	0x46,
	BGP		=	0x47,
	OBP0	=	0x48,
	OBP1	=	0x49,
	WY		=	0x4A,
	WX		=	0x4B,
	SPD		=	0x4D, //color
	VBK		=	0x4F, //color
	DMASH	=	0x51, //color
	DMASL	=	0x52, //color
	DMADH	=	0x53, //color
	DMADL	=	0x54, //color
	DMAC	=	0x55, //color
	BGPI	=	0x68, //color
	BGPD	=	0x69, //color
	OBPI	=	0x6A, //color
	OBPD	=	0x6B, //color
	WBK		=	0x70, //color
	UD6C	=	0x6C, //color
	UD72	=	0x72, //color
	UD73	=	0x73, //color
	UD74	=	0x74, //color
	UD75	=	0x75, //color
	UD76	=	0x76, //color
	UD77	=	0x77, //color
	IE		= 	0xFF
};
/****************************************/

enum SpriteAttr
{
	PRIO,
	YFLIP,
	XFLIP,
	PAL,
	BANKNO,
	CPAL
};

enum CartridgeType
{
	NONE,
	MBC1,
	MBC2,
	MBC3,
	MBC5,
	HuC1,
	
};

enum Mode
{
	GBC_MODE,
	NON_GBC_MODE
};

#endif