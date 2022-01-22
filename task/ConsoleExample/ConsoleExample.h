#pragma once

#include <string>
using namespace std;

// 0 means auto IP. IP address can be set, i.e. "192.168.1.22". Note that IP is forced to the sensor.
#define EXAMPLE_IP_ADDR 0

#define SENSOR_FREQUENCY 100
#define SENSOR_LED_DURATION 10
#define SENSOR_PULSE_DIVIDER 1
#define SENSOR_IMAGE_OFFSETY 400
#define SENSOR_IMAGE_HEIGHT 600
#define DEFAULT_BATCH_LENGTH 1000

#define CAMERA_SENSOR_WIDTH 2048
#define CAMERA_SENSOR_HEIGHT 1088
#define HS_CAMERA_SENSOR_HEIGHT 1400

#define NUM_FRAME_BUFFERS   3
#define DEFAULT_SAVED_IMAGE_HEIGHT 2000

int sensorFrequency = SENSOR_FREQUENCY;
int ledDuration = SENSOR_LED_DURATION;
int imageHeight = SENSOR_IMAGE_HEIGHT;
int imageOffsetY = SENSOR_IMAGE_OFFSETY;
int encoderPulseDivider = SENSOR_PULSE_DIVIDER;
int reorderingEnable = 0;
int batchLength = DEFAULT_BATCH_LENGTH;
int layerCount = 1;

int imageOffsetX = 0;
int imageWidth = CAMERA_SENSOR_WIDTH;

char z_calib_file[4096] = { 0 };
char x_calib_file[4096] = { 0 };

enum BatchStatus
{
    batch_status_unknown = 0,
    batch_status_initialized,
    batch_status_running,
    batch_status_ready,
    batch_status_processing,
    batch_status_completed,
    batch_status_informed,
    batch_status_stopped
};

// Commandline parameters supported
enum SensorParam
{
    enum_unknown = 0,
    enum_frequency,
    enum_led_duration,
    enum_image_height,
    enum_x_calib_file,
    enum_z_calib_file,
    enum_reordering_enable,
    enum_encoder_pulse_divider,
    enum_batch_length,
    enum_layer_count,
    enum_show_help
};

inline SensorParam OptionToParam(const std::string& str)
{
    if (str == "-f" || str == "-F") return enum_frequency;
    if (str == "-d" || str == "-D") return enum_led_duration;
    if (str == "-x" || str == "-X") return enum_x_calib_file;
    if (str == "-z" || str == "-Z") return enum_z_calib_file;
    if (str == "-i" || str == "-I") return enum_image_height;
    if (str == "-r" || str == "-R") return enum_reordering_enable;
    if (str == "-p" || str == "-P") return enum_encoder_pulse_divider;
    if (str == "-b" || str == "-B") return enum_batch_length;
    if (str == "-l" || str == "-L") return enum_layer_count;
    if (str == "-h" || str == "-H") return enum_show_help;
    return enum_unknown;
}

