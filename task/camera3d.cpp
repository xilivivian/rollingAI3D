#include "camera3d.h"

#include <QDebug>

#include <QTime>
#include "emitobject.h"

#include "ConsoleExample/ConsoleExample.h"
#include "API/PeakStructure.h"
#include "API/CameraDll.h"
#include "API/CameraStatus.h"
#include "API/VevoParameterDefinitions.h"
#include "ConsoleExample/FrameBuffer.h"

#include <cstdio>
#include <cfloat>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <cstddef>
#include <chrono>
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include "resultdata.h"

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#include <conio.h>
#endif

// Include the import lib for the FSAPI.
#ifdef _WIN64
#pragma comment(lib, "../API/x64/vevo.lib")
#else
//#pragma comment(lib, "../API/vevo.lib")
#pragma comment(lib, "vevo.lib")
#endif

#ifdef __linux__
#include <linux/limits.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <string.h>
#ifndef MAX_PATH
#define MAX_PATH PATH_MAX
#endif
#ifndef errno_t
#define errno_t int
#endif

// Help from:
// https://stackoverflow.com/questions/29335758/using-kbhit-and-getch-on-linux
// http://man7.org/linux/man-pages/man3/termios.3.html
bool _kbhit() {
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;       // disable canonical mode to get characters immediately
    tcsetattr(0, TCSANOW, &term2);  // set attributes; flag TCSANOW = apply immediately

    int bytesWaiting = 0;
    ioctl(0, FIONREAD, &bytesWaiting);

    tcsetattr(0, TCSANOW, &term);   // restore terminal settings
    return (bytesWaiting > 0);
}

#ifndef _getch
#define _getch getchar
#endif

/**
 * @brief Compatible implementation of Microsoft fopen_s()
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/fopen-s-wfopen-s?view=vs-2019
 */
errno_t fopen_s(FILE** f, const char* fname, const char* mode) {
    if (f && fname && mode) {
        *f = fopen(fname, mode);
        return 0;
    }
    return EINVAL;
}

#endif  // __linux__

#ifdef min
#undef min
#endif

#define OPEN_TIMEOUT 2000
#define MAX_CAM_SERIAL_LEN 1024
#define MAX_CAM_DEVICE_LEN 1024
#define SENSOR_OUTPUT_MODE 1

#define QUIT '1'
#define START_3D '2'
#define START_RAW '3'
#define START_LINE '4'
#define START_BATCH '5'
#define STOP '6'
#define START_GRABBING '7'
#define STOP_GRABBING '8'

#define GUIDE "Enter command:\n\n1 = quit, 2 = start 3D profiles, 3 = start raw image, 4 = start line interface, 5 = start batch run, 6 = stop callback, 7 = start grabbing, 8 = stop grabbing"


float* zValuesCopy_Batch = NULL ;
float* intensityValuesCopy_Batch = NULL;
int image_width_Batch;
int image_height_Batch;
double xStep_Batch;
double yStep_Batch;
HEADER *headersCopy_Batch = NULL;

EmitObject emitObject;

EmitObject resultToPLCemitObject;






extern void ProfileCallbackHandler(char *cameraID, STRUCT_POINT *points, int *point_count, HEADER *header);
extern void RawImageCallbackHandler(char *cameraID, unsigned char *image, int *bytes, HEADER *header);
extern void LineCallbackHandler(char *cameraID, int layerId, float *zValues, float *intensityValues, int line_length, double xStep, HEADER *header);
extern void BatchCallbackHandler(char *cameraID, int layerId, float *zValues, float *intensityValues, int lineLength, int batchLength, double xStep, HEADER *lineHeaders);

void ShowHelpInfo();
int ProcessCmdAttributes(int argc, wchar_t* argv[]);

static void WriteToFile(const STRUCT_POINT *points, int point_count, uint32_t index);
static bool SaveBatchPointCloud(int layerId, const float *zValues, const float *intensityValues, int image_width, int image_height, double xStep, double yStep, const HEADER* lineHeaders);

std::vector<FrameBuffer*> g_frameBuffers;
unsigned int g_currentFrame = 0;


QTime timeXX;

/**
 * @brief Simple example application that prepares and starts data transmission from a measurement head using FocalSpec Camera API (FSAPI).
 *
 * @param	[in]	argc	Command line argument count.
 * @param	[in]	argv	Command line arguments.
 *
 * @return		0 if program executed OK, 1 in case of an error.
 */





int camera_count = 1;

char **camera_ids = nullptr;

char camera_device_version[MAX_CAM_DEVICE_LEN];

bool run = true;
char recipeName[512];



