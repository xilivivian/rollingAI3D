#pragma once

#ifndef __CAMERA_DLL_H
#define __CAMERA_DLL_H

#define EXPORTING_DLL   //新增

#include "PeakStructure.h"
#include "Callback.h"

#ifdef _WIN32
#ifdef EXPORTING_DLL

extern "C" void _declspec (dllexport) _Close();
extern "C" int _declspec (dllexport) _GetSerialNumber(char *cameraID, char *serialNumber);
extern "C" int _declspec (dllexport) _GetDeviceVersion(char *cameraID, char *deviceVersion);
extern "C" int _declspec (dllexport) _Open(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int _declspec (dllexport) _Discover(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int _declspec (dllexport) _Connect(char *cameraId, char *ipAddress);
extern "C" int _declspec (dllexport) _GetCameraID(int sensorIndex, char *sensorID);
extern "C" int _declspec (dllexport) _UseVirtualCamera(char *pathToPoints, char separator);
extern "C" int _declspec (dllexport) _StartGrabbing(char *cameraID);
extern "C" int _declspec (dllexport) _StopGrabbing(char *cameraID);

extern "C" int _declspec (dllexport) _SetRawImageCallback(char *cameraID, RawImageCallback callback);
extern "C" int _declspec (dllexport) _SetProfileCallback(char *cameraID, ProfileCallback callback);
extern "C" int _declspec (dllexport) _SetLineCallback(char *cameraID,int layerId, LineCallback callback);
extern "C" int _declspec (dllexport) _SetBatchCallback(char *cameraID,int layerId, BatchCallback callback);

extern "C" int _declspec (dllexport) _IsConnected(char *cameraID);
extern "C" int _declspec (dllexport) _SetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int _declspec (dllexport) _SetIntParameter(char *cameraID, char *paramName, int value);
extern "C" int _declspec (dllexport) _SetFloatParameter(char *cameraID, char *paramName, float value);
extern "C" int _declspec (dllexport) _SetDoubleParameter(char *cameraID, char *paramName, double value);
extern "C" int _declspec (dllexport) _GetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int _declspec (dllexport) _GetIntParameter(char *cameraID, char *paramName, int *value);
extern "C" int _declspec (dllexport) _GetFloatParameter(char *cameraID, char *paramName, float *value);
extern "C" int _declspec (dllexport) _GetDoubleParameter(char *cameraID, char *paramName, double *value);
extern "C" int _declspec (dllexport) _SetLineSortingOrder(char *cameraID, int sortingOrder);

extern "C" int _declspec (dllexport) _FieldCalibrationStart(char* cameraID, char* xCalibFile, char *zCalibFile, double* grooveHeightsExpected, int numOfGrooveHeightsExpected, FieldCalibrationResults* results);
extern "C" int _declspec (dllexport) _FieldCalibrationSave(char* cameraId, char* fileName);

extern "C" int _declspec (dllimport) _DynamicSensorCtrlParameters(char* camera_id, int param_set_count, DynSensorControl* control_parameters);
extern "C" int _declspec (dllimport) _GetDynamicSensorCtrlParameters(char* cameraID, int* param_set_count, DynSensorControl* control_parameters);

extern "C" int _declspec (dllexport) _GetParameterSupport(char* cameraID, char* paramName, bool* is_supported);
extern "C" int _declspec (dllexport) _AdjustRoiAndFps(char* cameraId, int operation, int inValue, int* outValue);
extern "C" int _declspec (dllexport) _SetLayerIntParameter(char* cameraID, int layerId, char* paramName, int value);
extern "C" int _declspec (dllexport) _SetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float value);
extern "C" int _declspec (dllexport) _GetLayerIntParameter(char* cameraID, int layerId, char* paramName, int* value);
extern "C" int _declspec (dllexport) _GetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float* value);

extern "C" int _declspec (dllexport) _SetPeakDetectionParameters(char* cameraID, MaterialType type, DetectionSensitivity sensitivity);

