/*
 *	Molotok Editor, 2014 (C) PrinceOfDispersia
 *	Serializer.h - serializer interface
 *
 **/

#ifndef Serializer_h__
#define Serializer_h__

namespace ME_Framework
{
	class ISerializer
	{
	public:
		virtual void WriteByte(byte b) = 0;
		virtual void WriteShort(short s) = 0;
		virtual void WriteInt(int i) = 0; 
		virtual void WriteData(byte *  pData,size_t sz) = 0;
		virtual void WriteFloat(float f) = 0;

		virtual void ReadByte(byte & b) = 0;
		virtual void ReadShort(short & s) = 0;
		virtual void ReadInt(int & i) = 0;
		virtual void ReadData(byte ** ppData, size_t sz) = 0;
		virtual void ReadFloat(float & f) = 0;

	};

	class XML_Serializer: public ISerializer
	{
		tinyxml2::XMLDocument * m_pDocument;
	public:
		virtual void WriteByte( byte b );
		virtual void WriteShort( short s );
		virtual void WriteInt( int i );
		virtual void WriteData( byte * pData,size_t sz );
		virtual void WriteFloat( float f );

		virtual void ReadByte( byte & b );
		virtual void ReadShort( short & s );
		virtual void ReadInt( int & i );
		virtual void ReadData( byte ** ppData, size_t sz );
		virtual void ReadFloat( float & f );

		XML_Serializer();
		~XML_Serializer();
	};

}

#endif // Serializer_h__