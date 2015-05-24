#pragma once
#include "GenericConstants.h"
#include <VirtualSpace/ISceneObject.h>

class DigitalElevationModelParser
{
private:

	class IDEMRecord
	{

	};

	class RecordTypeA : public IDEMRecord
	{
	};

	class RecordTypeB : public IDEMRecord
	{
	};

	class RecordTypeC : public IDEMRecord
	{
	};

private:
	IDEMRecord* mp_demRecords;
	Int16 m_i16RecordCount;

public:
	DigitalElevationModelParser(void);
	~DigitalElevationModelParser(void);
	bool ParseToMesh(char* a_chrContent, Int64 a_i64ContentSize, ISceneObject* a_meshOut);

};
