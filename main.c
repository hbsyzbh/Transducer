#include "si_efm8sb2_register_enums.h"
#include "radio_config_Si4355.h"
#include "bsp.h"
char not_do = 1;
unsigned char lightOn = 0;
void LM4991(bool on)
{
	unsigned long delay = 1000000;
	if (on){
		P1_B4 = 1;
		//while(delay--);
		//P1_B5 = 1;
		P1_B6 = 0;
		P1_B3 = 0;
	}else{
		P1_B4 = 0;
		//P1_B5 = 0;
		P1_B6 = 1;
		P1_B3 = 1;
	}
}

#define X20M
void BoardInit()
{
		PCA0MD = PCA0MD_WDTE__DISABLED | PCA0MD_CPS__SYSCLK;

#ifdef X20M	
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__LPOSC;  //20M  SYS_CLK
#else
	HFO0CN = 0x8F;
	while(HFO0CN != 0xCF) ;
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__HFOSC;  //24.5M
#endif
	
	//CKCON0 = CKCON0_T1M__SYSCLK;	//TIMER1 ?? SYS_CLK
	CKCON0 = CKCON0_T1M__PRESCALE | CKCON0_SCA__SYSCLK_DIV_12 | CKCON0_T3ML__SYSCLK | CKCON0_T3MH__SYSCLK;
	TMOD = TMOD_T1M__MODE2;
	
#ifdef X20M	
	TL1 = TH1 = 169;
#else
	TL1 = TH1 = 150;
#endif	
	
	//TL1 = TH1 = 126;
	//TL1 = TH1 = 97;
	SCON0 = 0x50;
	
	TCON = TCON_TR1__RUN;
	
	SPI0CFG = SPI0CFG_MSTEN__MASTER_ENABLED;
	SPI0CN0 = SPI0CN0_NSSMD__3_WIRE | SPI0CN0_SPIEN__ENABLED;
	
	XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__ENABLED; // | XBR0_CP0E__ENABLED;
	XBR1 = XBR1_PCA0ME__CEX0;
	XBR2 = XBR2_XBARE__ENABLED;
	P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__PUSH_PULL | P0MDOUT_B3__PUSH_PULL |
						P0MDOUT_B4__PUSH_PULL | P0MDOUT_B5__OPEN_DRAIN | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__PUSH_PULL ;
						
	P0SKIP =  P0SKIP_B0__NOT_SKIPPED | P0SKIP_B1__NOT_SKIPPED
					| P0SKIP_B2__NOT_SKIPPED | P0SKIP_B3__SKIPPED
					| P0SKIP_B4__NOT_SKIPPED | P0SKIP_B5__NOT_SKIPPED
					| P0SKIP_B6__SKIPPED     | P0SKIP_B7__SKIPPED;
	
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__OPEN_DRAIN | P1MDOUT_B3__PUSH_PULL |
						P1MDOUT_B4__PUSH_PULL | P1MDOUT_B5__OPEN_DRAIN;

	P1SKIP =  P1SKIP_B0__SKIPPED 		 | P1SKIP_B1__SKIPPED 
					| P1SKIP_B2__SKIPPED		 | P1SKIP_B3__SKIPPED | P1SKIP_B4__SKIPPED | P1SKIP_B5__SKIPPED;
	
	IE = IE_EA__ENABLED | IE_ES0__ENABLED;
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
			if(lightOn)
			{
				P1_B2 = 0;
			}
		} else {
			P1_B2 = 1;
		}

	
	if(division == 0) {
		if (lightOn) 
			lightOn--;
	}
}


U8 SpiReadWrite(U8 byte)
{
	U8 ret;
	SPI0DAT = byte;
	while( ! SPI0CN0_SPIF);
		
	ret = SPI0DAT;
	SPI0CN0_SPIF = 0;
	return ret;
}

void SpiWriteData(U8 byteCount, U8 *pData)
{
	U8 i = 0;
	for(i = 0; i<byteCount; i++)
	{
			SPI0DAT = pData[i];
			while( ! SPI0CN0_SPIF);
			SPI0CN0_SPIF = 0;
	}
}

