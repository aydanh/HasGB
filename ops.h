#define READOP mmu->read
#define READOPB mmu->readB
#define WRITEOP mmu->write
#define WRITEOPB mmu->writeB

case LD_A_n: { A = READOP(PC++);	} break;
case LD_B_n: { B = READOP(PC++);	} break;
case LD_C_n: { C = READOP(PC++);	} break;
case LD_D_n: { D = READOP(PC++);	} break;
case LD_E_n: { E = READOP(PC++);	} break;
case LD_H_n: { H = READOP(PC++);	} break;
case LD_L_n: { L = READOP(PC++);	} break;

case LD_A_A: {  } break;
case LD_A_B: { A = B; } break;
case LD_A_C: { A = C; } break;
case LD_A_D: { A = D; } break;
case LD_A_E: { A = E; } break;
case LD_A_H: { A = H; } break;
case LD_A_L: { A = L; } break;

case LD_B_A: { B = A; } break;
case LD_B_B: {  } break;
case LD_B_C: { B = C; } break;
case LD_B_D: { B = D; } break;
case LD_B_E: { B = E; } break;
case LD_B_H: { B = H; } break;
case LD_B_L: { B = L; } break;

case LD_C_A: { C = A; } break;
case LD_C_B: { C = B; } break;
case LD_C_C: {  } break;
case LD_C_D: { C = D; } break;
case LD_C_E: { C = E; } break;
case LD_C_H: { C = H; } break;
case LD_C_L: { C = L; } break;

case LD_D_A: { D = A; } break;
case LD_D_B: { D = B; } break;
case LD_D_C: { D = C; } break;
case LD_D_D: {  } break;
case LD_D_E: { D = E; } break;
case LD_D_H: { D = H; } break;
case LD_D_L: { D = L; } break;

case LD_E_A: { E = A; } break;
case LD_E_B: { E = B; } break;
case LD_E_C: { E = C; } break;
case LD_E_D: { E = D; } break;
case LD_E_E: {  } break;
case LD_E_H: { E = H; } break;
case LD_E_L: { E = L; } break;

case LD_H_A: { H = A; } break;
case LD_H_B: { H = B; } break;
case LD_H_C: { H = C; } break;
case LD_H_D: { H = D; } break;
case LD_H_E: { H = E; } break;
case LD_H_H: {  } break;
case LD_H_L: { H = L; } break;

case LD_L_A: { L = A; } break;
case LD_L_B: { L = B; } break;
case LD_L_C: { L = C; } break;
case LD_L_D: { L = D; } break;
case LD_L_E: { L = E; } break;
case LD_L_H: { L = H; } break;
case LD_L_L: {  } break;

case LD_A_HL: { A = READOP(HL);	 } break;
case LD_B_HL: { B = READOP(HL);	 } break;
case LD_C_HL: { C = READOP(HL);	 } break;
case LD_D_HL: { D = READOP(HL);	 } break;
case LD_E_HL: { E = READOP(HL);	 } break;
case LD_H_HL: { H = READOP(HL);	 } break;
case LD_L_HL: { L = READOP(HL);	 } break;

case LD_HL_A: { WRITEOP(HL, A);	 } break;
case LD_HL_B: { WRITEOP(HL, B);	 } break;
case LD_HL_C: { WRITEOP(HL, C);	 } break;
case LD_HL_D: { WRITEOP(HL, D);	 } break;
case LD_HL_E: { WRITEOP(HL, E);	 } break;
case LD_HL_H: { WRITEOP(HL, H);	 } break;
case LD_HL_L: { WRITEOP(HL, L);	 } break;
case LD_HL_n: { WRITEOP(HL, READOP(PC++));	 } break;

case LDD_HL_A: { WRITEOP(HL--, A);	} break;
case LDI_HL_A: { WRITEOP(HL++, A);	} break;
case LDD_A_HL: { A = READOP(HL--);	} break;
case LDI_A_HL: { A = READOP(HL++);	} break;

