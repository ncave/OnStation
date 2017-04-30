// Clipboard support class

class CClipboard
{
	public:
		static BOOL StringToClipboard(CString& CS,BOOL bCustomFormat);
		static BOOL IsClipboardEmpty();
		static BOOL StringFromClipboard(CString& CS,BOOL bCustomFormat);
};
