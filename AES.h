/*
 * 2018-07-29
 * xibaocao
 * */
#ifndef __AES_H__
#define __AES_H__

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

unsigned char sBox[256]={
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};
unsigned char sInverseBox[256]={
	0x52, 0x09, 0x6A, 0xD5, 0x30, 0x36, 0xA5, 0x38, 0xBF, 0x40, 0xA3, 0x9E, 0x81, 0xF3, 0xD7, 0xFB,
	0x7C, 0xE3, 0x39, 0x82, 0x9B, 0x2F, 0xFF, 0x87, 0x34, 0x8E, 0x43, 0x44, 0xC4, 0xDE, 0xE9, 0xCB,
	0x54, 0x7B, 0x94, 0x32, 0xA6, 0xC2, 0x23, 0x3D, 0xEE, 0x4C, 0x95, 0x0B, 0x42, 0xFA, 0xC3, 0x4E,
	0x08, 0x2E, 0xA1, 0x66, 0x28, 0xD9, 0x24, 0xB2, 0x76, 0x5B, 0xA2, 0x49, 0x6D, 0x8B, 0xD1, 0x25,
	0x72, 0xF8, 0xF6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xD4, 0xA4, 0x5C, 0xCC, 0x5D, 0x65, 0xB6, 0x92,
	0x6C, 0x70, 0x48, 0x50, 0xFD, 0xED, 0xB9, 0xDA, 0x5E, 0x15, 0x46, 0x57, 0xA7, 0x8D, 0x9D, 0x84,
	0x90, 0xD8, 0xAB, 0x00, 0x8C, 0xBC, 0xD3, 0x0A, 0xF7, 0xE4, 0x58, 0x05, 0xB8, 0xB3, 0x45, 0x06,
	0xD0, 0x2C, 0x1E, 0x8F, 0xCA, 0x3F, 0x0F, 0x02, 0xC1, 0xAF, 0xBD, 0x03, 0x01, 0x13, 0x8A, 0x6B,
	0x3A, 0x91, 0x11, 0x41, 0x4F, 0x67, 0xDC, 0xEA, 0x97, 0xF2, 0xCF, 0xCE, 0xF0, 0xB4, 0xE6, 0x73,
	0x96, 0xAC, 0x74, 0x22, 0xE7, 0xAD, 0x35, 0x85, 0xE2, 0xF9, 0x37, 0xE8, 0x1C, 0x75, 0xDF, 0x6E,
	0x47, 0xF1, 0x1A, 0x71, 0x1D, 0x29, 0xC5, 0x89, 0x6F, 0xB7, 0x62, 0x0E, 0xAA, 0x18, 0xBE, 0x1B,
	0xFC, 0x56, 0x3E, 0x4B, 0xC6, 0xD2, 0x79, 0x20, 0x9A, 0xDB, 0xC0, 0xFE, 0x78, 0xCD, 0x5A, 0xF4,
	0x1F, 0xDD, 0xA8, 0x33, 0x88, 0x07, 0xC7, 0x31, 0xB1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xEC, 0x5F,
	0x60, 0x51, 0x7F, 0xA9, 0x19, 0xB5, 0x4A, 0x0D, 0x2D, 0xE5, 0x7A, 0x9F, 0x93, 0xC9, 0x9C, 0xEF,
	0xA0, 0xE0, 0x3B, 0x4D, 0xAE, 0x2A, 0xF5, 0xB0, 0xC8, 0xEB, 0xBB, 0x3C, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2B, 0x04, 0x7E, 0xBA, 0x77, 0xD6, 0x26, 0xE1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0C, 0x7D
};
unsigned char	shiftOffsets[] = {
	0,3,2,1
};
unsigned char shiftInverseOffsets[] = {
	0,1,2,3
};
unsigned char mixMatrix[] = {
	2,3,1,1,
	1,2,3,1,
	1,1,2,3,
	3,1,1,2
};
unsigned char mixInverseMatrix[] = {
	0xe,0xb,0xd,0x9,
	0x9,0xe,0xb,0xd,
	0xd,0x9,0xe,0xb,
	0xb,0xd,0x9,0xe
};

/***
 * implementation for Advance Encrypt Standard(Danejael)
 * References:
 * 	AES specification https://csrc.nist.gov/csrc/media/publications/fips/197/final/documents/fips-197.pdf
 */
class AES
{
	public:
		AES(){
			master_keys= NULL;
			expanded_keys=NULL;
		}

		~AES(){
			if(NULL != master_keys)
			{
				delete master_keys;
				master_keys=NULL;
			}
			if(NULL != expanded_keys)
			{
				delete expanded_keys;
				expanded_keys=NULL;
			}
		}

