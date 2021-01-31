#include "core/meta/MetaType.hpp"

#include "core/meta/Meta.hpp"


MetaType::MetaType(
	CStr name,
	const std::type_info& typeInfo,
	Type type)
	: mName(name)
	, mTypeInfo(typeInfo)
	, mType(type)
{
	Meta::get().addType(*this);
}

MetaType::~MetaType() 
{
	Meta::get().removeType(*this);
}