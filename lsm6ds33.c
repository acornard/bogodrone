/**
 * Get information from LSM6
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
#define LSM6_ADDR		  		      0x6A

#define LSM6_RA_CTRL1_XL        0x10
#define LSM6_RA_CTRL2_G         0x11
#define LSM6_RA_CTRL3_C         0x12

#define LSM6_RA_OUTX_L_G        0x22
#define ALTIMU10_RA_WHO_I_AM_ID 0x0F
#define ORIENT_CFG_G            0x0B


typedef struct {
	int16_t gyro_xout;
	int16_t gyro_yout;
	int16_t gyro_zout;
	int16_t accel_xout;
	int16_t accel_yout;
	int16_t accel_zout;
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
	//printf("write([%02X, %02X]) = %d\n", buf[0], buf[1], r);
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
 * Initialize LSM6 using I2C connexion.
 * @return	0 for success, -1 for error.
 */
int altimu10_init(void) {
	
	/* open the I2C file */
	altimu10_fd = open(ALTIMU10_I2C, O_RDWR);
	if(altimu10_fd < 0) {
		printf("cannot connect to ALTIMU10_I2C\n");
		return -1;
	}
	
	/* select slave address to configure LSM6*/
	if(ioctl(altimu10_fd, I2C_SLAVE, LSM6_ADDR) < 0) {
		printf("cannot set LSM6 slave\n");
		return -1;
	}
	
	uint8_t b;
	if(i2c_get_byte(altimu10_fd, ALTIMU10_RA_WHO_I_AM_ID, &b) < 0){
		printf("Cannot read WHO_I_AM\n");
		return -1;
	}
	else{
		if(b!=0x69)
		printf("WHO_I_AM not same %02x\n", b);
	}


	/* set the GYRO configuration */
	if(i2c_set_byte(altimu10_fd, LSM6_RA_CTRL2_G, 0x80) < 0) {
		printf("LSM6: cannot set CTRL2_G\n");
		return -1;
	}


	/* set the ACCELERO configuration */
	if(i2c_set_byte(altimu10_fd, LSM6_RA_CTRL1_XL, 0x80) < 0) {
		printf("LSM6: cannot set CTRL1_XL");
		return -1;
	}


	/* set global configuration configuration */
	if(i2c_set_byte(altimu10_fd, LSM6_RA_CTRL3_C, 0x04) < 0){
		printf("LSM6: cannot set CTRL3_C\n");
		return -1;
	}


	/* change sensor orientation */
	if(i2c_set_byte(altimu10_fd, ORIENT_CFG_G, 0x02) < 0){
		printf("LSM6: cannot set ORIENT_CFG_G\n");
		return -1;
	}
	
	return 0;
}


int altimu10_get_measure(altimu10_measure_t *m) {
	int r = i2c_get_block(altimu10_fd, LSM6_RA_OUTX_L_G, (uint8_t *)m, sizeof(altimu10_measure_t));
	if(r < 0)
		return r;
	m->gyro_xout;
	m->gyro_yout;
	m->gyro_zout;
	m->accel_xout;
	m->accel_yout;
	m->accel_zout;
	return 0;
}



int main(int argc, char **argv) {
	// Init i2c
	int status;
	int xl_flag = 1;
	int g_flag = 1;
	altimu10_measure_t m;
	
	if(argc==2){
		if(!strcmp(argv[1],"-x"))
			g_flag = 0;
		else if(!strcmp(argv[1],"-g"))
			xl_flag = 0;
		else{
			printf("Invalid argument %s", argv[1]);
			return -1;
		}
	}

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
		if(g_flag)
			printf("x_g=%5d \t y_g=%5d \t z_g=%5d\n", m.gyro_xout, m.gyro_yout, m.gyro_zout);

		if(xl_flag)
			printf("x_xl=%5d \t y_xl=%5d \t z_xl=%5d\n", m.accel_xout, m.accel_yout, m.accel_zout);
	}
}
