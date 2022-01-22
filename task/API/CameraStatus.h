#pragma once

/*! \addtogroup CameraStatus Return values
*  @{
*/

/// <summary>
/// This enum defines the possible return values for the Vevo.dll.
/// </summary>
enum CameraStatus
{
	/// <summary>
	/// The operation was executed successfully.
	/// </summary>
	CAMERA_OK=0,

	/// <summary>
	/// Could not instantiate the image receiver.
	/// </summary>
	CAMERA_ERROR_INSTATION_FAILED=1,

	/// <summary>
	/// Camera is not connected
	/// </summary>
	CAMERA_ERROR_NOT_CONNECTED=2,

	/// <summary>
	/// Image reception is not installed.
	/// </summary>
	CAMERA_ERROR_IMAGE_RECEPTION_NOT_INSTALLED=3,

	/// <summary>
	/// Call to get image method failed.
	/// </summary>
	CAMERA_ERROR_GET_IMAGE_FAILED=4,

	/// <summary>
	/// Couldn't start image grabbing.
	/// </summary>
	CAMERA_ERROR_START_GRABBING_FAILED=5,

	/// <summary>
	/// Couldn't stop image grabbing.
	/// </summary>
	CAMERA_ERROR_STOP_GRABBING_FAILED=6,

	/// <summary>
	/// Error occured when trying to get image peaks.
	/// </summary>
	CAMERA_ERROR_GET_PEAKS_FAILED=7,

	/// <summary>
	/// Error occured when trying to set the frame grabbed callback.
	/// </summary>
	CAMERA_ERROR_FAILED_TO_SET_FRAME_GRABBED_CALLBACK=8,

	/// <summary>
	/// Could not set a parameter.
	/// </summary>
	CAMERA_ERROR_SET_PARAMETER_FAILED=9,

	/// <summary>
	/// Trying to access unknown parameter.
	/// </summary>
	CAMERA_ERROR_SET_UNKNOWN_PARAMETER=10,

	/// <summary>
	/// The requested camera was not found.
	/// </summary>
	CAMERA_ERROR_CAMERA_NOT_FOUND=11,

	/// <summary>
	/// The requested parameter was not found.
	/// </summary>
	CAMERA_ERROR_PARAM_NOT_FOUND=12,

	/// <summary>
	/// The requested recipe could not be found.
	/// </summary>
	CAMERA_ERROR_RECIPE_NOT_FOUND=13,

	/// <summary>
	/// The parameter type is not correct.
	/// </summary>
	CAMERA_ERROR_PARAM_TYPE_INVALID=14,

	/// <summary>
	/// Failed to load required DLL.
	/// </summary>
	CAMERA_ERROR_LOAD_DLL_FAILED=15,

	/// <summary>
	/// Provided file does not exist.
	/// </summary>
	CAMERA_ERROR_FILE_NOT_FOUND = 16,

	/// <summary>
	/// The parameter value is not correct.
	/// </summary>
	CAMERA_ERROR_PARAM_VALUE_INVALID = 17,

	/// <summary>
	/// Operation requires that sensor calibration file has been set.
	/// </summary>
	CAMERA_ERROR_SENSOR_CALIBRATION_FILE_NOT_SET = 18,

	/// <summary>
	/// FIR length is not supported.
	/// </summary>
	CAMERA_ERROR_INVALID_FIR_LENGTH = 19,

	/// <summary>
	/// Sensor hardware does not support the action.
	/// </summary>
	CAMERA_ERROR_HW_NOT_SUPPORTED = 20,

	/// <summary>
	/// API is already loaded.
	/// </summary>
	CAMERA_ERROR_API_ALREADY_LOADED = 21,

	/// <summary>
	/// A library file is missing.
	/// </summary>
	CAMERA_ERROR_MISSING_LIBRARY_FILES = 22,

	/// <summary>
	/// Calibration file is invalid.
	/// </summary>
	CAMERA_ERROR_INVALID_CALIBRATION_FILE = 23,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_FLASH_DATA_LENGTH_EXCEEDED = 24,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_FLASH_WRITE = 25,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_FLASH_WRITE_FINISH = 26,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_FLASH_READ = 27,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_FILE_WRITE = 28,

	/// <summary>
	/// Internal error code.
	/// </summary>
	CAMERA_ERROR_UNINITIALIZED_FLASH = 29,

	/// <summary>
	/// Parameter is read-only.
	/// </summary>
	CAMERA_ERROR_PARAMETER_READ_ONLY = 30,

	/// <summary>
	/// Selftest failed at the program start
	/// </summary>
	CAMERA_ERROR_SELFTEST_GRABBING_FAILED = 31,

	/// <summary>
	/// Missing license for the camera 
	/// </summary>
	CAMERA_ERROR_CAMERA_NO_LISENCE = 32,

	/// <summary>
	/// Identifying attributes like sensor id or mac address, not available in calibration file.
	/// </summary>
	CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_NOT_FOUND = 33,
	
	/// <summary>
	/// Calibration file attribute or value is incorrect.
	/// </summary>
	CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_INVALID = 34,

	/// <summary>
	/// Software component version not valid for current product.
	/// </summary>
    CAMERA_ERROR_INVALID_SOFTWARE_VERSION = 35,

	/// <summary>
	/// Not supported by current firmware version.
	/// </summary>
    CAMERA_ERROR_FIRMWARE_NOT_SUPPORTING = 36,
	
	/// <summary>
	/// Any other error.
	/// </summary>
	CAMERA_ERROR_UNKNOWN=100
};

/*! @} */