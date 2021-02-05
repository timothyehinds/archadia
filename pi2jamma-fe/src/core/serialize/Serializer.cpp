#include "core/serialize/Serializer.hpp"

#include "core/meta/MetaSystem.hpp"

Result<Success> metaSystemLoad(
	const std::type_info& typeInfo,
	void* pObject,
	ObjectReadStream& readStream)
{
	MetaType* pType = nullptr;
	Result<Success> r = Meta::get().findType(pType,typeInfo);
	if(!r) {
		return r;
	}

	ASSERTFMT(
		!pType->isPrimitive(),
		"Primitive serialization function not registered! :%s",
		pType->getName().c_str());
	return pType->load(pObject, readStream);	
}

Result<Success> metaSystemSave(
	const std::type_info& typeInfo,
	const void* pObject,
	ObjectWriteStream& writeStream)
{
	MetaType* pType = nullptr;
	Result<Success> r = Meta::get().findType(pType,typeInfo);
	if(!r) {
		return r;
	}
	
	ASSERTFMT(
		!pType->isPrimitive(),
		"Primitive serialization function not registered! :%s",
		pType->getName().c_str());	
	return pType->save(pObject, writeStream);
}