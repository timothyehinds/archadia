#pragma once

#include "core/RefCounted.hpp"
#include "core/Result.hpp"
#include "core/StringUtil.hpp"

#include "core/container/RbTree.hpp"
#include "core/serialize/ObjectReadStream.hpp"
#include "core/serialize/ObjectWriteStream.hpp"

class MetaType
{
public:
	enum class Type { Primitive, Class, Enum };

	MetaType(
		CStr name,
		const std::type_info& typeInfo,
		Type type);

	virtual ~MetaType();

	CStr getName(void) const {
		return mName;
	}

 	const std::type_info& getTypeInfo() const {
 		return mTypeInfo;
 	}

 	Type getType() const;
 	
 	bool isPrimitive() const;
 	bool isClass() const;
 	bool isEnum() const;

	virtual Result load(void* pItem, ObjectReadStream& readStream) const = 0;
	virtual Result save(const void* pItem, ObjectWriteStream& writeStream) const = 0;

private:

	CStr mName;
	const std::type_info& mTypeInfo;
	Type mType;

public:

	RbTreeNode mByNameTreeNode;
	RbTreeNode mByTypeInfoTreeNode;		
};

inline MetaType::Type MetaType::getType() const
{
	return mType;
}

inline bool MetaType::isPrimitive() const
{
	return Type::Primitive == mType;
}

inline bool MetaType::isClass() const
{
	return Type::Class == mType;
}

inline bool MetaType::isEnum() const
{
	return Type::Enum == mType;
}
