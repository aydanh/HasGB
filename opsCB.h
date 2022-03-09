#define READOP mmu->read
#define READOPB mmu->readB
#define WRITEOP mmu->write
#define WRITEOPB mmu->writeB

case SWAP_A: { A = (A>>4) | ((A&15)<<4);	 F = A?0:0x80;	 } break;
case SWAP_B: { B = (B>>4) | ((B&15)<<4);	 F = B?0:0x80;	 } break;
case SWAP_C: { C = (C>>4) | ((C&15)<<4);	 F = C?0:0x80;	 } break;
case SWAP_D: { D = (D>>4) | ((D&15)<<4);	 F = D?0:0x80;	 } break;
case SWAP_E: { E = (E>>4) | ((E&15)<<4);	 F = E?0:0x80;	 } break;
case SWAP_H: { H = (H>>4) | ((H&15)<<4);	 F = H?0:0x80;	 } break;
case SWAP_L: { L = (L>>4) | ((L&15)<<4);	 F = L?0:0x80;	 } break;
case SWAP_HL: { u8 temp = READOP(HL); 	temp = (temp>>4) | ((temp&15)<<4);		F = temp?0:0x80;	WRITEOP(HL, temp);	 } break;

case RLC_A2: {u8 c = 0x00;	F = 0x00; 	if(A&0x80){ F |= 0x10; c=0x01; }		A = (A<<1) + c ;	if(A==0) { F |= 0x80; }		 } break;
case RLC_B: { u8 c = 0x00;	F = 0x00; 	if(B&0x80){ F |= 0x10; c=0x01; }		B = (B<<1) + c ;	if(B==0) { F |= 0x80; }		 } break;
case RLC_C: { u8 c = 0x00;	F = 0x00; 	if(C&0x80){ F |= 0x10; c=0x01; }		C = (C<<1) + c ;	if(C==0) { F |= 0x80; }		 } break;
case RLC_D: { u8 c = 0x00;	F = 0x00; 	if(D&0x80){ F |= 0x10; c=0x01; }		D = (D<<1) + c ;	if(D==0) { F |= 0x80; }		 } break;
case RLC_E: { u8 c = 0x00;	F = 0x00; 	if(E&0x80){ F |= 0x10; c=0x01; }		E = (E<<1) + c ;	if(E==0) { F |= 0x80; }		 } break;
case RLC_H: { u8 c = 0x00;	F = 0x00; 	if(H&0x80){ F |= 0x10; c=0x01; }		H = (H<<1) + c ;	if(H==0) { F |= 0x80; }		 } break;
case RLC_L: { u8 c = 0x00;	F = 0x00; 	if(L&0x80){ F |= 0x10; c=0x01; }		L = (L<<1) + c ;	if(L==0) { F |= 0x80; }		 } break;
case RLC_HL: {u8 t = READOP(HL); 		u8 c = 0x00;		F = 0x00; 		if(t&0x80){ F |= 0x10; c=0x01; }		t = (t<<1) + c;		if(t==0) { F |= 0x80; }		WRITEOP(HL, t);	 } break;

case RL_A2: {u8 c = (F&0x10)?1:0;		F = 0x00;		if(A&0x80){ F |= 0x10; }		A = (A<<1) + c;		if(A==0) { F |= 0x80; }		 } break;
case RL_B: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(B&0x80){ F |= 0x10; }		B = (B<<1) + c;		if(B==0) { F |= 0x80; }		 } break;
case RL_C: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(C&0x80){ F |= 0x10; }		C = (C<<1) + c;		if(C==0) { F |= 0x80; }		 } break;
case RL_D: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(D&0x80){ F |= 0x10; }		D = (D<<1) + c;		if(D==0) { F |= 0x80; }		 } break;
case RL_E: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(E&0x80){ F |= 0x10; }		E = (E<<1) + c;		if(E==0) { F |= 0x80; }		 } break;
case RL_H: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(H&0x80){ F |= 0x10; }		H = (H<<1) + c;		if(H==0) { F |= 0x80; }		 } break;
case RL_L: { u8 c = (F&0x10)?1:0;		F = 0x00;		if(L&0x80){ F |= 0x10; }		L = (L<<1) + c;		if(L==0) { F |= 0x80; }		 } break;
case RL_HL: {u8 t = READOP(HL);	 u8 c = (F&0x10)?1:0;		F = 0x00;		if(t&0x80){ F |= 0x10; }		t = (t<<1) + c;		if(t==0) { F |= 0x80; }		WRITEOP(HL, t);	 } break;

