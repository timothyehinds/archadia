#pragma once

#include "core/StringUtil.hpp"
#include "core/serialize/ObjectReadStream.hpp"
#include "core/parse/Parse.hpp"

#include <stack>

template<typename ParserType>
class JsonReadStream : public ObjectReadStream
{
public:

	using CharType = char;

	JsonReadStream(ParserType parser)
		: mParser(std::move(parser))
	{}

	virtual Result readNativeInteger(int64_t& integer) override;
	virtual Result readNativeFloat(double& flt) override
	{
		return Result::makeFailureWithStringLiteral("readFloat not implemented.");
	}

	virtual Result readString(std::string& str) override;
    virtual Result peekString(bool& isString) override;

	virtual Result readBoolean(bool& boolean) override;

	virtual Result readCVariableName(std::string& str) override
	{
		return readString(str);	
	}

	virtual Result beginArray() override;
	virtual Result nextArrayItem(bool& hasItem) override;
    virtual Result endArray() override;
    
	virtual Result peekObject(bool& isObject) override;
	virtual Result beginObject() override;
	virtual Result beginField(bool& done, std::string& name) override;
	virtual Result endField() override;
	virtual Result endObject() override;

    virtual Result makeError(CStr message) override;

private:
	
	Result parseUnicodeSequence(CharType* pRes);

	Result makeEofError();

    struct StackFrame {

        enum class Type {Object, Array};

        StackFrame(Type type)
            : mType(type)
            , mItemCount(0)
            , mInField(false)
        {
        }

        Type mType;
        unsigned int mItemCount;
        bool mInField;
    };

    ParserType mParser;
    std::stack<StackFrame> mStack;
};

template<typename ParserType>
Result JsonReadStream<ParserType>::readNativeInteger(int64_t& integer)
{
    integer = 0;

    OmParseEatWhite(&mParser);

    CharType c;
    if(!mParser.Next(&c)) {
        return makeEofError();
    }

    int64_t negMult = 1;

    if(c == '-')
    {
        negMult = -1;
        if(!mParser.Next(&c)) {
            return makeEofError();
        }
    }

    // Remaining number

    if(!OmCharIsNumber(c)) {
        return
            Result::makeFailureWithString(
                formatString(
                    "Expected decimal digit, not '%c'", c));
    }

    char digit = c - '0';
    // LogFmt("FirstDigit: %d\n", (int)digit);

    integer = (integer *10) + digit;

    // Remaining digits
    while(true) {
        if(!mParser.Peek(&c) || (!OmCharIsNumber(c))) {
            break;
        }

        char digit = c - '0';
        // LogFmt("Digit: %d\n", (int)digit);

        integer = (integer * 10) + digit;

        mParser.Next(&c);
    }

    integer*= negMult;

    // LogFmt("Integer! %d\n", (int) integer);

    return Result::makeSuccess();    
}

template<typename ParserType>
Result JsonReadStream<ParserType>::peekString(bool& isString)
{
    CharType c;
    isString = (mParser.Peek(&c) && ('\"' == c));
    return Result::makeSuccess();
}