int initCamera()
{




    puts("Discovering sensors...");
    int initStat = 0;
    auto status = static_cast<CameraStatus>(_Open(&camera_count, &camera_ids, OPEN_TIMEOUT));

    if (status == CameraStatus::CAMERA_ERROR_API_ALREADY_LOADED)
    {
        puts("FSAPI is already loaded. Close running FSAPI applications.");
        return 1;
    }

    if (status == CameraStatus::CAMERA_ERROR_MISSING_LIBRARY_FILES)
    {
        puts("Could not find all required library files.");
        return 1;
    }

    if (camera_count > 0)
    {
        puts("Following sensors were found:");

        for (int i = 0; i < camera_count; i++)
        {
            ; //printf("%s\n", camera_ids[i]);
        }
    }
    else
    {
        puts("No sensors found. Check instructions from ConsoleExample README.md file.");
        return 1;
    }

    // Assign IPv4 address and get camera ID, which we are going to use later on when communicating with a specific camera.
    status = static_cast<CameraStatus>(_Connect(camera_ids[0], EXAMPLE_IP_ADDR));
    if (status == CameraStatus::CAMERA_ERROR_SELFTEST_GRABBING_FAILED)
    {
         //printf("Sensor self test failed.\n");

         //printf("Please check that the VEVO filter driver is correctly installed and enabled on the ethernet card.\n");
         //printf("From Ethernet adapter enable Jumbo Frames by setting the value 9014 Bytes for the Packet size and set Receive Buffer Size to 2048.\n");

        return 1;
    }

    if (status != CameraStatus::CAMERA_OK)
    {
         //printf("ERROR: failed to connect to sensor (FSAPI error: %d)\n", status);
        return 1;
    }

    // Get the basic information of the camera. Notice how we use camera ID in each command.
    status = static_cast<CameraStatus>(_GetDeviceVersion(camera_ids[0], camera_device_version));
    if (status != CameraStatus::CAMERA_OK)
    {
         //printf("FSAPI error: %d\n", status);
        return 1;
    }
     //printf("Sensor information\n");
     //printf("- Identifier     : %s\n", camera_ids[0]);
     //printf("- Device version : %s\n", camera_device_version);

    int calibrations_in_camera = 0;
    status = static_cast<CameraStatus>(_GetIntParameter(camera_ids[0], const_cast<char*>(PARAM_SENSOR_DATA_IN_FLASH), &calibrations_in_camera));
    if (status != CameraStatus::CAMERA_OK)
    {
         //printf("FSAPI error: %d. Failed to check whether calibration files are found in sensor.\n", status);
        return 1;
    }

    // if calibrations not found from camera, needs to be defined.
    if(!calibrations_in_camera)
    {
         //printf("Calibration files not in sensor, must be provided by user\n");
        status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_SENSOR_CALIBRATION_FILE), z_calib_file));

        if (status != CameraStatus::CAMERA_OK && !(status == CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_NOT_FOUND || status == CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_INVALID))
        {
             //printf("FSAPI error: %d. Failed to set Z calibration file.\n", status);
            return 1;
        }

        status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_SENSOR_X_CALIBRATION_FILE), x_calib_file));

        if (status != CameraStatus::CAMERA_OK && !(status == CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_NOT_FOUND || status == CAMERA_ERROR_CALIB_FILE_ATTRIBUTE_INVALID))
        {
             //printf("FSAPI error: %d Failed to set X calibration file.\n", status);
            return 1;
        }
    }
    int sensor_type;
    status = static_cast<CameraStatus>(_GetIntParameter(camera_ids[0], const_cast<char*>(PARAM_SENSOR_TYPE), &sensor_type));
    if (status != CAMERA_OK)
    {
         //printf("PARAM_SENSOR_TYPE FSAPI error: %d\n", status);
        return 1;
    }
    /*
       // This is sample code section for dynamic sensor control parameter functionality
       // from 1 to max 10 parameter sets
       DynSensorControl my_dynamic_control[] =
       { // location,	height, offset, pulse width and threshold (for 10GbE camera)
           {10000,		200,	0,	10.0, 15},
           {20000,		500,	0,	20.0, 20},
           {30000,		500,	0,	10.0, 25},
           {40000,		300,	0,	40.0,  0},
           {50000,		100,	0,	   0, 30}
       };

       const int parameter_set_count = sizeof(my_dynamic_control)/ sizeof(my_dynamic_control[0]);

       for (auto i=0; i < parameter_set_count; i++)
       {
           // Offset not given in table but calculated from sensor image height
           if(my_dynamic_control[i].dyn_ctrl_height > 0)
               my_dynamic_control[i].dyn_ctrl_offset = (CAMERA_SENSOR_HEIGHT - my_dynamic_control[i].dyn_ctrl_height) / 2;
       }

       // Pass parameters to the sensor using dedicated interface function
       status = static_cast<CameraStatus>(_DynamicSensorCtrlParameters(camera_ids[0], parameter_set_count, my_dynamic_control));
       if (status != CameraStatus::CAMERA_OK)
       {
            //printf("FSAPI error: %d\n", status);
           return 1;
       }

       // activate functionality, by default it is deactivated
       status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_DYN_SENSOR_CTRL_ENABLE), 1));
       if (status != CameraStatus::CAMERA_OK)
       {
            //printf("FSAPI error: %d\n", status);
           return 1;
       }
       // <end>
       */
    // char recipeName[512];
    // snprintf(recipeName, 512, "ConsoleRecipe_%d", sensor_type);
    snprintf(recipeName, 512, "2022-01-12");
    status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_LOAD_RECIPE), recipeName));
    if (status != CameraStatus::CAMERA_OK)
    {
         //printf("Error loading recipe %s, trying to save a new recipe with same name\n", recipeName);

        status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_SAVE_RECIPE), recipeName));
        if (status != CameraStatus::CAMERA_OK) {
            ; //printf("Error saving recipe %s, continuing anyway\n", recipeName);
        } else {
             ;//printf("Saved new recipe %s\n", recipeName);
        }
    }
    else {
         //printf("Recipe loaded from %s, parameters overwritten\n", recipeName);
    }

    for (int buffers=0; buffers < NUM_FRAME_BUFFERS; buffers++)
    {
        g_frameBuffers.push_back(new FrameBuffer(imageWidth, DEFAULT_SAVED_IMAGE_HEIGHT));
    }

    //~~~~~~~下面配置批处理外部触发，外部触发不需要软件启动相机拍照~~~~~~~~~~~~~~~~~~