case LD_BC_A: { WRITEOP(BC, A);	 } break;
case LD_DE_A: { WRITEOP(DE, A);	 } break;
case LD_nn_A: { WRITEOP(READOPB((PC)), A);	PC += 2;	 } break;

case LD_A_BC: { A = READOP(BC);	 } break;
case LD_A_DE: { A = READOP(DE);	 } break;
case LD_A_nn: { A = READOP(READOPB((PC))); 	PC += 2;	 } break;

case LD_A_FFC: { A = READOP(0xFF00 + C);	 } break;
case LD_FFC_A: { WRITEOP(0xFF00 + C, A);	 } break;
case LD_A_FFn: { A = READOP(0xFF00 + READOP(PC++));	 } break;
case LD_FFn_A: { WRITEOP(0xFF00 +READOP(PC++), A);	 } break;

case LD_BC_nn: { BC = READOPB(PC);	 	 PC += 2;	 } break;
case LD_DE_nn: { DE = READOPB(PC);	 	 PC += 2; 	 } break;
case LD_HL_nn: { HL = READOPB(PC);	 	 PC += 2;	 } break;
case LD_SP_nn: { SP = READOPB(PC);		 PC += 2;	 } break;

case LD_SP_HL: { SP = HL; } break;
case LD_HL_SPn: { 
	u8 n = READOP(PC++); 
	
	F = 0x00;
	
	
	if(((SP&15) + (n&15)) > 15) { F |= 0x20; }
	if(((SP&255) + n) > 255) { F |= 0x10; }
	
	if(n>127) { HL = SP + (signed char)n; }
	else { HL = SP + n;	}
} break;

case LD_nn_SP: { u16 m = READOPB(PC);	WRITEOPB(m, SP); 	PC += 2;	 } break;

case PUSH_AF: {  SP -= 2;	WRITEOPB(SP, AF);  } break;
case PUSH_BC: {  SP -= 2;	WRITEOPB(SP, BC);  } break;
case PUSH_DE: {  SP -= 2;	WRITEOPB(SP, DE);  } break;
case PUSH_HL: {  SP -= 2;	WRITEOPB(SP, HL);  } break;

case POP_AF: { F = (READOP(SP++)&0xF0); A = READOP(SP++); } break;
case POP_BC: { BC = READOPB(SP);	 SP += 2; } break;
case POP_DE: { DE = READOPB(SP);	 SP += 2; } break;
case POP_HL: { HL = READOPB(SP);	 SP += 2; } break;

