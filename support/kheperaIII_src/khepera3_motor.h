/*!
 * (c) 2006 - 2008 EPFL, Lausanne, Switzerland
 * Thomas Lochmatter
 */

#ifndef KHEPERA3_MOTOR
#define KHEPERA3_MOTOR

#include "i2cal.h"

//! Motor status flags
enum eKhepera3MotorStatusFlags {
	cKhepera3MotorStatusFlags_Moving = (1 << 0),					//!< Movement detected
	cKhepera3MotorStatusFlags_Direction = (1 << 1),					//!< Direction (0 = negative, 1 = positive)
	cKhepera3MotorStatusFlags_OnSetpoint = (1 << 2),				//!< On setpoint
	cKhepera3MotorStatusFlags_NearSetpoint = (1 << 3),				//!< Near setpoint
	cKhepera3MotorStatusFlags_CommandSaturated = (1 << 4),			//!< Command saturated
	cKhepera3MotorStatusFlags_AntiResetWindup = (1 << 5),			//!< Antireset windup active
	cKhepera3MotorStatusFlags_SoftwareCurrentControl = (1 << 6),	//!< Software current control active
	cKhepera3MotorStatusFlags_SoftStop = (1 << 7),					//!< Softstop active
};

//! Motor error flags
enum eKhepera3MotorErrorFlags {
	cKhepera3MotorErrorFlags_SampleTimeTooSmall = (1 << 0),	//!< Sample time too small
	cKhepera3MotorErrorFlags_WatchdogOverflow = (1 << 1),	//!< Watchdog timer overflow
	cKhepera3MotorErrorFlags_BrownOut = (1 << 2),			//!< Brown-out
	cKhepera3MotorErrorFlags_SoftwareStop = (1 << 3),		//!< Software stopped motor (if softstop enabled)
	cKhepera3MotorErrorFlags_MotorBlocked = (1 << 4),		//!< Motor blocked (if motor blocking enabled)
	cKhepera3MotorErrorFlags_PositionOutOfRange = (1 << 5),	//!< Position out of range
	cKhepera3MotorErrorFlags_SpeedOutOfRange = (1 << 6),	//!< Speed out of range
	cKhepera3MotorErrorFlags_TorqueOutOfRange = (1 << 7),	//!< Torque out of range
};

//! Motor software options
enum eKhepera3MotorSoftwareOptions {
	cKhepera3MotorSoftwareOptions_AlternatePID = (1 << 0),					//!< Use alternate algorithm for the derivative input of the PID. (The derivate part is calculated using the process variable rather than the error.)
	cKhepera3MotorSoftwareOptions_AntiResetWindup = (1 << 1),				//!< Activate the anti reset windup routine
	cKhepera3MotorSoftwareOptions_StopAtMinPosition = (1 << 2),				//!< Stop the motor if the min position is reached
	cKhepera3MotorSoftwareOptions_StopAtMaxPosition = (1 << 3),				//!< Stop the motor if the max position is reached
	cKhepera3MotorSoftwareOptions_ErrorAtPositionLimits = (1 << 4),			//!< Generate an error when position is out of limits. (In this case the error must be reset before any further commands can be executed.)
	cKhepera3MotorSoftwareOptions_StopIfBlocked = (1 << 5),					//!< Stop the motor if the blocked condition is met
	//cKhepera3MotorSoftwareOptions_SoftwareCurrentLimitation = (1 << 6),		//!< Activate software current limitation (not implemented)
	cKhepera3MotorSoftwareOptions_InvertMotorDirection = (1 << 7),			//!< Invert the motor direction
};

