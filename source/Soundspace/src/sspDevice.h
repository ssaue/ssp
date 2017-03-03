// sspDevice.h

#ifndef SSP_DEVICE
#define SSP_DEVICE

#include "sspDefines.h"
#include "SSpObject.h"

class sspDeviceGroup : public sspIObase
{
protected:
	sspIntVec m_nDevices;
  bool      m_bInitialized;

	// protected constructor
	sspDeviceGroup();
	virtual ~sspDeviceGroup();

	friend class sspDeviceManager;

  // The internal implementation is virtual
	virtual bool initializeImpl(LPVOID hWnd) = 0;
	virtual bool beginImpl() = 0;
	virtual bool endImpl() = 0;
	virtual bool terminateImpl() = 0;

public:
	void Serialize(CArchive& ar);

	// General device accessors
	virtual unsigned int		getAllDevices() = 0;
	virtual unsigned int		getDeviceCount() = 0;
	virtual sspString	getDeviceName(int nDevice) = 0;

	// Initialization and termination
	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	bool initialize(LPVOID hWnd);
	bool begin();
  bool end() {return endImpl();}
  bool terminate() {return terminateImpl();}

	// Subset accessors
	void setSubset(const sspIntVec& nDevices) {m_nDevices = nDevices;}
	const sspIntVec& getSubset() const {return m_nDevices;}
	unsigned int getSubsetSize() const {return m_nDevices.size();}
	sspString getSubsetName(int nSubsetIndex);
};

#endif