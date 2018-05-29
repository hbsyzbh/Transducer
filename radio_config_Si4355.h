/*! @file radio_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.11.0
 * @n Device: Si4355 Rev.: C2                                 
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2017 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef RADIO_CONFIG_H_
#define RADIO_CONFIG_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 30000000    Crys_tol(ppm): 90    IF_mode: 2    High_perf_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 0    
// MOD_type: 2    Rsymb(sps): 2400    Fdev(Hz): 30000    RXBW(Hz): 185000    Manchester: 0    AFC_en: 1    Rsymb_error: 0.0    Chip-Version: 2    
// RF Freq.(MHz): 915    API_TC: 28    fhst: 250000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 0    
// API_ARR_Det_en: 0    Fdev_error: 0    API_ETSI: 0    
// 
// # WB filter 2 (BW = 412.25 kHz);  NB-filter 2 (BW = 412.25 kHz) 

// 
// Modulation index: 25
*/


// CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ                     30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER                    0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH               0x07
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP        0x03
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       0xF000

//#include "..\drivers\radio\Si4455\si4455_patch.h"


// CONFIGURATION COMMANDS

/*
// Command:                  RF_POWER_UP
// Description:              Command to power-up the device and select the operational mode and functionality.
*/
#define RF_POWER_UP 0x02, 0x81, 0x00, 0x01, 0xC9, 0xC3, 0x80

/*
// Set properties:           RF_INT_CTL_ENABLE_1
// Number of properties:     1
// Group ID:                 0x01
// Start ID:                 0x00
// Default values:           0x04, 
// Descriptions:
//   INT_CTL_ENABLE - This property provides for global enabling of the three interrupt groups (Chip, Modem and Packet Handler) in order to generate HW interrupts at the NIRQ pin.
*/
#define RF_INT_CTL_ENABLE_1 0x11, 0x01, 0x01, 0x00, 0x00

/*
// Set properties:           RF_FRR_CTL_A_MODE_4
// Number of properties:     4
// Group ID:                 0x02
// Start ID:                 0x00
// Default values:           0x01, 0x02, 0x09, 0x00, 
// Descriptions:
//   FRR_CTL_A_MODE - Fast Response Register A Configuration.
//   FRR_CTL_B_MODE - Fast Response Register B Configuration.
//   FRR_CTL_C_MODE - Fast Response Register C Configuration.
//   FRR_CTL_D_MODE - Fast Response Register D Configuration.
*/
#define RF_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x08, 0x06, 0x04, 0x0A

/*
// Set properties:           RF_EZCONFIG_XO_TUNE_1
// Number of properties:     1
// Group ID:                 0x24
// Start ID:                 0x03
// Default values:           0x40, 
// Descriptions:
//   EZCONFIG_XO_TUNE - Configure the internal capacitor frequency tuning bank for the crystal oscillator.
*/
#define RF_EZCONFIG_XO_TUNE_1 0x11, 0x24, 0x01, 0x03, 0x52

/*
// Command:                  RF_EZCONFIG_ARRAY_WRITE
// Description:              Writes data byte(s) to the EZConfig array.
*/
#define RF_EZCONFIG_ARRAY_WRITE 0x66, 0xDC, 0xCF, 0x2B, 0xC7, 0xE7, 0xFC, 0x5A, 0xD4, 0xD3, 0x13, 0xA9, 0x81, 0x2A, 0x43, 0xA4, 0xF0, 0x09, 0x13, 0x7B, \
0x93, 0x42, 0x2E, 0x09, 0xBB, 0x67, 0x4C, 0xD0, 0xE1, 0x5E, 0xA8, 0x1E, 0x94, 0xCF, 0xDB, 0x8F, 0x5D, 0x58, 0x3F, 0x6B, \
0x73, 0x8A, 0x69, 0x68, 0x2F, 0xA9, 0xCB, 0xA7, 0xC6, 0x8E, 0xB4, 0x33, 0x92, 0xA0, 0x6C, 0x0D, 0x2E, 0xE0, 0x01, 0xE4, \
0x18, 0xFA, 0x0B, 0x16, 0x4C, 0xE5, 0x8B, 0xBE, 0xE8, 0xF0, 0x55, 0xD0, 0xB2, 0xEC, 0xC3, 0x4B, 0xD1, 0x54, 0xE3, 0x9E, \
0xA9, 0xB5, 0x4C, 0x3C, 0x83, 0x06, 0x45, 0xA5, 0xE5, 0x60, 0x3B, 0x0E, 0x29, 0x14, 0x67, 0x52, 0xA3, 0x51, 0x44, 0xC6, \
0x79, 0x7B, 0x73, 0x6F, 0xC6, 0x14, 0x11, 0x47, 0x5A, 0x4C, 0xBB, 0x2B, 0x22, 0x92

