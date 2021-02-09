#include "core/meta/Meta.hpp"

#include "core/meta/MetaPrimitive.hpp"

Meta* Meta::spSingleton{nullptr};

void Meta::initialize()
{
	ASSERT(!spSingleton);
	new Meta();
}

void Meta::terminate()
{	
	delete spSingleton;
	spSingleton = nullptr;
}

Meta::Meta()
{
	spSingleton = this;

	META_PRIMITIVE(char);
	META_PRIMITIVE2(unsigned, char);
	META_PRIMITIVE(short);
	META_PRIMITIVE2(unsigned, short);
	META_PRIMITIVE(int);
	META_PRIMITIVE2(unsigned, int);
	META_PRIMITIVE(long);
	META_PRIMITIVE2(unsigned, long);
	META_PRIMITIVE2(long, long);
	META_PRIMITIVE3(unsigned, long, long);

	META_PRIMITIVE(float);
	META_PRIMITIVE(double);

	META_PRIMITIVE(bool);

	using String = std::string;
	META_PRIMITIVE(String);
}

void Meta::addType(MetaType& metaType)
{
	LogFmt(
		"registering meta-type: name: '%s' typeinfo: '%s\n",
		metaType.getName().c_str(),
		metaType.getTypeInfo().name());

	mTypesByName.insert(metaType);
	mTypesByTypeInfo.insert(metaType);
}

void Meta::removeType(MetaType& metaType)
{
	mTypesByName.remove(metaType);
	mTypesByTypeInfo.remove(metaType);
}

MetaType* Meta::findType(const std::type_info& typeInfo) {
	MetaType* pMetaType = mTypesByTypeInfo.find(typeInfo);
	ASSERTFMT(
		(nullptr != pMetaType),
		"%s is not registered with the meta system.",
		typeInfo.name());
	return pMetaType;
}

Result<Success> Meta::findType(
	MetaType*& pMetaTypeOut,
	const std::type_info& typeInfo)
{
	pMetaTypeOut = findType(typeInfo);
	if(!pMetaTypeOut)
	{
		return
			Result<Success>::makeFailureWithString(
				formatString(
					"%s is not registered with the meta system.",
					typeInfo.name()));
	}

	return Result{Success{}};
}


MetaType* Meta::findType(CStrArg name) {
	MetaType* pMetaType = mTypesByName.find(name);
	ASSERTFMT(
		(nullptr != pMetaType),
		"%s is not registered with the meta system.",
		name.c_str());
	return pMetaType;	
}

