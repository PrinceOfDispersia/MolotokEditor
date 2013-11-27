/*
*	Molotok Editor, 2013 (C) PrinceOfDispersia
*	ConfigVars.h - configuration variables
*
**/

#ifndef CONFIG_VARS_H
#define CONFIG_VARS_H

namespace ME_Framework
{
	enum class eCfgVarTypes
	{
		CHAR_STRING,
		INTEGER,
		VEC_T,		
		COLOR32
	};

	typedef struct cfgVar_s
	{
		char strName[256];
		char strValue[256];
	}cfgVar_t;

	
	class CConfigVarsManager
	{
		std::vector<cfgVar_t> m_vVars;
		String m_strFilePath;
	public:
		CConfigVarsManager(TCHAR * strFileName);
		~CConfigVarsManager();

		/*
		 *	Queries variable index or creates new with default value 
		 *	if not found 
		 **/
		int QueryVariable(char * strVariableName,char * defaultValue);
		
		/*
		 *	Value getters-setters
		 **/
		char *						GetStringValue(int varIdx);
		int							GetIntegerValue(int varIdx);
		vec_t						GetVecValue(int varIdx);
		color32_t					GetColorValue(int varIdx);

		void						SetStringValue(int varIdx,char * value);
		void						SetIntegerValue(int varIdx,int val);
		void						SetVecValue(int varIdx,vec_t v);
		void						SetColorValue(int varIdx,color32_t c);
		
	};


}



#endif