case RRC_A2: {u8 c = 0x00;	F = 0x00; 	if(A&0x01){ F |= 0x10; c=0x80; }		A = (A>>1) + c ;	if(A==0) { F |= 0x80; }		 } break;
case RRC_B: { u8 c = 0x00;	F = 0x00; 	if(B&0x01){ F |= 0x10; c=0x80; }		B = (B>>1) + c ;	if(B==0) { F |= 0x80; }		 } break;
case RRC_C: { u8 c = 0x00;	F = 0x00; 	if(C&0x01){ F |= 0x10; c=0x80; }		C = (C>>1) + c ;	if(C==0) { F |= 0x80; }		 } break;
case RRC_D: { u8 c = 0x00;	F = 0x00; 	if(D&0x01){ F |= 0x10; c=0x80; }		D = (D>>1) + c ;	if(D==0) { F |= 0x80; }		 } break;
case RRC_E: { u8 c = 0x00;	F = 0x00; 	if(E&0x01){ F |= 0x10; c=0x80; }		E = (E>>1) + c ;	if(E==0) { F |= 0x80; }		 } break;
case RRC_H: { u8 c = 0x00;	F = 0x00; 	if(H&0x01){ F |= 0x10; c=0x80; }		H = (H>>1) + c ;	if(H==0) { F |= 0x80; }		 } break;
case RRC_L: { u8 c = 0x00;	F = 0x00; 	if(L&0x01){ F |= 0x10; c=0x80; }		L = (L>>1) + c ;	if(L==0) { F |= 0x80; }		 } break;
case RRC_HL: {u8 t = READOP(HL);		u8 c = 0x00;	F = 0x00; 	if(t&0x01){ F |= 0x10; c=0x80; }		t = (t>>1) + c ;	if(t==0) { F |= 0x80; }		WRITEOP(HL, t);	 } break;

case RR_A2: {u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(A&0x01){ F |= 0x10; }		A = (A>>1) + c;		if(A==0) { F |= 0x80; }		 } break;
case RR_B: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(B&0x01){ F |= 0x10; }		B = (B>>1) + c;		if(B==0) { F |= 0x80; }		 } break;
case RR_C: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(C&0x01){ F |= 0x10; }		C = (C>>1) + c;		if(C==0) { F |= 0x80; }		 } break;
case RR_D: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(D&0x01){ F |= 0x10; }		D = (D>>1) + c;		if(D==0) { F |= 0x80; }		 } break;
case RR_E: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(E&0x01){ F |= 0x10; }		E = (E>>1) + c;		if(E==0) { F |= 0x80; }		 } break;
case RR_H: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(H&0x01){ F |= 0x10; }		H = (H>>1) + c;		if(H==0) { F |= 0x80; }		 } break;
case RR_L: { u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(L&0x01){ F |= 0x10; }		L = (L>>1) + c;		if(L==0) { F |= 0x80; }		 } break;
case RR_HL: {u8 t = READOP(HL);	u8 c = ((F&0x10)?1:0)<<7;		F = 0x00;		if(t&0x01){ F |= 0x10; }		t = (t>>1) + c;		if(t==0) { F |= 0x80; }		WRITEOP(HL, t);	 } break;

case SLA_A: { F = 0x00;	F |= (A&0x80)?0x10:0x00;	A <<= 1;	if(A == 0){ F |= 0x80; }	 } break;
case SLA_B: { F = 0x00;	F |= (B&0x80)?0x10:0x00;	B <<= 1;	if(B == 0){ F |= 0x80; }	 } break;
case SLA_C: { F = 0x00;	F |= (C&0x80)?0x10:0x00;	C <<= 1;	if(C == 0){ F |= 0x80; }	 } break;
case SLA_D: { F = 0x00;	F |= (D&0x80)?0x10:0x00;	D <<= 1;	if(D == 0){ F |= 0x80; }	 } break;
case SLA_E: { F = 0x00;	F |= (E&0x80)?0x10:0x00;	E <<= 1;	if(E == 0){ F |= 0x80; }	 } break;
case SLA_H: { F = 0x00;	F |= (H&0x80)?0x10:0x00;	H <<= 1;	if(H == 0){ F |= 0x80; }	 } break;
case SLA_L: { F = 0x00;	F |= (L&0x80)?0x10:0x00;	L <<= 1;	if(L == 0){ F |= 0x80; }	 } break;
case SLA_HL: { u8 t = READOP(HL);	 F = 0x00;	F |= (t&0x80)?0x10:0x00;		t <<= 1;	if(t == 0){ F |= 0x80; }		 WRITEOP(HL, t);		 } break;

