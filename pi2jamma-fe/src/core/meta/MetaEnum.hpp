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

	Result findValue(const MetaEnumValueBase*& vb, CStr name) const {
		vb = findValue(name);
		if(nullptr == vb) {
			return Result::makeFailureWithString(
				formatString(
					"%s is not a valid %s enum value.",
					name.c_str(),
					getName().c_str()));
		}

		return Result::makeSuccess();
	}

	Result findValue(const MetaEnumValueBase*& vb, uint64_t value) const {
		vb = findValue(value);
		if(nullptr == vb) {
			return Result::makeFailureWithString(
				formatString(
					"%d is not a valid %s enum value.",
					(int) value,
					getName().c_str()));
		}

		return Result::makeSuccess();
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

	virtual Result load(void* pVoidEnum, ObjectReadStream& readStream) const override
	{
		std::string valueName;
		Result r = readStream.readCVariableName(valueName);
		if(r.peekFailed()){
			return r;
		}

		const MetaEnumValueBase* pValueBase = 0;

		r = findValue(pValueBase, valueName.c_str());

		if(r.peekFailed()) {
			return r;
		}

		const MetaEnumValue<T>* pValue =
			downCast<const MetaEnumValue<T>*>(
				pValueBase);

		T* pT = static_cast<T*>(pVoidEnum);

		(*pT) = static_cast<T>(pValue->getTypedValue());

		return Result::makeSuccess();
	}

	virtual Result save(const void* pVoidEnum, ObjectWriteStream& writeStream) const override
	{
		const T* pEnum = static_cast<const T*>(pVoidEnum);

		const MetaEnumValueBase* pValueBase = nullptr;

		Result r = findValue(pValueBase, static_cast<uint64_t>(*pEnum));
		if(r.peekFailed()) {
			return r;
		}

		return writeStream.writeCVariableName(pValueBase->getName());
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