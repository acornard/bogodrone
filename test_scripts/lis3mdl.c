/**
 * Get information from LIS3MDL
 */

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#define ALTIMU10_I2C	  	      "/dev/i2c-2"
#define LIS3MDL_ADDR		  		  0x1E

#define LIS3MDL_CTRL1           0x20
#define LIS3MDL_CTRL2           0x21
#define LIS3MDL_CTRL3           0x22
#define LIS3MDL_CTRL4           0x23

#define LIS3MDL_RA_OUTX_L           0x28
#define LIS3MDL_RA_WHO_I_AM_ID      0x0F


typedef struct {
	int16_t xout;
	int16_t yout;
	int16_t zout;
} altimu10_measure_t;

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
	if(ioctl(altimu10_fd, I2C_SLAVE, LIS3MDL_ADDR) < 0) {
		printf("cannot set LIS3MDL slave\n");
		return -1;
	}
	
	uint8_t b;
	if(i2c_get_byte(altimu10_fd, LIS3MDL_RA_WHO_I_AM_ID, &b) < 0){
		printf("Cannot read WHO_I_AM\n");
		return -1;
	}
	else{
		if(b!=0x3D)
		printf("WHO_I_AM not same %02x\n", b);
	}


	if(i2c_set_byte(altimu10_fd, LIS3MDL_CTRL1, 0x70) < 0) {
		printf("Cannot set CTRL1\n");
		return -1;
	}


	if(i2c_set_byte(altimu10_fd, LIS3MDL_CTRL2, 0x00) < 0) {
		printf("Cannot set CTRL2\n");
		return -1;
	}


	if(i2c_set_byte(altimu10_fd, LIS3MDL_CTRL3, 0x00) < 0){
		printf("Cannot set CTRL3\n");
		return -1;
	}


	if(i2c_set_byte(altimu10_fd, LIS3MDL_CTRL4, 0x0C) < 0){
		printf("Cannot set CTRL4\n");
		return -1;
	}
	
	return 0;
}


int altimu10_get_measure(altimu10_measure_t *m) {
	int r = i2c_get_block(altimu10_fd, LIS3MDL_RA_OUTX_L, (uint8_t *)m, sizeof(altimu10_measure_t));
	if(r < 0)
		return r;
	m->xout;
	m->yout;
	m->zout;
	return 0;
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
		status =  altimu10_get_measure(&m);
		if(status < 0){
			printf("cannot get metrics\n");
			return -1;
		}
		printf("x=%5d \t y=%5d \t z=%5d\n", m.xout, m.yout, m.zout);
	}
}
