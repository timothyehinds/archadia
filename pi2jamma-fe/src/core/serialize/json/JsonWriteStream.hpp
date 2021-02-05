#pragma once

#include "core/serialize/ObjectWriteStream.hpp"
#include <stack>

template<typename StreamType>
class JsonWriteStream : public ObjectWriteStream
{
public:
	JsonWriteStream(StreamType stream);

	virtual Result<Success> writeNativeFloat(double flt) override;
	virtual Result<Success> writeNativeInt(int64_t i) override;
	virtual Result<Success> writeBoolean(bool b) override;
	virtual Result<Success> writeCVariableName(StringSpan s) override;
	virtual Result<Success> writeString(StringSpan s) override;

	virtual Result<Success> beginObject() override;
	virtual Result<Success> beginField(StringSpan s) override;
	virtual Result<Success> endField() override;
	virtual Result<Success> endObject() override;
	virtual Result<Success> beginArray() override;
	virtual Result<Success> beginArrayItem() override;
	virtual Result<Success> endArrayItem() override;
	virtual Result<Success> endArray() override;
private:

	Result<Success> write(StringSpan s);
	Result<Success> write(char c);
	Result<Success> writeIndent();

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
Result<Success> JsonWriteStream<StreamType>::writeNativeFloat(double flt)
{
	char s[256];
	auto numChars = sprintf(s, "%f", flt);
	return mStream.write(s, numChars);
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::writeNativeInt(int64_t i)
{
	char s[256];
	auto numChars = sprintf(s, "%d", (int) i );
	return mStream.write(s, numChars);
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::writeBoolean(bool b)
{
	const char* pStr = b ? "true" : "false";

	return write(pStr);
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::writeCVariableName(StringSpan s)
{
	return writeString(s);
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::writeString(StringSpan s)
{
	OmStringEscape(mWorkArea, s, OM_STRING_C_ESCAPE_ITEMS );

	Result<Success> r = write('\"');
	if(!r) {
		return r;
	}

	r = mStream.write(mWorkArea.c_str(), mWorkArea.size());
	if(!r) {
		return r;
	}

	return write('\"');
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::beginObject()
{
	mStack.push(StackFrame(StackFrame::Type::Object));

	return write('{');
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::beginField(StringSpan name)
{
	static StringSpan sColon(" : ");

	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	if(stackFrame.mItemCount > 0) {
		Result<Success> r = write(',');
		if(!r) {
			return r;
		}
	}

	Result<Success> r = write('\n');
	if(!r) {
		return r;
	}

	r = writeIndent();
	if(!r) {
		return r;
	}

	r = writeString(name);

	if(!r) {
		return r;
	}

	r = write(sColon);
	if(!r) {
		return r;
	}

	stackFrame.mItemCount++;
	stackFrame.mInItem = true;
	return Result{Success{}};
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::endField()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	ASSERT(stackFrame.mItemCount > 0);
	ASSERT(stackFrame.mInItem);
	stackFrame.mInItem = false;
	return Result{Success{}};
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::endObject()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Object == stackFrame.mType);
	ASSERT(stackFrame.mInItem == false);
	mStack.pop();
	
	Result<Success> r = write('\n');
	if(!r) {
		return r;
	}

	r = writeIndent();
	if(!r) {
		return r;
	}
	return write('}');
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::beginArray()
{
	mStack.push(StackFrame(StackFrame::Type::Array));
	return write('[');

}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::beginArrayItem()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(!stackFrame.mInItem);
	stackFrame.mInItem = true;

	if(stackFrame.mItemCount > 0) {
		Result<Success> r = write(',');
		if(!r) {
			return r;
		}
	}

	Result<Success> r = write('\n');
	if(!r) {
		return r;
	}

	r = writeIndent();
	if(!r) {
		return r;
	}

	return Result{Success{}};
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::endArrayItem()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(stackFrame.mInItem);

	stackFrame.mInItem = false;
	stackFrame.mItemCount++;

	return Result{Success{}};
}

template<typename StreamType>
Result<Success> JsonWriteStream<StreamType>::endArray()
{
	ASSERT(mStack.size() > 0);
	StackFrame& stackFrame = mStack.top();
	ASSERT(StackFrame::Type::Array == stackFrame.mType);
	ASSERT(!stackFrame.mInItem);

	mStack.pop();

	return write( ']');
}

template<typename StreamType>
inline Result<Success> JsonWriteStream<StreamType>::write(StringSpan s)
{
	return mStream.write(s.begin(), s.size());
}

template<typename StreamType>
inline Result<Success> JsonWriteStream<StreamType>::write(char c)
{
	return mStream.write(& c, 1);
}

template<typename StreamType>
inline Result<Success> JsonWriteStream<StreamType>::writeIndent()
{
	static StringSpan indent("    ");

	for(unsigned int i = 0; i < mStack.size(); i ++ ) {
		Result<Success> r = write(indent);
		if(!r) {
			return r;
		}
	}

	return Result{Success{}};
}
