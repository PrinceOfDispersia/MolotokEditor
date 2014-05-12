/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	Serializer.cpp - serializer interface
 *
 **/

#include <Platform/Common/ApplicationCommon.h>


void XML_Serializer::WriteByte( byte b )
{
	
}

void ME_Framework::XML_Serializer::WriteShort( short s )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::WriteInt( int i )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::WriteData( byte * pData,size_t sz )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::WriteFloat( float f )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::ReadByte( byte & b )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::ReadShort( short & s )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::ReadInt( int & i )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::ReadData( byte ** ppData, size_t sz )
{
	throw std::exception("The method or operation is not implemented.");
}

void ME_Framework::XML_Serializer::ReadFloat( float & f )
{
	throw std::exception("The method or operation is not implemented.");
}
