#pragma once

#include <stdint.h>
//#include "API/PeakStructure.h"
#include "../API/PeakStructure.h"
#ifdef _WIN32
#include <xstring>
#else
#include <string>
#endif

class FrameBuffer
{
	uint32_t m_startLine;
	int m_linesAdded;

	double m_stepX;
	double m_rowHeight;

	int m_width;
	int m_height;
	float* m_zValues;
	float* m_IntensityValues;
	bool m_frameReady;

    std::string m_path;
    std::string m_fileName;
    bool m_savingOngoing;   /// @todo Need to be atomic

	bool WriteBMPGrayscale(const std::string& fullPath, bool heightMap) const;
    void SaveOutputFiles_thread(std::string& path, std::string& fileName);
	static std::wstring StringToWidechar(const std::string& s);

	public:

	FrameBuffer(int width, int height);
	~FrameBuffer();

	void Clear();

	bool CopyLine(float *zValues, float *intensityValues, int line_length, double xStep, HEADER *header);

	bool IsFull() const
	{
		return m_frameReady || (m_linesAdded >= m_height);
	}

	bool TriggerSave(std::string& path, std::string& fileName);
};

class BatchBuffer
{

    //float *m_zValues;
   // float *m_intensityValues;
   // int m_width;
   // int m_height;
   // double m_xStep;
   // double m_yStep;

    // BatchBuffer(int width,int height);
    // ~BatchBuffer();

};

