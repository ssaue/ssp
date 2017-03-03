// sspDScontrolFile.h: interface for the sspDScontrolFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSCONTROLFILE_H__27438142_8E11_11D3_901C_82AFE64A2656__INCLUDED_)
#define AFX_DSCONTROLFILE_H__27438142_8E11_11D3_901C_82AFE64A2656__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpMessage.h"
#include "sspScheduler.h"

#include <fstream>
#include <vector>

typedef std::vector<float> sspDSvalues;

struct sspDScontrolPoint
{
	float		m_fTime;
	sspDSvalues	m_values;

	sspDScontrolPoint(float fTime, size_t size);
};
typedef std::vector<sspDScontrolPoint*> sspDScontrolGraph;

class sspDSchannelControl;
typedef std::vector<sspDSchannelControl*> sspDSbufferControl;

class sspDSbuffer;

class sspDScontrolFile : public sspScheduleObject
{
private:
	std::ifstream m_file;
	sspDScontrolGraph m_vElement;
	sspDScontrolGraph::iterator m_vIterator;
	sspDSbuffer* m_pBuffer;
	sspDSbufferControl m_pControl;

	sspMsgHandler* m_pOwner;
	double	m_dDuration;

public:
	sspDScontrolFile();
	virtual ~sspDScontrolFile();

	bool open(LPCTSTR pFileName);
	bool readData(void);
	void close();
	void initialize(sspDSbuffer* pBuf);

	bool begin();
	bool update();
	void end();

	unsigned int getDuration() const;
	void setOwner(sspMsgHandler* pOwner);
};

#endif // !defined(AFX_DSCONTROLFILE_H__27438142_8E11_11D3_901C_82AFE64A2656__INCLUDED_)