extern "C" int _declspec (dllexport) _IntensityCalibrationStart(char* cameraID, char* xCalibrationFile, char* zCalibrationFile, char* intensityCalibrationFile, bool smoothHorizontal, double gain);
extern "C" double _declspec (dllexport) _IntensityCalibrationProgress(char* cameraID, char* progressBitmapFile);
extern "C" void _declspec (dllexport) _IntensityCalibrationStop(char* cameraID, char* correctedBitmapFile);
extern "C" bool _declspec (dllexport) _IntensityCalibrationSave(char* cameraID, char* outputFile);

extern "C" int _declspec (dllexport) _ClearBatchBuffers(char* cameraID);
extern "C" int _declspec (dllexport) _ConvertPixelsToMicrometers(char* camera_id, float* x_value, float* z_value);

extern "C" int _declspec (dllexport) _ZCompensationCalibrationStart(char* cameraID, int direction);
extern "C" void _declspec (dllexport) _ZCompensationCalibrationStartNewLevel(char* cameraID, int direction);
extern "C" void _declspec (dllexport) _ZCompensationCalibrationProgress(char* cameraID, char* progressBitmapFile, double* levelsX, int levelsMaxX, int* levelsCountX, double* levelsY, int levelsMaxY, int* levelsCountY);
extern "C" bool _declspec (dllexport) _ZCompensationCalibrationSave(char* cameraID, char* outputFile);
extern "C" int _declspec (dllexport) _ZCompensationCalibrationGetStatus(char* cameraId);
extern "C" bool _declspec (dllexport) _ZCompensationCalibrationRemoveLatest(char* cameraID, int direction);
extern "C" bool _declspec (dllexport) _ZCompensationCalibrationVisualizeResult(char* calibrationFile, int direction, char* darkToBrightFile, char* brightToDarkFile);
extern "C" bool _declspec (dllexport) _ZCompensationCalibrationVisualizeCurrent(char* cameraID, int direction, char* darkToBrightFile, char* brightToDarkFile);
#else

extern "C" void __declspec (dllimport) _Close();
extern "C" int __declspec (dllimport) _GetSerialNumber(char *cameraID, char *serialNumber);
extern "C" int __declspec (dllimport) _GetDeviceVersion(char *cameraID, char *deviceVersion);
extern "C" int __declspec (dllimport) _Open(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int __declspec (dllimport) _Discover(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int __declspec (dllimport) _Connect(char *cameraId, char *ipAddress);
extern "C" int __declspec (dllimport) _GetCameraID(int sensorIndex, char *sensorID);
extern "C" int __declspec (dllimport) _UseVirtualCamera(char *pathToPoints, char separator);
extern "C" int __declspec (dllimport) _StartGrabbing(char *cameraID);
extern "C" int __declspec (dllimport) _StopGrabbing(char *cameraID);

extern "C" int __declspec (dllimport) _SetRawImageCallback(char *cameraID, RawImageCallback callback);
extern "C" int __declspec (dllimport) _SetProfileCallback(char *cameraID, ProfileCallback callback);
extern "C" int __declspec (dllimport) _SetLineCallback(char *cameraID, int layerId, LineCallback callback);
extern "C" int __declspec (dllimport) _SetBatchCallback(char *cameraID, int layerId, BatchCallback callback);

extern "C" int __declspec (dllimport) _IsConnected(char *cameraID);
extern "C" int __declspec (dllimport) _SetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int __declspec (dllimport) _SetIntParameter(char *cameraID, char *paramName, int value);
extern "C" int __declspec (dllimport) _SetFloatParameter(char *cameraID, char *paramName, float value);
extern "C" int __declspec (dllimport) _SetDoubleParameter(char *cameraID, char *paramName, double value);
extern "C" int __declspec (dllimport) _GetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int __declspec (dllimport) _GetIntParameter(char *cameraID, char *paramName, int *value);
extern "C" int __declspec (dllimport) _GetFloatParameter(char *cameraID, char *paramName, float *value);
extern "C" int __declspec (dllimport) _GetDoubleParameter(char *cameraID, char *paramName, double *value);
extern "C" int __declspec (dllimport) _SetLineSortingOrder(char *cameraID, int sortingOrder);

extern "C" int __declspec (dllimport) _FieldCalibrationStart(char* cameraID, char* xCalibFile, char *zCalibFile, double* grooveHeightsExpected, int numOfGrooveHeightsExpected, FieldCalibrationResults* results);
extern "C" int __declspec (dllimport) _FieldCalibrationSave(char* cameraId, char* fileName);

extern "C" int __declspec (dllimport) _DynamicSensorCtrlParameters(char* camera_id, int param_set_count, DynSensorControl* control_parameters);
extern "C" int __declspec (dllimport) _GetDynamicSensorCtrlParameters(char* cameraID, int* param_set_count, DynSensorControl* control_parameters);


extern "C" int __declspec (dllimport) _GetParameterSupport(char *cameraID, char *paramName, bool *is_supported);
extern "C" int __declspec (dllimport) _AdjustRoiAndFps(char* cameraId, int operation, int inValue, int* outValue);
extern "C" int __declspec (dllimport) _SetLayerIntParameter(char* cameraID, int layerId, char* paramName, int value);
extern "C" int __declspec (dllimport) _SetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float value);
extern "C" int __declspec (dllimport) _GetLayerIntParameter(char* cameraID, int layerId, char* paramName, int* value);
extern "C" int __declspec (dllimport) _GetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float* value);