void SpiReadData(U8 byteCount, U8 *pData)
{	
	U8 i = 0;
	for(i = 0; i<byteCount; i++)
	{
			SPI0DAT = 0xFF;
			while( ! SPI0CN0_SPIF);
			pData[i] = SPI0DAT;
			SPI0CN0_SPIF = 0;
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

void SetSoundLevel(unsigned char level)
{
	unsigned char i;
	unsigned char sum;

	unsigned char cmd[6] = {0x7E, 0x04, 0xAE, 31, 0xD1, 0xEF };

	if (level > 31) {
		level = 31;
	}

	sum = 0x04 + 0xAE + level;
	cmd[3] = level;
	cmd[4] = sum;
		
	SCON0_TI = 0;
	for(i = 0; i < sizeof(cmd); i++)
	{
		SBUF0 = cmd[i];
		while( ! SCON0_TI);
		SCON0_TI = 0;
	}
}

void Play()
{
	unsigned char i;
	//code unsigned char cmd[] = {0x7E, 0x03, 0xAA, 0xAD, 0xEF };
	code unsigned char cmd[] = {0x7E, 0x02, 0x01, 0xEF };
	//LM4991(1);
	//SetSoundLevel(15);
	SCON0_TI = 0;
	for(i = 0; i < sizeof(cmd); i++)
	{
		SBUF0 = cmd[i];
		while( ! SCON0_TI);
		SCON0_TI = 0;
	}
}

void delay()
{
	unsigned long delay = 2000000;
	while(delay--);
}

void delay2()
{
	unsigned long delay = 95000;
	while(delay--);
}

#if 1
void qSoundlev()
{
	unsigned char i;
	//code unsigned char cmd[] = {0x7E, 0x03, 0xC2, 0xC5, 0xEF };
	code unsigned char cmd[] = {0x7E, 0x02, 0x11, 0xEF };
	
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 9;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
		//
		//PCON0 |= 0x01; // set IDLE bit
		//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
		//delay();
	}
}

void qcmd(char thecmd)
{
	unsigned char i;
	unsigned char cmd[] = {0x7E, 0x02, 0, 0xEF };
	
	unsigned char ret[10];

	cmd[2] = thecmd;
	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 10;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
		//
		//PCON0 |= 0x01; // set IDLE bit
		//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
		//delay();
	}
}

void setSoundlev()
{
	unsigned char i;
	//code unsigned char cmd[] = {0x7E, 0x03, 0xC2, 0xC5, 0xEF };
	code unsigned char cmd[] = {0x7E, 0x03, 0x31, 22, 0xEF };
	//code unsigned char cmd[] = {0x7E, 0x03, 0x31, 31, 0xEF };
	
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 4;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
		//
		//PCON0 |= 0x01; // set IDLE bit
		//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
		//delay();
	}
}

#endif
void Q008Sleep()
{
	unsigned char i;
	code unsigned char cmd[] = {0x7E, 0x03, 0x35, 0x03, 0xEF };
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 4;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
	}
}


void Play2()
{
	unsigned char i;
	//code unsigned char cmd[] = {0x7E, 0x03, 0xC2, 0xC5, 0xEF };
		code unsigned char cmd[] = {0x7E, 0x02, 0x03, 0xEF };
	
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 4;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
		//
		//PCON0 |= 0x01; // set IDLE bit
		//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
		//delay();
	}
}

void StopPlay()
{
	unsigned char i;
	code unsigned char cmd[] = {0x7E, 0x02, 0x0E, 0xEF };
	
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
		
		for(i = 0; i < 4;i++)
		{
			while(SCON0_RI == 0);
			ret[i] = SBUF0;
			SCON0_RI = 0;
		}
		break;
	}
}


