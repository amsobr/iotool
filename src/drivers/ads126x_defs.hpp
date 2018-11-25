#ifndef ADS126X_HPP
#define ADS126X_HPP

typedef enum {
    ADS126X_REG_ID          = 0x00 ,
    ADS126X_REG_POWER       = 0x01 ,
    ADS126X_REG_INTERFACE   = 0x02 ,
    ADS126X_REG_MODE0       = 0x03 ,
    ADS126X_REG_MODE1       = 0x04 ,
    ADS126X_REG_MODE2       = 0x05 ,
    ADS126X_REG_INPMUX      = 0x06 ,
    ADS126X_REG_OFCAL0      = 0x07 ,
    ADS126X_REG_OFCAL1      = 0x08 ,
    ADS126X_REG_OFCAL2      = 0x09 ,
    ADS126X_REG_FSCAL0      = 0x0a ,
    ADS126X_REG_FSCAL1      = 0x0b ,
    ADS126X_REG_FSCAL2      = 0x0c ,
    ADS126X_REG_IDACMUX     = 0x0d ,
    ADS126X_REG_IDACMAG     = 0x0e ,
    ADS126X_REG_REFMUX      = 0x0f ,
    ADS126X_REG_TDACP       = 0x10 ,
    ADS126X_REG_TDACN       = 0x11 ,
    ADS126X_REG_GPIOCON     = 0x12 ,
    ADS126X_REG_GPIODIR     = 0x13 ,
    ADS126X_REG_GPIODAT     = 0x14 ,
    ADS126X_REG_ADC2CFG     = 0x15 ,
    ADS126X_REG_ADC2MUX     = 0x16 ,
    ADS126X_REG_ADS2OFC0    = 0x17 ,
    ADS126X_REG_ADC2OFC1    = 0x18 ,
    ADS126X_REG_ADC2FSC0    = 0x19 ,
    ADS126X_REG_ADC2FSC1    = 0x1a ,
} Ads126xRegister;

typedef enum {
    ADS126X_CMD_NOP         = 0x00 ,
    ADS126X_CMD_RESET       = 0x06 ,
    ADS126X_CMD_START1      = 0x08 ,
    ADS126X_CMD_STOP1       = 0x0a ,
    ADS126X_CMD_START2      = 0x0c ,
    ADS126X_CMD_STOP2       = 0x0e ,
    ADS126X_CMD_RDATA1      = 0x12 ,
    ADS126X_CMD_RDATA2      = 0x14 ,
    ADS126X_CMD_SYOCAL1     = 0x16 ,
    ADS126X_CMD_SYGCAL1     = 0x17 ,
    ADS126X_CMD_SFOCAL1     = 0x19 ,
    ADS126X_CMD_SYOCAL2     = 0x1b ,
    ADS126X_CMD_SYGCAL2     = 0x1c ,
    ADS126X_CMD_SFOCAL2     = 0x1e ,
    ADS126X_CMD_RREG        = 0x20 ,
    ADS126X_CMD_WREG        = 0x40 ,
} Ads126xCommand;

typedef enum {
    ADS126X_GAIN_1          = 0x00 ,
    ADS126X_GAIN_2          = 0x01 ,
    ADS126X_GAIN_4          = 0x02 ,
    ADS126X_GAIN_8          = 0x03 ,
    ADS126X_GAIN_16         = 0x04 ,
    ADS126X_GAIN_32         = 0x05 ,
} Ads126xGain;


enum {
    ADS126X_INTERFACE_STATUS_INCLUDED   = 0x04 ,
    ADS126X_INTERFACE_CKSUM             = 0x01 ,
    ADS126X_INTERFACE_CRC               = 0x10 ,

    ADS126X_POWER_VBIAS_OFF             = 0x00 ,
    ADS126X_POWER_VBIAS_ON              = 0x02 ,
    ADS126X_POWER_INTREF_ON             = 0x01 ,
};




#endif /* !defined(ADS126X_DEFS_HPP) */
