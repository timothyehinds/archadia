#pragma once

#include "core/meta/MetaType.hpp"
#include "core/serialize/Serializer.hpp"

#define META_PRIMITIVE(Type) \
	static MetaPrimitive<Type> gMetaPrimitive##Type(#Type)

#define META_PRIMITIVE2(Type1,Type2) \
	static MetaPrimitive<Type1 Type2> gMetaPrimitive##Type1##Type2(#Type1 " " #Type2)	

#define META_PRIMITIVE3(Type1,Type2,Type3) \
	static MetaPrimitive<Type1 Type2 Type3> gMetaPrimitive##Type1##Type2##Type3(#Type1 " " #Type2 " " #Type3)

class MetaPrimitiveBase :public MetaType
{
public:
	MetaPrimitiveBase(CStrArg name, const std::type_info& typeInfo) :
		MetaType(
			name,
			typeInfo,
			Type::Primitive)
	{}
};

template<typename T>
class MetaPrimitive final : public MetaPrimitiveBase
{
public:
	MetaPrimitive(CStrArg name)
		: MetaPrimitiveBase(name,typeid(T)) {
		}

	virtual Result load(void* pItem, ObjectReadStream& readStream) const override {
		return Serializer<T>::load(*static_cast<T*>(pItem), readStream);
	}

	virtual Result save(const void* pItem, ObjectWriteStream& writeStream) const override {
		return Serializer<T>::save(*static_cast<const T*>(pItem), writeStream);
	}

};
