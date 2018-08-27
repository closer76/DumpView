#include "GuidTranslator.h"
#include "wx/textfile.h"
#include "wx/regex.h"

GuidTranslator::GuidTranslator() :
	m_reGuid(0)
{
	m_reGuid = new wxRegEx( "[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
}

GuidTranslator::~GuidTranslator()
{
	delete m_reGuid;
}

int GuidTranslator::ReadTranslationTable(wxString &filename)
{
	wxTextFile fp(filename);

	if (!fp.Open())
	{
		return -1;
	}

	wxString line = fp.GetFirstLine();
	if ( line != "[GUID]")
	{
		// Format error
		return -2;
	}

	int pos;
	int symbolCount = 0;
	wxString key, value;

	m_guidMap.clear();
	for ( line = fp.GetNextLine(); !fp.Eof(); line = fp.GetNextLine())
	{
		if ( wxNOT_FOUND != (pos = line.Find( '=')) )
		{
			key = line.Mid( 0, pos).Upper();
			value = line.Mid( pos+1);
			m_guidMap[key] = value;
			symbolCount++;
		}
	}

	return symbolCount;
}

wxString GuidTranslator::TranslateString(wxString &org_str)
{
	wxString tmp;
	if ( m_reGuid->Matches( org_str))
	{
		tmp = m_reGuid->GetMatch(org_str);
		tmp = tmp.Upper();

		GuidMap::iterator itor = m_guidMap.find( tmp);
		if ( itor != m_guidMap.end())
		{
			m_reGuid->Replace( &org_str, itor->second);
		}
	}
	return org_str;
}