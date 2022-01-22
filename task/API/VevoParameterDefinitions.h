#pragma once

/*! \addtogroup API_Params Parameters for Camera
*  @{
*/


/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LoadRecipe
//! </pre>
/// Loads a sensor configuration from the specified recipe.
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Name of the recipe. Loads from the default folder (C:\\FocalSpec\\SDK Recipes\\ in Windows, ~/.local/share/fssdk/recipes/ in Linux) if path is not given.
/// </summary>
#define PARAM_LOAD_RECIPE "load_recipe"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SaveRecipe
//! </pre>
/// Saves the current sensor configuration to the specified recipe.
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Name of the recipe. Saves to the default folder (C:\\FocalSpec\\SDK Recipes\\ in Windows, ~/.local/share/fssdk/recipes/ in Linux) if path is not given.
/// </summary>
#define PARAM_SAVE_RECIPE "save_recipe"


/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakEnabled
//! </pre>
/// Controls the output mode of the camera. If enabled, peaks are output to \ref ProfileCallback, \ref LineCallback and \ref BatchCallback functions. If disabled, raw images are output to \ref RawImageCallback.
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = raw image output is active, 1 = peak cloud output is active.
/// </summary>
#define PARAM_PEAK_ENABLE "peak_enable"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakThreshold
//! </pre>
/// The intensity threshold value used in peak detection. If the peak intensity is below this limit, the peak gets discarded
/// \li <b>Parameter type:</b> int
/// \li <b>Unit:</b> 8-bit grayscale intensity value (2-200). Default value is 20.
/// </summary>
#define PARAM_PEAK_THRESHOLD "peak_treshold"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FirLength
//! </pre>
/// FIR parameter is used to find the detected peak position in the z-direction. Higher the FIR, more accurate the peak position is. With lower FIR, you can detect thinner layers in transparent layered material. 
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : Eligible values are 2, 4, 6, 8, 10, 12, 14 or 16.
/// </summary>
#define PARAM_PEAK_FIR_LENGTH "peak_fir_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorDataInFlash
//! </pre>
///  Read flag if e.g. Sensor type and calibration data is written to sensor flash memory. 
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : 1 if supported, 0 if not.
/// </summary>
#define PARAM_SENSOR_DATA_IN_FLASH "sensor_data_in_flash"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCalibrationFile
//! </pre>
/// Path of the sensor Z calibration file. No need to set if calibration files are in the sensor. 
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated string path to the file.
/// </summary>
#define PARAM_SENSOR_CALIBRATION_FILE "sensor_calibration_file"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.XCalibrationFile
//! </pre>
/// Path of the sensor X calibration file. No need to set if calibration files are in the sensor. 
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated string path to the file.
/// </summary>
#define PARAM_SENSOR_X_CALIBRATION_FILE "sensor_x_calibration_file"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.IntensityCalibrationFile
//! </pre>
/// Path of the sensor intensity calibration file.  
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated string path to the file.
/// </summary>
#define PARAM_SENSOR_INTENSITY_CALIBRATION_FILE "sensor_intensity_calibration_file"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorZCompensationFile
//! </pre>
/// Path of the sensor Z-Compensation calibration file.  
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated string path to the file.
/// </summary>
#define PARAM_SENSOR_Z_COMPENSATION_FILE "sensor_z_compensation_file"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakYUnit
//! </pre>
/// Controls the unit of measure of Y of the peaks. Note that this parameter has effect only if peak cloud output is active (\ref PARAM_PEAK_ENABLE is set to 1).
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = peak Y is in sensor pixel unit, 1 = peak Y is in micrometers.
/// </summary>
#define PARAM_PEAK_Y_UNIT "peak_y_unit"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakXUnit
//! </pre>
/// Controls the unit of measure of X of the peaks. Note that this parameter has effect only if peak cloud output is active (\ref PARAM_PEAK_ENABLE is set to 1).
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = peak X is in sensor pixel unit, 1 = peak X is in micrometers.
/// </summary>
#define PARAM_PEAK_X_UNIT "peak_x_unit"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.IntensityCorrectionEnable
//! </pre>
/// Enables or disables the intensity correction for the peak data.
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = intensity correction is disabled (default), 1 = intensity correction is disabled.
/// </summary>
#define PARAM_INTENSITY_CORRECTION_ENABLE "intensity_correction_enable"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorType
//! </pre>
/// Returns sensor type identifier without LCI prefix.
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 
//! <pre>
///             400 = LCI400 
///            1200 = LCI1200 
///            1600 = LCI1600
///             401 = LCI401 
///            1201 = LCI1201
///            1220 = LCI1220
///            1620 = LCI1620 
///			     -1 if not set
//! </pre>
/// </summary>
#define PARAM_SENSOR_TYPE "sensor_type"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Gain
//! </pre>
/// Gain sets the sensitivity of the sensor. The higher the gain, the more sensitive the sensor. Image grabbing must be stopped before setting 
/// the gain in LCI1220 and LCI1620 sensors.
/// The default setting is suitable for the most of the applications. Use higher Gain setting only when the signal is very weak or short LED Pulse Width is required due to high measurement frequency. Increasing Gain increases also noise and background signal level, therefore Gain increase might also require increasing the Threshold. Optimal Gain value for specific measurement must be found experimentally.
/// \li <b>Parameter type:</b> double
/// \li <b>Range</b> : 1.0 (default) - 3.2 for LCI401, LCI1200, LCI1201 and LCI1600 sensors
/// \n      2.0 (default) - 8.0 for LCI1220 and LCI1620 sensors
/// </summary>
#define PARAM_GAIN "gain" 

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Height
//! </pre>
/// Height of the sensor image in pixel unit. Small values can be used to achieve higher frequency. Used in conjunction with \ref PARAM_IMAGE_OFFSETY. 
/// Sum of image height and image offset y shall not exceed the maximum sensor height.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : pixels
/// \li <b>Range</b> : 8 - 1088 for LCI401, LCI1200, LCI1201 and LCI1600 sensors
/// \n      8 - 1400 for LCI1220 and LCI1620 sensors
/// </summary>
#define PARAM_IMAGE_HEIGHT "image_height"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.OffsetY
//! </pre>
/// Set the Y offset for the sensor. Used in conjunction with \ref PARAM_IMAGE_HEIGHT. 
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : pixels
/// \li <b>Range</b> : -1 - 1080 for LCI401, LCI1200, LCI1201 and LCI1600 sensors. -1 - 1392 for LCI1220 and LCI1620 sensors. If value is greater than 0, it should be noticed that the sum of \ref PARAM_IMAGE_OFFSETY and
/// \ref PARAM_IMAGE_HEIGHT should not exceed the maximum sensor height. If value is -1 offset is calculated automatically from the calibration zero height (\ref PARAM_IMAGE_HEIGHT must be set first).
/// </summary>
#define PARAM_IMAGE_OFFSETY "image_offsety"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SurfaceMode
//! </pre>
///  Sort peaks and filter top, bottom or brightest profile. 
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : 
/// \n Sensors LCI401, LCI1200, LCI1220 and LCI1620:  0 = disabled (default), 1 = bottom, 2 = top, 3 = brightest, 4 = two two top-most, 5 = two top-most.
/// \n Sensors LCI1201 and LCI1600: 0 = disabled (default), 1 = top, 2 = bottom, 3 = brightest, 4 = two two top-most, 5 = two two top-most.
/// </summary>
#define PARAM_SURFACE_MODE "surface_mode"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.HdrEnabled
//! </pre>
/// Enables or disables the HDR mode, where the dynamic range of luminosity is greater than normally.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : 0 = disabled, 1 = enabled.
/// </summary>
#define PARAM_HDR_ENABLED "hdr_enable"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Kp1Pos
//! </pre>
/// The knee point 1 position for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> int
/// \li <b>Valid range:</b> 1 - 99
/// </summary>
#define PARAM_HDR_KP1_POS "hdr_kp1_pos"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Kp2Pos
//! </pre>
/// The knee point 2 position for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_HDR_KP2_POS "hdr_kp2_pos"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.VLow2
//! </pre>
/// The VLow2 (sensitivity, reference voltage) value for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_HDR_VLOW2  "hdr_vlow2"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.VLow3
//! </pre>
/// The VLow3 (sensitivity, reference voltage) value for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_HDR_VLOW3 "hdr_vlow3"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Kp1PosFloat
//! </pre>
/// The knee point 1 position in % for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> float
/// \li <b>Valid range:</b> 0.02 - 99.99
/// </summary>
#define PARAM_HDR_KP1_POS_FLOAT "hdr_kp1_pos_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Kp2PosFloat
//! </pre>
/// The knee point 2 position in % for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_HDR_KP2_POS_FLOAT "hdr_kp2_pos_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.VLow2Float
//! </pre>
/// The VLow2 (sensitivity, reference voltage) value for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_HDR_VLOW2_FLOAT  "hdr_vlow2_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.VLow3Float
//! </pre>
/// The VLow3 (sensitivity, reference voltage) value for the HDR. This parameter is used only when the HDR is enabled.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_HDR_VLOW3_FLOAT "hdr_vlow3_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.XLimitMin
//! </pre>
/// Lower X threshold for statistics collection (\ref PARAM_SATURATION_COUNT, \ref PARAM_AVERAGE_FRAME_INTENSITY) and AGC input profile.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// \li <b>Range</b> : Default value is 0.
/// </summary>
#define PARAM_X_LIMIT_MIN "x_limit_min"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.XLimitMax
//! </pre>
/// Upper X threshold for statistics collection (\ref PARAM_SATURATION_COUNT, \ref PARAM_AVERAGE_FRAME_INTENSITY) and AGC input profile.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// \li <b>Range</b> : Default value is 2047.
/// </summary>
#define PARAM_X_LIMIT_MAX "x_limit_max"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SaturationCount
//! </pre>
/// The number of sensor pixels in saturation state.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Count
/// </summary>
#define PARAM_SATURATION_COUNT "saturation_count"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AvgFrameIntensity
//! </pre>
/// Average intensity level of a frame.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : 8-bit grayscale intensity
/// </summary>
#define PARAM_AVERAGE_FRAME_INTENSITY "average_frame_intensity"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.DynSensorCtrlEnable
//! </pre>
/// Determines if dynamic sensor control is enabled or not.
/// Dynamic control parameters given by interface function xxx to set register parameters
/// REG_DYN_SENSOR_CTRL_N_LOCATION, REG_DYN_SENSOR_CTRL_N_HEIGHT, REG_DYN_SENSOR_CTRL_N_HEIGHT_OFFSET, REG_DYN_SENSOR_CTRL_N_PULSE_WIDTH
/// N = 1, 2, ..., 10.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : 1 enabled, 0 disabled.
/// </summary>
#define PARAM_DYN_SENSOR_CTRL_ENABLE "dyn_sensor_control_enable"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.MaxPointCount
//! </pre>
/// Maximum point count per profile. Change this before calling _StartGrabbing(). Default value is fine if number of surfaces is less than 5.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Max number of points. Default value is 10000.
/// </summary>
#define PARAM_MAX_POINT_COUNT "max_point_count"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Mtu
//! </pre>
/// Sets Maximum Transmission Unit (MTU). Available only with LCI401, LCI1200, LCI1201 and LCI1600 sensors.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : bytes
/// </summary>
#define PARAM_MTU "mtu"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Ifg
//! </pre>
/// Sets Interpacket Delay. Min. value is 20. Available only with LCI401, LCI1200, LCI1201 and LCI1600 sensors.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : bit times
/// </summary>
#define PARAM_IFG "ifg"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Heartbeat
//! </pre>
/// Communication heartbeat value. Heartbeat is the interval in milliseconds to check if the connection is alive.
/// \li <b>Parameter type:</b> int
/// \li <b>Values:</b>  In production use the heartbeat should be set to ~5000. For application debugging purposes, greater values, e.g. 99999, are often needed.
/// </summary>
#define PARAM_HEARTBEAT "heartbeat"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ImageHeightZeroPosition
//! </pre>
///  Returns image row z coordinate where height has been tuned nearest to '0' by calibration file on use.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : pixel location
/// </summary>
#define PARAM_IMAGE_HEIGHT_ZERO_POSITION "image_height_zero_position"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.DeviceSerialNumber
//! </pre>
/// Device Serial Number, defined by production.
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated string of product serial number. "" if the serial number is not written (products manufactured before 02/2019).
/// </summary>
#define PARAM_DEVICE_SERIAL_NUMBER "device_serial_number"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.UserFlashWriteTime
//! </pre>
/// UTC timestamp when user data was written when available.
/// \li <b>Parameter type:</b>  string
/// \li <b>Values</b> : Null-terminated formatted string from time of date value.
/// </summary>
#define PARAM_USER_FLASH_WRITE_TIME "user_flash_write_time"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SignalDetectionFilterLength
//! </pre>
/// Detection filter is used to average the signal to reduce noise before threshold (\ref PARAM_PEAK_THRESHOLD). Low detection filter length is useful when the detected signal is rather faint. 
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : Eligible values are 2, 4, 6, 8, 10, 12, 14 or 16.
/// </summary>
#define PARAM_SIGNAL_DETECTION_FILTER_LENGTH "signal_detection_filter_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakAverageIntensityFilterLength
//! </pre>
/// The filter is used for averaging intensity values of the detected peaks. With high average intensity filter length, the best possible intensity image is produced.  
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : Eligible values are 2, 4, 6, 8, 10, 12, 14 or 16.
/// </summary>
#define PARAM_PEAK_AVERAGE_INTENSITY_FILTER_LENGTH "peak_average_intensity_filter_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LayerIntensityType
//! </pre>
/// Specifies which filter is used for intensity values in linecallback and batchcallback functions.
/// _SetLayerIntParameter function shall be used for this parameter.
/// \li <b>Parameter type:</b> int 
/// \li <b>Layer:</b> layer id (0=first layer)
/// \li <b>Values</b> : 0 = Average intensity filter, 1 = Signal detection filter
/// </summary>
#define PARAM_LAYER_INTENSITY_TYPE "layer_intensity_type"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LayerMaxThickness
//! </pre>
/// Specifies maximum thickness for the layer. Used for missing surface detection.
/// _SetLayerFloatParameter function shall be used for this parameter.
/// \li <b>Parameter type:</b> float 
/// \li <b>Layer:</b> layer id (0=first layer)
/// \li <b>Values</b> : Thickness in um (0, if not used)
/// </summary>
#define PARAM_LAYER_MAX_THICKNESS "layer_max_thickness"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LayerMinThickness
//! </pre>
/// Specifies minimum thickness for the layer.
/// _SetFloatParameter function sets same value for all layers.
/// _SetLayerFloatParameter function shall be used if layer specific value is needed.
/// \li <b>Parameter type:</b> float 
/// \li <b>Layer:</b> layer id (0=first layer)
/// \li <b>Values</b> : Thickness in um (0, if not used)
/// </summary>
#define PARAM_LAYER_MIN_THICKNESS "layer_min_thickness"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LayerRefractiveIndex
//! </pre>
/// Specifies the effective refractive index of a material. 
/// _SetLayerFloatParameter function shall be used for this parameter.
/// \li <b>Parameter type:</b> float 
/// \li <b>Layer:</b> layer id (0=first layer)
/// \li <b>Values</b> : The effective refractive index (1.0 by default)
/// </summary>
#define PARAM_LAYER_EFFECTIVE_REFRACTIVE_INDEX "layer_refractive_index"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ThicknessMode
//! </pre>
/// Calculates thicknesses for layers in line and batch callback functions. The first surface is returned as a world z-coordinate and  
/// following layers are calculated as thickness values. Note that thickness mode parameter is not saved to the recipe.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : 1 - thickness mode enabled, 0 - disabled (default)
/// </summary>
#define PARAM_THICKNESS_MODE "thickness_mode"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AverageXPixelSize
//! </pre>
/// Reads average x-pixel size in micrometers from the selected image ROI. Parameter is read only.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_AVERAGE_X_PIXEL_SIZE "average_x_pixel_size"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AverageZPixelSize
//! </pre>
/// Reads average z-pixel size in micrometers from the selected image ROI. Parameter is read only.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_AVERAGE_Z_PIXEL_SIZE "average_z_pixel_size"

