#ifndef ACTUATORS_BOGO_H_
#define ACTUATORS_BOGO_H_

#include <stdint.h>
#include "beaglebone.h"

#ifndef ACTUATORS_BOGO_NB
#define ACTUATORS_BOGO_NB 4
#endif

#define EPWM_GET(base, off) (*((volatile uint16_t *)((uint8_t *)(base) + off + 0x200)))

#define EPWM_CMPA(base)	EPWM_GET(base, 0x12)
#define EPWM_CMPB(base)	EPWM_GET(base, 0x14)

#define EPWM_SIZE 0x400

#define SERVOS_TICS_OF_USEC(_v) ((_v) * BB_PERIOD_TICK) / BB_PERIOD_NS

#define ActuatorBOGOSet(_i, _v) { actuators_pwm_values[_i] = _v; }
#define ActuatorsBOGOCommit() actuators_bogo_commit();
#define ActuatorsBOGOInit() actuators_bogo_init();

extern uint16_t actuators_pwm_values[ACTUATORS_BOGO_NB];

extern void actuators_bogo_commit(void);
extern void actuators_bogo_init(void);

int actuators_bogo_cmd(uint8_t cmd, uint8_t *reply, int replylen);
void actuators_bogo_set_pwm(uint16_t pwm0, uint16_t pwm1, uint16_t pwm2, uint16_t pwm3);
void actuators_bogo_close(void);

#endif /* ACTUATORS_BOGO_H_ */