		/**
		 * _Nb must be 4 in AES
		 * _Nk can be 4/6/8
		 * master keys length = Nk * 4 (Bytes)
		 */
		int init(unsigned int _Nb,unsigned int _Nk,unsigned char _master_keys[])
		{
			Nb = _Nb;
			Nk = _Nk;
			Nc = Nb*4;

			switch(Nk)
			{
				case 4:
					Nr=10;
					break;
				case 6:
					Nr=12;
					break;
				case 8:
					Nr=14;
					break;
				default:
					return -1;
			}
			master_keys=new unsigned char[Nk*4+1];
			expanded_keys=new unsigned char[4*10*20];
			for(unsigned int i=0;i<(Nk*4);i++)
				master_keys[i] = _master_keys[i];

			expandedKeys();
#ifdef DEBUG
			printf("Nb=%d,Nk=%d,Nc=%d\n",Nb,Nk,Nc);
#endif
			return 0;
		}

		// state's size = 4*4 in AES
		void encrypt(unsigned char *state)
		{
			int r=0;
			statePrint("input",r,state);
			// pre
			statePrint("key",r,expanded_keys);
			addRoundKey(state,expanded_keys);

			// middle
			for(unsigned int i=1;i<Nr;i++)
			{
				r++;
				statePrint("start",r,state);

				subBytes(state);
				statePrint("s_box",r,state);

				shiftRows(state);
				statePrint("s_row",r,state);

				mixColumns(state);
				statePrint("m_col",r,state);

				addRoundKey(state,expanded_keys + 4*i*Nk);
				statePrint("key",r,expanded_keys+i*Nk);
#ifdef DEBUG
				printf("\n");
#endif
			}

			// end
			r++;
			subBytes(state);
			statePrint("s_box",r,state);
			shiftRows(state);
			statePrint("s_row",r,state);
			addRoundKey(state,expanded_keys + (Nr)*Nk*4);
			statePrint("key",r,expanded_keys + Nr*Nk);
			statePrint("output",r,state);
			return ;
		}

		void deencrypt(unsigned char *state)
		{
			addRoundKey(state, expanded_keys + (Nr)*Nk*4);
			shiftInverseRows(state);
			subInverseBytes(state);

			for(unsigned int i=(Nr-1);i>0;i--)
			{
				addRoundKey(state, expanded_keys + i*Nk*4);
				mixInverseColumns(state);
				shiftInverseRows(state);
				subInverseBytes(state);
			}

			addRoundKey(state, expanded_keys);
		}

		// public for test
	public:
		void wordPrint(const char *s,unsigned int idx,unsigned char *word)
		{
#ifdef DEBUG 
			printf("%s[%.2d] = %02x%02x%02x%02x\n",s,idx,word[0],word[1],word[2],word[3]);
#endif
		}

		void statePrint(const char *s,int r,unsigned char *state)
		{
#ifdef DEBUG 
			printf("state round[%.2d].%.6s	",r,s);
			for(int i=0;i<4*Nb;i++)
				printf("%.2x",state[i]);
			printf("\n");
#endif
		}


		void subWord(unsigned char *word)
		{
			for(unsigned int i=0;i<4;i++)
				word[i] = sBox[word[i]];
			return ;
		}

		void subInverseWord(unsigned char *word)
		{
			for(unsigned int i=0;i<4;i++)
				word[i] = sInverseBox[word[i]];
			return ;
		}

		void rotWord(unsigned char *word)
		{
			// a0,a1,a2,a3
			// ->
			// a1,a2,a3,a0
			unsigned char c = word[0];
			word[0] = word[1];
			word[1] = word[2];
			word[2] = word[3];
			word[3] = c;
			return;
		}

		void rotInverseWord(unsigned char *word)
		{
			// a1,a2,a3,a0
			// ->
			// a0,a1,a2,a3
			unsigned char c = word[2];
			word[0] = word[4];
			word[1] = word[0];
			word[2] = word[1];
			word[3] = c;
		}


		void expandedKeys()
		{

			for(unsigned int i=0;i<(Nk*4);i++)
			{
				expanded_keys[i] = master_keys[i];
#ifdef DEBUG
				printf("%.2x",expanded_keys[i]);
				if((i+1)%4 == 0)
					printf("\n");
#endif

			}

			unsigned int i=Nk;
			assert(i >= 1);
			unsigned char word[10];	// temp
			while( i < Nb*(Nr+1))
			{

				memmove((char*)word,(const char*)(expanded_keys + 4*(i-1)),4);
#ifdef DEBUG
				printf("i:%.2d\n",i);
#endif
				wordPrint("	prev",i,expanded_keys + 4*(i-1));
				wordPrint("	temp",i,word);

				if( i % Nk == 0)
				{
					rotWord(word);
					subWord(word);
					word[0] = word[0] ^ polynomialMultiShift(1,i/Nk-1);
				}
				else if( Nk > 6 && (i % Nk) == 4)
				{
					subWord(word);
				}

				wordPrint("	w[i-Nk]",i-Nk,expanded_keys+4*(i-Nk));

				for(unsigned int j=0;j<4;j++)
					expanded_keys[4*i+j] = word[j] ^ expanded_keys[4*(i-Nk)+j];

				wordPrint("	w[i]",i,expanded_keys+4*(i));
				i++;
			}

			// print for debug 
			for(unsigned int i=0;i<Nb*(Nr+1);i++)
			{	
				wordPrint("w",i,expanded_keys+i*4);
			}
			return;
		}