void PlayEnd()
{
	unsigned char i;
	code unsigned char cmd[] = {0x7E, 0x03, 0xC2, 0xC5, 0xEF };
	
	unsigned char ret[10];

	for(;;)
	{
			SCON0_RI = 0;
			i = SBUF0;
			SCON0_RI = 0;
		
		
			SCON0_TI = 0;
			for(i = 0; i < sizeof(cmd); i++)
			{
				SBUF0 = cmd[i];
				while( ! SCON0_TI);
				SCON0_TI = 0;
			}
			
			i = 0;
			while(SCON0_RI == 0);
			ret[0] = SBUF0;
			SCON0_RI = 0;
			
			if (ret[0] == 0xC2) {
				while(SCON0_RI == 0);
				ret[1] = SBUF0;
				SCON0_RI = 0;
				if(ret[1] == 0x02) {
					//LM4991(0);
					break;
				}
			}
		//
		PCON0 |= 0x01; // set IDLE bit
		PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
		delay();
	}
}

void PlayEnd2()
{
	delay2();
	while(P1_B5 == 0);
	LM4991(0);
}

#define uart0RxBuffSize (32)
unsigned short uart0RxIndex = 0;
xdata unsigned char  uart0RxBuff[uart0RxBuffSize];

void uart0() interrupt UART0_IRQn
{
	if (SCON0_TI) 
	{
		/*
		if (uart0TxIndex < uart0TxLen) {
				SBUF0 = uart0TxBuff[uart0TxIndex++];
		}
		
		SCON0_TI = 0;
		*/
	}		

	if (SCON0_RI) 
	{
		uart0RxBuff[((uart0RxIndex++) % uart0RxBuffSize)] = SBUF0;
		SCON0_RI = 0;

	}
}

typedef struct
{
  U32         ChipID;
  U8          Flags;          /* 4 bit - Buttons */
  U16         RollingCounter;
} tRadioPacket;

void setRTC(unsigned char Addr, unsigned char value)
{
	if (RTC0KEY != RTC0KEY_RTC0ST__UNLOCKED)
	{
		RTC0KEY = RTC0KEY_RTC0ST__KEY1;
		RTC0KEY = RTC0KEY_RTC0ST__KEY2;
	}
	
	RTC0ADR = Addr;
	RTC0DAT = value;
}

unsigned char getRTC(unsigned char Addr)
{
	if (RTC0KEY != RTC0KEY_RTC0ST__UNLOCKED)
	{
		RTC0KEY = RTC0KEY_RTC0ST__KEY1;
		RTC0KEY = RTC0KEY_RTC0ST__KEY2;
	}
	
	RTC0ADR = Addr;
	return RTC0DAT;
}

void cpuSleep()
{
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__RTC;
	PMU0CF = PMU0CF_SLEEP__START;
}

void cpuStop()
{
	CLKSEL = CLKSEL_CLKDIV__SYSCLK_DIV_1 | CLKSEL_CLKSL__RTC;
	PCON0 = PCON0_STOP__STOP;
}


#if 1

char PlayFlag = 0;
typedef enum
{
	ms_init,
	ms_startRF,
	ms_waitRF,
}	MAIN_STATE;

MAIN_STATE state;

static void doInit(void)
{
	lightOn = 1;

	P1_B3 = 1;
	selectTimer3Freq();
	BoardInit();
	vRadio_Init();
}

static void doStartRF(void)
{
	Radio_StartRX(0u);
	si4455_fifo_info(0x02);
}

