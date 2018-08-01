#include <stdio.h>
#include "AES.h"

unsigned char hex(const char *s)
{
        unsigned char v=0;
        if('a' <= s[0] && 'f' >= s[0])
        {
                v=10 + (s[0] - 'a');
        }
        else
        {
                v=s[0] - '0';
        }

        v <<= 4;
        if('a' <= s[1] && 'f' >= s[1])
        {
                v +=10 + (s[1] - 'a');
        }
        else
        {
                v+=s[1] - '0';
        }

        return v;
}

void P(unsigned char *state)
{
	for(int i=0;i<(4*4);i++)
	{
		printf("%.2x ",state[i]);
		if((i+1)%4 == 0)
			printf("\n");
	}
}


int main(int argc,char **argv)
{

	// hexdecimal
	const char *plaintext="00112233445566778899aabbccddeeff";
	const char *key=      "000102030405060708090a0b0c0d0e0f";

	// debug mix columns
	const char *mix_str=  "d4bf5d30e0b452ae0000000000000000";	
	unsigned char mix[4*4]={};
	// debug expanded keys
	// const char *key=      "2b7e151628aed2a6abf7158809cf4f3c";

	unsigned int Nb=4;
	unsigned int Nk=4;
	unsigned char master_keys[4*4]={};

	unsigned char state[4*4]={};
	unsigned char prev_state[4*4]={};
	for(int i=0;i<(4*4);i++)
	{
		state[i]='a' + i%26;
		master_keys[i] = 'A' + i%26;

		state[i] = hex(plaintext + 2*i);
		master_keys[i] = hex(key + 2*i);

		mix[i] = hex(mix_str + 2*i);

		prev_state[i] = state[i];
	}
	printf("\n");

	AES aes;
	assert(0 == aes.init(Nb,Nk,master_keys));

	if(false)
	{
		{
			unsigned char v1=0x57;
			unsigned char v2=0x13;
			printf("%.2x * %.2x in polynomial  = %.2x\n",v1,v2,aes.polynomialMulti(v1,v2));
			aes.mixColumns(mix);
			aes.statePrint("mix",0,mix);
		}	

		aes.shiftRows(state);
		aes.shiftInverseRows(state);
	}	

	aes.encrypt(state);

	printf("key:\n");
	P(master_keys);

	printf("\n");
	printf("userdata:\n");
	P(prev_state);

	printf("\nencrypt:\n");
	P(state);

	aes.deencrypt(state);

	printf("\ndecrypt:\n");
	P(state);
	printf("\n");

	return 0;
}

