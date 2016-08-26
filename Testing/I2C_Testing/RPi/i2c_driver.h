#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

void sendBuffer(uint8_t * buffer, uint8_t size);

void receiveBuffer(uint8_t * buffer);

#endif