static int doWaitRF(void)
{
	static unsigned char Key[3];
	static unsigned char count = 0;
	tRadioPacket RadioPacket;
	

	si4455_get_int_status(0u, 0u, 0u);
			
			 /* check the reason for the IT */
	if (Si4455Cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
	{	/* Packet RX */
		si4455_read_rx_fifo(sizeof(tRadioPacket), (U8 *) &RadioPacket);
		lightOn = 3;
		Key[(count++) % 3] = RadioPacket.Flags;
		if (count == 3) {
			count = 0;
			if(( Key[0] == 'D') && (Key[1] == 'D') && (Key[2] == 'D'))
				{
						return 2;
				}
		}
		
		return 1;
	}
	
	return 0;
}

static void mainState(void)
{
	switch(state)
	{
		case ms_init:
			doInit();
			state++;
			break;
		
		case ms_startRF:
			doStartRF();
			state++;
			break;
		
		case ms_waitRF:
			{
				int ret = doWaitRF();
				
				if (ret)  {
					state = ms_startRF;
				}
					
				if (ret == 2)
					PlayFlag = 1;
			}
			
			break;
		
		default:
			state = ms_init;
			break;
	}
}

typedef enum
{
	ps_stop,
	ps_play,
	ps_waitFor_end
}	PLAY_STATE;

PLAY_STATE play_st;

static void PlayState()
{
	if(PlayFlag) {
		PlayFlag = 0;
		switch(play_st)
		{
			case ps_stop:
				LM4991(1);
				P0MDOUT |= P0MDOUT_B4__PUSH_PULL;
				delay2();
				//	qcmd(0x17);
				setSoundlev();
				qSoundlev();
				Play2();
				delay2();
				play_st = ps_play;
				break;
			
			case ps_play:
			default:
				StopPlay();
				break;
					
		}
	}
	
	if (play_st == ps_play) {
		if(P1_B5 == 1) {
			LM4991(0);
			play_st = ps_stop;
		}	
	}
}

int main()
{
	for(; ;)
	{
			mainState();
			PlayState();
	}
}

#else
int main()
{
	unsigned char Key[3];
	unsigned char count = 0;


/*	
	LM4991(1);
	delay2();
	Q008Sleep();
	P1_B5 = 0;
	P0_B7 = 1;
*/
	//SetSoundLevel(16);
	//SBUF0 = 'a';
/*
	P0_B7 = 1;	
	setRTC(RTC0CN0, 0x80);
	CLKSEL = 0x33;
	while(CLKSEL != 0xB4);
	for( ; ;)
	{
#if 0	
		PCON0 |= 0x01; // set IDLE bit
		PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
#else
		PMU0CF = 0x80;
		{
		#pragma asm
			NOP;
			NOP;
			NOP;
			NOP;
		#pragma endasm
		}
#endif
	}
*/
	//Play();
	
	
	lightOn = 1;

	P1_B3 = 1;
	selectTimer3Freq();
	BoardInit();
	vRadio_Init();
 	
//	power4355up();
//	get_stats();
	for(;;)
	{
		Radio_StartRX(0u);
		si4455_fifo_info(0x02);
		//PCON0 |= 0x01; // set IDLE bit
		//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction

		for(;;)
		{
			tRadioPacket RadioPacket;
			si4455_get_int_status(0u, 0u, 0u);
			
			//P0_B7 = 1;
			//P0_B7 = 0;//sdn Radio on
			 /* check the reason for the IT */
		    if (Si4455Cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
		    {	/* Packet RX */
					si4455_read_rx_fifo(sizeof(tRadioPacket), (U8 *) &RadioPacket);
					lightOn = 3;
					Key[(count++) % 3] = RadioPacket.Flags;
					if (count == 3) {
						count = 0;
						if(( Key[0] == 'D') && (Key[1] == 'D') && (Key[2] == 'D'))
							{
								LM4991(1);
								P0MDOUT |= P0MDOUT_B4__PUSH_PULL;
								if(not_do) {
									delay2();
								//	qcmd(0x17);
									setSoundlev();
									qSoundlev();
									Play2();
									//Play();
									//delay();
									//PlayEnd();
									PlayEnd2();
								}
						}
					}
					break;
		    }

				//PCON0 |= 0x01; // set IDLE bit
				//PCON0 = PCON0; // ... followed by a 3-cycle dummy instruction
				/*
				#if 0
				P0MDOUT &= ~P0MDOUT_B4__PUSH_PULL;
				P0MDOUT &= ~P0MDOUT_B5__PUSH_PULL;
				P0MDOUT |= 0x0F;
				P0 = 0xF0;
				SPI0CN0 = SPI0CN0_SPIEN__DISABLED;
				P1MDOUT &= 0xFE;
				
				cpuSleep();
				#endif
				*/
				//cpuStop();
		}
	}
		
}
#endif