/**
 *  Copyright 2008 Silicon Laboratories, Inc.
 *  http://www.silabs.com
 *
 *  @file Radio.c
 *  
 *  C File Description:
 *  @brief TODO
 *
 *  Project Name: dev_EzR2LCD_ROM3Demo 
 * 
 * 
 *  @author Sz. Papp
 *
 *  @date 2012.04.16.
 *
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 *  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  This software must be used in accordance with the End User License Agreement.
 */


/*! Compiler definitions */
#include "bsp.h"

/*****************************************************************************
 *  Local Macros & Definitions
 *****************************************************************************/

/*****************************************************************************
 *  Global Variables
 *****************************************************************************/
#define RADIO_MULTIPLE_CONFIG_NO_1
#include "radio_config.h"

/*# define SEGMENT_VARIABLE(name, vartype, locsegment) vartype locsegment name
tEzConfigArray SEG_CODE EzConfig_Array_316M66_FSK = EZCONFIG_ARRAY_DATA
tEzConfigArray是结构体类型, EzConfig_Array_316M66_FSK是变量名, EZCONFIG_ARRAY_DATA即是对其初始化的数据*/
const SEGMENT_VARIABLE(EzConfig_Array_316M66_FSK, tEzConfigArray, SEG_CODE) = \
                        EZCONFIG_ARRAY_DATA;
const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_316M66_FSK, tEzConfigArray, SEG_CODE, SEG_CODE) = \
                        &EzConfig_Array_316M66_FSK;
#undef RADIO_MULTIPLE_CONFIG_NO_1

#define RADIO_MULTIPLE_CONFIG_NO_2
#include "radio_config.h"

const SEGMENT_VARIABLE(EzConfig_Array_433M92_FSK, tEzConfigArray, SEG_CODE) = \
                        EZCONFIG_ARRAY_DATA;
const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_433M92_FSK, tEzConfigArray, SEG_CODE, SEG_CODE) = \
                        &EzConfig_Array_433M92_FSK;
#undef RADIO_MULTIPLE_CONFIG_NO_2

#define RADIO_MULTIPLE_CONFIG_NO_3
#include "radio_config.h"

const SEGMENT_VARIABLE(EzConfig_Array_868M30_FSK, tEzConfigArray, SEG_CODE) = \
                        EZCONFIG_ARRAY_DATA;
const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_868M30_FSK, tEzConfigArray, SEG_CODE, SEG_CODE) = \
                        &EzConfig_Array_868M30_FSK;
#undef RADIO_MULTIPLE_CONFIG_NO_3

#define RADIO_MULTIPLE_CONFIG_NO_4
#include "radio_config.h"

const SEGMENT_VARIABLE(EzConfig_Array_917M00_FSK, tEzConfigArray, SEG_CODE) = \
                        EZCONFIG_ARRAY_DATA;
const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_917M00_FSK, tEzConfigArray, SEG_CODE, SEG_CODE) = \
                        &EzConfig_Array_917M00_FSK;
#undef RADIO_MULTIPLE_CONFIG_NO_4

SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray, tEzConfigArray, SEG_CODE, SEG_XDATA);
/*****************************************************************************
 *  Local Function Declarations
 *****************************************************************************/
void vRadio_PowerUp(void);
U8 bRadio_Check_Ezconfig(U16);

/**
 *  Power up the Radio, set GPIOs to HiZ state, get Part Info
 *  then set Sleep mode.
 *
 *  @note
 *
 *****************************************************************************/
