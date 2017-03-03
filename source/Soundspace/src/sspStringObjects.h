// sspStringObjects.h: interface for the sspStringObject classes
//
//////////////////////////////////////////////////////////////////////

#ifndef SSP_STRINGOBJECTS_H
#define SSP_STRINGOBJECTS_H 1

#include "SSpBasic.h"

class sspSimpleString : public sspStringObject  
{
protected:
	sspString	m_str;
  DECLARE_SERIAL( sspSimpleString )

public:
	sspSimpleString();
	sspSimpleString(const sspSimpleString& other);
	sspSimpleString& operator= (const sspSimpleString& other);
	virtual ~sspSimpleString();

	void Serialize(CArchive& ar);

	// Virtual methods
	sspString	getString() { return m_str; }
	void	printASCII(sspOutStream& outStr);
  bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings) {return true;}

	// Accessors
	void setString(const sspString& str) { m_str = str; }
};

class sspAlphabetString : public sspStringObject  
{
protected:
	sspStrVec m_strVec;
  int m_nValueIndex;
  DECLARE_SERIAL( sspAlphabetString )

public:
	sspAlphabetString();
	sspAlphabetString(const sspAlphabetString& val);
	sspAlphabetString& operator= (const sspAlphabetString& val);
	virtual ~sspAlphabetString();

	void Serialize(CArchive& ar);

	// Virtual methods
	sspString	getString();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStrings(const sspStrVec& strVec) { m_strVec = strVec; }
	const sspStrVec& getStrings() const { return m_strVec; }

  void setValueIndex(int nIndex) {m_nValueIndex = nIndex;}
  int getValueIndex() const {return m_nValueIndex;}
};

class sspConcatString : public sspStringObject  
{
protected:
  sspIntVec m_strIndices;
  DECLARE_SERIAL( sspConcatString )

public:
	sspConcatString();
	sspConcatString(const sspConcatString& val);
	sspConcatString& operator= (const sspConcatString& val);
	virtual ~sspConcatString();

	void Serialize(CArchive& ar);

	// Virtual methods
	sspString	getString();
	void	printASCII(sspOutStream& outStr);
	bool	verify(sspOutStream& outStr, int& nErrors, int& nWarnings);

	// Accessors
	void setStringList(const sspIntVec& nStrings) {m_strIndices = nStrings;}
	const sspIntVec& getStringList() const {return m_strIndices;}
};

#endif