#if 1
    int recipeBatchLength = 0;
    _GetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), &recipeBatchLength);
    if (recipeBatchLength <= 0)
    {
        batchLength = DEFAULT_BATCH_LENGTH;
    }
    else
        batchLength = recipeBatchLength;



    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), SET_BATCH_LENGTH);

    // //printf("Starting batch run, batch length: %d\n", );
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    //~~~~~~~~~~为外部触发 新增 3行代码
    int ZMimage_height = 1;
    _AdjustRoiAndFps(camera_ids[0],0,SET_BATCH_FREQ,&ZMimage_height);//设置频率900
    status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0],const_cast<char*>("reg_pulse_freq"),0));//0:外触发

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
    _SetLineSortingOrder(camera_ids[0], SurfaceSorting::FROM_TOP_TO_BOTTOM);

    status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_X_DIRECTION), 1));
    if (status == CAMERA_OK)
    {
        ;
    }else
    {
       initStat = 1;
    }

         //printf("Z Compensation X direction enabled\n");

    status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_Y_DIRECTION), 1));
    if (status == CAMERA_OK)
        ;
         //printf("Z Compensation Y direction enabled\n");

    bool callbackOk = true;
    for (int i = 0; i < layerCount; i++)
    {
        //ZM:设置回调，ONLY 1
        status = static_cast<CameraStatus>(_SetBatchCallback(camera_ids[0], i, BatchCallbackHandler));
        if (status != CAMERA_OK)
        {
            callbackOk = false;
            initStat = 1;
             //printf("Batch callback set failed\n");
            break;
        }
    }
    if (callbackOk)
        _StartGrabbing(camera_ids[0]);
#endif
#if 0
    puts(GUIDE);
    while (run)
    {
        switch (_getch())
        {
        case QUIT:
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            _SetProfileCallback(camera_ids[0], nullptr);
            _SetLineCallback(camera_ids[0], 0, nullptr);
            status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_SAVE_RECIPE), recipeName));
            if (status != CameraStatus::CAMERA_OK) {
                 //printf("Error saving recipe %s, continuing anyway\n", recipeName);
            } else {
                 //printf("Saved recipe %s\n", recipeName);
            }

            run = false;
            break;

        case START_3D:
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
            _SetProfileCallback(camera_ids[0], ProfileCallbackHandler);
            _StartGrabbing(camera_ids[0]);
            break;

        case START_RAW:
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 0);
            _SetRawImageCallback(camera_ids[0], RawImageCallbackHandler);
            _StartGrabbing(camera_ids[0]);
            break;

        case START_LINE:
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
            _SetLineSortingOrder(camera_ids[0], SurfaceSorting::FROM_TOP_TO_BOTTOM);
            // Line callback function for the first layer
            // Must be called after x-calibration file is set
            _SetLineCallback(camera_ids[0], 0, LineCallbackHandler);

            _StartGrabbing(camera_ids[0]);
            break;

        case START_BATCH: {
            // - Batch length may be 0 from recipe (default value). That won't work, will cause segmentation fault.
            // - If user has not set batch length from command line, it will be the default value.
            // - In case batch length is still 0 (invalid value given by user), use default value.
            int recipeBatchLength = 0;
            _GetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), &recipeBatchLength);
            if (recipeBatchLength <= 0)
            {
                batchLength = DEFAULT_BATCH_LENGTH;
            }
            else
                batchLength = recipeBatchLength;

            _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), batchLength);

             //printf("Starting batch run, batch length: %d\n", batchLength);
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
            _SetLineSortingOrder(camera_ids[0], SurfaceSorting::FROM_TOP_TO_BOTTOM);

            status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_X_DIRECTION), 1));
            if (status == CAMERA_OK)
                 //printf("Z Compensation X direction enabled\n");

            status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_Y_DIRECTION), 1));
            if (status == CAMERA_OK)
                 //printf("Z Compensation Y direction enabled\n");

            bool callbackOk = true;
            for (int i = 0; i < layerCount; i++)
            {
                status = static_cast<CameraStatus>(_SetBatchCallback(camera_ids[0], i, BatchCallbackHandler));
                if (status != CAMERA_OK)
                {
                    callbackOk = false;
                     //printf("Batch callback set failed\n");
                    break;
                }
            }
            if (callbackOk)
                _StartGrabbing(camera_ids[0]);
        }
            break;
        case STOP:
            _StopGrabbing(camera_ids[0]);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            _SetProfileCallback(camera_ids[0], nullptr);
            _SetLineCallback(camera_ids[0], 0, nullptr);

            puts(GUIDE);
            break;
        case STOP_GRABBING:
            _StopGrabbing(camera_ids[0]);
            puts(GUIDE);
            break;

        case START_GRABBING:
            _StartGrabbing(camera_ids[0]);
            break;

        default:
            break;
        }
    }

    _Close();

    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    for (auto element : g_frameBuffers)
    {
        delete element;
    }
