                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                || (findNext.FindOneOf(_T("\n\r")) != -1))
			findNext = m_sFindNext;
		_CSTRING_NS::CString replaceWith = m_sReplaceWith;
		DWORD dwFlags = pT->GetFindReplaceDialogFlags();

		m_pFindReplaceDialog = pT->CreateFindReplaceDialog(bFindOnly,
			findNext, replaceWith, dwFlags, pT->operator HWND());
		ATLASSERT(m_pFindReplaceDialog != NULL);
		if(m_pFindReplaceDialog != NULL)
			m_bFindOnly = bFindOnly;
	}

	BOOL SameAsSelected(LPCTSTR lpszCompare, BOOL bMatchCase, BOOL /*bWholeWord*/)
	{
		T* pT = static_cast<T*>(this);

		// check length first
		size_t nLen = lstrlen(lpszCompare);
		LONG nStartChar = 0, nEndChar = 0;
		// Send EM_GETSEL so we can use both Edit and RichEdit
		// (CEdit::GetSel uses int&, and CRichEditCtrlT::GetSel uses LONG&)
		::SendMessage(pT->m_hWnd, EM_GETSEL, (WPARAM)&nStartChar, (LPARAM)&nEndChar);
		if(nLen != (size_t)(nEndChar - nStartChar))
			return FALSE;

		// length is the same, check contents
		_CSTRING_NS::CString selectedText;
		pT->GetSelText(selectedText);

		return (bMatchCase && selectedText.Compare(lpszCompare) == 0) ||
			(!bMatchCase && selectedText.CompareNoCase(lpszCompare) == 0);
	}

	void TextNotFound(LPCTSTR lpszFind)
	{
		T* pT = static_cast<T*>(this);
		m_bFirstSearch = TRUE;
		pT->OnTextNotFound(lpszFind);
	}

	_CSTRING_NS::CString GetTranslationText(enum TranslationTextItem eItem) const
	{
		_CSTRING_NS::CString text;
		switch(eItem)
		{
		case eText_OnReplaceAllMessage:
			text = _T("Replaced %d occurances of \"%s\" with \"%s\"");
			break;
		case eText_OnReplaceAllTitle:
			text = _T("Replace All");
			break;
		case eText_OnTextNotFoundMessage:
			text = _T("Unable to find the text \"%s\"");
			break;
		case eText_OnTextNotFoundTitle:
			text = _T("Text not found");
			break;
		}

		return text;
	}

