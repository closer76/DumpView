#ifndef __GUID_TRANSLATOR_H__
#define __GUID_TRANSLATOR_H__

#include "wx/hashmap.h"

class wxRegEx;

WX_DECLARE_STRING_HASH_MAP( wxString, GuidMap);

class GuidTranslator
{
	GuidMap m_guidMap;
	wxRegEx* m_reGuid;

public:
	GuidTranslator();
	~GuidTranslator();

	/*
	// Return Value:
	//		-1: File not found.
	//		-2: File format error.
	//		>= 0: Successfully loaded. Symbol count = returned value.
	*/
	int ReadTranslationTable( wxString& filename);

	wxString TranslateString( wxString& org_str);
};

#endif