#endif
    return initStat;
}



/**
    * @brief Receives point clouds from the camera library. Called from the camera library thread.
    *
    * @param	[in]	cameraID			Identifier of the camera that sent the points.
    * @param	[in]	points				Pointer to the point data.
    * @param	[in]	point_count			Number of points.
    * @param	[in]	header      		Profile header.
    *
    * @return		-
    */
void ProfileCallbackHandler(char *cameraID, STRUCT_POINT *points, int *point_count, HEADER *header)
{
    float min_x = FLT_MAX;
    float max_x = FLT_MIN;

    // Scan point cloud data to find minimum and maximum X.
    for (int i = 0; i < *point_count; i++)
    {
        min_x = (min_x < points[i].X) ? min_x : points[i].X;
        max_x = (max_x > points[i].X) ? max_x : points[i].X;
    }
    if (min_x == FLT_MAX)
        min_x = 0;
    if (max_x == FLT_MIN)
        max_x = 0;

    // Write line profile to csv on every 10th second
    if (header->reception_frequency > 10 && (header->index % static_cast<int>(header->reception_frequency * 10)) == 0)
        WriteToFile(points, *point_count, header->index);

    if (header->reception_frequency > 100)
    {
        // print only every 100th
        if ((header->index % 100) == 0)
            ;
             //printf("%d peaks received at %.2f Hz, index %d, X range %.2f-%.2f. Items in queue: %d.\n", *point_count, header->reception_frequency, header->index, min_x, max_x, header->reception_queue_size);
    }
    else
        ; //printf("%d peaks received at %.2f Hz, index %d, X range %.2f-%.2f. Items in queue: %d.\n", *point_count, header->reception_frequency, header->index, min_x, max_x, header->reception_queue_size);
}

/**
    * @brief Receives raw images from the camera library. Called from the camera library thread.
    *
    * @param	[in]	cameraID			Identifier of the camera that sent the image data.
    * @param	[in]	image				Raw image data.
    * @param	[in]	bytes				Number of bytes in the image data.
    * @param	[in]	header				Information about the image data.
    *
    * @return		-
    */
void RawImageCallbackHandler(char *cameraID, unsigned char *image, int *bytes, HEADER *header)
{
     //printf("%d bytes received\n", *bytes);
}

/**
    * @brief Receives configured layers as array of float values from the camera library. Called from the camera library thread.
    * @param cameraID			Identifier of the camera that sent the line
    * @param layerId			Layer identifier (0 is the first layer)
    * @param zValues			Array of heights
    * @param intensityValues	Array of intensity values
    * @param line_length		Length of the array
    * @param xStep				X resolution in micrometers
    * @param header			Header information about the scanned line
    */