/*! @} */

/*! \addtogroup API_Params4 Parameters for Acquisition
*  @{
*/

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PulseDivider
//! </pre>
/// Skips camera trigger pulses. If value if N, then every Nth pulses are triggered. If value is equal to one, then all trigger pulses are served.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit:</b> N
/// </summary>
#define PARAM_REG_PULSE_DIVIDER "reg_pulse_divider"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.EncoderPulseWidth
//! </pre>
/// Encoder pulse width for external triggering. <br>
/// If only single channel is used, pulse width is physical distance between rising edges of the signal. <br>
/// If two channels are used (quadrature encoder), pulse width must be divided by factor 2.
/// \li <b>Parameter type:</b>  float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_ENCODER_PULSE_WIDTH "encoder_pulse_width"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PulseFrequency
//! </pre>
/// Set the frequency for image capture.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit:</b> Hz
/// \li <b>Value:</b> 0 = external pulsing, > 0 internal pulsing.
/// </summary>
#define PARAM_REG_PULSE_FREQ "reg_pulse_freq"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Reordering
//! </pre>
/// Enable or disable line reordering according to header.index attribute. 0 = disabled (default), 1 = enabled
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_REORDERING "reordering"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ReorderingSpan
//! </pre>
///  Defines reordering time span max time window in milliseconds.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_REORDERING_SPAN "reordering_span"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ReorderingDeviation
//! </pre>
///  Defines reordering maximum index step deviation between subsequent lines. Exceeding triggers buffer clearing.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_REORDERING_DEVIATION "reordering_deviation"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FrequencyCalculation
//! </pre>
///  Enable or disable frequency calculation in header of the profile. 0 = disabled, 1 = enabled (default)
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_FREQUENCY_CALCULATION "frequency_calculation"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LineCallbackMaxLength
//! </pre>
/// Number of pixels expected from the line callback function. Can be called before grabbing but after initialization.
/// Parameter is read-only.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : number of pixels
/// </summary>
#define PARAM_LINE_CALLBACK_MAX_LENGTH "line_callback_max_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LineCallbackXRes
//! </pre>
///  Pixel size expected from the line callback function. Can be called before grabbing but after initialization. 
/// Parameter is read-only.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_LINE_CALLBACK_XRES "line_callback_xres"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.BatchLength
//! </pre>
/// Defines batch length (number of lines) on the batch run. 
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Line count
/// </summary>
#define PARAM_BATCH_LENGTH "batch_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.BatchTimeout
//! </pre>
///  Defines batch timeout timer. Interrupts collecting batch if total count lines not received before time expires.
///  Incomplete batch is delivered for callback.  
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Milliseconds
/// </summary>
#define PARAM_BATCH_TIMEOUT "batch_timeout"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Grabbing
//! </pre>
///  Read only value indicating the camera grabbing status.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> :  0 = stopped, 1 = grabbing is on.
/// </summary>
#define PARAM_GRABBING "grabbing"

