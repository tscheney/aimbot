import struct
import serial


class MotorController:
    """This class interfaces with the motors via the serial connection to the PSoC"""
    def __init__(self, p=1, i=1, qpps=500):
        self.ser = serial.Serial()
        self.serial_conn = False
        self.PULSE_PER_ROTATION = 116.2
        self.PIQ = (p, i, qpps)
        self.init_serial()
        self.setPID(0, p, i, qpps)
        #self.setPID(2, p, i, qpps)
        #self.setPID(3, p, i, qpps/2)
        self.setSpeed(0,0,0)
        self.setT(20, 50)

    def init_serial(self):
        """Init the serial connection.  If the process fails, the self.serial_conn is set to false and no
        other method in the class with run to prevent errors"""
        try:
            self.ser = serial.Serial('/dev/ttyAMA0', 115200, timeout=None)
            self.serial_conn = True
        except serial.SerialException:
            self.serial_conn = False
            print("Failed to start serial")

    def writeFloat(self, f):
        if self.serial_conn:
            self.ser.write(struct.pack('>i', int(f * 1000)))

    def readFloat(self):
        if self.serial_conn:
            return float(struct.unpack('>i', self.ser.read(4))[0]) / 1000

    def setPower(self, p1, p2, p3):
        if self.serial_conn:
            self.ser.write('p')
            self.writeFloat(p1)
            self.writeFloat(p2)
            self.writeFloat(p3)

    def setSpeed(self, s1, s2, s3):
        """Sends the speed command in qpps = (rotations per second)*PULSE_PER_ROTATION to the motor"""
        if self.serial_conn:
            self.ser.write('s')
            #TODO need minimum push?
            #if (abs(s1) < 1.5 * self.PULSE_PER_ROTATION):
            #    s1 = math.copysign(1.5 * self.PULSE_PER_ROTATION, s1)
            #if (abs(s2) < 1.5 * self.PULSE_PER_ROTATION):
            #    s2 = math.copysign(1.5 * self.PULSE_PER_ROTATION, s2)
            #if (abs(s3) < 1.5 * self.PULSE_PER_ROTATION):
            #    s3 = math.copysign(1.5 * self.PULSE_PER_ROTATION, s3)

            # reset = False
            # if( s1 < 200):
            #     self.setPID(1, 1, 1, 150)
            #     reset = True
            #
            # if( s2 < 200):
            #     self.setPID(2, 1, 1, 150/2)
            #     reset = True
            #
            # if (s3 < 200):
            #     self.setPID(3, 1, 1, 150)
            #     reset = True
            #
            # if (s1 < 100):
            #     self.setPID(1, 1, 1, 50)
            #     reset = True
            #
            # if (s2 < 100):
            #     self.setPID(2, 1, 1, 50/2)
            #     reset = True
            #
            # if (s3 < 100):
            #     self.setPID(3, 1, 1, 50)
            #     reset = True

            self.writeFloat(s1)
            self.writeFloat(s2)
            self.writeFloat(s3)

            # if reset:
            #     self.setPID(1, self.PIQ[0], self.PIQ[1], self.PIQ[2])
            #     self.setPID(2, self.PIQ[0], self.PIQ[1], self.PIQ[2]/2)
            #     self.setPID(3, self.PIQ[0], self.PIQ[1], self.PIQ[2])

    def setPID(self, motor, p, i, qpps):  #
        """Sets the PI values as well as the qpps value, use motor = 0 to set all motors"""
        if self.serial_conn:
            self.ser.write('k')
            self.ser.write(str(motor))
            self.writeFloat(p)
            self.writeFloat(i)
            self.writeFloat(qpps)

    def setT(self, period_ms, tau_ms):
        """Set tick period (triggers PID control) and velocity filter corner frequency"""
        if self.serial_conn:
            self.ser.write('t')
            self.writeFloat(period_ms)
            self.writeFloat(tau_ms)

    def getSpeed(self):
        if self.serial_conn:
            self.ser.write('v')
            return self.readFloat(), self.readFloat(), self.readFloat()

    def getEncoderCount(self):
        if self.serial_conn:
            self.ser.write('e')
            return self.readFloat(), self.readFloat(), self.readFloat()

    def disengage(self):
        if self.serial_conn:
            self.ser.write('d')
