#pragma once

#include "core/meta/MetaType.hpp"
#include <memory>
#include <unordered_map>

#define META_ENUM(E) \
	static MetaEnum<E> gMetaEnum##E(#E)

#define META_ENUM_VALUE(E, V) \
	static MetaEnumValue<E> gMetaEnumValue##E##V(Meta::get().findEnum<E>(), #V, E::V)

class MetaEnumBase;
template<typename T> class MetaEnum;

class MetaEnumValueBase
{
public:
	MetaEnumValueBase(
		MetaEnumBase* pMetaEnumBase,
		CStrArg name,
		uint64_t value);

	virtual ~MetaEnumValueBase();

	Arg<CStr>::Type getName() const {
		return mName;
	}

	virtual uint64_t getValue() const {
		return mValue;
	}

private:
	CStr mName;
	MetaEnumBase* mpMetaEnumBase;
	uint64_t mValue;

public:
	RbTreeNode mNameTreeNode;
	RbTreeNode mValueTreeNode;
};

template<typename T>
class MetaEnumValue final : public MetaEnumValueBase
{
public:

	using super = MetaEnumValueBase;

	MetaEnumValue(
		MetaEnum<T>* pEnum,
		CStrArg name,
		T value);
	

	T getTypedValue() const {
		return static_cast<T>(super::getValue());
	}	
};

class MetaEnumBase : public MetaType
{
public:
	MetaEnumBase(CStrArg name, size_t numBytes, const std::type_info& typeInfo);

	void addValue(MetaEnumValueBase& value) {
		mValuesByName.insert(value);
		mValuesByValue.insert(value);
	}

	void removeValue(MetaEnumValueBase& value) {
		mValuesByName.remove(value);
		mValuesByValue.remove(value);
	}

	const MetaEnumValueBase* findValue(uint64_t value) const {
		return mValuesByValue.find(value);
	}

	Result<const MetaEnumValueBase*> findValueResult(const CStr name) const
	{
		const MetaEnumValueBase* vb = findValue(name);
		if(nullptr == vb) {
			return Result<const MetaEnumValueBase*>::makeFailureWithString(
				formatString(
					"%s is not a valid %s enum value.",
					name.c_str(),
					getName().c_str()));
		}

		return Result<const MetaEnumValueBase*>{vb};
	}

	Result<const MetaEnumValueBase*> findValueResult(const uint64_t value) const
	{
		const MetaEnumValueBase* pV{findValue(value)};

		if(!pV)
		{
			return Result<const MetaEnumValueBase*>::makeFailureWithString(
				formatString(
					"%d is not a valid %s enum value.",
					(int) value,
					getName().c_str()));
		}

		return Result<const MetaEnumValueBase*>{pV};
	}

	const MetaEnumValueBase* findValue(CStr name) const {
		return mValuesByName.find(name);
	}

private:

	size_t mNumBytes;

	RbTree<
		MetaEnumValueBase,
		CStr,
		KeyFinderGetter<MetaEnumValueBase, CStr, & MetaEnumValueBase::getName>,
		NodeFinderField<
			MetaEnumValueBase,
			RbTreeNode,
			&MetaEnumValueBase::mNameTreeNode>> mValuesByName;

	RbTree<
		MetaEnumValueBase,
		uint64_t,
		KeyFinderGetter<MetaEnumValueBase, uint64_t, & MetaEnumValueBase::getValue>,
		NodeFinderField<
			MetaEnumValueBase,
			RbTreeNode,
			&MetaEnumValueBase::mValueTreeNode >> mValuesByValue;
};

template<typename T>
class MetaEnum final : public MetaEnumBase
{
public:
	MetaEnum(const char* pName)
		: MetaEnumBase(pName, sizeof(T), typeid(T))
	{}

	virtual Result<Success> load(void* pVoidEnum, ObjectReadStream& readStream) const override
	{
		std::string valueName;
		Result<Success> r = readStream.readCVariableName(valueName);
		if(!r)
		{
			return r;
		}

		Result<const MetaEnumValueBase*> resEnumValue = findValue(valueName.c_str());

		if(!resEnumValue)
		{
			return Result<Success>{resEnumValue.moveError()};
		}

		const MetaEnumValue<T>* pValue = downCast<const MetaEnumValue<T>*>(*resEnumValue);

		T* pT = static_cast<T*>(pVoidEnum);

		(*pT) = static_cast<T>(pValue->getTypedValue());

		return Result{Success{}};
	}

	virtual Result<Success> save(const void* pVoidEnum, ObjectWriteStream& writeStream) const override
	{
		const T* pEnum = static_cast<const T*>(pVoidEnum);

		Result<const MetaEnumValueBase*> r = findValue(static_cast<uint64_t>(*pEnum));
		if(!r)
		{
			return Result<Success>{r.moveError()};
		}

		return writeStream.writeCVariableName((*r)->getName());
	}
};

template<typename T>
MetaEnumValue<T>::MetaEnumValue(
	MetaEnum<T>* pEnum,
	CStrArg name,
	T value)
	: MetaEnumValueBase(
		pEnum,
		name,
		static_cast<uint64_t>(value))
{
}