//! Motor hardware options
enum eKhepera3MotorHardwareOptions {
	cKhepera3MotorHardwareOptions_StartupInControlMode = (1 << 0),			//!< Startup mode (0 = idle mode, 1 = control mode)
	//cKhepera3MotorHardwareOptions_AnalogSetpointInput = (1 << 1),			//!< Use analog input for setpoint (not implemented)
	//cKhepera3MotorHardwareOptions_Reserved2 = (1 << 2),					//!< Reserved
	cKhepera3MotorHardwareOptions_EncoderResolution = (1 << 3),				//!< Resolution for the encoder (0 = 100%, 1 = 25%)
	cKhepera3MotorHardwareOptions_InvertCurrentMeasurement = (1 << 4),		//!< Invert the internal current measurement
	//cKhepera3MotorHardwareOptions_Reserved5 = (1 << 5),					//!< Reserved
	//cKhepera3MotorHardwareOptions_Reserved6 = (1 << 6),					//!< Reserved
	//cKhepera3MotorHardwareOptions_Reserved7 = (1 << 7),					//!< Reserved
};

//! Motor modes
enum eKhepera3MotorMode {
	cKhepera3MotorMode_Idle			= 0,	//!< Idle
	cKhepera3MotorMode_Normal		= 1,	//!< Normal Control Mode
	cKhepera3MotorMode_StopMotor	= 2,	//!< Stop Motor
	cKhepera3MotorMode_Sleep		= 3,	//!< Sleep
	cKhepera3MotorMode_Reset		= 4,	//!< Reset
	cKhepera3MotorMode_SaveE2PROM	= 5,	//!< Save Config Param in E2PROM
	cKhepera3MotorMode_SearchLimit	= 6		//!< Search Limits
};


//! Motor setpoint sources
enum eKhepera3MotorSetpointSource {
	cKhepera3MotorSetpointSource_I2C		= 0,	//!< External I2C
	//cKhepera3MotorSetpointSource_Analog		= 1,	//!< External Analog (not implemented)
	cKhepera3MotorSetpointSource_SquareWave	= 2,	//!< Internal Square Wave Generator
	cKhepera3MotorSetpointSource_Triangle	= 3		//!< Internal Triangle Generator
	//cKhepera3MotorSetpointSource_Sinus		= 4		//!< Internal Sinus Generator (not implemented)
};

//! Motor control types
enum eKhepera3MotorControlType {
	cKhepera3MotorControlType_Unknown				= -1,	//!< Unknown control type
	cKhepera3MotorControlType_OpenLoop				= 0,	//!< Open loop
	cKhepera3MotorControlType_Position				= 1,	//!< Position
	cKhepera3MotorControlType_PositionUsingProfile	= 2,	//!< Position using the acceleration profile
	cKhepera3MotorControlType_Speed					= 3,	//!< Speed
	cKhepera3MotorControlType_SpeedUsingProfile		= 4,	//!< Speed using the acceleration profile
	cKhepera3MotorControlType_Torque				= 5,	//!< Torque (= current)
	//cKhepera3MotorControlType_ZeroFriction			= 6		//!< Zero friction (not implemented)
};

//! Motor 8 bit registers
enum eKhepera3MotorRegister8 {
	cKhepera3MotorRegister8_FirmwareVersion		= 0x00,
	cKhepera3MotorRegister8_Mode		 		= 0x28,	//!< eKhepera3MotorMode
	cKhepera3MotorRegister8_SetpointSource		= 0x29,	//!< eKhepera3MotorSetpointSource
	cKhepera3MotorRegister8_HardwareOptions		= 0x2A,	//!< eKhepera3MotorHardwareOptions
	cKhepera3MotorRegister8_SoftwareOptions		= 0x2B,	//!< eKhepera3MotorSoftwareOptions
	cKhepera3MotorRegister8_ControlType			= 0x2C,	//!< eKhepera3MotorControlType
	cKhepera3MotorRegister8_ErrorFlags			= 0x2D,	//!< eKhepera3MotorErrorFlags
	cKhepera3MotorRegister8_StatusFlags			= 0x2E,	//!< eKhepera3MotorStatusFlags
	cKhepera3MotorRegister8_Filter				= 0x33,
	cKhepera3MotorRegister8_BlockedTime			= 0x46,
	cKhepera3MotorRegister8_IntGenPeriod		= 0x47,
	cKhepera3MotorRegister8_IntGenAmplitude		= 0x48,
	cKhepera3MotorRegister8_IntGenOffset		= 0x49,
	cKhepera3MotorRegister8_Acceleration		= 0x52,	//!< Acceleration for trapezoidal speed profile (0..255)
	cKhepera3MotorRegister8_StaticFriction		= 0x54,	//!< Static Friction of the system (0..255)
	cKhepera3MotorRegister8_HWCurrentLimit		= 0x55,	//!< Generate PWM for current limiting with RC-filter [U = 5V / 255 * CurrentLimit]
	cKhepera3MotorRegister8_NearTargetMargin	= 0x60,
	cKhepera3MotorRegister8_Prescale			= 0x70,
	cKhepera3MotorRegister8_VelocityPrescaler	= 0x73
};

