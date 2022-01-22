#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#define __cdecl
#endif

#include "PeakStructure.h"
#include <stdint.h>


/*! \addtogroup API_Callbacks Callback definitions
*  @{
*/

/// <summary>
//! <pre>
/// <b>C# function:</b>  delegate void RawImageCallback(byte[] image, int bytes, Header header);
//! </pre>
/// Callback signature for raw image reception.
/// </summary>
/// <param name="cameraID">[in] Identifier of the camera. </param>
/// <param name="image">[in] Pointer to the received 8-bit grayscale image. Each pixel is represented by a 8-bit element. The array is composed so that first W elements are the top row, second W elements are next row etc. W is sensor image height that can be found from the sensor data sheet.</param>
/// <param name="bytes">[in] Number of bytes in the image. </param>
/// <param name="header">[in] Additional information related to the raw image.</param>
typedef void(__cdecl * RawImageCallback)(char *cameraID, unsigned char *image, int *bytes, HEADER *header);

/// <summary>
//! <pre>
/// <b>C# function:</b>  delegate void ProfileCallback(IList<Point> profile, Header header);
//! </pre>
/// Callback signature for profile reception.
/// </summary>
/// <param name="cameraID">[in] Identifier for the camera.</param>
/// <param name="points">[in] Pointer to the received profile data.</param>
/// <param name="point_count">[in] Number of points in the profile.</param>
/// <param name="header">[in] Additional information related to the profile.</param>
typedef void(__cdecl * ProfileCallback)(char *cameraID, STRUCT_POINT *points, int *point_count, HEADER *header);


/// <summary>
//! <pre>
/// <b>C# function:</b> delegate void LineCallback(
/// int layerId,
/// float[] zValues,
/// float[] intensityValues,
/// int lineLength,
/// double xStep,
/// Header header);
//! </pre>
/// Callback signature for line reception per configured layer.
/// </summary>
/// <param name="cameraID">[in] Identifier for the camera.</param>
/// <param name="layerId">[in] Layer identifier (0 is the first layer).</param>
/// <param name="zValues">[in] Array of heights.</param>
/// <param name="intensityValues">[in] Array of intensity values.</param>
/// <param name="line_length">[in] Length of the array.</param>
/// <param name="xStep">[in] X resolution in micrometers.</param>
/// <param name="header">[in] Additional information related to the line.</param>
typedef void(__cdecl * LineCallback)(char *cameraID, int layerId, float *zValues, float *intensityValues, int line_length, double xStep, HEADER *header);

/// <summary>
//! <pre>
/// <b>C# function:</b> public delegate void BatchCallback(
/// int layerId,
/// float[] zValues,
/// float[] intensityValues,
/// int lineLength,
/// int batchLength,
/// double xStep,
/// Header lineHeaders);
//! </pre>
/// Callback signature for batch results per configured layer.
/// </summary>
/// <param name="cameraID">[in] Identifier for the camera.</param>
/// <param name="layerId">[in] Layer identifier (0 is the first layer).</param>
/// <param name="intensityValues">[in] Array of intensity values.</param>
/// <param name="line_length">[in] Length of the array.</param>
/// <param name="batch_length">[in] Total count of line profiles in the array.</param>
/// <param name="xStep">[in] X resolution in micrometers.</param>
/// <param name="lineHeaders">[in] Collected header information of each line profile belonging for the batch run.</param>
typedef void(__cdecl * BatchCallback)(char *cameraID, int layerId, float *zValues, float *intensityValues, int line_length, int batch_length, double xStep, HEADER *lineHeaders);

/*! @} */

