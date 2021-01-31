#include "core/meta/MetaEnum.hpp"

#include "core/meta/Meta.hpp"

MetaEnumValueBase::MetaEnumValueBase(
	MetaEnumBase* pMetaEnumBase,
	CStrArg name,
	uint64_t value)
	: mName(name)
	, mpMetaEnumBase(pMetaEnumBase)
	, mValue(value)
{
	ASSERT(nullptr != pMetaEnumBase);
	pMetaEnumBase->addValue(*this);
}

MetaEnumValueBase::~MetaEnumValueBase()
{
	mpMetaEnumBase->removeValue(*this);
}

MetaEnumBase::MetaEnumBase(CStrArg name, size_t numBytes, const std::type_info& typeInfo)
	: MetaType(
		name,
		typeInfo,
		Type::Enum)
	, mNumBytes(numBytes)
{
}