extern "C" int __declspec (dllimport) _SetPeakDetectionParameters(char* cameraID, MaterialType type, DetectionSensitivity sensitivity);

extern "C" int __declspec (dllimport) _IntensityCalibrationStart(char* cameraID, char* xCalibrationFile, char* zCalibrationFile, char* intensityCalibrationFile, bool smoothHorizontal, double gain);
extern "C" double __declspec (dllimport)_IntensityCalibrationProgress(char* cameraID, char* progressBitmapFile);
extern "C" void __declspec (dllimport)_IntensityCalibrationStop(char* cameraID, char* correctedBitmapFile);
extern "C" bool __declspec (dllimport)_IntensityCalibrationSave(char* cameraID, char* outputFile);

extern "C" int __declspec (dllimport)_ClearBatchBuffers(char* cameraID);
extern "C" int __declspec (dllimport) _ConvertPixelsToMicrometers(char* camera_id, float* x_value, float* z_value);

extern "C" int __declspec (dllimport) _ZCompensationCalibrationStart(char* cameraID, int direction);
extern "C" void __declspec (dllimport) _ZCompensationCalibrationStartNewLevel(char* cameraID, int direction);
extern "C" void __declspec (dllimport) _ZCompensationCalibrationProgress(char* cameraID, char* progressBitmapFile, double* levelsX, int levelsMaxX, int* levelsCountX, double* levelsY, int levelsMaxY, int* levelsCountY);
extern "C" bool __declspec (dllimport) _ZCompensationCalibrationSave(char* cameraID, char* outputFile);
extern "C" int __declspec (dllimport) _ZCompensationCalibrationGetStatus(char* cameraID);
extern "C" bool __declspec (dllimport) _ZCompensationCalibrationRemoveLatest(char* cameraID, int direction);
extern "C" bool _declspec (dllimport) _ZCompensationCalibrationVisualizeResult(char* calibrationFile, int direction, char* darkToBrightFile, char* brightToDarkFile);
extern "C" bool _declspec (dllimport) _ZCompensationCalibrationVisualizeCurrent(char* cameraID, int direction, char* darkToBrightFile, char* brightToDarkFile);


#endif
#endif /* _WIN32 */