template< typename ParserType >
Result JsonReadStream< ParserType >::readString( std::string& s )
{
	OmParseEatWhite(&mParser);
	CharType c;

	if(!mParser.Next(&c)) {
		return makeEofError();
	}

	if( c != '\"') {
		return makeError("Expected '\"'");
	}

    bool escape = false;

	s.clear();


    while( 1 )
    {
        if ( ! mParser.Peek( & c ) )
        {
            return makeEofError();
        }
        
        if ( escape )
        {
            switch( c )
            {
                case '\"':
                case '\\':
                case '/':
                    break;
                case 'b': c = '\b'; break;
                case 'f': c = '\f'; break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                	s += c;
                    break;
                case 'u':
                {
                    mParser.Next();
                
                    Result r = parseUnicodeSequence( & c );
                    if(r.peekFailed()) {
                    	return r;
                    }
                    s += c;
                    escape = false;
                    continue;
                }
                    
                default:
                	return makeError(
                		formatString("Unrecognized escape charater: %c", c ));
            }
            
            escape = false;
        }
        else if ( c == '\\' )
        {
            escape = true;
            mParser.Next();
            continue;
        }
        else if ( c == '\"' )
        {
            mParser.Next();
            break;
        }
        
        s += c;
        mParser.Next();
    }
    
    // LogFmt("Read string: %s\n", s.c_str());

    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::beginArray()
{
    OmParseEatWhite(&mParser);
    CharType c;
    if (!mParser.Next(&c)) {
        return makeEofError();
    }

    if('[' != c ) {
        return makeError("Expected '[' opening array.");
    }

    mStack.push(StackFrame(StackFrame::Type::Array));
    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::nextArrayItem(bool& hasItem)
{
    OM_ASSERT(mStack.size() > 0);
    StackFrame& stackFrame = mStack.top();
    OM_ASSERT(StackFrame::Type::Array == stackFrame.mType);

    hasItem = false;
    OmParseEatWhite(&mParser);

    CharType c;
    if (!mParser.Peek(&c)) {
        return makeEofError();
    }

    if(stackFrame.mItemCount > 0) {
        if(c != ',') {
            return Result::makeSuccess();
        }
        if (!mParser.Next(&c)) {
            return makeEofError();
        }
    }

    hasItem = true;
    stackFrame.mItemCount++;

    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::endArray()
{
    ASSERT(mStack.size() > 0);
    StackFrame& stackFrame = mStack.top();
    ASSERT(StackFrame::Type::Array == stackFrame.mType);

    OmParseEatWhite(&mParser);

    CharType c;
    if(!mParser.Next(&c)) {
        return makeError("Unexpected EOF looking for ']' ending array.");
    }

    if(']' != c ) {
        return
            makeError(
                formatString(
                    "Expected ']' ending array, not '%c'",
                    c ));
    }

    mStack.pop();

    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::peekObject(bool& isObject)
{
    OmParseEatWhite(&mParser);

    CharType c;
    
    if (!mParser.Peek(&c) || (c != '{'))
    {
        isObject = false;
        return Result::makeSuccess();
    }

    isObject = true;

    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::beginObject()
{
	OmParseEatWhite(&mParser);

    CharType c;
    
    if ( ! mParser.Next( & c ) )
    {
        return makeEofError();
    }

    if( c != '{')
    {
    	return makeError(formatString("Expected '{', not %c", c));
    }

    mStack.push(StackFrame(StackFrame::Type::Object));

    //Log("StackFrame!\n");
    
    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::endObject()
{
    ASSERT(mStack.size() > 0);
    mStack.pop();

    OmParseEatWhite(&mParser);
    CharType c;
    if(!mParser.Next(&c)) {
        return makeEofError();
    }

    if( c != '}') {
        return makeError(
            formatString(
                "Expected '}' not '%c' closing stackFrame",
                c));
    }

    return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::beginField(bool& gotField, std::string& name)
{
    ASSERT(mStack.size() > 0);
    StackFrame& stackFrame = mStack.top(); 
    ASSERT(StackFrame::Type::Object == stackFrame.mType);  
    ASSERT(!stackFrame.mInField); 

    gotField = false;

	OmParseEatWhite(&mParser);

    CharType c;

    // LogFmt("FieldCount: %d\n", (int) stackFrame.mItemCount);

    if(stackFrame.mItemCount > 0) {
        if(!mParser.Peek(&c)) {
            return makeEofError();
        }
        // LogFmt("FieldCount > 0, char: %c\n", c);

        if(c != ',') {
            return Result::makeSuccess();
        }

        mParser.Next(&c);
        OmParseEatWhite(&mParser);
    }

    mParser.Peek(&c);

    // LogFmt("FieldChar %c\n", c);

    if( c != '\"') {
        return Result::makeSuccess();
    }

	Result r = readString(name);
	if(r.peekFailed()) {
		return r;
	}

	// LogFmt("FieldName: %s\n", name.c_str());
	OmParseEatWhite(&mParser);

	if( !mParser.Next(&c)) {
		return makeEofError();
	}

	// LogFmt("char: %c\n", c);

	if( c != ':') {
		return makeError("Expected ':', after field name.");
	}

	gotField = true;
	// LogFmt("Field! %s\n", name.c_str());

    stackFrame.mInField = true;
	return Result::makeSuccess();
}

template<typename ParserType>
Result JsonReadStream<ParserType>::endField()
{
    ASSERT(mStack.size() > 0);
    StackFrame& stackFrame = mStack.top();
    ASSERT(stackFrame.mInField);
    stackFrame.mInField = false;
    stackFrame.mItemCount++;

    OmParseEatWhite(&mParser);
    CharType c;

    if(!mParser.Peek(&c))
    {
        return makeEofError();
    }

    if(c == ',' )
    {
        return Result::makeSuccess();

    }

    if(c == '}')
    {
        return Result::makeSuccess();
    }

    return makeError(formatString("Expected ',' or '}', not %c", c));
}
template<typename ParserType>
Result JsonReadStream<ParserType>::readBoolean(bool& boolean)
{
    OmParseEatWhite(&mParser);

    if( OmParseMatch(&mParser, "true")) {
        boolean = true;
        return Result::makeSuccess();
    }

    if(OmParseMatch(&mParser, "false")) {
        boolean = false;
        return Result::makeSuccess();
    }

    return makeError("Expected 'true' or 'false'.");
}


template<typename ParserType>
Result JsonReadStream<ParserType>::makeEofError()
{
	return makeError("Unexpected end of file.");
}

template<typename ParserType>
Result JsonReadStream<ParserType>::makeError(CStr message)
{
	return
        Result::makeFailureWithString(
            formatString(
                "Error: %s\n\tFile:%s\n\tLine:%d\n\tColumn:%d\n",
                message.c_str(),
                mParser.GetFilename().c_str(),
                (int) mParser.GetLineCount(),
                (int) mParser.GetColumnCount()));
}

template< typename ParserType >
Result JsonReadStream< ParserType >::parseUnicodeSequence( CharType* pRes )
{
    CharType value = 0;
    
    OmLoopI( 4 )
    {
        CharType c;
        
        if ( ! mParser.Peek( & c ) )
        {
            return makeEofError();
        }
        
        CharType hexDigit;
    
        if ( ( c >= '0' ) && ( c <= '9' ) )
        {
            hexDigit = ( c  - '0' );
        }
        else if ( c >= 'a' && ( c <= 'f' ) )
        {
            hexDigit = 0xA + ( c - 'a' );
        }
        else if ( c >= 'A' && ( c <= 'F' ) )
        {
            hexDigit = 0xA + ( c - 'A' );
        }
        else
        {
            return makeError( "Not a hex digit in \\u unicode sequence" );
        }

        value |= ( hexDigit << ( 4 * ( 3 - i ) ) );
    
        mParser.Next();
    }
    
    *pRes = value;
    
    return Result::makeSuccess();
}
