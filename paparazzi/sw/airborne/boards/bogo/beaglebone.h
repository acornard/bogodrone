#ifndef BEAGLEBONE_H
#define BEAGLEBONE_H

/* there is way more MACRO and constant definition in this file than we realy need (at least if we use the linux driver for the i2c IMU) */

/* PWM */
#define BB_EPWM0_BASE	0x48300000 
#define BB_EPWM1_BASE	0x48302000 
#define BB_EPWM2_BASE	0x48304000

//#define BB_EPWM1A_PIN	"lcd_data10"
//#define BB_EPWM1B_PIN	"gpmc_a3"
//#define BB_EPWM2A_PIN	"gpmc_ad8"
//#define BB_EPWM2B_PIN	"gpmc_ad9"

#define BB_EPWM1A_MUX	0x12
#define BB_EPWM1B_MUX	0x16
#define BB_EPWM2A_MUX	0x14
#define BB_EPWM2B_MUX	0x14

#define BB_SYSCLKOUT		100000000ULL	// 100MHz
#define BB_DIV				32
#define BB_PERIOD_NS		20000ULL
#define BB_1S_NS			1000000ULL
#define BB_PERIOD_TICK		(BB_PERIOD_NS * BB_SYSCLKOUT / BB_DIV / BB_1S_NS)
/* #define BB_DUTY(v)			((v) * BB_PERIOD_TICK) / BB_PERIOD_NS */

/* ADC */
#define ADC_VIN		1800	/* mV */
#define ADC_RES		4096
#define SONAR	0	/* AIN0 */
#define BATT	1	/* AIN1 */

#endif	/* BEAGLEBONE_H */