void LineCallbackHandler(char *cameraID, int layerId, float *zValues, float *intensityValues, int line_length, double xStep, HEADER *header)
{
    if (layerId > 0) // no processing for other layers
        return;

    if (header->reception_frequency>100 || header->reception_frequency==0)
    {
        // print only every 1000th line
        if ((header->index % 10000) == 0)
        {
             //printf("10000 lines received (#%d), frame rate %.2lf FPS, queue: %d, pulse width: %.2lf\n", header->index, header->reception_frequency, header->reception_queue_size, header->pulse_width);
        }
    }
    else
    {
         //printf("Line #%d received, frame rate %.2lf FPS, pulse width: %.2lf\n", header->index, header->reception_frequency, header->pulse_width);
    }

    // Copy arrays to the frame buffer
    const auto copySucceed = g_frameBuffers[g_currentFrame]->CopyLine(zValues, intensityValues, line_length, xStep, header);

    // and save the image when all rows are received
    if (g_frameBuffers[g_currentFrame]->IsFull())
    {
        // Output file name. Extension is created automatically
        std::string fileName = "Frame" + std::to_string(g_currentFrame);
        std::string path;

        g_frameBuffers[g_currentFrame]->TriggerSave(path, fileName);

        g_currentFrame++;
        if (g_currentFrame >= g_frameBuffers.size())
        {
            g_currentFrame = 0;
        }

        // copy was failed since the frame was full
        if (!copySucceed)
        {
            g_frameBuffers[g_currentFrame]->CopyLine(zValues, intensityValues, line_length, xStep, header);
        }
    }
}

/**
    * @brief Receives configured layers as array of float values from the camera library. Called from the camera library thread.
    * @param cameraID			Identifier of the camera that sent the line
    * @param layerId			Layer identifier (0 is the first layer)
    * @param zValues			Array of heights
    * @param intensityValues	Array of intensity values
    * @param lineLength		Length of the array
    * @param batchLength      Line profile count
    * @param xStep				X resolution in micrometers
    * @param lineHeaders 		Profile header information for all the scanned lines on batch.
    */
void BatchCallbackHandler(char *cameraID, int layerId, float *zValues, float *intensityValues, int lineLength, int batchLength, double xStep, HEADER *lineHeaders)
{

   /* qDebug()<<"2222222222222222222:";
    for (int i = 0; i < batchLength*2040; i++)
    {
    qDebug()<<"zValues"<<zValues[i]<<"intensityValues"<<intensityValues[i];
    }*/

    resultToPLCemitObject.emitCamearFinished();

    float yStep;
    _GetFloatParameter(cameraID, const_cast<char*>(PARAM_ENCODER_PULSE_WIDTH), &yStep);
    if (yStep == 0)
        yStep = 1.0;
#if 0
    auto* zValuesCopy = new float[lineLength * batchLength];
    auto* intensityValuesCopy = new float[lineLength * batchLength];
    auto* headersCopy = new HEADER[batchLength];
    memcpy(zValuesCopy, zValues, sizeof(float) * lineLength * batchLength);
    memcpy(intensityValuesCopy, intensityValues, sizeof(float) * lineLength * batchLength);
    memcpy(headersCopy, lineHeaders, sizeof(HEADER) * batchLength);
#else

    zValuesCopy_Batch = new float[lineLength * batchLength];/*new float[lineLength * batchLength]*/
    intensityValuesCopy_Batch = new float[lineLength * batchLength];/* new float[lineLength * batchLength]*/
    headersCopy_Batch = new HEADER[batchLength];
    memcpy(zValuesCopy_Batch, zValues, sizeof(float) * lineLength * batchLength);
    memcpy(intensityValuesCopy_Batch, intensityValues, sizeof(float) * lineLength * batchLength);
    memcpy(headersCopy_Batch, lineHeaders, sizeof(HEADER) * batchLength);
    image_width_Batch = lineLength ;
    image_height_Batch = batchLength;
    xStep_Batch = xStep;
    yStep_Batch = yStep;

    static int index= 0;
    index++;

    emitObject.emitCamearFinished();
#endif

    _ClearBatchBuffers(camera_ids[0]);
    qDebug()<<"_ClearBatchBuffers~~~~~~~~~~~~~~~~~~index:"<<index <<"lineLength:"<<lineLength<<"batchLength:"<<batchLength;
    // _StopGrabbing(camera_ids[0]);
    // std::this_thread::sleep_for(std::chrono::milliseconds(300));
    // _SetProfileCallback(camera_ids[0], nullptr);
    // _SetLineCallback(camera_ids[0], 0, nullptr);





/*qDebug()<<"33333333333333333333333333333333333333:";
qDebug()<<"zValuesCopy_Batch"<<zValuesCopy_Batch[111]<<zValuesCopy_Batch[222]<<zValuesCopy_Batch[345]<<zValuesCopy_Batch[999];
qDebug()<<"intensityValuesCopy_Batch"<<intensityValuesCopy_Batch[111]<<intensityValuesCopy_Batch[222]<<intensityValuesCopy_Batch[345]<<intensityValuesCopy_Batch[999];
*/
#if 0
    std::thread fileWriter(&SaveBatchPointCloud, layerId,
                           zValuesCopy, intensityValuesCopy, lineLength, batchLength, xStep, yStep, headersCopy);
    fileWriter.detach();

    int error_count = 0;
    float minEncoderStep = FLT_MAX;
    for (int i = 0; i < batchLength; i++)
    {
        if(lineHeaders[i].time_stamp == 0)
            error_count++;
        if (i)
        {
            const auto diff = lineHeaders[i].location - lineHeaders[i-1].location;
            minEncoderStep = std::min((float)diff, minEncoderStep);
        }
    }

    auto lostProfiles = 0;
    if (minEncoderStep > 0) // external triggering
    {
        for (int i = 1; i < batchLength; i++)
        {
            const auto diff = lineHeaders[i].location - lineHeaders[i - 1].location;
            if (diff != minEncoderStep)
                lostProfiles++;
        }
    }

     //printf("Batch for layer#%d (%0.3fHz), %d profiles completed (indexes from %d to %d), %d lines missing",
           layerId, lineHeaders[batchLength/2].reception_frequency, batchLength, lineHeaders[0].index, lineHeaders[batchLength-1].index, error_count);
    if (lostProfiles)
    {
         //printf(", lost profiles %d", lostProfiles);
    }
     //printf("\n");

