// Sensors
sbit left2Sensor at PORTA.B3;	// Sensor -2
sbit left1Sensor at PORTA.B4;	// Sensor -1
sbit middleSensor at PORTA.B7;	// Sensor 0
sbit right1Sensor at PORTA.B0;	// Sensor 1
sbit right2Sensor at PORTA.B1;	// Sensor 2

// Motors
sbit leftMotorForward at PORTB.B3;
sbit leftMotorBackward at PORTB.B2;
sbit rightMotorForward at PORTB.B4;
sbit rightMotorBackward at PORTB.B5;

int on = 1;
int off = 0;

int atStopNumber = 0;

void stopMotors() {
	leftMotorForward = 0;
	leftMotorBackward = 0;
	rightMotorForward = 0;
	rightMotorBackward = 0;
}

void goForward() {
	stopMotors();
	leftMotorForward = 1;
	rightMotorForward = 1;
}

int sensorStatus(int l2S, int l1S, int mS, int r1S, int r2s) {
	if(left2Sensor == l2S && left1Sensor == l1S && middleSensor == mS && right1Sensor == r1S && right2Sensor == r2s) {
		return 1;
	} else {
		return 0;
	}
}

int allSensorsAreOff() {
	return sensorStatus(off, off, off, off, off);
}

void switchToBlackTape() {
	on = 0;
	off = 1;
}

void switchToWhiteTape() {
	on = 1;
	off = 0;
}

void slightLeftTurn() {
	rightMotorForward = 1;
	leftMotorForward = 0;
	delay_ms(1);
	leftMotorForward = 1;
	delay_ms(1);
}

void hardLeftTurn() {
	rightMotorForward = 1;
	leftMotorForward = 0;
}

void slightRightTurn() {
	leftMotorForward = 1;
	rightMotorForward = 0;
	delay_ms(1);
	rightMotorForward = 1;
	delay_ms(1);
}

void hardRightTurn() {
	leftMotorForward = 1;
	rightMotorForward = 0;
}

int whiteLine() {
	stopMotors();
	delay_ms(5);
	atStopNumber = atStopNumber + 1;
	delay_ms(5);
	if(atStopNumber == 1) {
		// Kopru uzerinde bekleme
		stopMotors();
		delay_ms(800);
		goForward();
		delay_ms(1);
		return 0;
	}
	delay_ms(5);
	if(atStopNumber == 2) {
		// Kisayola giris
		goForward();
		delay_ms(10);
		hardRightTurn();
		delay_ms(90);
		goForward();
		delay_ms(100);
		switchToBlackTape();
		delay_ms(10);
		goForward();
		return 0;
	}
	delay_ms(5);
	if(atStopNumber == 3) {
		// Kisayoldan cikis
		switchToWhiteTape();
		delay_ms(10);
		hardRightTurn();
		delay_ms(60);
		goForward();
		return 0;
	}
	delay_ms(5);
	if(atStopNumber == 4) {
		// Kopru sonrasi loop atlama
		hardRightTurn();
		delay_ms(210);
		goForward();
		return 0;
	}
	delay_ms(5);
	if(atStopNumber == 5) {
		// Baslangicta count resetleme
		atStopNumber = 0;
		delay_ms(15);
		goForward();
		return 0;
	}
	goForward();
	return 0;
}

void main() {
	PCON.OSCF = 1;
	CMCON = 0x07;
	TRISA = 0x9B; // Arrange A1, A0, A7, A4, A3 port as input, rest as output
	TRISB = 0x00; // Arrange B Ports as output
	PORTB = 0x00; // Giving values

	delay_ms(500);

	goForward();

	while(1) {
		if(sensorStatus(on, on, on, on, on) == 1) {
			whiteLine();
		}

		if(middleSensor == off && left1Sensor == on && left2Sensor == off) {
			while(left1Sensor == on && sensorStatus(on, on, on, on, on) == 0) {
				slightLeftTurn();
			}
		}

		if(sensorStatus(on, on, on, on, on) == 1) {
			whiteLine();
		}

		if(middleSensor == off && right1Sensor == on && right2Sensor == off) {
			while(right1Sensor == on && sensorStatus(on, on, on, on, on) == 0) {
				slightRightTurn();
			}
		}

		if(sensorStatus(on, on, on, on, on) == 1) {
			whiteLine();
		}

		if(left2Sensor == on && middleSensor == off) {
			while(middleSensor == off && sensorStatus(on, on, on, on, on) == 0) {
				hardLeftTurn();
			}
			leftMotorForward = 1;
		}

		if(sensorStatus(on, on, on, on, on) == 1) {
			whiteLine();
		}

		if(middleSensor == off && right2Sensor == on) {
			while(middleSensor == off && sensorStatus(on, on, on, on, on) == 0) {
				hardRightTurn();
			}
			rightMotorForward = 1;
		}
	}
}