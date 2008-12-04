
//#define TEST_SERVER

#ifndef TEST_SERVER
	//production server
	#define WEB_SERVER "www.koblo.com"
	#define INTERFACE_PATH "/KobloStudio/interface/"
#else // #ifndef TEST_SERVER
	//test server
	#define WEB_SERVER "localhost"
	#define INTERFACE_PATH "/3000/ks/interface/"
#endif // #else // #ifndef TEST_SERVER

#define CONNECT_TIMEOUT 30 //in seconds
#define MAX_PAGE_SIZE 200000 //Max no of bytes in web page

class CINetUtil
{
public:
	//! Constructor
	CINetUtil() {}

	//! Destructor
	virtual ~CINetUtil() {}

	//! Read a given file from a web server using HTTP and port 80
	//! Uses POST if Parameters != NULL else GET
	/*!
		\param Parameters [in]: inputstring (eg. userid=cri&password=cri), NULL if none
		\param FileName [in]: The actual file (eg. "checkxxx.aspx")
		\param OutputLength [out]: Number of bytes returned
		\param OutputBuffer [out]: Handle to a bytebuffer which is allocated and contains the web file
	*/
	static void GetWebFile(tchar* PostParameters, tchar* FileName, tint32* OutputLength, tchar** OutputBuffer);

	//! Find the value element of a string (<value>xxx</value>), allocate the buffer and copy the value (xxx) to it
	//! If no value element is found OutputLength is 0 and OutputBuffer is empty string
	//! Note: For speed optimization use GetValuePtr instead - that will save a buffer copy (Lasse)
	/*!
		\param SourceString [in]: inputstring to be searched
		\param OutputLength [out]: Number of bytes returned
		\param OutputBuffer [out]: Handle to a bytebuffer which is allocated and contains the value
		\return tbool: True if a value tag was found (also true for empty contents - check OutputLength for that instead)
	*/
	static tbool GetValue(tchar* SourceString, tint32* OutputLength, tchar** OutputBuffer);

	//! Find the first a tag element of a string (<tag>xxx</tag>). No buffer copying is done.
	/*!
		\param pszTagName [in]: The name of the tag to look for. <b>Don't include the '&lt;' and '&gt;' chars!</b>
		\param pszSourceString [in]: inputstring to be searched (doesn't have to be zero terminated)
		\param iSourceLen [in]: The length of the inputstring
		\param ppszOutput [out]: Pointer (into the inputstring buffer) to the value (not zero terminated)
		\param piOutputLength [out]: Number of chars in value string
		\return tbool: True on success
	*/
	static tbool GetTagContentsPtr(const tchar* pszTagName, tchar* pszSourceString, tint32 iSourceLen, tchar** ppszOutput, tint32* piOutputLength);

	//! Find the value element of a string (<value>xxx</value>). No buffer copying is done.
	//! If no value element is found OutputLength is 0 and OutputBuffer is NULL
	/*!
		\param szSourceString [in]: inputstring to be searched
		\param ppszOutput [out]: Pointer (into the inputstring buffer) to the value (not zero terminated)
		\param piOutputLength [out]: Number of chars in value string
		\return tbool: True on success
	*/
	static tbool GetValuePtr(tchar* szSourceString, tchar** ppszOutput, tint32* piOutputLength);

	static tbool FindLowerCaseString(tchar* pszLowerCaseString, tchar* pszBuffer, tint32 iBufferLen, tchar** ppszIndexAt, tchar** ppszIndexAfter);

	//! Deallocate the buffer allocated by GetWebFile or GetValue - can be called allways, but no need when value null
	//! \param Buffer [in]: Handle to the buffer	
	static void ReleaseBuffer(tchar** Buffer);

	//! Checks ok element in string with value ok (<result status="ok">)
	/*!
        \param SourceString [in]: inputstring to be searched
	*/
	static bool IsResultOK(tchar* SourceString);

	//! Reads web page and return integer result. Handles/trows exceptions
	/*!
		\param Parameters [in]: inputstring (eg. userid=cri&password=cri), NULL if none
		\param FileName [in]: The actual file (eg. "checkxxx.aspx")
	*/
    static tfloat64 GetNumericWebValue(tchar* Parameters, tchar* FileName) throw(IException*);

	//! Reads the next element from an XML string
	//! Assuming format: ...<element name="xxx" isFolder="True/False" price="ppp"/>...
	/*!
		\param S [in]: inputstring to be searched
		\param Name [out]: buffer to write name in (must be preallocated)
		\param NameLength [in]: length of buffer
		\param IsFolder [out]: true if True
		\param Price [Out]: The price
		\return: true if ok, false if not match
	*/
    static bool GetNextElement(tchar **S, tchar *Name, int NameLength, bool *IsFolder, tint32 *Price);

	//returns the middle string between matching start and end and advances the buffer
	static bool GetMiddleStr(tchar **s, const char *sStart, const char *sEnd, char *sMiddle);

protected:


};