#endif
}

/**
    * @brief Writes a point cloud into a CSV file (x;y;intensity) named using the running point cloud index.
    *
    * @param	[in]	points		Point cloud (array of points).
    * @param	[in]	point_count	Number of points in the cloud.
    * @param	[in]	index	    File index.
    *
    * @return		-
    */
void WriteToFile(const STRUCT_POINT *points, const int point_count, const uint32_t index)
{

    FILE *csv;
    char file_name[MAX_PATH] = {0};

    snprintf(file_name, MAX_PATH, "%d.csv", index);
    const errno_t err = fopen_s(&csv, file_name, "w");

    if (err != 0)
    {
        return;
    }

    for (int i = 0; i < point_count; i++)
    {
        fprintf(csv, "%f;%f;%f\n", points[i].X, points[i].Y, points[i].Intensity);
    }

    fclose(csv);

}

/**
    * @brief Saves point cloud into a file. This example used CSV format (x;y;z,intensity). File Named using batch_layerId# + image width + image height.
    * @param	[in]	layerId			Layer identifier (0 is the first layer)
    * @param	[in]    zValues			Array of heights
    * @param	[in]    intensityValues	Array of intensity values
    * @param	[in]    image_width		Equals to length of the profile array
    * @param	[in]    image_height    Equals to line profile count
    * @param	[in]    xStep			X resolution in micrometers
    * @param	[in]    yStep			Encoder resolution in micrometers
    * @param	[in]    lineHeaders		Header for each line
    * @return		boolean true if succeeded.
    */
bool SaveBatchPointCloud(int layerId, const float *zValues, const float *intensityValues, int image_width, int image_height, double xStep, double yStep, const HEADER* lineHeaders)
{
    static auto batchNumber = 0;
    const int max_column_pos = 50;

    const auto str = std::make_unique<char[]>(image_width * max_column_pos);
    double location = 0;
    const bool noEncoder = (lineHeaders[image_height - 1].location == lineHeaders[0].location);

    // skip other layers on this example code
    if(layerId == 0)
    {
        char yPosition[128];
        const std::string fullPath = "batch_" + std::to_string(batchNumber++) + "_layer#" + std::to_string(layerId) + "f.csv";
        ofstream ofs_cloud;
        ofs_cloud.open(fullPath, std::ofstream::out | ios::binary);
        if (ofs_cloud.fail())
        {
            return false;
        }

        for (auto j = 0; j < image_height; j++)
        {
            int pos=0;
            if (noEncoder)
                location += 10;
            else
                location = (double)lineHeaders[j].location * yStep / 2.0;

            snprintf(yPosition, sizeof(yPosition), ";%.2lf;", location);

            for (auto i = 0; i < image_width; i++)
            {
                auto intensity = intensityValues[j * image_width + i];
                auto zValue = zValues[j * image_width + i];
                if (NOT_MEASURED_POINT(intensity) || intensity < 0.1)
                {
                    intensity = 999;
                    continue;
                }
                //continue;
                if (NOT_MEASURED_POINT(fabs(zValue)))
                {
                    zValue = 999;
                    continue;
                }

                const auto x = static_cast<float>(xStep * i);
                const auto z = static_cast<float>(zValue);

                pos += snprintf(str.get() + pos, image_width * max_column_pos - pos, "%.2f%s%.2f;%.2f\r\n", x, yPosition, z, intensity);
            }

            ofs_cloud.write(str.get(), pos);
        }
        ofs_cloud.close();
         //printf("File %s saved.\n", fullPath.c_str());
    }
    else
    {
         ;//printf("Writing to file for layer#%d skipped.\n", layerId);
    }

    delete zValues;
    delete intensityValues;
    delete lineHeaders;

    return true;
}

/**
    * @brief ShowHelpInfo()	function that show the help information
    * @param
    */