// Overrideable Handlers
	void OnFindNext(LPCTSTR lpszFind, BOOL bFindDown, BOOL bMatchCase, BOOL bWholeWord)
	{
		T* pT = static_cast<T*>(this);

		m_sFindNext = lpszFind;
		m_bMatchCase = bMatchCase;
		m_bWholeWord = bWholeWord;
		m_bFindDown = bFindDown;

		if(!pT->FindTextSimple(m_sFindNext, m_bMatchCase, m_bWholeWord, m_bFindDown))
			pT->TextNotFound(m_sFindNext);
		else
			pT->AdjustDialogPosition(m_pFindReplaceDialog->operator HWND());
	}

	void OnReplaceSel(LPCTSTR lpszFind, BOOL bFindDown, BOOL bMatchCase, BOOL bWholeWord, LPCTSTR lpszReplace)
	{
		T* pT = static_cast<T*>(this);

		m_sFindNext = lpszFind;
		m_sReplaceWith = lpszReplace;
		m_bMatchCase = bMatchCase;
		m_bWholeWord = bWholeWord;
		m_bFindDown = bFindDown;

		if(pT->SameAsSelected(m_sFindNext, m_bMatchCase, m_bWholeWord))
			pT->ReplaceSel(m_sReplaceWith);

		if(!pT->FindTextSimple(m_sFindNext, m_bMatchCase, m_bWholeWord, m_bFindDown))
			pT->TextNotFound(m_sFindNext);
		else
			pT->AdjustDialogPosition(m_pFindReplaceDialog->operator HWND());
	}

	void OnReplaceAll(LPCTSTR lpszFind, LPCTSTR lpszReplace, BOOL bMatchCase, BOOL bWholeWord)
	{
		T* pT = static_cast<T*>(this);

		m_sFindNext = lpszFind;
		m_sReplaceWith = lpszReplace;
		m_bMatchCase = bMatchCase;
		m_bWholeWord = bWholeWord;
		m_bFindDown = TRUE;

		// no selection or different than what looking for
		if(!pT->SameAsSelected(m_sFindNext, m_bMatchCase, m_bWholeWord))
		{
			if(!pT->FindTextSimple(m_sFindNext, m_bMatchCase, m_bWholeWord, m_bFindDown))
			{
				pT->TextNotFound(m_sFindNext);
				return;
			}
		}

		pT->OnReplaceAllCoreBegin();

		int replaceCount=0;
		do
		{
			++replaceCount;
			pT->ReplaceSel(m_sReplaceWith);
		} while(pT->FindTextSimple(m_sFindNext, m_bMatchCase, m_bWholeWord, m_bFindDown));

		pT->OnReplaceAllCoreEnd(replaceCount);
	}

	void OnReplaceAllCoreBegin()
	{
		T* pT = static_cast<T*>(this);

		m_hOldCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT));

		pT->HideSelection(TRUE, FALSE);

	}

	void OnReplaceAllCoreEnd(int replaceCount)
	{
		T* pT = static_cast<T*>(this);
		pT->HideSelection(FALSE, FALSE);

		::SetCursor(m_hOldCursor);

		_CSTRING_NS::CString message = pT->GetTranslationText(eText_OnReplaceAllMessage);
		if(message.GetLength() > 0)
		{
			_CSTRING_NS::CString formattedMessage;
			formattedMessage.Format(message,
				replaceCount, m_sFindNext, m_sReplaceWith);
			if(m_pFindReplaceDialog != NULL)
			{
				m_pFindReplaceDialog->MessageBox(formattedMessage,
					pT->GetTranslationText(eText_OnReplaceAllTitle),
					MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			}
			else
			{
				pT->MessageBox(formattedMessage,
					pT->GetTranslationText(eText_OnReplaceAllTitle),
					MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			}
		}
	}

	void OnTextNotFound(LPCTSTR lpszFind)
	{
		T* pT = static_cast<T*>(this);
		_CSTRING_NS::CString message = pT->GetTranslationText(eText_OnTextNotFoundMessage);
		if(message.GetLength() > 0)
		{
			_CSTRING_NS::CString formattedMessage;
			formattedMessage.Format(message, lpszFind);
			if(m_pFindReplaceDialog != NULL)
			{
				m_pFindReplaceDialog->MessageBox(formattedMessage,
					pT->GetTranslationText(eText_OnTextNotFoundTitle),
					MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			}
			else
			{
				pT->MessageBox(formattedMessage,
					pT->GetTranslationText(eText_OnTextNotFoundTitle),
					MB_OK | MB_ICONINFORMATION | MB_APPLMODAL);
			}
		}
		else
		{
			::MessageBeep(MB_ICONHAND);
		}
	}

	void OnTerminatingFindReplaceDialog(TFindReplaceDialog*& /*findReplaceDialog*/)
	{
	}
};


///////////////////////////////////////////////////////////////////////////////
// CEditFindReplaceImpl - Mixin class for implementing Find/Replace for CEdit
// based window classes.

// Chain to CEditFindReplaceImpl message map. Your class must also derive from CEdit.
// Example:
// class CMyEdit : public CWindowImpl<CMyEdit, CEdit>,
//                 public CEditFindReplaceImpl<CMyEdit>
// {
// public:
//      BEGIN_MSG_MAP(CMyEdit)
//              // your handlers...
//              CHAIN_MSG_MAP_ALT(CEditFindReplaceImpl<CMyEdit>, 1)
//      END_MSG_MAP()
//      // other stuff...
// };

template <class T, class TFindReplaceDialog = CFindReplaceDialog>
class CEditFindReplaceImpl : public CEditFindReplaceImplBase<T, TFindReplaceDialog>
{
protected:
	typedef CEditFindReplaceImpl<T, TFindReplaceDialog> thisClass;
	typedef CEditFindReplaceImplBase<T, TFindReplaceDialog> baseClass;

// Data members
	LPTSTR m_pShadowBuffer;     // Special shadow buffer only used in some cases.
	UINT m_nShadowSize;
	int m_bShadowBufferNeeded;  // TRUE, FALSE, < 0 => Need to check

public:
// Constructors
	CEditFindReplaceImpl() :
		m_pShadowBuffer(NULL),
		m_nShadowSize(0),
		m_bShadowBufferNeeded(-1)
	{
	}

	virtual ~CEditFindReplaceImpl()
	{
		if(m_pShadowBuffer != NULL)
		{
			delete [] m_pShadowBuffer;
			m_pShadowBuffer = NULL;
		}
	}

// Message Handlers
	BEGIN_MSG_MAP(thisClass)
	ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(baseClass, 1)
	END_MSG_MAP()

// Operations
	// Supported only for RichEdit, so this does nothing for Edit
	void HideSelection(BOOL /*bHide*/ = TRUE, BOOL /*bChangeStyle*/ = FALSE)
	{
	}

// Operations (overrideable)
	BOOL FindTextSimple(LPCTSTR lpszFind, BOOL bMatchCase, BOOL bWholeWord, BOOL bFindDown = TRUE)
	{
		T* pT = static_cast<T*>(this);

		ATLASSERT(lpszFind != NULL);
		ATLASSERT(*lpszFind != _T('\0'));

		UINT nLen = pT->GetBufferLength();
		int nStartChar = 0, nEndChar = 0;
		pT->GetSel(nStartChar, nEndChar);
		UINT nStart = nStartChar;
		int iDir = bFindDown ? +1 : -1;

		// can't find a match before the first character
		if(nStart == 0 && iDir < 0)
			return FALSE;

		LPCTSTR lpszText = pT->LockBuffer();

		bool isDBCS = false;
#ifdef _MBCS
		CPINFO info = { 0 };
		::GetCPInfo(::GetOEMCP(), &info);
		isDBCS = (info.MaxCharSize > 1);
#endif

		if(iDir < 0)
		{
			// always go back one for search backwards
			nStart -= int((lpszText + nStart) - ::CharPrev(lpszText, lpszText + nStart));
		}
		else if(nStartChar != nEndChar && pT->SameAsSelected(lpszFind, bMatchCase, bWholeWord))
		{
			// easy to go backward/forward with SBCS
#ifndef _UNICODE
			if(::IsDBCSLeadByte(lpszText[nStart]))
				nStart++;
#endif
			nStart += iDir;
		}

		// handle search with nStart past end of buffer
		UINT nLenFind = ::lstrlen(lpszFind);
		if(nStart + nLenFind - 1 >= nLen)
		{
			if(iDir < 0 && nLen >= nLenFind)
			{
				if(isDBCS)
				{
					// walk back to previous character n times
					nStart = nLen;
					int n = nLenFind;
					while(n--)
					{
						nStart -= int((lpszText + nStart) - ::CharPrev(lpszText, lpszText + nStart));
					}
				}
				else
				{
					// single-byte character set is easy and fast
					nStart = nLen - nLenFind;
				}
				ATLASSERT(nStart + nLenFind - 1 <= nLen);
			}
			else
			{
				pT->UnlockBuffer();
				return FALSE;
			}
		}

		// start the search at nStart
		LPCTSTR lpsz = lpszText + nStart;
		typedef int (WINAPI* CompareProc)(LPCTSTR str1, LPCTSTR str2);
		CompareProc pfnCompare = bMatchCase ? lstrcmp : lstrcmpi;

		if(isDBCS)
		{
			// double-byte string search
			LPCTSTR lpszStop = NULL;
			if(iDir > 0)
			{
				// start at current and find _first_ occurrance
				lpszStop = lpszText + nLen - nLenFind + 1;
			}
			else
			{
				// start at top and find _last_ occurrance
				lpszStop = lpsz;
				lpsz = lpszText;
			}

			LPCTSTR lpszFound = NULL;
			while(lpsz <= lpszStop)
			{
#ifndef _UNICODE
				if(!bMatchCase || (*lpsz == *lpszFind && (!::IsDBCSLeadByte(*lpsz) || lpsz[1] == lpszFind[1])))
#else
				if(!bMatchCase || (*lpsz == *lpszFind && lpsz[1] == lpszFind[1]))
#endif
				{
					LPTSTR lpch = (LPTSTR)(lpsz + nLenFind);
					TCHAR chSave = *lpch;
					*lpch = _T('\0');
					int nResult = (*pfnCompare)(lpsz, lpszFind);
					*lpch = chSave;
					if(nResult == 0)
					{
						lpszFound = lpsz;
						if(iDir > 0)
							break;
					}
				}
				lpsz = ::CharNext(lpsz);
			}
			pT->UnlockBuffer();

			if(lpszFound != NULL)
			{
				int n = (int)(lpszFound - lpszText);
				pT->SetSel(n, n + nLenFind);
				return TRUE;
			}
		}
		else
		{
			// single-byte string search
			UINT nCompare;
			if(iDir < 0)
				nCompare = (UINT)(lpsz - lpszText) + 1;
			else
				nCompare = nLen - (UINT)(lpsz - lpszText) - nLenFind + 1;

			while(nCompare > 0)
			{
				ATLASSERT(lpsz >= lpszText);
				ATLASSERT(lpsz + nLenFind - 1 <= lpszText + nLen - 1);

				LPSTR lpch = (LPSTR)(lpsz + nLenFind);
				char chSave = *lpch;
				*lpch = '\0';
				int nResult = (*pfnCompare)(lpsz, lpszFind);
				*lpch = chSave;
				if(nResult == 0)
				{
					pT->UnlockBuffer();
					int n = (int)(lpsz - lpszText);
					pT->SetSel(n, n + nLenFind);
					return TRUE;
				}

				// restore character at end of search
				*lpch = chSave;

				// move on to next substring
				nCompare--;
				lpsz += iDir;
			}
			pT->UnlockBuffer();
		}

		return FALSE;
	}

	LPCTSTR LockBuffer() const
	{
		const T* pT = static_cast<const T*>(this);

		ATLASSERT(pT->m_hWnd != NULL);

		BOOL useShadowBuffer = pT->UseShadowBuffer();
		if(useShadowBuffer)
		{
			if(m_pShadowBuffer == NULL || pT->GetModify())
			{
				ATLASSERT(m_pShadowBuffer != NULL || m_nShadowSize == 0);
				UINT nSize = pT->GetWindowTextLength() + 1;
				if(nSize > m_nShadowSize)
				{
					// need more room for shadow buffer
					T* pThisNoConst = const_cast<T*>(pT);
					delete[] m_pShadowBuffer;
					pThisNoConst->m_pShadowBuffer = NULL;
					pThisNoConst->m_nShadowSize = 0;
					pThisNoConst->m_pShadowBuffer = new TCHAR[nSize];
					pThisNoConst->m_nShadowSize = nSize;
				}

				// update the shadow buffer with GetWindowText
				ATLASSERT(m_nShadowSize >= nSize);
				ATLASSERT(m_pShadowBuffer != NULL);
				pT->GetWindowText(m_pShadowBuffer, nSize);
			}

			return m_pShadowBuffer;
		}

		HLOCAL hLocal = pT->GetHandle();
		ATLASSERT(hLocal != NULL);
		LPCTSTR lpszText = (LPCTSTR)::LocalLock(hLocal);
		ATLASSERT(lpszText != NULL);

		return lpszText;
	}

	void UnlockBuffer() const
	{
		const T* pT = static_cast<const T*>(this);

		ATLASSERT(pT->m_hWnd != NULL);

		BOOL useShadowBuffer = pT->UseShadowBuffer();
		if(!useShadowBuffer)
		{
			HLOCAL hLocal = pT->GetHandle();
			ATLASSERT(hLocal != NULL);
			::LocalUnlock(hLocal);
		}
	}

	UINT GetBufferLength() const
	{
		const T* pT = static_cast<const T*>(this);

		ATLASSERT(pT->m_hWnd != NULL);
		UINT nLen = 0;
		LPCTSTR lpszText = pT->LockBuffer();
		if(lpszText != NULL)
			nLen = ::lstrlen(lpszText);
		pT->UnlockBuffer();

		return nLen;
	}

	LONG EndOfLine(LPCTSTR lpszText, UINT nLen, UINT nIndex) const
	{
		LPCTSTR lpsz = lpszText + nIndex;
		LPCTSTR lpszStop = lpszText + nLen;
		while(lpsz < lpszStop && *lpsz != _T('\r'))
			++lpsz;
		return LONG(lpsz - lpszText);
	}

	LONG GetSelText(_CSTRING_NS::CString& strText) const
	{
		const T* pT = static_cast<const T*>(this);

		int nStartChar = 0, nEndChar = 0;
		pT->GetSel(nStartChar, nEndChar);
		ATLASSERT((UINT)nEndChar <= pT->GetBufferLength());
		LPCTSTR lpszText = pT->LockBuffer();
		LONG nLen = pT->EndOfLine(lpszText, nEndChar, nStartChar) - nStartChar;
		SecureHelper::memcpy_x(strText.GetBuffer(nLen), nLen * sizeof(TCHAR), lpszText + nStartChar, nLen * sizeof(TCHAR));
		strText.ReleaseBuffer(nLen);
		pT->UnlockBuffer();

		return nLen;
	}

	BOOL UseShadowBuffer(void) const
	{
		const T* pT = static_cast<const T*>(this);

		if(pT->m_bShadowBufferNeeded < 0)
		{
			T* pThisNoConst = const_cast<T*>(pT);

			OSVERSIONINFO ovi = { 0 };
			ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			::GetVersionEx(&ovi);

			bool bWin9x = (ovi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
			if(bWin9x)
			{
				// Windows 95, 98, ME
				// Under Win9x, it is necessary to maintain a shadow buffer.
				// It is only updated when the control contents have been changed.
				pThisNoConst->m_bShadowBufferNeeded = TRUE;
			}
			else
			{
				// Windows NT, 2000, XP, etc.
				pThisNoConst->m_bShadowBufferNeeded = FALSE;

#ifndef _UNICODE
				// On Windows XP (or later), if common controls version 6 is in use
				// (such as via theming), then EM_GETHANDLE will always return a UNICODE string.
				// If theming is enabled and Common Controls version 6 is in use,
				// you're really not suppose to superclass or subclass common controls
				// with an ANSI windows procedure (so its best to only theme if you use UNICODE).
				// Using a shadow buffer uses GetWindowText instead, so it solves
				// this problem for us (although it makes it a little less efficient).

				if((ovi.dwMajorVersion == 5 && ovi.dwMinorVersion >= 1) || (ovi.dwMajorVersion > 5))
				{
					// We use DLLVERSIONINFO_private so we don't have to depend on shlwapi.h
					typedef struct _DLLVERSIONINFO_private
					{
						DWORD cbSize;
						DWORD dwMajorVersion;
						DWORD dwMinorVersion;
						DWORD dwBuildNumber;
						DWORD dwPlatformID;
					} DLLVERSIONINFO_private;

					HMODULE hModule = ::LoadLibrary("comctl32.dll");
					if(hModule != NULL)
					{
						typedef HRESULT (CALLBACK *LPFN_DllGetVersion)(DLLVERSIONINFO_private *);
						LPFN_DllGetVersion fnDllGetVersion = (LPFN_DllGetVersion)::GetProcAddress(hModule, "DllGetVersion");
						if(fnDllGetVersion != NULL)
						{
							DLLVERSIONINFO_private version = { 0 };
							version.cbSize = sizeof(DLLVERSIONINFO_private);
							if(SUCCEEDED(fnDllGetVersion(&version)))
							{
								if(version.dwMajorVersion >= 6)
								{
									pThisNoConst->m_bShadowBufferNeeded = TRUE;

									ATLTRACE2(atlTraceUI, 0, _T("Warning: You have compiled for MBCS/ANSI but are using common controls version 6 or later (likely through a manifest file).\r\n"));
									ATLTRACE2(atlTraceUI, 0, _T("If you use common controls version 6 or later, you should only do so for UNICODE builds.\r\n"));
								}
							}
						}

						::FreeLibrary(hModule);
						hModule = NULL;
					}
				}
#endif // !_UNICODE
			}
		}

		return (pT->m_bShadowBufferNeeded == TRUE);
	}
};


///////////////////////////////////////////////////////////////////////////////
// CRichEditFindReplaceImpl - Mixin class for implementing Find/Replace for CRichEditCtrl
// based window classes.

// Chain to CRichEditFindReplaceImpl message map. Your class must also derive from CRichEditCtrl.
// Example:
// class CMyRichEdit : public CWindowImpl<CMyRichEdit, CRichEditCtrl>,
//                     public CRichEditFindReplaceImpl<CMyRichEdit>
// {
// public:
//      BEGIN_MSG_MAP(CMyRichEdit)
//              // your handlers...
//              CHAIN_MSG_MAP_ALT(CRichEditFindReplaceImpl<CMyRichEdit>, 1)
//      END_MSG_MAP()
//      // other stuff...
// };

template <class T, class TFindReplaceDialog = CFindReplaceDialog>
class CRichEditFindReplaceImpl : public CEditFindReplaceImplBase<T, TFindReplaceDialog>
{
protected:
	typedef CRichEditFindReplaceImpl<T, TFindReplaceDialog> thisClass;
	typedef CEditFindReplaceImplBase<T, TFindReplaceDialog> baseClass;

public:
	BEGIN_MSG_MAP(thisClass)
	ALT_MSG_MAP(1)
		CHAIN_MSG_MAP_ALT(baseClass, 1)
	END_MSG_MAP()

// Operations (overrideable)
	BOOL FindTextSimple(LPCTSTR lpszFind, BOOL bMatchCase, BOOL bWholeWord, BOOL bFindDown = TRUE)
	{
		T* pT = static_cast<T*>(this);

		ATLASSERT(lpszFind != NULL);
		FINDTEXTEX ft = { 0 };

		pT->GetSel(ft.chrg);
		if(m_bFirstSearch)
		{
			if(bFindDown)
				m_nInitialSearchPos = ft.chrg.cpMin;
			else
				m_nInitialSearchPos = ft.chrg.cpMax;
			m_bFirstSearch = FALSE;
		}

#if (_RICHEDIT_VER >= 0x0200)
		ft.lpstrText = (LPTSTR)lpszFind;
#else // !(_RICHEDIT_VER >= 0x0200)
		USES_CONVERSION;
		ft.lpstrText = T2A((LPTSTR)lpszFind);
#endif // !(_RICHEDIT_VER >= 0x0200)

		if(ft.chrg.cpMin != ft.chrg.cpMax) // i.e. there is a selection
		{
			if(bFindDown)
			{
				ft.chrg.cpMin++;
			}
			else
			{
				// won't wraparound backwards
				ft.chrg.cpMin = max(ft.chrg.cpMin, 0);
			}
		}

		DWORD dwFlags = bMatchCase ? FR_MATCHCASE : 0;
		dwFlags |= bWholeWord ? FR_WHOLEWORD : 0;

		ft.chrg.cpMax = pT->GetTextLength() + m_nInitialSearchPos;

		if(bFindDown)
		{
			if(m_nInitialSearchPos >= 0)
				ft.chrg.cpMax = pT->GetTextLength();

			dwFlags |= FR_DOWN;
			ATLASSERT(ft.chrg.cpMax >= ft.chrg.cpMin);
		}
		else
		{
			if(m_nInitialSearchPos >= 0)
				ft.chrg.cpMax = 0;

			dwFlags &= ~FR_DOWN;
			ATLASSERT(ft.chrg.cpMax <= ft.chrg.cpMin);
		}

		BOOL bRet = FALSE;

		if(pT->FindAndSelect(dwFlags, ft) != -1)
		{
			bRet = TRUE;   // we found the text
		}
		else if(m_nInitialSearchPos > 0)
		{
			// if the original starting point was not the beginning
			// of the buffer and we haven't already been here
			if(bFindDown)
			{
				ft.chrg.cpMin = 0;
				ft.chrg.cpMax = m_nInitialSearchPos;
			}
			else
			{
				ft.chrg.cpMin = pT->GetTextLength();
				ft.chrg.cpMax = m_nInitialSearchPos;
			}
			m_nInitialSearchPos = m_nInitialSearchPos - pT->GetTextLength();

			bRet = (pT->FindAndSelect(dwFlags, ft) != -1) ? TRUE : FALSE;
		}

		return bRet;
	}

	long FindAndSelect(DWORD dwFlags, FINDTEXTEX& ft)
	{
		T* pT = static_cast<T*>(this);
		LONG index = pT->FindText(dwFlags, ft);
		if(index != -1) // i.e. we found something
			pT->SetSel(ft.chrgText);

		return index;
	}
};

}; // namespace WTL

#endif // __ATLFIND_H__
