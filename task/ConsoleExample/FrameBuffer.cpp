#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif

// Include FSAPI header files.
#include "../API/Callback.h"
#include "FrameBuffer.h"
//#include "../Lib/bitmap/bitmap_image.hpp"

#include "../Lib/bitmap/bitmap_image.hpp"
#include <thread>
#include <string.h>

FrameBuffer::FrameBuffer(int width, int height)
{
	m_width = width;
	m_height = height;

	m_zValues = new float[m_width*m_height];
	m_IntensityValues = new float[m_width*m_height];
	m_savingOngoing = false;

	m_stepX = 5.5;
	m_rowHeight = 100;

	Clear();
}

FrameBuffer::~FrameBuffer()
{
	if (m_zValues)
		delete[] m_zValues;
	if (m_IntensityValues)
		delete[] m_IntensityValues;
}

void FrameBuffer::Clear()
{
	m_linesAdded = 0;
	int size = m_width*m_height;

	for (auto i=0;i<size;i++)
	{
		m_zValues[i] = NO_MEASUREMENT;
		m_IntensityValues[i] = 0;
	}
	m_frameReady = false;
}

bool FrameBuffer::CopyLine(float *zValues, float *intensityValues, int line_length, double xStep, HEADER *header)
{
	if (m_linesAdded==0)
	{
		m_startLine = header->index;
	}
	m_stepX = xStep;

	if (m_savingOngoing || m_linesAdded >= m_height)
	{
		return false;
	}
	if (line_length >= m_width)
	{
		line_length = m_width;
	}

	// TODO: overflow index handling

	// Note that rows are not always received in order
	int row = header->index - m_startLine;
	if (row >= m_height)
	{
		m_frameReady = true;
		return false;
	}
	if (row >= 0)
	{
		memcpy(m_zValues + m_width*row, zValues, line_length * sizeof(float));
		memcpy(m_IntensityValues + m_width*row, intensityValues, line_length * sizeof(float));
	}

	m_linesAdded++;
	return true;
}

bool FrameBuffer::TriggerSave(std::string& path, std::string& fileName)
{
	if (m_savingOngoing)
		return false;

    // copy to local stack, otherwise strings go out of scope and get deleted before thread starts
    m_path = path;
    m_fileName = fileName;

    // Start and forget thread
    std::thread filewriter(&FrameBuffer::SaveOutputFiles_thread, this, std::ref(m_path), std::ref(m_fileName));
    filewriter.detach();
    return true;
}

void FrameBuffer::SaveOutputFiles_thread(std::string& path, std::string& fileName)
{
	int filesWritten = 0;
	if (m_savingOngoing)
		return;
	m_savingOngoing = true;

    std::string fullPath = path + fileName + "_intensity.bmp";

    WriteBMPGrayscale(fullPath, false);

	fullPath = path + fileName + "_height.bmp";

	WriteBMPGrayscale(fullPath, true);

    Clear();
    printf("%s saved\n", fileName.c_str());
	m_savingOngoing = false;
}

std::wstring FrameBuffer::StringToWidechar(const std::string& s)
{
#ifdef _WIN32
	auto slength = static_cast<int>(s.length()) + 1;
	auto len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, nullptr, 0);
	auto buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
#endif
}

bool FrameBuffer::WriteBMPGrayscale(const std::string& fullPath, bool heightMap) const
{
    float MAX_Z_VALUE = 2500.0;
    bitmap_image image;
    image.setwidth_height(m_width, m_height, true);

    const int NUMBER_OF_COLORS = 256;
    rgb_t palette[NUMBER_OF_COLORS];

    memset(palette, 0, sizeof(rgb_t)); // first color: (0, 0, 0)

    // create the color palette
    for (auto i = 1; i < NUMBER_OF_COLORS; i++)
    {
        if (heightMap)
        {
            palette[i].blue = 50;
            palette[i].green = i;
            palette[i].red = 255 - i;
        }
        else
        {
            palette[i].blue = i;
            palette[i].green = i;
            palette[i].red = i;
        }
    }

    for (auto i = 0; i < m_height; i++)
    {
        float* row;
        if (heightMap)
        {
            row = m_zValues + i * m_width;
            for (int j = 0; j < m_width; j++)
            {
                // Scale Z value (micrometers) to palette color
                int paletteIndex = static_cast<int>((NUMBER_OF_COLORS / 2) * row[j] / MAX_Z_VALUE + (NUMBER_OF_COLORS / 2));
                if ((paletteIndex < 0) || (paletteIndex > 255))
                {
                    // out of range or not measured point
                    image.set_pixel(j, i, palette[0]);
                }
                else
                {
                    image.set_pixel(j, i, palette[paletteIndex]);
                }
            }
        }
        else
        {
            row = m_IntensityValues + i * m_width;
            for (auto j = 0; j < m_width; j++)
            {
                int paletteIndex = static_cast<int>(row[j]);
                if (paletteIndex >= NUMBER_OF_COLORS)
                {
                    // draw fully saturated pixel as black
                    image.set_pixel(j, i, palette[0]);
                }
                else
                {
                    image.set_pixel(j, i, palette[paletteIndex]);
                }
            }
        }
    }

    image.save_image(fullPath);
    return true;
}

//BatchBuffer::BatchBuffer(int width, int height)
//{

//}
