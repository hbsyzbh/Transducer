/**
 *  Copyright 2008 Silicon Laboratories, Inc.
 *  http://www.silabs.com
 *
 *  @file Radio.h
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

#ifndef RADIO_H_
#define RADIO_H_

/*****************************************************************************
 *  Global Macros & Definitions
 *****************************************************************************/
#define EZCONFIG_ARRAY_SIZE     256u
#define EZCONFIG_RADIO_CFG_SIZE 202u

/*****************************************************************************
 *  Global Typedefs & Enums
 *****************************************************************************/
typedef struct
{
    U8   Radio_Configuration[EZCONFIG_RADIO_CFG_SIZE];
    U16  Radio_Configuration_CRC;

    U8   Radio_BOOT_OPTIONS;
    U8   Radio_XTAL_OPTIONS;
    U32  Radio_XO_FREQ;

    U8   Radio_INT_CTL_ENABLE;
    U8   Radio_INT_CTL_PH_ENABLE;
    U8   Radio_INT_CTL_MODEM_ENABLE;
    U8   Radio_INT_CTL_CHIP_ENABLE;

    U8   Radio_FRR_CTL_A_MODE;
    U8   Radio_FRR_CTL_B_MODE;
    U8   Radio_FRR_CTL_C_MODE;
    U8   Radio_FRR_CTL_D_MODE;

    U8   Radio_SYNC_BITS_31_24;
    U8   Radio_SYNC_BITS_23_16;

    U8   Radio_PA_PWR_LVL;

    U8   Radio_EZCONFIG_MODULATION;
    U8   Radio_EZCONFIG_XO_TUNE;

    U8   Radio_FREQ_CONTROL_INTE;
    U8   Radio_FREQ_CONTROL_FRAC_2;
    U8   Radio_FREQ_CONTROL_FRAC_1;
    U8   Radio_FREQ_CONTROL_FRAC_0;

    U8   Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_1;
    U8   Radio_FREQ_CONTROL_CHANNEL_STEP_SIZE_0;

    U8   Radio_GPIO0_PIN_CFG;
    U8   Radio_GPIO1_PIN_CFG;
    U8   Radio_GPIO2_PIN_CFG;
    U8   Radio_GPIO3_PIN_CFG;
    U8   Radio_GPIO_NIRQ_MODE;
    U8   Radio_GPIO_SDO_MODE;
    U8   Radio_GPIO_GEN_CONFIG;

    U8   Radio_Mode_After_Power_Up;

    U16  Radio_Delay_Cnt_After_Reset;
} tEzConfigArray;

/*****************************************************************************
 *  Global Variable Declarations
 *****************************************************************************/
extern const SEGMENT_VARIABLE(EzConfig_Array_316M66_FSK, tEzConfigArray, SEG_CODE);
extern const SEGMENT_VARIABLE(EzConfig_Array_433M92_FSK, tEzConfigArray, SEG_CODE);
extern const SEGMENT_VARIABLE(EzConfig_Array_868M30_FSK, tEzConfigArray, SEG_CODE);
extern const SEGMENT_VARIABLE(EzConfig_Array_917M00_FSK, tEzConfigArray, SEG_CODE);

extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_316M66_FSK, tEzConfigArray, SEG_CODE, SEG_CODE);
extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_433M92_FSK, tEzConfigArray, SEG_CODE, SEG_CODE);
extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_868M30_FSK, tEzConfigArray, SEG_CODE, SEG_CODE);
extern const SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray_917M00_FSK, tEzConfigArray, SEG_CODE, SEG_CODE);

extern SEGMENT_VARIABLE_SEGMENT_POINTER(pEzConfigArray, tEzConfigArray, SEG_CODE, SEG_XDATA);
/*****************************************************************************
 *  Global Function Declarations
 *****************************************************************************/
void  vRadio_Init(void);
void  vRadio_SetChip();
U8    gRadio_CheckReceived(void);
void  Radio_StartRX(U8);


#endif /* RADIO_H_ */
