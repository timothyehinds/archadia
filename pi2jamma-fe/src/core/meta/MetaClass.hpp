#pragma once

#include "core/meta/MetaType.hpp"
#include "core/cast.hpp"
#include "core/container/DlList.hpp"
#include "core/serialize/Serializer.hpp"

#define META_CLASS(C) \
	static MetaClass<C> gMetaClass##C(#C)


#define META_CLASS_FIELD(C, P) \
	static MetaClassField<C,decltype(C::m##P)> gMetaClassField##C##P( \
		Meta::get().findClass<C>(), \
		Meta::get().findType<decltype(C::m##P)>(), \
		#P, \
		& C::m##P )

#define META_CLASS_PROPERTY(C, P) \
	using MetaClassGetSet##C##P##Type = decltype(std::declval<C>().get##P()); \
	using MetaClassGetSet##C##P##DecayType = std::decay<MetaClassGetSet##C##P##Type>::type; \
	static MetaClassGetSet<C,MetaClassGetSet##C##P##Type,MetaClassGetSet##C##P##DecayType> gMetaClassProperty##C##P( \
		Meta::get().findClass<C>(), \
		Meta::get().findType<MetaClassGetSet##C##P##DecayType>(), \
		#P, \
		& C::set##P, \
		& C::get##P)

class MetaClassBase;
template<typename T > class MetaClass;

class MetaClassProperty
{
public:
	MetaClassProperty(
		MetaClassBase* pMetaClassBase,
		MetaType* pPropertyType,
		CStrArg name );
	virtual ~MetaClassProperty();


	CStrArg getName() const {
		return mName;
	}

	MetaType* getType() const {
		return mpPropertyType;
	}

	virtual Result load(void* pVoidObject, ObjectReadStream& readStream) const = 0;
	virtual Result save(const void* pVoidObject, ObjectWriteStream& write) const = 0;

private:

	CStr mName;
	MetaType* mpPropertyType;
	MetaClassBase* mpMetaClassBase;

public:
	RbTreeNode mTreeNode;
	DlListNode mListNode;
};

template<typename ClassType, typename PropertyType>
class MetaClassPropertyBase : public MetaClassProperty
{
public:
	MetaClassPropertyBase(
		MetaClass<ClassType>* pClassType,
		MetaType* pPropertyType,
		CStrArg name )
		: MetaClassProperty(
			pClassType,
			pPropertyType,
			name)
	{
	}
};
template<typename ClassType, typename PropertyType>
class MetaClassField
	: public MetaClassPropertyBase<ClassType, PropertyType>
{
public:
	using Member = PropertyType(ClassType::*);

	MetaClassField(
		MetaClass<ClassType>* pClass,
		MetaType* pPropertyType,
		CStrArg name,
		Member member)		
		: MetaClassPropertyBase<ClassType,PropertyType>(
			pClass,
			pPropertyType,
			name)
		, mMember(member)
		{

		}

	virtual Result load(void* pVoidObject, ObjectReadStream& readStream) const override
	{
		ClassType* pObject = static_cast<ClassType*>(pVoidObject);

		PropertyType& prop = pObject->*mMember;

		return Serializer<PropertyType>::load(prop, readStream);
	}

	virtual Result save(const void* pVoidObject, ObjectWriteStream& writeStream) const override {
		const ClassType* pObject = static_cast<const ClassType*>(pVoidObject);
		const PropertyType& prop = pObject->*mMember;

		return Serializer<PropertyType>::save(prop, writeStream);
	}	

private:

	Member mMember;
};

template<typename ClassType, typename ParamType, typename DecayType>
class MetaClassGetSet
	: public MetaClassPropertyBase<ClassType,DecayType>
{
public:

	using Setter = void(ClassType::*)(ParamType);
	using Getter = ParamType (ClassType::*)(void) const;

	MetaClassGetSet(
		MetaClass<ClassType>* pClass,
		MetaType* pPropertyType,
		CStrArg name,
		Setter setter,
		Getter getter)
		: MetaClassPropertyBase<ClassType,DecayType>(
		pClass,
		pPropertyType,
		name)
		, mSetter(std::move(setter))
		, mGetter(std::move(getter)) {
	}

	virtual Result load(void* pVoidObject, ObjectReadStream& readStream) const override
	{
		DecayType value;

		Result result = Serializer<DecayType>::load(value, readStream);
		if(result.peekFailed()) {
			return result;
		}

		ClassType* pObject = static_cast<ClassType*>(pVoidObject);
		(pObject->*mSetter)(std::move(value));

		return Result::makeSuccess();
	}

	virtual Result save(const void* pVoidObject, ObjectWriteStream& writeStream) const override {
		const ClassType* pObject = static_cast<const ClassType*>(pVoidObject);		
		return Serializer<DecayType>::save((pObject->*mGetter)(), writeStream);
	}

private:

	Setter mSetter;
	Getter mGetter;
};


class MetaClassBase : public MetaType
{
public:

	MetaClassBase(CStrArg name, const std::type_info& typeInfo);

	MetaClassProperty* findProperty(CStrArg name) {
		return mProperties.find(name);
	}

	void addProperty(MetaClassProperty& metaClassProperty) {
		mProperties.insert(metaClassProperty);
		mPropertiesInDeclarationOrder.insertTail(metaClassProperty);
	}

	void removeProperty(MetaClassProperty& metaClassProperty) {
		mProperties.remove(metaClassProperty);
		mPropertiesInDeclarationOrder.remove(metaClassProperty);
	}

	virtual Result load(void* pVoidObject, ObjectReadStream& readStream) const override;

	virtual Result save(const void* pVoidObject, ObjectWriteStream& writeStream) const override;


private:

	static CStrArg getPropertyName(const MetaClassProperty& prop) {
		return prop.getName();
	}

	RbTree<
		MetaClassProperty,
		CStr,
		KeyFinderGetter<MetaClassProperty, CStr, & MetaClassProperty::getName>,
		NodeFinderField<
			MetaClassProperty,
			RbTreeNode,
			&MetaClassProperty::mTreeNode>> mProperties;

	DlList<
		MetaClassProperty,
		NodeFinderField<
			MetaClassProperty,
			DlListNode,
			& MetaClassProperty::mListNode>> mPropertiesInDeclarationOrder;
};

template<typename T>
class MetaClass final : public MetaClassBase
{
public:
	MetaClass(CStrArg name)
		: MetaClassBase(name, typeid(T))
	{}
};