//! Motor 16 bit registers (indicated register holds L byte, +1 holds H byte)
enum eKhepera3MotorRegister16 {
	cKhepera3MotorRegister16_CurrentTorque		= 0x5A,
	cKhepera3MotorRegister16_TorqueBias			= 0x3C,
	cKhepera3MotorRegister16_KpSpeed			= 0x3E,
	cKhepera3MotorRegister16_KdSpeed			= 0x40,
	cKhepera3MotorRegister16_KiSpeed			= 0x42,
	cKhepera3MotorRegister16_SampleTime			= 0x44,
	cKhepera3MotorRegister16_SoftwareCurrentLimit	= 0x56,	//!< Software current limit (0..127), temporary register usable in the Interrupt Service Routine
	cKhepera3MotorRegister16_MinSampleTime		= 0x58,	//!< Time used to pass one cycle
	cKhepera3MotorRegister16_PIDOut				= 0x5C,
	cKhepera3MotorRegister16_KpPosition			= 0x61,
	cKhepera3MotorRegister16_KdPosition			= 0x63,
	cKhepera3MotorRegister16_KiPosition			= 0x65,
	cKhepera3MotorRegister16_KpTorque			= 0x67,
	cKhepera3MotorRegister16_KdTorque			= 0x69,
	cKhepera3MotorRegister16_KiTorque			= 0x6B,
	cKhepera3MotorRegister16_MaxSpeed			= 0x6D,
	cKhepera3MotorRegister16_SpeedMultiplier	= 0x71
};

//! Motor 32 bit registers (indicated register holds LL byte, +1 holds LH byte, +2 holds HL byte, +3 hold HH byte)
enum eKhepera3MotorRegister32 {
	cKhepera3MotorRegister32_Setpoint			= 0x2F,
	cKhepera3MotorRegister32_CurrentPosition	= 0x34,
	cKhepera3MotorRegister32_CurrentSpeed		= 0x38,
	cKhepera3MotorRegister32_SoftStopMin		= 0x4A,
	cKhepera3MotorRegister32_SoftStopMax		= 0x4E
};

//! One motor
struct sKhepera3Motor {
	int i2c_address;								//!< I2C bus address
	int direction;									//!< -1 to invert the direction, otherwise +1
	unsigned int firmware_version;					//!< Firmware version of the motor controller (updated by khepera3_motor_firmware_version, example: 104 = 0x68 => version 6, revision 8)
	int current_speed;								//!< Current speed (updated by khepera3_motor_get_current_speed)
	int current_position;							//!< Current position (updated by khepera3_motor_get_current_position)
	int current_torque;								//!< Current torque (updated by khepera3_motor_get_current_torque)
	enum eKhepera3MotorStatusFlags status;			//!< Status (updated by khepera3_motor_get_status)
	enum eKhepera3MotorErrorFlags error;			//!< Error (updated by khepera3_motor_get_error)
	enum eKhepera3MotorControlType control_type;	//!< (private) Current control type (updated and used by all methods setting/getting the control type)
};

//! (private) Initializes this submodule.
void khepera3_motor_init();