void ShowHelpInfo()
{
    std::cout << "usage:   " << "ConsoleExample [-option] [argument]" << std::endl;
    std::cout << "option:  " << "-h show help information" << std::endl;
    std::cout << "         " << "-z path to Z calibration file" << std::endl;
    std::cout << "         " << "-x path to X calibration file" << std::endl;
    std::cout << "         " << "-f sensor frequency (Hz)" << std::endl;
    std::cout << "         " << "-d sensor led duration" << std::endl;
    std::cout << "         " << "-i image height in pixels" << std::endl;
    std::cout << "         " << "-r reordering enable: 1 / disable: 0" << std::endl;
    std::cout << "         " << "-p encoder pulse divider: pulse divider number / 0 = default (1)" << std::endl;
    std::cout << "         " << "-b batch run length in lines" << std::endl;
    std::cout << "         " << "-l layer count for line or batch run" << std::endl;
    std::cout << "example: " << "ConsoleExample -f 100 -d 10 -i 600" << std::endl;
}

/**
    * @brief WProcessCmdAttributes()	Class member parsing command line attributes.
    * @param	[in]	argc	Command line argument count.
    * @param	[in]	argv	Command line arguments.
    * @return		0 if program executed OK, 1 in case of an error.
    */
int ProcessCmdAttributes(int argc, wchar_t* argv[])
{
    // Read possible commandline attributes, skip command executable which is 1st attribute
    for (auto count = 1; count < argc; count++)
    {
        std::wstring ws = argv[count];
        const std::string option(ws.begin(), ws.end());  // C++ std::string TCHAR is wchar_t

        const SensorParam cmd_param = OptionToParam(option);
        if (cmd_param == enum_unknown)
        {
            std::cout << "Unknown command line attribute: " << option << std::endl;
            ShowHelpInfo();
            std::cin.ignore();
            return 1;
        }
        else if (cmd_param == enum_show_help)
        {
            ShowHelpInfo();
            std::cin.ignore();
            return 1;
        }
        // for valid attribute, value should follow next
        count++;
        ws = argv[count];
        const std::string s_value(ws.begin(), ws.end());
        auto param_value = std::stringstream(s_value);

        switch (cmd_param)
        {
        case enum_frequency:
            param_value >> sensorFrequency;
            std::cout << "Attribute: " << option << " SensorFrequency: " << s_value << " Hz" << std::endl;
            break;

        case enum_led_duration:
            param_value >> ledDuration;
            std::cout << "Attribute: " << option << " LedDuration: " << s_value << " us" << std::endl;
            break;

        case enum_x_calib_file:
            param_value >> x_calib_file;
            std::cout << "Attribute: " << option << " x_calib_file: " << s_value << std::endl;
            break;

        case enum_z_calib_file:
            param_value >> z_calib_file;
            std::cout << "Attribute: " << option << " z_calib_file: " << s_value << std::endl;
            break;

        case enum_image_height:
            param_value >> imageHeight;
            std::cout << "Attribute: " << option << " ImageHeight: " << s_value << " pixels" << std::endl;
            break;

        case enum_reordering_enable:
            param_value >> reorderingEnable;
            std::cout << "Attribute: " << option << " ReorderingEnable: " << s_value << std::endl;
            break;

        case enum_encoder_pulse_divider:
            param_value >> encoderPulseDivider;
            if (encoderPulseDivider > 0)
            {
                std::cout << "Attribute: " << option << " EncoderPulseDivider: " << s_value << std::endl;
            }
            else
            {
                encoderPulseDivider = SENSOR_PULSE_DIVIDER;
                std::cout << "Attribute: " << option << " EncoderPulseDivider: " << encoderPulseDivider << ", default value used" << std::endl;
            }
            break;

        case enum_layer_count:
            param_value >> layerCount;
            if (layerCount > 0 && layerCount < 11)
            {
                std::cout << "Attribute: " << option << " layerCount: " << s_value << std::endl;
            }
            else
            {
                layerCount = 1;
                std::cout << "Attribute: " << option << " layerCount: " << layerCount << ", default value used" << std::endl;
            }
            break;

        case enum_batch_length:
            param_value >> batchLength;
            if (batchLength > 0)
            {
                std::cout << "Attribute: " << option << " BatchLength: " << s_value << std::endl;
            }
            else
            {
                // no negative values allowed, 0 batch is disabled. NOTE: user can still try doing batch run. In that case,
                // set batch length to default value. Disable here so that batch buffers won't be allocated.
                batchLength = 0;
                std::cout << "Attribute: " << option << " BatchLength: " << batchLength << ", default value disabled applied" << std::endl;
            }
            break;

        default:
            std::cout << "Attribute: " << option << " NotDefined: " << s_value << std::endl;
            return 1;
        }
    }
    return 0;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


Camera3D::Camera3D(QObject *parent) : QObject(parent)
{

}

int Camera3D::initCamera3D()
{

    initStatus =initCamera();
    return  initStatus;
}

bool Camera3D::setupCamera3D()
{
    if(initStatus == 0)
    {

        timeXX.start();
        // focalSTART_BATCH();
        // focalSTART_LINE();
    }
    return  true;
}

bool Camera3D::stopCamera3D()
{
    focalSTOP();
    qDebug()<<"~~~~~~~~~~~~~~~~~~~~focalSTOP() time;::"<<timeXX.elapsed()/1000.0<<"ms";
    return true;
}

void Camera3D::focalQuit()
{
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    _SetProfileCallback(camera_ids[0], nullptr);
    _SetLineCallback(camera_ids[0], 0, nullptr);
    auto status = static_cast<CameraStatus>(_SetStringParameter(camera_ids[0], const_cast<char*>(PARAM_SAVE_RECIPE), recipeName));
    if (status != CameraStatus::CAMERA_OK) {
         //printf("Error saving recipe %s, continuing anyway\n", recipeName);
    } else {
         //printf("Saved recipe %s\n", recipeName);
    }

    run = false;
}

void Camera3D::focalSTART_3D()
{
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
    _SetProfileCallback(camera_ids[0], ProfileCallbackHandler);
    _StartGrabbing(camera_ids[0]);
}

void Camera3D::focalSTART_RAW()
{
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 0);
    _SetRawImageCallback(camera_ids[0], RawImageCallbackHandler);
    _StartGrabbing(camera_ids[0]);
}

