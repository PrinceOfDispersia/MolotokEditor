/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	ConfigVars.cpp - configuration variables
*
**/

#include <Platform/Common/ApplicationCommon.h>

using namespace tinyxml2;

/*
 * Manager contstructor
 **/
CConfigVarsManager::CConfigVarsManager(TCHAR * strFileName)
{
	m_strFilePath = strFileName;


	byte * pBuffer;
	size_t sz;
	pBuffer = g_pPlatform->FileSystem()->LoadFile(strFileName,&sz);

	tinyxml2::XMLDocument doc;
	doc.LoadFromMemory((char*)pBuffer,sz);
	g_pPlatform->FileSystem()->CloseFile(pBuffer);

	XMLElement * varsSection = doc.FirstChildElement("ConfigVars");

	if (varsSection)
	{


		XMLElement * pVar =  varsSection->FirstChildElement("Variable");

		if (pVar)
		{
			cfgVar_t v;
			memset(&v,0,sizeof(v));

			const char * pName = pVar->Attribute("name");
			const char * pValue = pVar->Attribute("value");

			if (pName && pValue)
			{
				strncpy(v.strName,pName,sizeof(v.strName) - 1);
				strncpy(v.strValue,pValue,sizeof(v.strName) - 1);

				m_vVars.push_back(v);
			}
			

			while(XMLElement * e = pVar->NextSiblingElement())
			{
				memset(&v,0,sizeof(v));

				const char * pName = pVar->Attribute("name");
				const char * pValue = pVar->Attribute("value");

				if (pName && pValue)
				{
					strncpy(v.strName,pName,sizeof(v.strName) - 1);
					strncpy(v.strValue,pValue,sizeof(v.strName) - 1);

					m_vVars.push_back(v);
				}
			}
		}
		
	}
}

/*
 *	Destructor	
 **/
CConfigVarsManager::~CConfigVarsManager()
{
	tinyxml2::XMLDocument d;
	XMLElement * varsSection = d.NewElement("ConfigVars");

	d.InsertFirstChild(varsSection);

	for(cfgVar_t v: m_vVars)
	{
		XMLElement * varDesc = d.NewElement("Variable");
		varDesc->SetAttribute("name",v.strName);
		varDesc->SetAttribute("value",v.strValue);

		varsSection->InsertEndChild(varDesc);
	}

	FILE * fp = g_pPlatform->FileSystem()->OpenFileLocal((TCHAR*)m_strFilePath.c_str(),_T("wb"));

	d.SaveFile(fp,false);
	
	fclose(fp);

	m_vVars.clear();
	m_vVars.shrink_to_fit();
}

int CConfigVarsManager::QueryVariable(char * strVariableName,char * defaultValue)
{
	int idx = 0;
	for(cfgVar_t v : m_vVars)
	{
		
		if (!_stricmp(strVariableName,v.strName))
			return idx;
		idx++;
	}

	cfgVar_t v;
	memset(&v,0,sizeof(v));
	
	strncpy(v.strName,strVariableName,sizeof(v.strName) - 1);
	strncpy(v.strValue,defaultValue,sizeof(v.strValue) - 1);
	
	m_vVars.push_back(v);

	return (int)m_vVars.size() - 1;
}
		
/*
*	Value getters-setters
**/
char * CConfigVarsManager::GetStringValue(int varIdx)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return "<Undefined>";

	return m_vVars[varIdx].strValue;
}

int	CConfigVarsManager::GetIntegerValue(int varIdx)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return 0;

	return atoi(m_vVars[varIdx].strValue);
}

vec_t CConfigVarsManager::GetVecValue(int varIdx)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return (vec_t)0;

	return atof(m_vVars[varIdx].strValue);
}

color32_t CConfigVarsManager::GetColorValue(int varIdx)
{
	color32_t black = {0,0,0,0};

	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return black;

	int rgba[4];
	sscanf(m_vVars[varIdx].strValue,"[%d %d %d %d]",&rgba[0],&rgba[1],&rgba[2],&rgba[3]);

	color32_t c = {rgba[0],rgba[1],rgba[2],rgba[3]};

	return c;
}

void CConfigVarsManager::SetStringValue(int varIdx,char * value)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return;

	strncpy(m_vVars[varIdx].strValue,value,sizeof(m_vVars[varIdx].strValue));
}

void CConfigVarsManager::SetIntegerValue(int varIdx,int val)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return;

	sprintf(m_vVars[varIdx].strValue,"%d",val);
}

void CConfigVarsManager::SetVecValue(int varIdx,vec_t v)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return;

	sprintf(m_vVars[varIdx].strValue,"%f",v);
}

void CConfigVarsManager::SetColorValue(int varIdx,color32_t c)
{
	if (varIdx < 0 || varIdx > ((int)m_vVars.size() - 1))
		return;

	sprintf(m_vVars[varIdx].strValue,"[%d %d %d %d]",c.r,c.g,c.b,c.a);
}