		void addRoundKey(unsigned char *state,unsigned char *expanded_keys)
		{
			for(unsigned int i=0;i<Nc;i++)
			{
				state[i] = state[i] ^ expanded_keys[i];
			}

			return ;
		}

		void subBytes(unsigned char *state)
		{
			for(unsigned int i=0;i<Nc;i++)
			{
				state[i] = sBox[state[i]];
			}

			return;
		}

		void subInverseBytes(unsigned char *state)
		{
			for(unsigned int i=0;i<Nc;i++)
			{

				state[i] = sInverseBox[state[i]];

			}
			return;
		}
		void shift(unsigned char *state,unsigned char offsets[])
		{
			for(unsigned int r=0;r<4;r++)
			{
				/*
				 * joy :)
				 * may be we can shift by  like:
				 * 1,2,3,4,5,6,7,8,9,10
				 * =>
				 * 10,9,8,7,6,5,4,3,2,1
				 * =>
				 * 5,6,7,8,9,10,1,2,3,4
				 * */
				unsigned int offset = offsets[r]; // only support one type !!
				if(0 == offset)
					continue;

				unsigned char row[10];
				for(unsigned int c=0;c<Nb;c++)
				{
					row[(c+offset)%Nb] = state[c*4 + r];
				}
				for(unsigned int c=0;c<Nb;c++)
				{
					state[c*4+r] = row[c];
				}
			}

			return;
		}

		void shiftInverseRows(unsigned char *state)
		{
			shift(state, shiftInverseOffsets);
			return;
		}

		void shiftRows(unsigned char *state)
		{
			shift(state, shiftOffsets);
			return;
		}

		unsigned char polynomialAdd(unsigned char v1,unsigned char v2)
		{
			return v1 ^ v2;
		}

		// polynomial multiplication * x^shift in GF(2^8)
		unsigned char polynomialMultiShift(unsigned char v1,unsigned char shift)
		{
			if(shift == 0) 
				return v1;
			// shift 1 bit
			// m(x)=x8 +x4 +x3 +x+1
			// 1,0001,1011
			if(shift != 1)
				v1 = polynomialMultiShift(v1,shift-1);
			if( (0x80 & v1) != 0)
			{
				v1 = (v1 << 1) ^ 0x1B;
			}
			else
			{
				v1 = (v1 << 1);
			}
			return v1;
		}


		// polynomial multiplication * in GF(2^8)
		unsigned char polynomialMulti(unsigned char v1,unsigned char v2)
		{
			unsigned char value=0;
			for(unsigned int i=0;i<8;i++)
			{
				if( ((1<<i) & v2) != 0)
				{
					value = polynomialAdd(value,
							polynomialMultiShift(v1,i));
				}
			}
			return value;
		}

		void mix(unsigned char *state,unsigned char *matrix)
		{
			// polynomial multiplication with coeffection * in GF(2^8) 
			// difference from above:
			// (ai) is one complete value in GF(2^8) ,range [0,255] not only [0,1]
			// format : ((a3*x^3 + a2*x^2 + a1*X^1 + a0) * (b3*x^3 + b2*x^2 + b1*X^1 + b0) ) modulo (x^4 + 1)
			// this can transfer into matrix computing
			unsigned char temp[4*4];
			memmove((char*)temp,state,4*4);
			for(unsigned int r=0;r<4;r++)
			{
				// Nb must be 4 in AES!
				for(unsigned int c=0;c<4;c++)
				{
					unsigned char value = 0;
					unsigned char t=0;
					for(unsigned int i=0;i<4;i++)
					{
						unsigned char  v1 = matrix[r*4+i];
						unsigned char v2 = temp[c*4+i];
						t = polynomialMulti(v1,v2);
						value = polynomialAdd(value,t);
					}
					state[c*4+r] = value;
				}
			}
			return;
		}
		void mixColumns(unsigned char *state)
		{
			return mix(state,mixMatrix);

		}

		void mixInverseColumns(unsigned char *state)
		{
			return mix(state,mixInverseMatrix);
		}

	private:
		unsigned int Nc;	// = 4*Nb;
		unsigned int Nb;	// state lenght of 4-bytes;
		unsigned int Nk;
		unsigned int Nr;	// rounds;
		unsigned char *master_keys;	// 128/196/256
		unsigned char *expanded_keys;	// expanded keys size; max
};

#endif 