case SRA_A: { F = 0x00; F |= (A&0x01)?0x10:0x00;	u8 msb = A&0x80;		A = (A>>1) + msb;		if(A == 0){ F |= 0x80; }	 } break;
case SRA_B: { F = 0x00; F |= (B&0x01)?0x10:0x00;	u8 msb = B&0x80;		B = (B>>1) + msb;		if(B == 0){ F |= 0x80; }	 } break;
case SRA_C: { F = 0x00; F |= (C&0x01)?0x10:0x00;	u8 msb = C&0x80;		C = (C>>1) + msb;		if(C == 0){ F |= 0x80; }	 } break;
case SRA_D: { F = 0x00; F |= (D&0x01)?0x10:0x00;	u8 msb = D&0x80;		D = (D>>1) + msb;		if(D == 0){ F |= 0x80; }	 } break;
case SRA_E: { F = 0x00; F |= (E&0x01)?0x10:0x00;	u8 msb = E&0x80;		E = (E>>1) + msb;		if(E == 0){ F |= 0x80; }	 } break;
case SRA_H: { F = 0x00; F |= (H&0x01)?0x10:0x00;	u8 msb = H&0x80;		H = (H>>1) + msb;		if(H == 0){ F |= 0x80; }	 } break;
case SRA_L: { F = 0x00; F |= (L&0x01)?0x10:0x00;	u8 msb = L&0x80;		L = (L>>1) + msb;		if(L == 0){ F |= 0x80; }	 } break;
case SRA_HL: { u8 t = READOP(HL);		F = 0x00; F |= (t&0x01)?0x10:0x00;		u8 msb = t&0x80;		t = (t>>1) + msb;		if(t == 0){ F |= 0x80; }	WRITEOP(HL, t);	 } break;

case SRL_A: { F = 0x00; F |= (A&0x01)?0x10:0x00;	A >>= 1;		if(A == 0){ F |= 0x80; }	 } break;
case SRL_B: { F = 0x00; F |= (B&0x01)?0x10:0x00;	B >>= 1;		if(B == 0){ F |= 0x80; }	 } break;
case SRL_C: { F = 0x00; F |= (C&0x01)?0x10:0x00;	C >>= 1;		if(C == 0){ F |= 0x80; }	 } break;
case SRL_D: { F = 0x00; F |= (D&0x01)?0x10:0x00;	D >>= 1;		if(D == 0){ F |= 0x80; }	 } break;
case SRL_E: { F = 0x00; F |= (E&0x01)?0x10:0x00;	E >>= 1;		if(E == 0){ F |= 0x80; }	 } break;
case SRL_H: { F = 0x00; F |= (H&0x01)?0x10:0x00;	H >>= 1;		if(H == 0){ F |= 0x80; }	 } break;
case SRL_L: { F = 0x00; F |= (L&0x01)?0x10:0x00;	L >>= 1;		if(L == 0){ F |= 0x80; }	 } break;
case SRL_HL: { u8 t = READOP(HL);	F = 0x00; F |= (t&0x01)?0x10:0x00;		t >>= 1;		if(t == 0){ F |= 0x80; }	WRITEOP(HL, t);	 } break;