#ifndef _WIN32
extern "C" void _Close();
extern "C" int _GetSerialNumber(char *cameraID, char *serialNumber);
extern "C" int _GetDeviceVersion(char *cameraID, char *deviceVersion);
extern "C" int _Open(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int _Discover(int *cameraCount, char*** cameraIds, int timeout);
extern "C" int _Connect(char *cameraId, char *ipAddress);
extern "C" int _GetCameraID(int sensorIndex, char *sensorID);
extern "C" int _UseVirtualCamera(char *pathToPoints, char separator);
extern "C" int _StartGrabbing(char *cameraID);
extern "C" int _StopGrabbing(char *cameraID);

extern "C" int _SetRawImageCallback(char *cameraID, RawImageCallback callback);
extern "C" int _SetProfileCallback(char *cameraID, ProfileCallback callback);
extern "C" int _SetLineCallback(char *cameraID,int layerId, LineCallback callback);
extern "C" int _SetBatchCallback(char *cameraID, int layerId, BatchCallback callback);
extern "C" int _IsConnected(char *cameraID);
extern "C" int _SetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int _SetIntParameter(char *cameraID, char *paramName, int value);
extern "C" int _SetFloatParameter(char *cameraID, char *paramName, float value);
extern "C" int _SetDoubleParameter(char *cameraID, char *paramName, double value);
extern "C" int _GetStringParameter(char *cameraID, char *paramName, char *value);
extern "C" int _GetIntParameter(char *cameraID, char *paramName, int *value);
extern "C" int _GetFloatParameter(char *cameraID, char *paramName, float *value);
extern "C" int _GetDoubleParameter(char *cameraID, char *paramName, double *value);
extern "C" int _GetParameterSupport(char *cameraID, char *paramName, bool *is_supported);
extern "C" int _SetLineSortingOrder(char *cameraID, int sortingOrder);

extern "C" int _FieldCalibrationStart(char* cameraID, char* xCalibFile, char *zCalibFile, double* grooveHeightsExpected, int numOfGrooveHeightsExpected, FieldCalibrationResults* results);
extern "C" int _FieldCalibrationSave(char* cameraId, char* fileName);

extern "C" int _DynamicSensorCtrlParameters(char* camera_id, int param_set_count, DynSensorControl* control_parameters);
extern "C" int _GetDynamicSensorCtrlParameters(char* cameraID, int* param_set_count, DynSensorControl* control_parameters);

extern "C" int _AdjustRoiAndFps(char* cameraId, int operation, int inValue, int* outValue);

extern "C" int _SetLayerIntParameter(char* cameraID, int layerId, char* paramName, int value);
extern "C" int _SetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float value);
extern "C" int _GetLayerIntParameter(char* cameraID, int layerId, char* paramName, int* value);
extern "C" int _GetLayerFloatParameter(char* cameraID, int layerId, char* paramName, float* value);
extern "C" int _SetPeakDetectionParameters(char* cameraID, MaterialType type, DetectionSensitivity sensitivity);

extern "C" int _IntensityCalibrationStart(char* cameraID, char* xCalibrationFile, char* zCalibrationFile, char* intensityCalibrationFile, bool smoothHorizontal, double gain);
extern "C" double _IntensityCalibrationProgress(char* cameraID, char* progressBitmapFile);
extern "C" void _IntensityCalibrationStop(char* cameraID, char* correctedBitmapFile);
extern "C" bool _IntensityCalibrationSave(char* cameraID, char* outputFile);

extern "C" int _ClearBatchBuffers(char* cameraID);
extern "C" int _ConvertPixelsToMicrometers(char* camera_id, float* x_value, float* z_value);

extern "C" int _ZCompensationCalibrationStart(char* cameraID, int direction);
extern "C" void _ZCompensationCalibrationStartNewLevel(char* cameraID, int direction);
extern "C" void _ZCompensationCalibrationProgress(char* cameraID, char* progressBitmapFile, double* levelsX, int levelsMaxX, int* levelsCountX, double* levelsY, int levelsMaxY, int* levelsCountY);
extern "C" bool _ZCompensationCalibrationSave(char* cameraID, char* outputFile);
extern "C" int _ZCompensationCalibrationGetStatus(char* cameraID);
extern "C" bool _ZCompensationCalibrationRemoveLatest(char* cameraID, int direction);
extern "C" bool _ZCompensationCalibrationVisualizeResult(char* calibrationFile, int direction, char* darkToBrightFile, char* brightToDarkFile);
extern "C" bool _ZCompensationCalibrationVisualizeCurrent(char* cameraID, int direction, char* darkToBrightFile, char* brightToDarkFile);

#endif


#endif /* __CAMERA_DLL_H */

