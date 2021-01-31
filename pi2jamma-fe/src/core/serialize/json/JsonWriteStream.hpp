#pragma once

#include "core/serialize/ObjectWriteStream.hpp"
#include <stack>

template<typename StreamType>
class JsonWriteStream : public ObjectWriteStream
{
public:
	JsonWriteStream(StreamType stream);

	virtual Result writeNativeFloat(double flt) override;
	virtual Result writeNativeInt(int64_t i) override;
	virtual Result writeBoolean(bool b) override;
	virtual Result writeCVariableName(StringSpan s) override;
	virtual Result writeString(StringSpan s) override;

	virtual Result beginObject() override;
	virtual Result beginField(StringSpan s) override;
	virtual Result endField() override;
	virtual Result endObject() override;
	virtual Result beginArray() override;
	virtual Result beginArrayItem() override;
	virtual Result endArrayItem() override;
	virtual Result endArray() override;
private:

	Result write(StringSpan s);
	Result write(char c);
	Result writeIndent();

	StreamType mStream;

	struct StackFrame {
		enum class Type { Object, Array };

		StackFrame(Type type)
			: mInItem(false)
			, mItemCount(0)
			, mType(type)
		{}

		bool mInItem;
		unsigned int mItemCount;
		Type mType;
	};

	std::stack<StackFrame> mStack;	
	std::string mWorkArea;

};

template<typename StreamType>
JsonWriteStream<StreamType>::JsonWriteStream(StreamType stream)
	: mStream(std::move(stream))
{
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::writeNativeFloat(double flt)
{
	char s[256];
	auto numChars = sprintf(s, "%f", flt);
	return mStream.write(s, numChars);
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::writeNativeInt(int64_t i)
{
	char s[256];
	auto numChars = sprintf(s, "%d", (int) i );
	return mStream.write(s, numChars);
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::writeBoolean(bool b)
{
	const char* pStr = b ? "true" : "false";

	return write(pStr);
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::writeCVariableName(StringSpan s)
{
	return writeString(s);
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::writeString(StringSpan s)
{
	OmStringEscape(mWorkArea, s, OM_STRING_C_ESCAPE_ITEMS );

	Result r = write('\"');
	if(r.peekFailed()) {
		return r;
	}

	r = mStream.write(mWorkArea.c_str(), mWorkArea.size());
	if(r.peekFailed()) {
		return r;
	}

	return write('\"');
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::beginObject()
{
	mStack.push(StackFrame(StackFrame::Type::Object));

	return write('{');
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::beginField(StringSpan name)
{
	static StringSpan sColon(" : ");

	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	if(stackFrame.mItemCount > 0) {
		Result r = write(',');
		if(r.peekFailed()) {
			return r;
		}
	}

	Result r = write('\n');
	if(r.peekFailed()) {
		return r;
	}

	r = writeIndent();
	if(r.peekFailed()) {
		return r;
	}

	r = writeString(name);

	if(r.peekFailed()) {
		return r;
	}

	r = write(sColon);
	if(r.peekFailed()) {
		return r;
	}

	stackFrame.mItemCount++;
	stackFrame.mInItem = true;
	return Result::makeSuccess();
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::endField()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	ASSERT(stackFrame.mItemCount > 0);
	ASSERT(stackFrame.mInItem);
	stackFrame.mInItem = false;
	return Result::makeSuccess();
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::endObject()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	ASSERT(stackFrame.mInItem == false);
	mStack.pop();
	
	Result r = write('\n');
	if(r.peekFailed()) {
		return r;
	}

	r = writeIndent();
	if(r.peekFailed()) {
		return r;
	}
	return write('}');
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::beginArray()
{
	mStack.push(StackFrame(StackFrame::Type::Array));
	return write('[');

}

template<typename StreamType>
Result JsonWriteStream<StreamType>::beginArrayItem()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(!stackFrame.mInItem);
	stackFrame.mInItem = true;

	if(stackFrame.mItemCount > 0) {
		Result r = write(',');
		if(r.peekFailed()) {
			return r;
		}
	}

	Result r = write('\n');
	if(r.peekFailed()) {
		return r;
	}

	r = writeIndent();
	if(r.peekFailed()) {
		return r;
	}

	return Result::makeSuccess();
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::endArrayItem()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(stackFrame.mInItem);

	stackFrame.mInItem = false;
	stackFrame.mItemCount++;

	return Result::makeSuccess();
}

template<typename StreamType>
Result JsonWriteStream<StreamType>::endArray()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(!stackFrame.mInItem);

	mStack.pop();

	return write( ']');
}

template<typename StreamType>
inline Result JsonWriteStream<StreamType>::write(StringSpan s)
{
	return mStream.write(s.begin(), s.size());
}

template<typename StreamType>
inline Result JsonWriteStream<StreamType>::write(char c)
{
	return mStream.write(& c, 1);
}

template<typename StreamType>
inline Result JsonWriteStream<StreamType>::writeIndent()
{
	static StringSpan indent("    ");

	for(unsigned int i = 0; i < mStack.size(); i ++ ) {
		Result r = write(indent);
		if(r.peekFailed()) {
			return r;
		}
	}

	return Result::makeSuccess();
}