void vRadio_PowerUp(void)
{
  SEGMENT_VARIABLE(wDelay, U16, SEG_XDATA) = 0u; //定义局部变量

/*重启si4455,并置si4455到shutdown(RF_PWRDN = 1拉高SDN,此管脚是si4455的PIN 2,
  但程序中的定义是SBIT(RF_PWRDN, SFR_P1, 5)?, 等状态稳定后再将SDN拉低 */
  si4455_reset();
  for (; wDelay < pEzConfigArray->Radio_Delay_Cnt_After_Reset; wDelay++);
/*Radio_BOOT_OPTIONS是tEzConfigArray的成员变量, 在 RADIO_MULTIPLE_CONFIG_NO_4处找到
EZCONFIG_ARRAY_DATA_RADIO_BOOT_OPTIONS_DEFAULT;
*/
  si4455_power_up(pEzConfigArray->Radio_BOOT_OPTIONS,   //0x01
                  pEzConfigArray->Radio_XTAL_OPTIONS,   //0x00
                  pEzConfigArray->Radio_XO_FREQ);       //30000000L
	for (; wDelay < pEzConfigArray->Radio_Delay_Cnt_After_Reset; wDelay++);
  radio_comm_PollCTS();// Waits for CTS to be high
}

/**
 *  Radio Initialization. Load the 2FSK config to be able to set RF_GPIOs to
 *  HiZ state.
 *
 *  @author Sz. Papp
 *
 *  @note
 *
 *****************************************************************************/
void vRadio_Init(void)
{
	pEzConfigArray = &EzConfig_Array_917M00_FSK;
  vRadio_SetChip();
}

/**
 *  Sets the radio according to the given configuration.
 *
 *  @note
 *
 *****************************************************************************/
void vRadio_SetChip()
{
  U8 lTemp;
	  SEGMENT_VARIABLE(wDelay, U16, SEG_XDATA) = 0u; //定义局部变量
  do
  {
    /* Power Up the radio chip */
    vRadio_PowerUp();
     /*之前已按eeprom中的数据对pEzConfigArray赋值*/
    /* Load the 1st part of the configuration array (128 bytes),
	This command is used to configure the chip using a generated EZConfig configuration array
	?但写入数据的具体含义不知,是否由WDS设置后生成的代码得到 */
    si4455_write_ezconfig_array(128u, pEzConfigArray->Radio_Configuration);//EZCONFIG_ARRAY_DATA的第一个成员,存储的就是对RF的配置信息
	
		for (; wDelay < pEzConfigArray->Radio_Delay_Cnt_After_Reset; wDelay++);
    /* Load the final part of the configuration array (128 bytes) */
    si4455_write_ezconfig_array(EZCONFIG_RADIO_CFG_SIZE - 128u, (U8 *) pEzConfigArray->Radio_Configuration + 128u);

    lTemp = bRadio_Check_Ezconfig(pEzConfigArray->Radio_Configuration_CRC);

    /* Check the return value */
    //if (lTemp == 1u) { LED4 = !LED4; }
    //if (lTemp == 2u) { LED3 = !LED3; }
  } while(lTemp != 0u);

  // Read ITs, clear pending ones,Returns the current interrupt status of ALL the possible interrupt events (both STATUS and PENDING) and optionally clears PENDING interrupts.
  si4455_get_int_status(0u, 0u, 0u);//

  /* Enable ITs */
  si4455_set_property(
      SI4455_PROP_GRP_ID_INT_CTL,
      SI4455_PROP_GRP_LEN_INT_CTL,
      SI4455_PROP_GRP_INDEX_INT_CTL_ENABLE,
      (U8) pEzConfigArray->Radio_INT_CTL_ENABLE,
      (U8) pEzConfigArray->Radio_INT_CTL_PH_ENABLE,
      (U8) pEzConfigArray->Radio_INT_CTL_MODEM_ENABLE,
      (U8) pEzConfigArray->Radio_INT_CTL_CHIP_ENABLE
  );

  // Configure Fast response registers
  si4455_set_property(
      SI4455_PROP_GRP_ID_FRR_CTL,
      SI4455_PROP_GRP_LEN_FRR_CTL,
      SI4455_PROP_GRP_INDEX_FRR_CTL_A_MODE,
      (U8) pEzConfigArray->Radio_FRR_CTL_A_MODE,
      (U8) pEzConfigArray->Radio_FRR_CTL_B_MODE,
      (U8) pEzConfigArray->Radio_FRR_CTL_C_MODE,
      (U8) pEzConfigArray->Radio_FRR_CTL_D_MODE
  );

  // Configure GPIO pins,设置si4x55的各GPIO的使用模式,具体见GPIO_PIN_CFG的说明
  si4455_gpio_pin_cfg(
      (U8) pEzConfigArray->Radio_GPIO0_PIN_CFG,
      (U8) pEzConfigArray->Radio_GPIO1_PIN_CFG,
      (U8) pEzConfigArray->Radio_GPIO2_PIN_CFG,
      (U8) pEzConfigArray->Radio_GPIO3_PIN_CFG,
      (U8) pEzConfigArray->Radio_GPIO_NIRQ_MODE,
      (U8) pEzConfigArray->Radio_GPIO_SDO_MODE,
      (U8) pEzConfigArray->Radio_GPIO_GEN_CONFIG
  );

  // Put the Radio into SLEEP state
  si4455_change_state(pEzConfigArray->Radio_Mode_After_Power_Up);

  // Read ITs, clear pending ones
  si4455_get_int_status(0u, 0u, 0u);

  // Get part info
  si4455_part_info();//Returns Part Number, Part Version, ROM ID, etc
}

