#if 0
#include "core/json/Json.hpp"

ref<JsonNull> JsonNull::sSingleton = make_ref<JsonNull>();

JsonBase::Ref JsonBase::GetValueForKey(CStr str)
{
    return JsonNull::singleton();
}

void JsonBase::SetValueForKey(
	std::string key,
    Arg< JsonBase::Ref >::Type refJson )
{
};

void JsonBase::SetIntegerValueForKey(
	std::string key,
    Arg< JsonIntType >::Type val )
{
}

void JsonBase::SetStringValueForKey(
	std::string key,
    Arg< std::string >::Type val )
{
}

#endif