/*! @} */

/*! \addtogroup API_Params1_1 Parameters for Illumination
*  @{
*/
/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PulseWidthFloat
//! </pre>
/// Illumination pulse width. Replaces \ref PARAM_LED_DURATION.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit:</b> Microseconds
/// </summary>
#define PARAM_REG_PULSE_WIDTH_FLOAT "reg_pulse_width_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AgcEnabled
//! </pre>
/// Is Automatic Gain Control (AGC) enabled. If enabled, AGC will automatically tune the pulse width based on the signal from the surface(s).
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 0,1. Default value is 0 (disabled).
/// </summary>
#define PARAM_AGC_ENABLED "agc_enabled"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AgcWiLimit
//! </pre>
/// AGC pulse width upper limit.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Microseconds
/// \li <b>Range</b> : Default value is 2000.
/// </summary>
#define PARAM_AGC_PULSE_WIDTH_LIMIT "agc_pulse_width_limit"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AgcMinPulseWidthLimit
//! </pre>
/// AGC pulse width minimum limit.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Microseconds
/// \li <b>Range</b> : Default value is 1.
/// </summary>
#define PARAM_AGC_MIN_PULSE_WIDTH_LIMIT "agc_minimum_pulse_width_limit"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AgcGain
//! </pre>
/// Gain for AGC P-controller.
/// \li <b>Parameter type:</b> float
/// \li <b>Range</b> : Default value is 1.2F.
/// </summary>
#define PARAM_AGC_GAIN "agc_gain"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AgcTarget
//! </pre>
/// AGC target intensity (8-bit grayscale).
/// \li <b>Parameter type:</b> float
/// \li <b>Range</b> : 0.0F-255.0F. Default value is 80.0f.
/// </summary>
#define PARAM_AGC_TARGET "agc_target"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PulseCurrentMiddle
//! </pre>
/// Pulse current in the middle section of the sensor. 
/// \li <b>Parameter type:</b> float
/// \li <b>Range</b> : 0.1 - 1.0 ampere 
/// </summary>
#define PARAM_PULSE_CURRENT_MIDDLE "pulse_current_middle"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PulseCurrentEdges
//! </pre>
/// Pulse current in the edges of the sensor.
/// \li <b>Parameter type:</b> float
/// \li <b>Range</b> : 0.1 - 1.0 ampere 
/// </summary>
#define PARAM_PULSE_CURRENT_EDGES "pulse_current_edges"

