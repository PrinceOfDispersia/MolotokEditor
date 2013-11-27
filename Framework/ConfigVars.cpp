/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	ConfigVars.cpp - configuration variables
*
**/

#include <Platform/Common/ApplicationCommon.h>

/*
 * Manager contstructor
 **/
CConfigVarsManager::CConfigVarsManager(TCHAR * strFileName)
{
	m_strFilePath = strFileName;
}

/*
 *	Destructor	
 **/
CConfigVarsManager::~CConfigVarsManager()
{
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