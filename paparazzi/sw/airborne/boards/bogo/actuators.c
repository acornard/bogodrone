#include "subsystems/actuators.h"
#include "actuators.h"

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdint.h>
#include <sys/mman.h> /* memory management (mmap) */

uint16_t actuators_pwm_values[ACTUATORS_BOGO_NB];


// PWM bases (global variables like evrybody likes)
static void *epwm1 = NULL;
static void *epwm2 = NULL;

static int mem_fd = -1;

/**
 * Set speed for engine backward-left.
 * @param v		Engine speed in ns (between 1100 and 1900).
 */
void pwm_set_backward_left(uint32_t v);

/**
 * Set speed for engine forward-left.
 * @param v		Engine speed in ns (between 1100 and 1900).
 */
void pwm_set_forward_left(uint32_t v);

/**
 * Set speed for engine forward-right.
 * @param v		Engine speed in ns (between 1100 and 1900).
 */
void pwm_set_forward_right(uint32_t v);


/**
 * Set speed for engine backward-right.
 * @param v		Engine speed in ns (between 1100 and 1900).
 */
void pwm_set_backward_right(uint32_t v);

/**
 * Initialize PWM module.
 * @param mem	Memory FD.
 * @return		0 for success, -1 else.
 */
void pwm_init(int mem){

#	ifdef NO_ENGINE
		printf("NO_ENGINE\n");
		return 0;
#	else

	// initialize PWM1
	epwm1 = mmap(NULL, EPWM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem, BB_EPWM1_BASE);
	if(epwm1 == (void *) -1) {
		error("cannot map EPWM1");
		return;
	}
	
	// initialize PWM2
	epwm2 = mmap(NULL, EPWM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem, BB_EPWM2_BASE);
	if(epwm2 == (void *) -1)
		error("cannot map EPWM2");
	printf("PWM_INIT DONE\n");
#	endif
}

void actuators_bogo_init(void){
#	ifdef NO_ENGINE
	printf("NO_ENGINE\n");
	return;
#	else
	mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (mem_fd < 0) {
		error("cannot open /dev/mem");
		return;
	}
	pwm_init(mem_fd);
	printf("ACTUATOR INIT DONE\n");
#	endif
}

int actuators_bogo_cmd(uint8_t cmd, uint8_t *reply, int replylen){
	//printf("CMD");
	return 0;
}

void actuators_bogo_commit(void){
#ifndef NO_ENGINE
//    printf("0 NW: %d | 1 NE: %d | 2 SE: %d | 3 SW: %d\n", actuators_pwm_values[0], actuators_pwm_values[1], actuators_pwm_values[2], actuators_pwm_values[3]);
    actuators_bogo_set_pwm(actuators_pwm_values[0],actuators_pwm_values[1],actuators_pwm_values[2],actuators_pwm_values[3]);
#endif
}

void actuators_bogo_set_pwm(uint16_t pwm0, uint16_t pwm1, uint16_t pwm2, uint16_t pwm3){
//    pwm_set_backward_left(pwm0);
//    pwm_set_forward_left(pwm1);
//    pwm_set_forward_right(pwm2);
//    pwm_set_backward_right(pwm3);

    /**/

	pwm_set_backward_right(pwm0);
	pwm_set_backward_left(pwm1);
	pwm_set_forward_left(pwm2);
	pwm_set_forward_right(pwm3);
}

void actuators_bogo_close(void){
	printf("CLOSE");
	close(mem_fd);
}


void pwm_set_forward_left(uint32_t v){
#	ifdef PWM_VERBOSE
		printf("PWM: %04d\t", v);
#	endif
#	ifndef NO_ENGINE
		EPWM_CMPA(epwm2) = SERVOS_TICS_OF_USEC(v);
#	endif
}


void pwm_set_backward_left(uint32_t v){
#	ifdef PWM_VERBOSE
		printf("%04d\t", v);
#	endif
#	ifndef NO_ENGINE
		EPWM_CMPB(epwm2) = SERVOS_TICS_OF_USEC(v);
#	endif
}


void pwm_set_forward_right(uint32_t v){
#	ifdef PWM_VERBOSE
		printf("%04d\t", v);
#	endif
#	ifndef NO_ENGINE
		EPWM_CMPA(epwm1) = SERVOS_TICS_OF_USEC(v);
#	endif
}

void pwm_set_backward_right(uint32_t v){
#	ifdef PWM_VERBOSE
		printf("%04d\n", v);
#	endif
#	ifndef NO_ENGINE
		EPWM_CMPB(epwm1) = SERVOS_TICS_OF_USEC(v);
#	endif
}