/*! @} */

/*! \addtogroup API_Params2 Parameters for I/O 
*  @{
*/

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakCountLimit
//! </pre>
/// If the peak core detects more peaks than this threshold, a warning LED is enabled.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : Default value is 10000.
/// </summary>
#define PARAM_PEAK_COUNT_LIMIT "peak_count_limit"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.OutputPinState
//! </pre>
/// Bitmask for driving outputs.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 4 valid outputs (0x1 - 0xF). Default value is 0.
/// </summary>
#define PARAM_OUTPUT_PIN_STATE "output_pin_state"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.InputPinState
//! </pre>
/// Bitmask for reading inputs.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 4 input channels (0x1 - 0xF). Default value is 0.
/// </summary>
#define PARAM_INPUT_PIN_STATE "input_pin_state"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.YToleranceMin
//! </pre>
/// Lower threshold for height indicator LED.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// \li <b>Range</b> : Default value is 200.
/// </summary>
#define PARAM_Y_TOLERANCE_MIN "y_tolerance_min"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.YToleranceMax
//! </pre>
/// Upper threshold for height indicator LED.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// \li <b>Range</b> : Default value is 800.
/// </summary>
#define PARAM_Y_TOLERANCE_MAX "y_tolerance_max"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TriggerSource
//! </pre>
/// Trigger input source pin number.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 0-3. Default is 1. Disabled is 0
/// </summary>
#define PARAM_TRIGGER_SOURCE "trigger_source"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TriggerDisableSource
//! </pre>
/// Trigger disable input source pin number.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 0-3. Default is 0, disabled.
/// </summary>
#define PARAM_TRIGGER_DISABLE_SOURCE "trigger_disable_source"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TriggerZeroSource
//! </pre>
/// Trigger zero input source pin number.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 0-3. Default is 3. Disabled is 0.
/// </summary>
#define PARAM_TRIGGER_ZERO_SOURCE "trigger_zero_source"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TriggerMinusSource
//! </pre>
/// Trigger minus input source pin number.
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : 0-3. Default is input 2. Disabled is 0.
/// </summary>
#define PARAM_TRIGGER_MINUS_SOURCE "trigger_minus_source"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TriggerDebounce
//! </pre>
/// Digital filter debounce time for inputs. Pulses below this threshold are ignored.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Microseconds
/// \li <b>Range</b> : Default value is 1.0.
/// </summary>
#define PARAM_TRIGGER_DEBOUNCE "trigger_debounce"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorBoardTemperature
//! </pre>
/// Read temperature [Celsius degrees] on sensor board. This is available only with LCI401, LCI1200, LCI1201 and LCI1600 sensors.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_SENSOR_BOARD_TEMPERATURE "sensor_board_temperature"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorTemperature
//! </pre>
/// Read temperature [Celsius degrees] on sensor chip. This is available only with LCI401, LCI1200, LCI1201 and LCI1600 sensors.
/// \li <b>Parameter type:</b> int
/// </summary>
#define PARAM_SENSOR_TEMPERATURE "sensor_temperature"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ChipTemperature
//! </pre>
/// Read temperature [Celsius degrees] on sensor chip. This is available only with LCI1220 and LCI1620 sensors.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_CHIP_TEMPERATURE "chip_temperature"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorBoardTemperatureFloat
//! </pre>
/// Read temperature [Celsius degrees] on sensor board. This is available only with LCI1220 and LCI1620 sensors.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_SENSOR_BOARD_TEMPERATURE_FLOAT "sensor_board_temperature_float"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.IlluminatorTemperature
//! </pre>
/// Read temperature [Celsius degrees] on illuminator. This is available only with LCI1220 and LCI1620 sensors.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_ILLUMINATOR_TEMPERATURE "illuminator_temperature"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FrontPanelTemperature
//! </pre>
/// Read temperature [Celsius degrees] on sensor front panel. This is available only with LCI1220 and LCI1620 sensors.
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_FRONT_PANEL_TEMPERATURE "front_panel_temperature"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.SensorGpIo
//! </pre>
/// General Purpose I/O GPIO 3-bits for rev-6-ele, 2-bits for G3. 
/// \li <b>Parameter type:</b> int
/// \li <b>Range</b> : . (0 = disabled, 1=output 1, 2=output 3, 3=output 3 and 7=all three)
/// </summary>
#define PARAM_SENSOR_GP_IO "sensor_gp_io"