/**
 *  Check if Packet received IT flag is pending.
 *
 *  @return   TRUE - Packet successfully received / FALSE - No packet pending.
 *
 *  @note
 *
 *****************************************************************************/
#if 0
U8 gRadio_CheckReceived(void)
{
  if (RF_NIRQ == FALSE)
  {
    /* Read ITs, clear pending ones */
    si4455_get_int_status(0u, 0u, 0u);

    /* check the reason for the IT */
    if (Si4455Cmd.GET_INT_STATUS.PH_PEND & SI4455_CMD_GET_INT_STATUS_REP_PACKET_RX_PEND_BIT)
    {
      /* Packet RX ,
	    typedef struct
		{
		  U32         ChipID;
		  U8          Flags;          // 4 bit - Buttons 
		  U16         RollingCounter;
		} tRadioPacket;

	  */
//si4455_read_rx_fifo从RX中读取数据,sizeof(tRadioPacket)是接收的字节数,但与si4010的格式不符啊
      si4455_read_rx_fifo(sizeof(tRadioPacket), (U8 *) &RadioPacket);
      
      /* Generate beep,启动TMR3后与中端配合产生蜂鸣 */
      ReceiverDemo_InternalData.ReceiverDemo_BeepLength = 128u;
      StartTimer3(0xD025);  /* ~1kHz,TMR3RL的重载值 */

      return TRUE;
    }

    //Radio_StartRX(0u);
    /* Reset RX FIFO */
    //si4455_fifo_info(0x02);
  }

  return FALSE;
}
#endif

/**
 *  Set Radio to RX mode, fixed packet length.
 *
 *  @param channel Freq. Channel
 *
 *  @note
 *
 *****************************************************************************/
void Radio_StartRX(U8 channel)//
{
  // Read ITs, clear pending ones
  si4455_get_int_status(0u, 0u, 0u);

  /* Start Receiving packet, channel 0, START immediately, Packet size by PH
  1:channel； 2:保留; 3~4:RX_LEN;   ;
  ?channel的作用, 现在的参数中RX_LEN是0
  */
  si4455_start_rx(channel, 0u, 0u,
                  SI4455_CMD_START_RX_ARG_RXTIMEOUT_STATE_ENUM_RX,
                  SI4455_CMD_START_RX_ARG_RXVALID_STATE_ENUM_SLEEP,
                  SI4455_CMD_START_RX_ARG_RXINVALID_STATE_ENUM_RX );
}

/**
 *  Local function to check the loaded EzConfig Array CRC.
 *
 *  @param[in]  crc CRC of EzConfig array.
 *
 *  @return     Result of the command.
 *
 *  @note
 *
 *****************************************************************************/
U8 bRadio_Check_Ezconfig(U16 crc)
{
  si4455_ezconfig_check(crc);

  return radioCmd[0u];
}
