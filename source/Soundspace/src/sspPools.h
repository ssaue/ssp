// sspPools.h: interface for the SSpValuePool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POOLS_H__9576A664_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
#define AFX_POOLS_H__9576A664_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "SSpObject.h"
#include <afxtempl.h>

class sspValue;
class sspConditional;
class sspStringObject;
class SSpPlayTask;
class SSpStream;
class sspPlayObject;

class SSpValuePool : public sspIObase  
{
	CTypedPtrArray<CObArray, sspValue*> m_arValues;
	CStringArray m_arNames;

public:
	SSpValuePool();
	virtual ~SSpValuePool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();

	int LoadValue(sspValue* pValue, LPCTSTR strName);
	void ExchangeValue(sspValue* pValue, LPCTSTR strName, int nIndex);
	void ExchangeValueName(LPCTSTR strName, int nIndex);
	sspValue* getValue(int nIndex) const;
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class SSpConditionalPool : public sspIObase  
{
	CTypedPtrArray<CObArray, sspConditional*> m_arConditionals;
	CStringArray m_arNames;

public:
	SSpConditionalPool();
	virtual ~SSpConditionalPool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();

	int LoadConditional(sspConditional* pConditional, LPCTSTR strName);
	void ExchangeConditional(sspConditional* pConditional, LPCTSTR strName, int nIndex);
	void ExchangeConditionalName(LPCTSTR strName, int nIndex);
	sspConditional* GetConditional(int nIndex) const;
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class sspStringPool : public sspIObase  
{
	CTypedPtrArray<CObArray, sspStringObject*> m_arStrings;
	CStringArray m_arNames;

public:
	sspStringPool();
	virtual ~sspStringPool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();

	int LoadString(sspStringObject* pString, LPCTSTR strName);
	void ExchangeString(sspStringObject* pString, LPCTSTR strName, int nIndex);
	void ExchangeStringName(LPCTSTR strName, int nIndex);
	sspStringObject* getString(int nIndex) const;
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class SSpPlayObjectPool : public sspIObase  
{
	CTypedPtrArray<CObArray, sspPlayObject*> m_arPlayObjects;
	CStringArray m_arNames;

public:
	SSpPlayObjectPool();
	virtual ~SSpPlayObjectPool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();

	int LoadPlayObject(sspPlayObject* pPlayObject, LPCTSTR strName);
	void ExchangePlayObject(sspPlayObject* pPlayObject, LPCTSTR strName, int nIndex);
	void ExchangeObjectName(LPCTSTR strName, int nIndex);
	sspPlayObject* GetPlayObject(int nIndex) const;
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class SSpPlayTaskPool : public sspIObase  
{
	CTypedPtrArray<CObArray, SSpPlayTask*> m_arPlayTasks;
	CStringArray m_arNames;

public:
	SSpPlayTaskPool();
	virtual ~SSpPlayTaskPool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();

	int LoadPlayTask(SSpPlayTask* pPlayTask, LPCTSTR strName);
	void ExchangePlayTask(SSpPlayTask* pPlayTask, LPCTSTR strName, int nIndex);
	void ExchangeName(LPCTSTR strName, int nIndex);
	SSpPlayTask* GetPlayTask(int nIndex) const;
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class SSpStreamPool : public sspIObase  
{
	CTypedPtrArray<CObArray, SSpStream*> m_arStreams;
	CStringArray m_arNames;

public:
	SSpStreamPool();
	virtual ~SSpStreamPool();
	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();
  void terminate();

	int LoadStream(SSpStream* pStream, LPCTSTR strName);
	void ExchangeStream(SSpStream* pStream, LPCTSTR strName, int nIndex);
	void ExchangeStreamName(LPCTSTR strName, int nIndex);
	SSpStream* GetStream(int nIndex) const;
  SSpStream* getRoot() const {return GetStream(0);}
	LPCTSTR GetName(int nIndex) const;
	unsigned int GetSize() const;
	BOOL IsLoaded() const;
};

class sspPool : public sspIObase  
{
private:
	static sspPool* m_pInstance;
	static bool m_bDestroyed;

public:
	SSpValuePool		values;
	SSpConditionalPool	conditionals;
  sspStringPool strings;
	SSpPlayObjectPool	objects;
	SSpPlayTaskPool		tasks;
	SSpStreamPool		streams;

	static sspPool& Instance();

	void Serialize(CArchive& ar);

	void printASCII(sspOutStream& outStr);
	bool verify(sspOutStream& outStr, int& nErrors, int& nWarnings);
	void initContents();
	void clearContents();
	bool initialize();
  void terminate();

private:
	sspPool();
	sspPool(const sspPool&);
	sspPool& operator=(const sspPool&);
	~sspPool();

	static void create();
	static void onDeadReference();
};

#endif // !defined(AFX_POOLS_H__9576A664_9118_11D3_901C_B11BC2A3FAB6__INCLUDED_)
