#include "12c_driver.h"

static const char *DevName_v2 = "/dev/i2c-1";

static const char *DevName_v1 = "/dev/i2c-0";

void sendBuffer(uint8_t * buffer, uint8_t size, uint8_t address)
{
	int file;

	if ((file = open(DevName_v2, O_RDWR)) < 0)
  		/*Could not access device*/
	else if(ioctl(file, I2C_SLAVE, address) < 0)
  		/*Could not aquire bus access*/
	else
		write(file, buffer, size);

	close(file);
}

void receiveBuffer(uint8_t * buffer, uint8_t size)
{
	if ((file = open(devName, O_RDWR)) < 0)
  		/*Could not access device*/
	else if(ioctl(file, I2C_SLAVE, address) < 0)
  		/*Could not aquire bus access*/
	else
		read(file, buffer, size);

	close(file);
}