case BIT0_A: { F &= 0x10; F |= 0x20;	if((A&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_B: { F &= 0x10; F |= 0x20;	if((B&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_C: { F &= 0x10; F |= 0x20;	if((C&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_D: { F &= 0x10; F |= 0x20;	if((D&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_E: { F &= 0x10; F |= 0x20;	if((E&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_H: { F &= 0x10; F |= 0x20;	if((H&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_L: { F &= 0x10; F |= 0x20;	if((L&0x01) == 0){ F |= 0x80; }	 } break;
case BIT0_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x01) == 0){ F |= 0x80; }	 } break;

case BIT1_A: { F &= 0x10; F |= 0x20;	if((A&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_B: { F &= 0x10; F |= 0x20;	if((B&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_C: { F &= 0x10; F |= 0x20;	if((C&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_D: { F &= 0x10; F |= 0x20;	if((D&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_E: { F &= 0x10; F |= 0x20;	if((E&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_H: { F &= 0x10; F |= 0x20;	if((H&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_L: { F &= 0x10; F |= 0x20;	if((L&0x02) == 0){ F |= 0x80; }	 } break;
case BIT1_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x02) == 0){ F |= 0x80; }	 } break;

case BIT2_A: { F &= 0x10; F |= 0x20;	if((A&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_B: { F &= 0x10; F |= 0x20;	if((B&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_C: { F &= 0x10; F |= 0x20;	if((C&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_D: { F &= 0x10; F |= 0x20;	if((D&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_E: { F &= 0x10; F |= 0x20;	if((E&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_H: { F &= 0x10; F |= 0x20;	if((H&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_L: { F &= 0x10; F |= 0x20;	if((L&0x04) == 0){ F |= 0x80; }	 } break;
case BIT2_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x04) == 0){ F |= 0x80; }	 } break;

case BIT3_A: { F &= 0x10; F |= 0x20;	if((A&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_B: { F &= 0x10; F |= 0x20;	if((B&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_C: { F &= 0x10; F |= 0x20;	if((C&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_D: { F &= 0x10; F |= 0x20;	if((D&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_E: { F &= 0x10; F |= 0x20;	if((E&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_H: { F &= 0x10; F |= 0x20;	if((H&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_L: { F &= 0x10; F |= 0x20;	if((L&0x08) == 0){ F |= 0x80; }	 } break;
case BIT3_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x08) == 0){ F |= 0x80; }	 } break;

case BIT4_A: { F &= 0x10; F |= 0x20;	if((A&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_B: { F &= 0x10; F |= 0x20;	if((B&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_C: { F &= 0x10; F |= 0x20;	if((C&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_D: { F &= 0x10; F |= 0x20;	if((D&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_E: { F &= 0x10; F |= 0x20;	if((E&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_H: { F &= 0x10; F |= 0x20;	if((H&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_L: { F &= 0x10; F |= 0x20;	if((L&0x10) == 0){ F |= 0x80; }	 } break;
case BIT4_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x10) == 0){ F |= 0x80; }	 } break;

case BIT5_A: { F &= 0x10; F |= 0x20;	if((A&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_B: { F &= 0x10; F |= 0x20;	if((B&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_C: { F &= 0x10; F |= 0x20;	if((C&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_D: { F &= 0x10; F |= 0x20;	if((D&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_E: { F &= 0x10; F |= 0x20;	if((E&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_H: { F &= 0x10; F |= 0x20;	if((H&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_L: { F &= 0x10; F |= 0x20;	if((L&0x20) == 0){ F |= 0x80; }	 } break;
case BIT5_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x20) == 0){ F |= 0x80; }	 } break;

case BIT6_A: { F &= 0x10; F |= 0x20;	if((A&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_B: { F &= 0x10; F |= 0x20;	if((B&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_C: { F &= 0x10; F |= 0x20;	if((C&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_D: { F &= 0x10; F |= 0x20;	if((D&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_E: { F &= 0x10; F |= 0x20;	if((E&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_H: { F &= 0x10; F |= 0x20;	if((H&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_L: { F &= 0x10; F |= 0x20;	if((L&0x40) == 0){ F |= 0x80; }	 } break;
case BIT6_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x40) == 0){ F |= 0x80; }	 } break;

case BIT7_A: { F &= 0x10; F |= 0x20;	if((A&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_B: { F &= 0x10; F |= 0x20;	if((B&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_C: { F &= 0x10; F |= 0x20;	if((C&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_D: { F &= 0x10; F |= 0x20;	if((D&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_E: { F &= 0x10; F |= 0x20;	if((E&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_H: { F &= 0x10; F |= 0x20;	if((H&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_L: { F &= 0x10; F |= 0x20;	if((L&0x80) == 0){ F |= 0x80; }	 } break;
case BIT7_HL: {F &= 0x10; F |= 0x20;	u8 t = READOP(HL); 	if((t&0x80) == 0){ F |= 0x80; }	 } break;

case SET0_A: { A |= 0x01;	 } break;
case SET0_B: { B |= 0x01;	 } break;
case SET0_C: { C |= 0x01;	 } break;
case SET0_D: { D |= 0x01;	 } break;
case SET0_E: { E |= 0x01;	 } break;
case SET0_H: { H |= 0x01;	 } break;
case SET0_L: { L |= 0x01;	 } break;
case SET0_HL: { u8 t = READOP(HL);	t |= 0x01;		WRITEOP(HL, t);		 } break;

case SET1_A: { A |= 0x02;	 } break;
case SET1_B: { B |= 0x02;	 } break;
case SET1_C: { C |= 0x02;	 } break;
case SET1_D: { D |= 0x02;	 } break;
case SET1_E: { E |= 0x02;	 } break;
case SET1_H: { H |= 0x02;	 } break;
case SET1_L: { L |= 0x02;	 } break;
case SET1_HL: { u8 t = READOP(HL);	t |= 0x02;		WRITEOP(HL, t);		 } break;

case SET2_A: { A |= 0x04;	 } break;
case SET2_B: { B |= 0x04;	 } break;
case SET2_C: { C |= 0x04;	 } break;
case SET2_D: { D |= 0x04;	 } break;
case SET2_E: { E |= 0x04;	 } break;
case SET2_H: { H |= 0x04;	 } break;
case SET2_L: { L |= 0x04;	 } break;
case SET2_HL: { u8 t = READOP(HL);	t |= 0x04;		WRITEOP(HL, t);		 } break;

case SET3_A: { A |= 0x08;	 } break;
case SET3_B: { B |= 0x08;	 } break;
case SET3_C: { C |= 0x08;	 } break;
case SET3_D: { D |= 0x08;	 } break;
case SET3_E: { E |= 0x08;	 } break;
case SET3_H: { H |= 0x08;	 } break;
case SET3_L: { L |= 0x08;	 } break;
case SET3_HL: { u8 t = READOP(HL);	t |= 0x08;		WRITEOP(HL, t);		 } break;

case SET4_A: { A |= 0x10;	 } break;
case SET4_B: { B |= 0x10;	 } break;
case SET4_C: { C |= 0x10;	 } break;
case SET4_D: { D |= 0x10;	 } break;
case SET4_E: { E |= 0x10;	 } break;
case SET4_H: { H |= 0x10;	 } break;
case SET4_L: { L |= 0x10;	 } break;
case SET4_HL: { u8 t = READOP(HL);	t |= 0x10;		WRITEOP(HL, t);		 } break;

case SET5_A: { A |= 0x20;	 } break;
case SET5_B: { B |= 0x20;	 } break;
case SET5_C: { C |= 0x20;	 } break;
case SET5_D: { D |= 0x20;	 } break;
case SET5_E: { E |= 0x20;	 } break;
case SET5_H: { H |= 0x20;	 } break;
case SET5_L: { L |= 0x20;	 } break;
case SET5_HL: { u8 t = READOP(HL);	t |= 0x20;		WRITEOP(HL, t);		 } break;

case SET6_A: { A |= 0x40;	 } break;
case SET6_B: { B |= 0x40;	 } break;
case SET6_C: { C |= 0x40;	 } break;
case SET6_D: { D |= 0x40;	 } break;
case SET6_E: { E |= 0x40;	 } break;
case SET6_H: { H |= 0x40;	 } break;
case SET6_L: { L |= 0x40;	 } break;
case SET6_HL: { u8 t = READOP(HL);	t |= 0x40;		WRITEOP(HL, t);		 } break;

case SET7_A: { A |= 0x80;	 } break;
case SET7_B: { B |= 0x80;	 } break;
case SET7_C: { C |= 0x80;	 } break;
case SET7_D: { D |= 0x80;	 } break;
case SET7_E: { E |= 0x80;	 } break;
case SET7_H: { H |= 0x80;	 } break;
case SET7_L: { L |= 0x80;	 } break;
case SET7_HL: { u8 t = READOP(HL);	t |= 0x80;		WRITEOP(HL, t);		 } break;

case RES0_A: { A &= 0xFE;	 } break;
case RES0_B: { B &= 0xFE;	 } break;
case RES0_C: { C &= 0xFE;	 } break;
case RES0_D: { D &= 0xFE;	 } break;
case RES0_E: { E &= 0xFE;	 } break;
case RES0_H: { H &= 0xFE;	 } break;
case RES0_L: { L &= 0xFE;	 } break;
case RES0_HL: { u8 t = READOP(HL);	t &= 0xFE;		WRITEOP(HL, t);		 } break;

case RES1_A: { A &= 0xFD;	 } break;
case RES1_B: { B &= 0xFD;	 } break;
case RES1_C: { C &= 0xFD;	 } break;
case RES1_D: { D &= 0xFD;	 } break;
case RES1_E: { E &= 0xFD;	 } break;
case RES1_H: { H &= 0xFD;	 } break;
case RES1_L: { L &= 0xFD;	 } break;
case RES1_HL: { u8 t = READOP(HL);	t &= 0xFD;		WRITEOP(HL, t);		 } break;

case RES2_A: { A &= 0xFB;	 } break;
case RES2_B: { B &= 0xFB;	 } break;
case RES2_C: { C &= 0xFB;	 } break;
case RES2_D: { D &= 0xFB;	 } break;
case RES2_E: { E &= 0xFB;	 } break;
case RES2_H: { H &= 0xFB;	 } break;
case RES2_L: { L &= 0xFB;	 } break;
case RES2_HL: { u8 t = READOP(HL);	t &= 0xFB;		WRITEOP(HL, t);		 } break;

case RES3_A: { A &= 0xF7;	 } break;
case RES3_B: { B &= 0xF7;	 } break;
case RES3_C: { C &= 0xF7;	 } break;
case RES3_D: { D &= 0xF7;	 } break;
case RES3_E: { E &= 0xF7;	 } break;
case RES3_H: { H &= 0xF7;	 } break;
case RES3_L: { L &= 0xF7;	 } break;
case RES3_HL: { u8 t = READOP(HL);	t &= 0xF7;		WRITEOP(HL, t);		 } break;

case RES4_A: { A &= 0xEF;	 } break;
case RES4_B: { B &= 0xEF;	 } break;
case RES4_C: { C &= 0xEF;	 } break;
case RES4_D: { D &= 0xEF;	 } break;
case RES4_E: { E &= 0xEF;	 } break;
case RES4_H: { H &= 0xEF;	 } break;
case RES4_L: { L &= 0xEF;	 } break;
case RES4_HL: { u8 t = READOP(HL);	t &= 0xEF;		WRITEOP(HL, t);		 } break;

case RES5_A: { A &= 0xDF;	 } break;
case RES5_B: { B &= 0xDF;	 } break;
case RES5_C: { C &= 0xDF;	 } break;
case RES5_D: { D &= 0xDF;	 } break;
case RES5_E: { E &= 0xDF;	 } break;
case RES5_H: { H &= 0xDF;	 } break;
case RES5_L: { L &= 0xDF;	 } break;
case RES5_HL: { u8 t = READOP(HL);	t &= 0xDF;		WRITEOP(HL, t);		 } break;

case RES6_A: { A &= 0xBF;	 } break;
case RES6_B: { B &= 0xBF;	 } break;
case RES6_C: { C &= 0xBF;	 } break;
case RES6_D: { D &= 0xBF;	 } break;
case RES6_E: { E &= 0xBF;	 } break;
case RES6_H: { H &= 0xBF;	 } break;
case RES6_L: { L &= 0xBF;	 } break;
case RES6_HL: { u8 t = READOP(HL);	t &= 0xBF;		WRITEOP(HL, t);		 } break;

case RES7_A: { A &= 0x7F;	 } break;
case RES7_B: { B &= 0x7F;	 } break;
case RES7_C: { C &= 0x7F;	 } break;
case RES7_D: { D &= 0x7F;	 } break;
case RES7_E: { E &= 0x7F;	 } break;
case RES7_H: { H &= 0x7F;	 } break;
case RES7_L: { L &= 0x7F;	 } break;
case RES7_HL: { u8 t = READOP(HL);	t &= 0x7F;		WRITEOP(HL, t);		 } break;