/*! @} */

/*! \addtogroup API_Params3 Parameters for Filtering
*  @{
*/
/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.DetectMissingFirstLayer
//! </pre>
///  Correct the first surface by detecting areas where the next layer is visible but the first one missing (i.e. holes in the first layer).
///  Takes effect for line callback functions.
///  0, disabled (default)
///  >1, enabled, minimum distance between two layers in micrometers (typically 30 um is used)
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_DETECT_MISSING_FIRST_LAYER "detect_missing_first_layer"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.DetectMissingFirstLayerX
//! </pre>
///  Correct the first surface by detecting areas where the next layer is visible but the first one missing (i.e. holes in the first layer).
///  Takes effect for line callback functions and only if PARAM_DETECT_MISSING_FIRST_LAYER is set.
///  0, disabled (default)
///  >1, enabled, maximum x-coordinate distance between two points in micrometers 
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_DETECT_MISSING_FIRST_LAYERX "detect_missing_first_layer_x"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.DetectMissingFirstLayerLength
//! </pre>
///  Defines minimum length of the first surface. Can be used to reduce wrong detection of noise points to first layer.
///  0, disabled (default)
///  >1, enabled, minimum length in micrometers 
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_DETECT_MISSING_FIRST_LAYER_MIN_LENGTH "detect_missing_first_layer_min_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ResampleLineXResolution
//! </pre>
///  Change the resolution for the profile given in the line callback function.
///  0, disabled (default)
///  >0, new resolution in micrometers. Must be bigger than the camera native resolution (i.e. for LCI1200 this must be bigger than 5.5 micrometers)
/// \li <b>Parameter type:</b> float
/// </summary>
#define PARAM_RESAMPLE_LINE_X_RESOLUTION "resample_line_x_resolution"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FillGapXmax
//! </pre>
///  Defines maximum gap of missing measurement points filled by interpolation. 
///  PARAM_LAYER_MIN_THICKNESS parameter defines maximum Z difference for interpolated points. For opaque materials, a user can set parameter for the first layer.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers, 0 if feature is disabled.
/// </summary>
#define PARAM_FILL_GAP_X_MAX "fill_gap_x_max"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AverageZFilterSize
//! </pre>
///  Size of the averaging filter for the line callback Z-values. 
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_AVERAGE_Z_FILTER_SIZE "average_z_filter_size"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.AverageIntensityFilterSize
//! </pre>
///  Size of the averaging filter for the line callback intensity values. 
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_AVERAGE_INTENSITY_FILTER_SIZE "average_intensity_filter_size"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.MedianZFilterSize
//! </pre>
///  Size of the median filter for the line callback intensity values. If an even value is specified, the function adds 1 and uses the odd value of the filter mask for median filtering.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// </summary>
#define PARAM_MEDIAN_Z_FILTER_SIZE "median_z_filter_size"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.MedianIntensityFilterSize
//! </pre>
///  Size of the median filter for the line callback intensity values. If an even value is specified, the function adds 1 and uses the odd value of the filter mask for median filtering.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : Pixels
/// </summary>
#define PARAM_MEDIAN_INTENSITY_FILTER_SIZE "median_intensity_filter_size"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.NoiseRemoval
//! </pre>
/// Noise removal algorithm for the line callback. 
/// Noise filter suppresses the noise and allows to use lower threshold which improves the data acquisition. Noise filter removes single data points which do not have adjacent daqta points.
/// Note! If noise removal for Y is enabled line callback has 1 profile delay
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = noise removal is disabled, 1 = noise removal is enabled for X, 2 = noise removal is enabled for Y, 3 = noise removal is enabled for X and Y.
/// </summary>
#define PARAM_NOISE_REMOVAL "noise_removal"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationYDirection
//! </pre>
/// Calculate Z-Compensation for Y-direction based on surface texture variations.<br>
/// The calibration file is loaded once when the calculation is enabled. By default, the file is loaded from c:\\FocalSpec\\Calibration folder if it is not specified by \ref PARAM_SENSOR_Z_COMPENSATION_FILE.<br>
/// Calculation is done for \ref BatchCallback functions. Z-Compensation is processed only for the first layer.<br>
/// Intensity calibration must be disabled when the Z-Compensation is used.<br>
/// Calculation works only with external triggering and \ref PARAM_ENCODER_PULSE_WIDTH must be set properly.
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = calculation is disabled (default), 1 = calculation is enabled 
/// </summary>
#define PARAM_Z_COMPENSATION_Y_DIRECTION "z_compensation_y_direction"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationXDirection
//! </pre>
/// Calculate Z-Compensation for X-direction based on surface texture variations.<br>
/// The calibration file is loaded once when the calculation is enabled. By default, the file is loaded from c:\\FocalSpec\\Calibration folder if it is not specified by \ref PARAM_SENSOR_Z_COMPENSATION_FILE.<br>
/// Calculation is done for \ref LineCallback and \ref BatchCallback functions. Z-Compensation is processed only for the first layer.<br>
/// Intensity calibration must be disabled when the Z-Compensation is used.<br>
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = calculation is disabled (default), 1 = calculation is enabled 
/// </summary>
#define PARAM_Z_COMPENSATION_X_DIRECTION "z_compensation_x_direction"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationThreshold
//! </pre>
/// Remove all Z-coordinates where calculated Z-Compensation would be greater than specified threshold. Intensity values are not removed.<br>
/// \li <b>Parameter type:</b>  float
/// \li <b>Unit</b> : Micrometers
/// \li <b>Values</b> Threshold or 0.0 if removal is disabled (default):
/// </summary>
#define PARAM_Z_COMPENSATION_THRESHOLD "z_compensation_threshold"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationInterpolate
//! </pre>
/// Interpolate Z-values for areas removed by removal function (\ref PARAM_Z_COMPENSATION_THRESHOLD).<br>
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = interpolation is disabled (default), 1 = interpolation is enabled 
/// </summary>
#define PARAM_Z_COMPENSATION_INTERPOLATE "z_compensation_interpolate"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationXBrightToDarkMul
//! </pre>
/// Z-Compensation multiplication factor for X-edges from bright to dark. Direction is from left to right.<br>
/// Parameter can be used to adjust compensation for different materials.
/// \li <b>Parameter type:</b>  float
/// \li <b>Values</b> : 0 = no compensation, 1.0 default 
/// </summary>
#define PARAM_Z_COMPENSATION_X_BRIGHT_TO_DARK_MUL "z_compensation_x_bright_to_dark_mul"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationXDarkToBrightMul
//! </pre>
/// Z-Compensation multiplication factor for X-edges from dark to bright. Direction is from left to right.<br>
/// Parameter can be used to adjust compensation for different materials.
/// \li <b>Parameter type:</b>  float
/// \li <b>Values</b> : 0 = no compensation, 1.0 default 
/// </summary>
#define PARAM_Z_COMPENSATION_X_DARK_TO_BRIGHT_MUL "z_compensation_x_dark_to_bright_mul"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationYBrightToDarkMul
//! </pre>
/// Z-Compensation multiplication factor for Y-edges from bright to dark. Profile order is defined by /ref PARAM_MOVING_DIRECTION .<br>
/// Parameter can be used to adjust compensation for different materials.
/// \li <b>Parameter type:</b>  float
/// \li <b>Values</b> : 0 = no compensation, 1.0 default 
/// </summary>
#define PARAM_Z_COMPENSATION_Y_BRIGHT_TO_DARK_MUL "z_compensation_y_bright_to_dark_mul"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ZCompensationYDarkToBrightMul
//! </pre>
/// Z-Compensation multiplication factor for Y-edges from dark to bright. Profile order is defined by /ref PARAM_MOVING_DIRECTION .<br>
/// Parameter can be used to adjust compensation for different materials.
/// \li <b>Parameter type:</b>  float
/// \li <b>Values</b> : 0 = no compensation, 1.0 default 
/// </summary>
#define PARAM_Z_COMPENSATION_Y_DARK_TO_BRIGHT_MUL "z_compensation_y_dark_to_bright_mul"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.MovingDirection
/// Movement in y direction. 
//! </pre>
/// \li <b>Parameter type:</b>  int
/// \li <b>Values</b> : 0 = towards (default), 1 = backwards 
/// </summary>
#define PARAM_MOVING_DIRECTION "moving_direction"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.PeakXFilter
//! </pre>
/// Set X-filter length to smooth the image before the peak detection. 
/// X-filter suppresses noise effectively, which allows to use of lower threshold. X-filter functionality can be used to smooth image in X direction before the peak detection.Valid values are 0 (disabled), 3, 5 or 7 pixels. Filter function averages (moving average) the raw image signal in X direction using the given number of pixels.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : Default value is 0 = disabled. Eligible values are 3, 5 and 7.
/// </summary>
#define PARAM_PEAK_X_FILTER "peak_x_filter"


