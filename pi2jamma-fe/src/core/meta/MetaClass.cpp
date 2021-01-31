#include "core/debug.hpp"
#include "core/meta/MetaClass.hpp"
#include "core/meta/Meta.hpp"

MetaClassBase::MetaClassBase(CStrArg name, const std::type_info& typeInfo)
	: MetaType(
		name,
		typeInfo,
		Type::Class)
{
}

MetaClassProperty::MetaClassProperty(
	MetaClassBase* pMetaClassBase,
	MetaType* pPropertyType,
	CStrArg name)
	: mName(name)
	, mpPropertyType(pPropertyType)
	, mpMetaClassBase(pMetaClassBase)
{
	ASSERT(nullptr != pMetaClassBase);
	ASSERT(nullptr != pPropertyType);
	
	pMetaClassBase->addProperty(*this);
}

MetaClassProperty::~MetaClassProperty()
{
	mpMetaClassBase->removeProperty(*this);
}


Result MetaClassBase::load(void* object, ObjectReadStream& readStream) const
{
	Result r = readStream.beginObject();
	if(r.peekFailed()) {
		return r;
	}

	while(1)
	{
		std::string name;
		bool gotField = true;

		r = readStream.beginField(gotField, name);
		if(r.peekFailed()) {
			return r;
		}
		
		if(!gotField) {
			break;
		}

		const MetaClassProperty* pProp = mProperties.find(name);

		if(nullptr == pProp) {
			return
				readStream.makeError(
					formatString("%s is not a member of %s",
					name.c_str(),
					getName().c_str()));
		}

		r = pProp->load(object, readStream);
		if(r.peekFailed()) {
			return r;
		}

		r = readStream.endField();
		if(r.peekFailed()) {
			return r;
		}
	}

	return readStream.endObject();
}

Result MetaClassBase::save(const void* pVoidObject, ObjectWriteStream& writeStream) const
{
	Result r = writeStream.beginObject();
	if(r.peekFailed()) {
		return r;
	}

	//
	// TODO: Here write out in declaration order (need another list):
	//
	for(auto&& property : mPropertiesInDeclarationOrder) {

		r = writeStream.beginField(property.getName());
		if(r.peekFailed()) {
			return r;
		}

		r = property.save(pVoidObject, writeStream);

		if( r.peekFailed()) {
			return r;
		}

		r = writeStream.endField();
		if(r.peekFailed()) {
			return r;
		}
	}

	r = writeStream.endObject();
	if(r.peekFailed()) {
		return r;
	}
	
	return Result::makeSuccess();
}


