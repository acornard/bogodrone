/**
 * Get information from LPS25H
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <math.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define ALTIMU10_I2C	  	      "/dev/i2c-2"
#define LPS25H_ADDR		  		    0x5D

#define LPS25H_CTRL1            0x20

#define LPS25H_P_OUTX_XL           0x28
#define LPS25H_RA_WHO_I_AM_ID      0x0F

static int altimu10_fd;


/**
 * Get a byte register.
 * @param fd	I2C file descriptor.
 * @param ra	Register address.
 * @param ptr	Pointer to store the byre to.
 * @return		0 for success, -1 for error.
 */
int i2c_get_byte(int fd, uint8_t ra, uint8_t *ptr) {
	uint8_t buf[] = { ra };
	if(write(fd, buf, sizeof(buf)) != sizeof(buf))
		return -1;	
	if(read(fd, ptr, sizeof(uint8_t)) != sizeof(uint8_t))
		return -1;
	else
		return 0;
}


/**
 * Set a register value wit a byte.
 * @param fd	I2C file descriptor.
 * @param ra	Regsiter address.
 * @param val	Value to set.
 * @return		0 for success, -1 error.
 */
int i2c_set_byte(int fd, uint8_t ra, uint8_t val) {
	uint8_t buf[] = { ra, val };
	int r = write(fd, buf, sizeof(buf));
	if(r != sizeof(buf))
		return -1;
	else
		return 0;
}


/**
 * Get a block of bytes.
 * @param fd	I2C file descriptor.
 * @param ra	Register address.
 * @param size	Size  of read block.
 * @param ptr	Pointer to store the bytes to.
 * @return		0 for success, -1 for error.
 */
int i2c_get_block(int fd, uint8_t ra, uint8_t *ptr, int size) {
	uint8_t buf[] = { ra };
	if(write(fd, buf, sizeof(buf)) != sizeof(buf))
		return -1;	
	if(read(fd, ptr, size) != size)
		return -1;
	else
		return 0;
}


/**
 * Initialize LIS3MDL using I2C connexion.
 * @return	0 for success, -1 for error.
 */
int altimu10_init(void) {
	
	/* open the I2C file */
	altimu10_fd = open(ALTIMU10_I2C, O_RDWR);
	if(altimu10_fd < 0) {
		printf("cannot connect to ALTIMU10_I2C\n");
		return -1;
	}
	
	/* select slave address to configure LIS3MDL*/
	if(ioctl(altimu10_fd, I2C_SLAVE, LPS25H_ADDR) < 0) {
		printf("cannot set LIS3MDL slave\n");
		return -1;
	}
	
	uint8_t b;
	if(i2c_get_byte(altimu10_fd, LPS25H_RA_WHO_I_AM_ID, &b) < 0){
		printf("Cannot read WHO_I_AM\n");
		return -1;
	}
	else{
		if(b!=0xBD)
		printf("WHO_I_AM not same %02x\n", b);
	}


	if(i2c_set_byte(altimu10_fd, LPS25H_CTRL1, 0xB0) < 0) {
		printf("Cannot set CTRL1\n");
		return -1;
	}

	return 0;
}


int32_t altimu10_get_measure(altimu10_measure_t *m) {
	uint8_t pxl, pl, ph;
	int r = i2c_get_block(altimu10_fd, LPS25H_P_OUTX_XL, &pxl, 1);
	if(r < 0)
		return r;
	r = i2c_get_block(altimu10_fd, LPS25H_P_OUTX_XL+1, &pl, 1);
	if(r < 0)
		return r;
	r = i2c_get_block(altimu10_fd, LPS25H_P_OUTX_XL+2, &ph, 1);
	if(r < 0)
		return r;
	
	return (int32_t)(int8_t) ph <<16 | (uint16_t) pl << 8 | pxl;
}


float pressureToAltMeters(float pressure_mbar, float altimeter_setting_mbar){
	return (1-pow((pressure_mbar/altimeter_setting_mbar), 0.190263)) * 4430.8;
}

int main(int argc, char **argv) {
	// Init i2c
	int status;
	altimu10_measure_t m;
	

	status = altimu10_init();
	if(status<0){
		printf("Error during initialization\n");
		return -1;
	}

	// Get metrics 
	while(1){
		int32_t pressure =  altimu10_get_measure(&m);
		if(pressure < 0){
			printf("cannot get metrics\n");
			return -1;
		}
		float pressureMBa = (float) pressure /4096;
		float meter = pressureToAltMeters(pressureMBa, 1013.25);
		printf("Pression: %f\t Altitude: %f\t\n", pressureMBa, meter);
	}
}
