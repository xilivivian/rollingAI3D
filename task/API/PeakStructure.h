#pragma once

#include <stdint.h>

/*! \addtogroup Data_Structures Data structures
*  @{
*/

/// <summary>
/// Position of input state enable bit inside HEADER::enabled_fields.
/// </summary>
#define FS_HEADER_INPUT_STATE_BIT (1 << 0)

/// <summary>
/// Position of pulse width enable bit inside HEADER::enabled_fields.
/// </summary>
#define FS_HEADER_PULSE_WIDTH_BIT (1 << 1)

/// <summary>
/// Position of location enable bit inside HEADER::enabled_fields.
/// </summary>
#define FS_HEADER_LOCATION_BIT (1 << 2)

/// <summary>
/// Sets a bit inside a bitfield.
/// </summary>
#define BIT_SET(field, bit) (field |= bit)

/// <summary>
/// Clears a bit inside a bitfield.
/// </summary>
#define BIT_CLEAR(field, bit) (field &= ~bit)

/// <summary>
/// Checks if a bit inside field is set.
/// </summary>
#define BIT_IS_SET(field, bit) (field & bit)

/// <summary>
/// Not measured point in line and batch callback functions
/// </summary>
#define NO_MEASUREMENT (float)(9999999)

/// <summary>
/// Maximum number of layers
/// </summary>
#define MAX_LAYERS 10

/// <summary>
/// Macro to check whether the point is measured or not
/// </summary>
#define NOT_MEASURED_POINT(x) ((x)>9999998)

/// <summary>
/// Enumeration for the interleaving phase.
/// </summary>
enum InterleavePhase
{
	/// <summary>
	/// Peak is taken with long exposure time.
	/// </summary>
	Long=0,
	/// <summary>
	/// Peak is taken with short exposure time.
	/// </summary>
	Short = 1
};

/// <summary>
/// Defines a point received by a callback handler for (\ref ProfileCallback).
/// </summary>
struct STRUCT_POINT
{
	/// <summary>
	/// The x coordinate.
	/// </summary>
	float X;

	/// <summary>
	/// The y coordinate.
	/// </summary>
	float Y;

	/// <summary>
	/// The intensity.
	/// </summary>
	float Intensity;

	union
	{
		/// <summary>
		/// Intensity calculated with the detection filter. Set only if the feature is supported by the firmware.
		/// </summary>
		float IntensitySecondary;

		/// <summary>
		/// Value indicates the exposure phase when using interleaving. If the interleaving is not used, point phase is set to long.
		/// </summary>
		InterleavePhase interleavePhase;
	};
};

/// <summary>
/// Header associated to each raw image or profile received from the camera.
/// </summary>
struct HEADER
{
	/// <summary>
	/// Bitmask telling, which <b>optional</b> fields of this struct are set. To check if an optional field is set, you can use \ref BIT_IS_SET and bit defines such as \ref FS_HEADER_PULSE_WIDTH_BIT.
	/// </summary>
	uint64_t enabled_fields;

	/// <summary>
	/// Number of point clouds in a queue inside API. If this value starts to accumulate, it is usually a sign of too slow application point cloud callback handler. Always set.
	/// </summary>
	uint32_t reception_queue_size;

	union
	{
		/// <summary>
		/// Frequency (in Hz) at which the API receives data. Set only if frequency calculation is enabled (default). 
		/// </summary>
		double reception_frequency;

		/// <summary>
		/// Time stamp (in 0.1 us) at which the API receives data. Set only if frequency calculation is disabled. 
		/// </summary>
		unsigned long long time_stamp;
	};

	/// <summary>
	/// Zero-based point cloud reception index. Always set.
	/// </summary>
	uint32_t index;

	/// <summary>
	/// HW input pin state bitmask: | b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 |. <b>Optional.</b>
	/// </summary>
	uint8_t	input_state;
	
	/// <summary>
	/// us pulse width of the frame. <b>Optional.</b>
	/// </summary>
	float pulse_width;
	
	/// <summary>
	/// Generic location. <b>Optional.</b>
	/// </summary>
	int32_t location;
};

enum SurfaceSorting
{
	/// <summary>
	/// Sorting layers from top to bottom
	/// </summary>
	FROM_TOP_TO_BOTTOM =0,

	/// <summary>
	/// Sorting layers from bottom to top
	/// </summary>
	FROM_BOTTOM_TO_TOP =1,

	/// <summary>
	/// Sorting layers according to intensity value. Highest intensity is layer 0.
	/// </summary>
	FROM_MAX_INTENSITY_TO_LOWER =2,

	/// <summary>
	/// Sorting layers according to intensity value and then from top to bottom.
	/// </summary>
	FROM_MAX_INTENSITY_TO_LOWER_AND_TOP_TO_BOTTOM = 3,
};

