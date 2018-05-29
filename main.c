#include "si_efm8sb2_register_enums.h"
#include "radio_config_Si4355.h"

void LM4991(bool on)
{
	if (on){
		P1_B4 = 1;
	}else{
		P1_B4 = 0;
	}
}

void BoardInit()
{
		PCA0MD = PCA0MD_WDTE__DISABLED | PCA0MD_CPS__SYSCLK;

#if 1	
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LPOSC;  //20M  SYS_CLK
#else
	HFO0CN = 0x8F;
	while(HFO0CN != 0xCF) ;
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;  //24.5M
#endif
	
	//CKCON0 = CKCON0_T1M__SYSCLK;	//TIMER1 ?? SYS_CLK
	CKCON0 = CKCON0_T1M__PRESCALE | CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T3ML__SYSCLK | CKCON0_T3MH__SYSCLK;
	TMOD = TMOD_T1M__MODE2;
	//TL1 = TH1 = 150;
	//TL1 = TH1 = 169;
	TL1 = TH1 = 126;
	//TL1 = TH1 = 97;
	SCON0 = 0x50;
	
	TCON = TCON_TR1__RUN;
	
	SPI0CFG = SPI0CFG_MSTEN__MASTER_ENABLED;
	SPI0CN0 = SPI0CN0_NSSMD__3_WIRE | SPI0CN0_SPIEN__ENABLED;
	
	XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__ENABLED; // | XBR0_CP0E__ENABLED;
	XBR1 = XBR1_PCA0ME__CEX0;
	XBR2 = XBR2_XBARE__ENABLED;
	P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__PUSH_PULL | P0MDOUT_B3__PUSH_PULL |
						P0MDOUT_B4__PUSH_PULL | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B7__PUSH_PULL ;
						
	P0SKIP =  P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED
					| P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__SKIPPED
					| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
					| P0SKIP_B6__SKIPPED     | P0SKIP_B7__SKIPPED;
	
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__PUSH_PULL | P1MDOUT_B3__PUSH_PULL |
						P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__PUSH_PULL | P1MDOUT_B5__PUSH_PULL;

	P1SKIP =  P1SKIP_B0__SKIPPED 		 | P1SKIP_B1__SKIPPED 
					| P1SKIP_B2__SKIPPED		 | P1SKIP_B3__SKIPPED;
	
	//IE = IE_EA__ENABLED | IE_ES0__ENABLED;
	IE = IE_EA__ENABLED;
	EIE1 = EIE1_ET3__ENABLED;
	TMR3CN0 = TMR3CN0_TR3__RUN | TMR3CN0_T3SPLIT__16_BIT_RELOAD;
	
	//PCA0PWM = PCA0PWM_CLSEL__8_BITS | PCA0PWM_ARSEL__AUTORELOAD;
	//PCA0CPM0 = PCA0CPM0_PWM__ENABLED;
	LM4991(0);
}

//#define FREQ 11025
// 20M/8 - 305.2k/8     2.5M - 38.15
//void selectTimer3Freq(unsigned int freq)
void selectTimer3Freq(void)
{
/*	unsigned short time = 2500000 / freq;
	time = 65536 - time;

	TMR3RLH = TMR3H = time / 256;
	TMR3RLL = TMR3L = time % 256;
	TMR3RL = time;
	
	//TMR3 = TMR3RL = 65308;
	//20M//TMR3 = TMR3RL = 63722;
*/	
	TMR3 = TMR3RL = 15536;
}

void timer3() interrupt TIMER3_IRQn
{
	static unsigned short count = 0;
	unsigned short division = count++ % 200; 
	TMR3CN0 = TMR3CN0_TR3__RUN | TMR3CN0_T3SPLIT__16_BIT_RELOAD;
	if (division < 5) {
		P1_B3 = 0;
	} else {
		P1_B3 = 1;
	}
}

unsigned char Manufacturer[6];
void get_stats()
{
	unsigned char i;
	P0_B3 = 1;
	//dealy(10);
	P0_B3 = 0;
	//dealy(100);
	
	for(i =0; i<6; i++)
	{
		SPI0DAT = 0x0;
		while( ! SPI0CN0_SPIF);
		
		Manufacturer[i] = SPI0DAT;
		SPI0CN0_SPIF = 0;
	}
	P0_B3 = 1;
}

void power4355up()
{
	unsigned char i;
	unsigned char cmd[] = {
		RF_POWER_UP
	};
	
		
	P0_B3 = 1;
	//dealy(10);
	P0_B3 = 0;
	//dealy(100);
	
	for(i =0; i<sizeof(cmd); i++)
	{
		SPI0DAT = cmd[i];
		while( ! SPI0CN0_SPIF);
		
		SPI0CN0_SPIF = 0;
	}
	P0_B3 = 1;
}

void Play()
{
	unsigned char i;
	code unsigned char cmd[] = {0x7E, 0x03, 0xAC, 0xAF, 0xEF };
	
	SCON0_TI = 0;
	for(i = 0; i < sizeof(cmd); i++)
	{
		SBUF0 = cmd[i++];
		while( ! SCON0_TI);
		SCON0_TI = 0;
	}
}

int main()
{
	P0_B7 = 1;
	selectTimer3Freq();
	BoardInit();
  Play();
	P1_B3 = 1;
	P0_B7 = 0;
	power4355up();
	get_stats();
	for(;;)
	{
		PCON0 |= 0x01; // set IDLE bit
		PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
	}
		
}