/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.TrimEdges
//! </pre>
/// Edge filter to remove artificial points detected at the end of the surfaces. The filter can be disabled/enabled at runtime in the line callback function and it takes effect on the next processed profiles.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : 0 = filter is disabled (default), 1 = filter is enabled.
/// </summary>
#define PARAM_TRIM_EDGES "trim_edges"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ClusteringDistanceX
//! </pre>
/// Maximum X distance between two points in a cluster. 0 if feature is disabled.
/// \li <b>Parameter type:</b> float 
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_CLUSTERING_DISTANCE_X "clustering_distance_x"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ClusteringDistanceZ
//! </pre>
/// Maximum Z distance between two points in a cluster. 0 if feature is disabled.
/// \li <b>Parameter type:</b> float 
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_CLUSTERING_DISTANCE_Z "clustering_distance_z"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.ClusterMinimumLength
//! </pre>
/// Clusters which are shorter than specified length are removed. 0 if feature is disabled.
/// \li <b>Parameter type:</b> float 
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_CLUSTER_MINIMUM_LENGTH "cluster_minimum_length"

/*! @} */


/*! \addtogroup API_Params5 Deprecated Parameters 
*  @{
*/

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FirAverLength
//! </pre>
/// The length of averaging filter used for both detection and intensity calculation. 
/// Parameter is deprecated. Please use parameters PARAM_SIGNAL_DETECTION_FILTER_LENGTH and PARAM_PEAK_AVERAGE_INTENSITY_FILTER_LENGTH.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : Eligible values are 2, 4, 6, 8, 10, 12, 14 or 16.
/// </summary>
#define PARAM_PEAK_AVER_FIR_LENGTH "peak_aver_fir_length"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.Width
//! </pre>
/// Width of the sensor image in pixel unit.
/// \li <b>Parameter type:</b>  Int.
/// \li <b>Unit</b> : Pixels
/// \li <b>Values</b> : For full camera sensor 2048.
/// </summary>
#define PARAM_IMAGE_WIDTH "image_width"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.OffsetX
//! </pre>
/// Set the X offset for the image capture.
/// \li <b>Parameter type:</b>  Int.
/// \li <b>Unit</b> : Pixels
/// \li <b>Values</b> : Typically 0. If value is non-zero, it should be noticed that the sum of offset X and
/// image width shall not exceed the sensor width.
/// </summary>
#define PARAM_IMAGE_OFFSETX "image_offsetx"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.InterleaveFactor
//! </pre>
/// Sets the value used to enable or disable interleaving. Interleaving applies different exposure for even and odd rows of the sensor.
/// The exposure for odd rows is as-is, whereas even rows have short exposure, namely current_exposure / \ref PARAM_INTERLEAVE_FACTOR.
/// \li <b>Parameter type:</b> float
/// \li <b>Values</b> : 1.0 disables interleaving. Any other value divides the exposure of even rows, but leaves the exposure of odd rows as-is.
/// </summary>
#define PARAM_INTERLEAVE_FACTOR "interleavefactor"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.InterleaveFilterMode
//! </pre>
/// Sets the interleaving filter. Used only if \ref PARAM_INTERLEAVE_FACTOR is other than 1.0.
/// \li <b>Parameter type:</b> int
/// \li <b>Values</b> : 0 = do not filter (the application should filter), 1 = filter using \ref PARAM_INTERLEAVE_TARGET_INTENSITY and \ref PARAM_INTERLEAVE_Y_THRESHOLD
/// </summary>
#define PARAM_INTERLEAVE_FILTER "interleavefilter"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.InterleaveFilterTargetIntensity
//! </pre>
/// Sets the target intensity for the interleaving. See \ref PARAM_INTERLEAVE_FILTER for more information.
/// \li <b>Parameter type:</b> float
/// \li <b>Values</b> : 8-bit grayscale intensity (0.0 - 255.0)
/// </summary>
#define PARAM_INTERLEAVE_TARGET_INTENSITY "interleavetargetintensity"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.InterleaveFilterYThreshold
//! </pre>
/// Sets the Y threshold [px] for interleaving. See \ref PARAM_INTERLEAVE_FILTER for more information.
/// \li <b>Parameter type:</b> float
/// \li <b>Values</b> : Y threshold [px] value.
/// </summary>
#define PARAM_INTERLEAVE_Y_THRESHOLD "interleaveythreshold"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.LedDuration
//! </pre>
/// The time which the LED is ON. LED cannot be ON all the time between two trigger pulses (\ref PARAM_REG_PULSE_FREQ).
/// To achieve this see chapter \ref subsection3_2.
/// \li <b>Parameter type:</b>  int
/// \li <b>Unit:</b> Microseconds
/// \li <b>Values</b> : Values should be greater than 1.
/// </summary>
#define PARAM_LED_DURATION "led_duration"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FlipX0
//! </pre>
/// Set the X0[um] of flip x. If \ref PARAM_FLIP_X_ENABLED is enabled, the X[um] of the points (X[um], Z[um]) are flipped with respect to X0.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_FLIP_X_X0 "flip_x_x0"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FlipXEnabled
//! </pre>
/// Determines if flip x is enabled or not. See \ref PARAM_FLIP_X_X0 on how to define X0.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : 1 enabled, 0 disabled.
/// </summary>
#define PARAM_FLIP_X_ENABLED "flip_x_enabled"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FlipZ0
//! </pre>
/// Set the Z0[um] of flip z. If \ref PARAM_FLIP_Z_ENABLED is enabled, the Z[um] of the points (X[um], Z[um]) are flipped with respect to Z0.
/// \li <b>Parameter type:</b> float
/// \li <b>Unit</b> : Micrometers
/// </summary>
#define PARAM_FLIP_Z_Z0 "flip_z_z0"

/// <summary>
//! <pre>
/// <b>C# name:</b>  SensorParameter.FlipZEnabled
//! </pre>
/// Determines if flip z is enabled or not. See \ref PARAM_FLIP_Z_Z0 on how to define Z0.
/// \li <b>Parameter type:</b> int
/// \li <b>Unit</b> : 1 enabled, 0 disabled.
/// </summary>
#define PARAM_FLIP_Z_ENABLED "flip_z_enabled"

/*! @} */