/// <summary>
/// Enumeration for the material type.
/// </summary>
enum MaterialType
{
	/// <summary>
	/// Opaque is used when a top surface has opaque areas.
	/// </summary>
	Opaque = 0,

	/// <summary>
	/// Mirror is used for highly reflective surfaces.
	/// </summary>
	Mirror = 1,

	/// <summary>
	/// Transparent is used for materials having transparent surfaces.
	/// </summary>
	Transparent = 2
};

/// <summary>
/// Enumeration for sensitivity used in peak detection configuration.
/// </summary>
enum DetectionSensitivity
{
	/// <summary>
	/// BestAccuracy is used when when the accuracy is prioritized.
	/// </summary>
	BestAccuracy = 0,

	/// <summary>
	/// BestSensitivity used when dark areas need to be detected.
	/// </summary>
	BestSensitivity = 1
};

/// <summary>
/// Results of field calibration function. Measurements in micrometers.
/// </summary>

#define MAX_GROOVE_MEASUREMENTS 5
struct FieldCalibrationResults
{
	/// <summary>
	/// Average standard deviation of sequential measurements. Indicates noise of the camera.
	/// </summary>
	double averageStd;
	/// <summary>
	/// Maximum standard deviation of sequential measurements. Indicates noise of the camera.
	/// </summary>
	double maximumStd;
	/// <summary>
	/// Minimum standard deviation of sequential measurements. Indicates noise of the camera.
	/// </summary>
	double minimumStd;
	/// <summary>
	/// Average intensity of measured profiles.
	/// </summary>
	double averageIntensity;
	/// <summary>
	/// Used pulse length for measured profiles.
	/// </summary>
	double usedPulseLength;

	/// <summary>
	/// Detected calibration block (0 = not detected, 1 = mirror, 2 = groove)
	/// </summary>
	uint64_t calibrationBlock;

	/// <summary>
	/// Average flatness correction calculated from the calibration block
	/// </summary>
	double averageAbsoluteDeviation;
	/// <summary>
	/// Maximum flatness correction calculated from the calibration block
	/// </summary>
	double maximumAbsoluteDeviation;

	/// <summary>
	/// Measured groove heights in micrometers.
	/// </summary>
	double grooveMeasurements[MAX_GROOVE_MEASUREMENTS];
	/// <summary>
	/// Number of measured groove heights.
	/// </summary>
	uint64_t numOfGrooveMeasurements;
	/// <summary>
	/// Calculated z-correction.
	/// </summary>
	double calculatedGrooveCorrection;

	/// <summary>
	/// Measured profile. Array of z-values.
	/// </summary>
	double profileZValues[2048];

	/// <summary>
	/// Stdev for each x-coordinate in the profile.
	/// </summary>
	double profileStdDevValues[2048];
};

/// <summary>
/// Stucture hold dynamic sensor control parameters.
/// </summary>

struct DynSensorControl
{
	/// <summary>
	/// Encoder location position.
	/// </summary>
	uint32_t	dyn_ctrl_location;
	
	/// <summary>
	/// Sensor image height.
	/// </summary>
	uint32_t	dyn_ctrl_height;
	
	/// <summary>
	/// Sensor image offset.
	/// </summary>
	uint32_t	dyn_ctrl_offset;
	
	/// <summary>
	/// Illuminator pulse width in microseconds.
	/// </summary>
	float		dyn_ctrl_pulse_width;

	/// <summary>
	/// Sensor image 8-bit gray scale intensity value (0-255) threshold.
    /// Supported by LCI1220 and LCI1620 sensors.
	/// </summary>
	uint32_t	dyn_ctrl_threshold;
};

/// <summary>
/// Enumeration for status in ZCompensationCalibration.
/// </summary>
enum ZCompensationCalibrationStatus
{
	/// <summary>
	/// State when system is not started.
	/// </summary>
	NotStarted = 0,

	/// <summary>
	/// State when a new scan is requested by UI.
	/// </summary>
	Waiting = 1,

	/// <summary>
	/// Scanning.
	/// </summary>
	Scanning = 2,

	/// <summary>
	/// Scanning completed successfully.
	/// </summary>
	Completed = 3,

	/// <summary>
	/// Scanning completed but profiles were missing. Probably moving speed was too high for the sensor.
	/// </summary>
	ProfilesMissing = 4,

	/// <summary>
	/// Scanning completed but points were missing. Probably led pulse length was too low.
	/// </summary>
	NotMeasuredPoints = 5,

	/// <summary>
	/// Scanning completed but proper texture was not found from the measurement. 
	/// </summary>
	TextureNotFound = 6,

	/// <summary>
	/// Scanning completed but a duplicate height was found. 
	/// </summary>
	DuplicateHeight = 7
};

/*! @} */