case ADD_A_A: { F = 0x00; 	if((((A)&15) + ((A)&15)) > 15) {F |= 0x20;} 	if((A + A) > 255) {F |= 0x10;}		A += A;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_B: { F = 0x00;	if((((A)&15) + ((B)&15)) > 15) {F |= 0x20;}		if((A + B) > 255) {F |= 0x10;}		A += B;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_C: { F = 0x00;	if((((A&15)) + ((C)&15)) > 15) {F |= 0x20;}		if((A + C) > 255) {F |= 0x10;}		A += C;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_D: { F = 0x00; 	if((((A)&15) + ((D)&15)) > 15) {F |= 0x20;}		if((A + D) > 255) {F |= 0x10;}		A += D;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_E: { F = 0x00; 	if((((A)&15) + ((E)&15)) > 15) {F |= 0x20;} 	if((A + E) > 255) {F |= 0x10;}		A += E;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_H: { F = 0x00; 	if((((A)&15) + ((H)&15)) > 15) {F |= 0x20;}		if((A + H) > 255) {F |= 0x10;}		A += H;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_L: { F = 0x00;	if((((A)&15) + ((L)&15)) > 15) {F |= 0x20;}		if((A + L) > 255) {F |= 0x10;}		A += L;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_HL: {F = 0x00;	u8 n = READOP(HL);		if(((n&15) + (A&15)) > 15) {F |= 0x20;}		if((A + n) > 255) {F |= 0x10;}		A += n;		if(A == 0x00){F |= 0x80;}		 } break;
case ADD_A_n: { F = 0x00; 	u8 n = READOP(PC++);		if(((n&15) + (A&15)) > 15) {F |= 0x20;}		if((A + n) > 255) {F |= 0x10;}		A += n;		if(A == 0x00){F |= 0x80;}		 } break;

case ADC_A_A: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (A&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + A + c) > 255) {F |= 0x10;}		A += A + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_B: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (B&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + B + c) > 255) {F |= 0x10;}		A += B + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_C: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (C&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + C + c) > 255) {F |= 0x10;}		A += C + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_D: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (D&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + D + c) > 255) {F |= 0x10;}		A += D + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_E: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (E&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + E + c) > 255) {F |= 0x10;}		A += E + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_H: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (H&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + H + c) > 255) {F |= 0x10;}		A += H + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_L: { u8 c = (F&0x10)?1:0; 	F = 0x00; 		u8 l = (A&15) + (L&15) + c; 	if(l > 15) {F |= 0x20;} 		if((A + L + c) > 255) {F |= 0x10;}		A += L + c;	if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_HL: {u8 c = (F&0x10)?1:0; 	F = 0x00;		u8 n = READOP(HL);		u8 l = (n&15) + (A&15) + c;	if(l > 15) {F |= 0x20;}		if((A + n + c) > 255) {F |= 0x10;}		A += n + c;		if(A == 0x00){ F |= 0x80; }		 } break;
case ADC_A_n: { u8 c = (F&0x10)?1:0;	F = 0x00;		u8 n = READOP(PC++);		u8 l = (n&15) + (A&15) + c;	if(l > 15) {F |= 0x20;}		if((A + n + c) > 255) {F |= 0x10;}		A += n + c;		if(A == 0x00){ F |= 0x80; }		 } break;

case SUB_A_A: { A = 0x00; 	F = 0xC0;		  } break;
case SUB_A_B: { F = 0x00;	F |= 0x40;		if((A&15) < (B&15)){ F |= 0x20; }	if(A < B){ F |= 0x10; }	A -= B;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_C: { F = 0x00;	F |= 0x40;		if((A&15) < (C&15)){ F |= 0x20; }	if(A < C){ F |= 0x10; }	A -= C;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_D: { F = 0x00;	F |= 0x40;		if((A&15) < (D&15)){ F |= 0x20; }	if(A < D){ F |= 0x10; }	A -= D;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_E: { F = 0x00;	F |= 0x40;		if((A&15) < (E&15)){ F |= 0x20; }	if(A < E){ F |= 0x10; }	A -= E;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_H: { F = 0x00;	F |= 0x40;		if((A&15) < (H&15)){ F |= 0x20; }	if(A < H){ F |= 0x10; }	A -= H;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_L: { F = 0x00;	F |= 0x40;		if((A&15) < (L&15)){ F |= 0x20; }	if(A < L){ F |= 0x10; }	A -= L;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_HL: {F = 0x00;	F |= 0x40;		u8 n = READOP(HL);	if((A&15) < (n&15)){ F |= 0x20; }	if(A < n){ F |= 0x10; }		A -= n;	if(A == 0){ F |= 0x80; }	 } break;
case SUB_A_n: { F = 0x00;	F |= 0x40;		u8 n = READOP(PC++);	if((A&15) < (n&15)){ F |= 0x20; }	if(A < n){ F |= 0x10; }		A -= n;	if(A == 0){ F |= 0x80; }	 } break;

case SBC_A_A: { u8 c = (F&0x10)?1:0;		if(c){A = 0xFF; F = 0x70; } else{A = 0x00; F = 0xC0;}	  } break;
case SBC_A_B: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (B&15) + c){ F |= 0x20; }	if(A < B + c){ F |= 0x10; }	A -= (B + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_C: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (C&15) + c){ F |= 0x20; }	if(A < C + c){ F |= 0x10; }	A -= (C + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_D: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (D&15) + c){ F |= 0x20; }	if(A < D + c){ F |= 0x10; }	A -= (D + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_E: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (E&15) + c){ F |= 0x20; }	if(A < E + c){ F |= 0x10; }	A -= (E + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_H: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (H&15) + c){ F |= 0x20; }	if(A < H + c){ F |= 0x10; }	A -= (H + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_L: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		if((A&15) < (L&15) + c){ F |= 0x20; }	if(A < L + c){ F |= 0x10; }	A -= (L + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_HL: {u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		u8 n = READOP(HL);	if((A&15) < (n&15) + c){ F |= 0x20; }	if(A < n + c){ F |= 0x10; }	A -= (n + c);	if(A == 0){ F |= 0x80; }	 } break;
case SBC_A_n: { u8 c = (F&0x10)?1:0;		F = 0x00;	F |= 0x40;		u8 n = READOP(PC++);	if((A&15) < (n&15) + c){ F |= 0x20; }	if(A < n + c){ F |= 0x10; }	A -= (n + c);	if(A == 0){ F |= 0x80; }	 } break;

case AND_A_A: { F = 0x20;		if(A == 0){F |= 0x80;}	} break;
case AND_A_B: { F = 0x20;		A &= B;	if(A == 0){F |= 0x80;}		} break;
case AND_A_C: { F = 0x20;		A &= C;	if(A == 0){F |= 0x80;}		} break;
case AND_A_D: { F = 0x20;		A &= D;	if(A == 0){F |= 0x80;}		} break;
case AND_A_E: { F = 0x20;		A &= E;	if(A == 0){F |= 0x80;}		} break;
case AND_A_H: { F = 0x20;		A &= H;	if(A == 0){F |= 0x80;}		} break;
case AND_A_L: { F = 0x20;		A &= L;	if(A == 0){F |= 0x80;}		} break;
case AND_A_HL: {F = 0x20;		A &= READOP(HL);		if(A == 0){F |= 0x80;}		} break;
case AND_A_n: { F = 0x20;		A &= READOP(PC++);	if(A == 0){F |= 0x80;}		} break;

case OR_A_A: { F = 0x00; 	 if(A == 0){F |= 0x80;}} break;
case OR_A_B: { F = 0x00;	A |= B;	if(A == 0){F |= 0x80;}	} break;
case OR_A_C: { F = 0x00;	A |= C;	if(A == 0){F |= 0x80;}	} break;
case OR_A_D: { F = 0x00;	A |= D;	if(A == 0){F |= 0x80;}	} break;
case OR_A_E: { F = 0x00;	A |= E;	if(A == 0){F |= 0x80;}	} break; 
case OR_A_H: { F = 0x00;	A |= H;	if(A == 0){F |= 0x80;}	} break;
case OR_A_L: { F = 0x00;	A |= L;	if(A == 0){F |= 0x80;}	} break;
case OR_A_HL: {F = 0x00;	A |= READOP(HL);		if(A == 0){F |= 0x80;}	} break;
case OR_A_n: { F = 0x00;	A |= READOP(PC++);	if(A == 0){F |= 0x80;}	} break;

case XOR_A_A: { F = 0x00;	A = 0x00;		F |= 0x80;		} break;
case XOR_A_B: { F = 0x00;	A ^= B;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_C: { F = 0x00;	A ^= C;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_D: { F = 0x00;	A ^= D;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_E: { F = 0x00;	A ^= E;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_H: { F = 0x00;	A ^= H;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_L: { F = 0x00;	A ^= L;		if(A == 0){F |= 0x80;}		} break;
case XOR_A_HL: {F = 0x00; 	A ^= READOP(HL);	if(A == 0){F |= 0x80;}		} break;
case XOR_A_n: { F = 0x00; 	A ^= READOP(PC++);if(A == 0){F |= 0x80;}		} break;

case CP_A_A: { F = 0xC0;	 } break;
case CP_A_B: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((B)&15)){F |= 0x20;}		if(A < B){ F |= 0x10; }		if(A == B){ F |= 0x80; }		 } break;
case CP_A_C: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((C)&15)){F |= 0x20;}		if(A < C){ F |= 0x10; }		if(A == C){ F |= 0x80; }		 } break;
case CP_A_D: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((D)&15)){F |= 0x20;}		if(A < D){ F |= 0x10; }		if(A == D){ F |= 0x80; }		 } break;
case CP_A_E: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((E)&15)){F |= 0x20;}		if(A < E){ F |= 0x10; }		if(A == E){ F |= 0x80; }		 } break;
case CP_A_H: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((H)&15)){F |= 0x20;}		if(A < H){ F |= 0x10; }		if(A == H){ F |= 0x80; }		 } break;
case CP_A_L: { F = 0x00;	F |= 0x40;		if(((A)&15) < ((L)&15)){F |= 0x20;}		if(A < L){ F |= 0x10; }		if(A == L){ F |= 0x80; }		 } break;
case CP_A_HL: {F = 0x00;	F |= 0x40;		u8 n = READOP(HL);	 		if(((A)&15) < (n&15)){F |= 0x20;}		if(A < n){ F |= 0x10; }		if(A == n){ F |= 0x80; }		 } break;
case CP_A_n: { F = 0x00;	F |= 0x40;		u8 n = READOP(PC++);			if(((A)&15) < (n&15)){F |= 0x20;}		if(A < n){ F |= 0x10; }		if(A == n){ F |= 0x80; }		 } break;

case INC_A: { F &= 0x10;  		if((A&15) == 15){F |= 0x20;}		A++;		if(A == 0){F |= 0x80;}		 } break;
case INC_B: { F &= 0x10; 		if((B&15) == 15){F |= 0x20;}		B++;		if(B == 0){F |= 0x80;}		 } break;
case INC_C: { F &= 0x10;  		if((C&15) == 15){F |= 0x20;}		C++;		if(C == 0){F |= 0x80;}		 } break;
case INC_D: { F &= 0x10; 		if((D&15) == 15){F |= 0x20;}		D++;		if(D == 0){F |= 0x80;}		 } break;
case INC_E: { F &= 0x10;  		if((E&15) == 15){F |= 0x20;}		E++;		if(E == 0){F |= 0x80;}		 } break;
case INC_H: { F &= 0x10;  		if((H&15) == 15){F |= 0x20;}		H++;		if(H == 0){F |= 0x80;}		 } break;
case INC_L: { F &= 0x10;  		if((L&15) == 15){F |= 0x20;}		L++;		if(L == 0){F |= 0x80;}		 } break;
case INC_HL: {F &= 0x10;		u8 n = READOP(HL); 	if((n&15) == 15){F |= 0x20;}		n++;	if(n == 0){F |= 0x80;}		WRITEOP(HL, n);		 } break;

case DEC_A: { F &= 0x10;	F |= 0x40;		if((A&15) == 0){F |= 0x20;}		A--;	if(A == 0){F |= 0x80;}		} break;
case DEC_B: { F &= 0x10; 	F |= 0x40;		if((B&15) == 0){F |= 0x20;}		B--;	if(B == 0){F |= 0x80;}		} break;
case DEC_C: { F &= 0x10; 	F |= 0x40;		if((C&15) == 0){F |= 0x20;}		C--;	if(C == 0){F |= 0x80;}		} break;
case DEC_D: { F &= 0x10; 	F |= 0x40;		if((D&15) == 0){F |= 0x20;}		D--;	if(D == 0){F |= 0x80;}		} break;
case DEC_E: { F &= 0x10; 	F |= 0x40;		if((E&15) == 0){F |= 0x20;}		E--;	if(E == 0){F |= 0x80;}		} break;
case DEC_H: { F &= 0x10; 	F |= 0x40;		if((H&15) == 0){F |= 0x20;}		H--;	if(H == 0){F |= 0x80;}		} break;
case DEC_L: { F &= 0x10; 	F |= 0x40;		if((L&15) == 0){F |= 0x20;}		L--;	if(L == 0){F |= 0x80;}		} break;
case DEC_HL: {F &= 0x10;	F |= 0x40;		u8 n = READOP(HL); 	if((n&15) == 0){F |= 0x20;}		n--;	if(n == 0){F |= 0x80;}		WRITEOP(HL, n);		 } break;

case ADD_HL_BC: { F &= 0x80;		if((HL&0xFFF) + (BC&0xFFF) > 0xFFF){F |= 0x20;}			if(HL + BC > 0xFFFF){ F |= 0x10;}		HL += BC;		 } break;
case ADD_HL_DE: { F &= 0x80;		if((HL&0xFFF) + (DE&0xFFF) > 0xFFF){F |= 0x20;}			if(HL + DE > 0xFFFF){ F |= 0x10;}		HL += DE;		 } break;
case ADD_HL_HL: { F &= 0x80;		if((HL&0xFFF) + (HL&0xFFF) > 0xFFF){F |= 0x20;}			if(HL + HL > 0xFFFF){ F |= 0x10;}		HL += HL;		 } break;
case ADD_HL_SP: { F &= 0x80;		if((HL&0xFFF) + (SP&0xFFF) > 0xFFF){F |= 0x20;}			if(HL + SP > 0xFFFF){ F |= 0x10;}		HL += SP;		 } break;

case ADD_SP_n: { 
	u8 n = READOP(PC++); 
	
	F = 0x00;

	if(((SP&15) + (n&15)) > 15) { F |= 0x20; }
	if(((SP&255) + n) > 255) { F |= 0x10; }

	if(n>127) { SP += (signed char)n; }
	else { SP += n;	}
} break;

case INC_BC: { BC++; } break;
case INC_DE: { DE++; } break;
case INC_HL2:{ HL++; } break;
case INC_SP: { SP++; } break;

case DEC_BC: { BC--; } break;
case DEC_DE: { DE--; } break;
case DEC_HL2: {HL--; } break;
case DEC_SP: { SP--; } break;

case DAA_A: { 
	u16 a = A;
	
	if(!(F&0x40))
	{
		if((F&0x20) || ((a&0xF)>9))
		{
			a += 0x6;
		}
		if((F&0x10) || (a>0x9F))
		{
			a += 0x60;
		}
	}
	else
	{
		if(F&0x20)
		{
			a = (a - 0x6)&255;
		}
		if(F&0x10)
		{
			a -=0x60;
		}
	}
	
	if(a&0x100) { F |= 0x10; }
	
	a &= 255;
	
	if(a==0) { F |= 0x80; }
	else { F &= 0x70; }
	
	A = a;
	
	F &= 0xD0;


} break;
case CPL_A: {F |= 0x60;	A ^= 255;	 } break;

case CCF: { F ^= 0x10;  	F &= 0x90;	 } break;
case SCF: { F |= 0x10;		F &= 0x90;	 } break;
/********************MISC*************************/
case NOP: {  } break;
case HALT: { /*if(!mmu->getHDMAstate() || halt){*/halt = 1; haltFlag = (*mmu->intrFlag);	PC--;/*}*/	 } break;
case STOP: { if(mmu->speedMode == 0){ speedMode = 2; mmu->speedMode = 1; }else{speedMode = 1; mmu->speedMode = 0; } mmu->speedSwitch = 0; } break;
case DI: { IME = 0;  } break;
case EI: { IME = 1;  } break;
/***************************************************/
case RLC_A: { u8 c = 0x00;	F &= 0x00; 	if(A&0x80)		{ F |= 0x10; c=0x01; }			A = (A<<1) + c ;	/*if(A==0) { F |= 0x80; }*/		 } break;
case RRC_A: { u8 c = 0x00;	F &= 0x00; 	if(A&0x01)		{ F |= 0x10; c=0x80; }			A = (A>>1) + c ;	/*if(A==0) { F |= 0x80; }*/		 } break;
case RL_A: { u8 c = (F&0x10)?1:0;		F &= 0x00;		if(A&0x80){ F |= 0x10; }		A = (A<<1) + c;		/*if(A==0) { F |= 0x80; }*/		 } break;
case RR_A: { u8 c = ((F&0x10)?1:0)<<7;	F &= 0x00;		if(A&0x01){ F |= 0x10; }		A = (A>>1) + c;		/*if(A==0) { F |= 0x80; }*/		 } break;

case JP_nn: { 					u16 a = READOPB(PC);		PC = a;		 } break;
case JP_NZ_nn: 	{ if((!(F&0x80)))	{	u16 a = READOPB(PC);		PC = a; 	opCycle += 4;} else { PC += 2; }		 } break;
case JP_Z_nn:	{ if((F&0x80)) 	{		u16 a = READOPB(PC);		PC = a; 	opCycle += 4;} else { PC += 2; }		 } break;
case JP_NC_nn: 	{ if((!(F&0x10))) 	{	u16 a = READOPB(PC);		PC = a; 	opCycle += 4;} else { PC += 2; }		 } break;
case JP_C_nn: 	{ if((F&0x10)) 	{		u16 a = READOPB(PC);		PC = a; 	opCycle += 4;} else { PC += 2; }		 } break;
case JP_HL: {	PC = HL;		 } break;

case JR_n: 		{ 					s8 n = (signed char)READOP(PC++);		PC += n;						} break;
case JR_NZ_n: 	{ if((!(F&0x80))){ 	s8 n = (signed char)READOP(PC++);		PC += n;	opCycle += 4;	}else{ PC += 1; }		 } break;
case JR_Z_n:	{ if((F&0x80)){		s8 n = (signed char)READOP(PC++);		PC += n;	opCycle += 4;	}else{ PC += 1; }		 } break;
case JR_NC_n: 	{ if((!(F&0x10))){	s8 n = (signed char)READOP(PC++);		PC += n; 	opCycle += 4;	}else{ PC += 1; }		 } break;
case JR_C_n: 	{ if((F&0x10)){		s8 n = (signed char)READOP(PC++);		PC += n;	opCycle += 4;	}else{ PC += 1; }		 } break;

case CALL_nn: 	{ 				 		 SP -= 2;		WRITEOPB(SP, PC+2);		u16 a = READOPB(PC);		PC = a;		 } break;
case CALL_NZ_nn: 	{ if((!(F&0x80))){	 SP -= 2;		WRITEOPB(SP, PC+2);		u16 a = READOPB(PC);		PC = a; 	opCycle += 12;} else { PC += 2; }		 } break;
case CALL_Z_nn:	{ 		if((F&0x80)) {	 SP -= 2;		WRITEOPB(SP, PC+2);		u16 a = READOPB(PC);		PC = a; 	opCycle += 12;} else { PC += 2; }		 } break;
case CALL_NC_nn: 	{ if((!(F&0x10))){	 SP -= 2;		WRITEOPB(SP, PC+2);		u16 a = READOPB(PC);		PC = a; 	opCycle += 12;} else { PC += 2; }		 } break;
case CALL_C_nn: 	{ 	if((F&0x10)) {	 SP -= 2;		WRITEOPB(SP, PC+2);		u16 a = READOPB(PC);		PC = a; 	opCycle += 12;} else { PC += 2; }		 } break;

case RST_00: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0000;	 } break;
case RST_08: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0008;	 } break;
case RST_10: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0010;	 } break;
case RST_18: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0018;	 } break;
case RST_20: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0020;	 } break;
case RST_28: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0028;	 } break;
case RST_30: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0030;	 } break;
case RST_38: {  SP -= 2;	WRITEOPB(SP, PC);		PC = 0x0038;	 } break;

case RET: 	{ 			   			PC = READOPB(SP); 	SP += 2;		 } break;
case RET_NZ:	{ if((!(F&0x80))){ 	PC = READOPB(SP); 	SP += 2; 	opCycle += 12;}	 } break;
case RET_Z:		{ if((F&0x80)) { 	PC = READOPB(SP); 	SP += 2;	opCycle += 12;}	 } break;
case RET_NC: 	{ if((!(F&0x10))){ 	PC = READOPB(SP); 	SP += 2; 	opCycle += 12;}	 } break;
case RET_C:		{ if((F&0x10)) { 	PC = READOPB(SP); 	SP += 2; 	opCycle += 12;}	 } break;
case RETI: 	{	IME = 1;   	PC = READOPB(SP); 	SP += 2;		 } break;