void Camera3D::focalSTART_LINE()
{
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
    _SetLineSortingOrder(camera_ids[0], SurfaceSorting::FROM_TOP_TO_BOTTOM);
    // Line callback function for the first layer
    // Must be called after x-calibration file is set
    _SetLineCallback(camera_ids[0], 0, LineCallbackHandler);

    _StartGrabbing(camera_ids[0]);
}

void Camera3D::focalSTART_BATCH()//ZM:批处理模式获取3D数据
{

    if(initStatus > 0)
    {

        qDebug()<<"void FocalWorkData::focalSTART_BATCH() error  checkStat:"<<initStatus;
        return;
    }

    qDebug()<<"~~~~~~~~~~~into void Camera3D::focalSTART_BATCH()";

    // - Batch length may be 0 from recipe (default value). That won't work, will cause segmentation fault.
    // - If user has not set batch length from command line, it will be the default value.
    // - In case batch length is still 0 (invalid value given by user), use default value.
    int recipeBatchLength = 0;
    _GetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), &recipeBatchLength);
    if (recipeBatchLength <= 0)
    {
        batchLength = DEFAULT_BATCH_LENGTH;
    }
    else
        batchLength = recipeBatchLength;


    // //printf("Starting batch run, batch length: %d\n", batchLength);
    qDebug()<<"111111111111111111111111111111111111111111111111111111111111111111111:";
    _StopGrabbing(camera_ids[0]);
      qDebug()<<"2222222222222222222222222222222222222222222222:";
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_BATCH_LENGTH), 1000);

    //~~~~~~~~~~新增 3行代码
    int ZMimage_height = 1;
    _AdjustRoiAndFps(camera_ids[0],0,500,&ZMimage_height);//设置频率900
    auto status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0],const_cast<char*>("reg_pulse_freq"),500));//0:外触发,大于0内触发

    _SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_PEAK_ENABLE), 1);
    _SetLineSortingOrder(camera_ids[0], SurfaceSorting::FROM_TOP_TO_BOTTOM);

    status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_X_DIRECTION), 1));
    if (status == CAMERA_OK)
        ; //printf("Z Compensation X direction enabled\n");

    status = static_cast<CameraStatus>(_SetIntParameter(camera_ids[0], const_cast<char*>(PARAM_Z_COMPENSATION_Y_DIRECTION), 1));
    if (status == CAMERA_OK)
         ;//printf("Z Compensation Y direction enabled\n");

    bool callbackOk = true;
    for (int i = 0; i < layerCount; i++)
    {
        //ZM:设置回调，ONLY 1
        status = static_cast<CameraStatus>(_SetBatchCallback(camera_ids[0], i, BatchCallbackHandler));
        if (status != CAMERA_OK)
        {
            callbackOk = false;
             //printf("Batch callback set failed\n");
            break;
        }
    }
    if (callbackOk)
        _StartGrabbing(camera_ids[0]);

}

void Camera3D::focalSTOP()
{
    _StopGrabbing(camera_ids[0]);
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    _SetProfileCallback(camera_ids[0], nullptr);
    _SetLineCallback(camera_ids[0], 0, nullptr);

    puts(GUIDE);
}

void Camera3D::focalSTOP_GRABBING()
{
    _StopGrabbing(camera_ids[0]);
    puts(GUIDE);
}

void Camera3D::focalSTART_GRABBING()
{
    _StartGrabbing(camera_ids[0]);
}

int Camera3D::getInitStatus()
{
    return initStatus;
}