//! Reads an 8 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 4 bytes.
int khepera3_motor_read_register8_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister8 reg, unsigned int *result);
//! Reads a 16 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 8 bytes.
int khepera3_motor_read_register16_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister16 reg, unsigned int *result);
//! Reads a 32 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 16 bytes.
int khepera3_motor_read_register32_p(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister32 reg, unsigned int *result);

//! Writes an 8 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 3 bytes.
int khepera3_motor_write_register8(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister8 reg, unsigned int value);
//! Reads a 16 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 6 bytes.
int khepera3_motor_write_register16(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister16 reg, unsigned int value);
//! Reads a 32 bit register. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 12 bytes.
int khepera3_motor_write_register32(struct sKhepera3Motor *motor, enum eKhepera3MotorRegister32 reg, unsigned int value);

//! Initializes a motor (sets PID, speed profile, and various other stuff). The return value indicates success (-1) or failure (0). Transfer on I2C bus: approx. 100 bytes.
int khepera3_motor_initialize(struct sKhepera3Motor *motor);
//! Sets the control type if it isn't already set to that value. Setting it to cKhepera3MotorControlType_Unknown forces it to be written next time. Returns -1 on success and 0 otherwise. Transfer on I2C bus: 0 - 3 bytes.
int khepera3_motor_set_control_type(struct sKhepera3Motor *motor, enum eKhepera3MotorControlType control_type);

//! Stops the motor immediately (holding their current position). Transfer on I2C bus: 3 bytes.
int khepera3_motor_stop(struct sKhepera3Motor *motor);
//! Put the motors in normal (started) mode. Transfer on I2C bus: 3 bytes.
int khepera3_motor_start(struct sKhepera3Motor *motor);
//! Puts the motors in idle mode (control disabled). Transfer on I2C bus: 3 bytes.
int khepera3_motor_idle(struct sKhepera3Motor *motor);
//! Sets the motor speed. Transfer on I2C bus: 12 - 15 bytes.
int khepera3_motor_set_speed(struct sKhepera3Motor *motor, int speed);
//! Sets the motor speed and uses the acceleration profile to reach that speed. Transfer on I2C bus: 12 - 15 bytes.
int khepera3_motor_set_speed_using_profile(struct sKhepera3Motor *motor, int speed);
//! Drives the motor to a specific position. Transfer on I2C bus: 12 - 15 bytes.
int khepera3_motor_goto_position(struct sKhepera3Motor *motor, int position);
//! Drives the motor to a specific position using the speed profile. Transfer on I2C bus: 12 - 15 bytes.
int khepera3_motor_goto_position_using_profile(struct sKhepera3Motor *motor, int position);
//! Sets the current position of the motor to a specific value (without moving the motor). Transfer on I2C bus: 12 bytes.
int khepera3_motor_set_current_position(struct sKhepera3Motor *motor, int position);

//! Reads the firmware revision of one motor. Transfer on I2C bus: 4 bytes.
int khepera3_motor_firmware_version(struct sKhepera3Motor *motor);
//! Reads the status register and updates the corresponding field in the motor structure. Transfer on I2C bus: 4 bytes.
int khepera3_motor_get_status(struct sKhepera3Motor *motor);
//! Reads the error register and updates the corresponding field in the motor structure. Transfer on I2C bus: 4 bytes.
int khepera3_motor_get_error(struct sKhepera3Motor *motor);
//! Reads the current motor speed and updates the corresponding field in the motor structure. Transfer on I2C bus: 16 bytes.
int khepera3_motor_get_current_speed(struct sKhepera3Motor *motor);
//! Reads the current motor position and updates the corresponding field in the motor structure. Transfer on I2C bus: 16 bytes.
int khepera3_motor_get_current_position(struct sKhepera3Motor *motor);
//! Reads the current motor torque and updates the corresponding field in the motor structure. Transfer on I2C bus: 16 bytes.
int khepera3_motor_get_current_torque(struct sKhepera3Motor *motor);

#endif
