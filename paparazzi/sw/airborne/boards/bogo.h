#ifndef CONFIG_BOGO
#define CONFIG_BOGO

#define BOARD_BOGO

#define UART0_DEV /dev/ttyO0
#define UART1_DEV /dev/ttyO1
#define UART2_DEV /dev/ttyO2
#define UART3_DEV /dev/ttyO3
#define UART4_DEV /dev/ttyO4
#define UART5_DEV /dev/ttyO5

#define DEFAULT_ACTUATORS "boards/bogo/actuators.h"
#define ActuatorDefaultSet(_x,_y) ActuatorBOGOSet(_x,_y)
#define ActuatorsDefaultInit() ActuatorsBOGOInit()
#define ActuatorsDefaultCommit() ActuatorsBOGOCommit()

#endif /* CONFIG_BOGO */