/*
// Command:                  RF_NOP
// Description:              No Operation command.
*/
#define RF_NOP 0x00

/*
// Command:                  RF_EZCONFIG_ARRAY_WRITE_1
// Description:              Writes data byte(s) to the EZConfig array.
*/
#define RF_EZCONFIG_ARRAY_WRITE_1 0x66, 0x15, 0xB0, 0x17, 0xBA, 0x3C, 0x32, 0xF6, 0x12, 0x77, 0x66, 0x3A, 0x41, 0x1F, 0xED, 0xF3, 0x8F, 0x1E, 0xCE, 0x2E, \
0xB2, 0xAA, 0x75, 0x8D, 0xC8, 0xAC, 0x17, 0x1B, 0xF3, 0x69, 0x60, 0xDD, 0x58, 0x07, 0xF8, 0xED, 0x1F, 0x69, 0x19, 0x9C, \
0x15, 0xF8, 0x44, 0xAE, 0x87, 0xFE, 0x34, 0x16, 0x0E, 0x6B, 0xDC, 0xB5, 0x81, 0xA7, 0xFF, 0x75, 0xCE, 0x91, 0xEA, 0xBA, \
0x18, 0xC1, 0xFD, 0x21, 0x3F, 0x49, 0x1F, 0x57, 0x41, 0x70, 0x8E, 0xAF, 0x34, 0x43, 0x1A, 0x76, 0x90, 0x59, 0x9B, 0x26, \
0x76, 0xE1, 0x56, 0x1D, 0x4E, 0xEE, 0xE4, 0x99, 0xF0, 0x72, 0x28, 0x61, 0x6F, 0x73, 0xAF, 0x6C, 0x11, 0x06, 0x1C, 0xF5, \
0x88, 0x04, 0xF3, 0x8F, 0xF8, 0x39, 0xA2, 0xB3, 0x96, 0x57, 0xC1, 0xCE

/*
// Command:                  RF_EZCONFIG_CHECK
// Description:              Validates the EZConfig array was written correctly.
*/
#define RF_EZCONFIG_CHECK 0x19, 0x6E, 0x7A

/*
// Command:                  RF_GPIO_PIN_CFG
// Description:              Configures the GPIO pins.
*/
#define RF_GPIO_PIN_CFG 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00


// AUTOMATICALLY GENERATED CODE! 
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------

#ifndef FIRMWARE_LOAD_COMPILE
#define RADIO_CONFIGURATION_DATA_ARRAY { \
        SI446X_PATCH_CMDS, \
        0x07, RF_POWER_UP, \
        0x05, RF_INT_CTL_ENABLE_1, \
        0x08, RF_FRR_CTL_A_MODE_4, \
        0x05, RF_EZCONFIG_XO_TUNE_1, \
        0x72, RF_EZCONFIG_ARRAY_WRITE, \
        0x01, RF_NOP, \
        0x70, RF_EZCONFIG_ARRAY_WRITE_1, \
        0x03, RF_EZCONFIG_CHECK, \
        0x08, RF_GPIO_PIN_CFG, \
        0x00 \
 }
#else
#define RADIO_CONFIGURATION_DATA_ARRAY { 0 }
#endif

// DEFAULT VALUES FOR CONFIGURATION PARAMETERS
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT                     30000000L
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT                    0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT               0x10
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT        0x01
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT       0x1000

#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_INCLUDED                      0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH_SIZE                          0x00
#define RADIO_CONFIGURATION_DATA_RADIO_PATCH                               {  }

#ifndef RADIO_CONFIGURATION_DATA_ARRAY
#error "This property must be defined!"
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ
#define RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ          RADIO_CONFIGURATION_DATA_RADIO_XO_FREQ_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER
#define RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER         RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH
#define RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH    RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP
#define RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP   RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP_DEFAULT 
#endif

#ifndef RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET
#define RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET  RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET_DEFAULT 
#endif

#define RADIO_CONFIGURATION_DATA { \
                            Radio_Configuration_Data_Array,                            \
                            RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,                   \
                            RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH,              \
                            RADIO_CONFIGURATION_DATA_RADIO_STATE_AFTER_POWER_UP,       \
                            RADIO_CONFIGURATION_DATA_RADIO_DELAY_CNT_AFTER_RESET       \
                            }

#endif /* RADIO_CONFIG_H_ */
