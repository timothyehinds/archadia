#pragma once

#if 0
#include "core/container/RbTree.hpp"
#include "core/RefCounted.hpp"
#include "core/StringUtil.hpp"
#include "core/Type.hpp"
#include <vector>

using JsonIntType = uint64_t;
using JsonFloatType = double;

class JsonBase : public RefCounted
{
    public:
    
        typedef ref< JsonBase > Ref;

        enum class Type { Null, Class, Array, String, Integer, Real, Boolean };

        inline Type GetType( void ) const { return mType; }
    
        virtual const std::string& GetString( ) { return gEmptyString; }
    
        // Formatted for debugging
    
        virtual const std::string Dump( JsonIntType indent ) = 0;
    
        // Mimized
    
        virtual const std::string Stringify( ) const = 0;

        virtual size_t GetCount( void ) { return 0; }
        virtual Ref GetAt( JsonIntType i ) { return NULL; }
    
    	virtual void push_back( Arg< JsonBase::Ref >::Type refValue ) { }
    
        virtual Ref GetValueForKey( CStr key );
        virtual void SetValueForKey( std::string key, Arg< JsonBase::Ref >::Type refJson );
    	virtual void SetIntegerValueForKey( std::string key, Arg< JsonIntType >::Type val );
        virtual void SetStringValueForKey( std::string key, Arg< std::string >::Type val );
    
        virtual Ref GetFirst( ) { return NULL; }
        virtual Ref GetNext( ) { return NULL; }

        virtual const bool GetBool( ) { return false; }
        virtual const JsonFloatType GetFloat64( void ) { return 0.0; }
        virtual const JsonIntType GetUInt( void ) { return 0; }
    
        virtual ~JsonBase( ) { }
    
    protected:
    
        JsonBase( Type type ) : mType( type ) { }
    
    private:
    
        Type mType;
};

class JsonNull : public JsonBase
{
    public:
    
        static const ref<JsonNull>& singleton() {
            return sSingleton;
        }

        JsonNull( void ) : JsonBase( Type::Null ) { }
    
        virtual const std::string Dump( JsonIntType indent ) override
        {
            return Sl( "null" );
        }
    
        virtual const std::string Stringify( ) const override
        {
        	return Sl( "null" );
    	}

    private:

        static ref<JsonNull> sSingleton;
};

class JsonString : public JsonBase
{
    public:
    
        inline JsonString( Arg< std::string >::Type s ) : JsonBase( Type::String ), mString( s ) { }
    
        virtual const std::string& GetString( void ) override { return mString; }
    
        virtual const std::string Dump( JsonIntType i ) override
        {
            return Fs( "\"%s\"", mString.c_str() );
        }
    
        virtual const std::string Stringify( ) const override
    	{
        	const std::string s =
            	Sl( "\"" ) +
                OmStringEscape( mString, OM_STRING_C_ESCAPE_ITEMS ) +
                Sl( "\"" );
            
            return s;
    	}
    
    private:
    
        std::string mString;
};

class JsonInteger : public JsonBase
{
    public:
    
        inline JsonInteger( JsonIntType value )
            :
            JsonBase( Type::Integer ),
            mValue( value )
        { }
    
        virtual const std::string Dump( JsonIntType i ) override
        {
            return Fs( "%i", (int) mValue );
        }
    
    	virtual const std::string Stringify( ) const override
    	{
        	return Fs( "%i", (int) mValue );
        }
    
        virtual const JsonFloatType GetFloat64( void ) override
        {
            return static_cast< JsonFloatType >( mValue );
        }
    
        virtual const JsonIntType GetUInt( void ) override
        {
            return static_cast< JsonIntType >( mValue );
        }
    
    private:
    
        JsonIntType mValue;
        
};

class JsonReal : public JsonBase
{
    public:
    
        inline JsonReal( JsonFloatType real ) : JsonBase( Type::Real ), mReal( real ) { }
    
        virtual const std::string Dump( JsonIntType indent ) override
        {
            return Stringify();
        }
    
        virtual const std::string Stringify(  ) const override
        {
            return Fs( "%g", mReal );
        }
    
        virtual const JsonFloatType GetFloat64( void ) override
        {
            return mReal;
        }
    
        virtual const JsonIntType GetUInt( void ) override
        {
            return static_cast< JsonIntType >( mReal );
        }    
    
    private:
    
        JsonFloatType mReal;
};


class JsonBoolean : public JsonBase
{
    public:
    
        inline JsonBoolean( bool value ) : JsonBase( Type::Boolean ), mValue( value ) { }
    
        virtual const std::string Dump( JsonIntType indent ) override
        {
        	return Stringify();
        }
    
        virtual const std::string Stringify( ) const override
    	{
            return mValue ? Sl( "true" ) : Sl( "false" );
        }
    
        virtual const bool GetBool( void ) override
        {
            return mValue;
        }
    
    private:
    
        bool mValue;
};

class JsonObject : public JsonBase
{
    public:

        class Field
        {
            public:

                Field(std::string key, ref<JsonBase> refValue)
                    : mName(std::move(key))
                    , mrefValue(std::move(refValue))
                {
                }

                CStr getName() const {
                    return mName.c_str();
                }

                void setName(std::string name) {
                    mName = std::move(name);
                }

                ref<JsonBase> getValue() const {
                    return mrefValue;
                }

                void setValue(ref<JsonBase> value) {
                    mrefValue = std::move(value);
                }

            private:

                std::string mName;
                ref<JsonBase> mrefValue;

            public:

                RbTreeNode mTreeNode;
        };
    
        using DictType = RbTree<
            Field,
            CStr,
            KeyFinderGetter<Field, CStr, &Field::getName>,
            NodeFinderField<Field, & Field::mTreeNode>,
            LifetimePolicyDelete<Field>>;
    
        inline JsonObject( void ) : JsonBase( Type::Class ) { }

        virtual JsonBase::Ref GetValueForKey(CStr key ) override
        {
            Field* pField = mDictionary.find(key);
            if(nullptr == pField) {
                return JsonNull::singleton();                
            }

            return pField->getValue();
        }
    
        virtual void SetValueForKey(
        	std::string key,
            Arg< JsonBase::Ref >::Type refJson ) override
        {
            Field* pField = mDictionary.find(key.c_str());
            if(pField) {
                pField->setValue(refJson);
                return;
            }
            
            pField = new Field(std::move(key), refJson);

            mDictionary.insert(*pField);
        }
    
    	virtual void SetIntegerValueForKey(
        	std::string key,
            Arg< JsonIntType >::Type intValue ) override
    	{
        	SetValueForKey( std::move(key), new JsonInteger( intValue ) );
        }
    
        virtual void SetStringValueForKey(
        	std::string key,
            Arg< std::string >::Type value ) override
    	{
        	SetValueForKey( std::move(key), new JsonString( value ) );
        }
    
        virtual const std::string Dump( JsonIntType indentAmount ) override
        {
            std::string s = Sl( "{\n" );
            
            for(auto&& field: mDictionary)
            {
                s += indent( indentAmount + 1 ) + field.getName().c_str();
                s += Sl( " : " );
                s += field.getValue()->Dump( indentAmount + 2 );
                s += Sl( "\n" );
            }
            
            s += Sl( "\n" ) + indent( indentAmount ) + Sl( "}" );
            
            return s;
        }
    
        virtual const std::string Stringify( ) const override
    	{
           	std::string s = Sl( "{" );
            
            bool first = true;
            for(auto&& field : mDictionary)
            {
                if(first) {
                    first = false;
                }
                else {
                    first = false;
                	s += Sl( "," );
            	}
                
                s = s + Sl( "\"" ) + field.getName().c_str() + Sl( "\"" );
                s = s + Sl( ":" );
                s = s + field.getValue()->Stringify();
            }
            
            s += Sl( "}" );
            
            return s;
    	}
    
    private:
    
        DictType mDictionary;
};

class JsonArray : public JsonBase
{
    public:
    
        inline JsonArray( void ) : JsonBase( Type::Array ) { }

        inline void PushBack( Arg< JsonBase::Ref >::Type refJson )
        {
            mVector.push_back( refJson );
        }
    
        virtual const std::string Dump( JsonIntType indentAmount ) override
        {
            std::string s = Sl( "\n" ) + indent( indentAmount ) + Fs( "[ (%d)\n", mVector.size() );
            
            for(size_t i = 0; i < mVector.size(); i ++ )
            {
                s += indent( indentAmount + 1 ) + Fs( "%d. ", i ) + mVector[ i ]->Dump( indentAmount + 2 ) + Sl( "\n" );
            }
            
            s += indent( indentAmount ) + Sl( "]" );
            
            return s;
        }
    
        virtual const std::string Stringify( ) const override
    	{
          	std::string s = Sl( "[" );
            
            for(size_t i = 0; i < mVector.size(); i ++ )
            {
            	if ( i > 0 )
            	{
                	s += Sl( "," );
            	}
                
                s += mVector[ i ]->Stringify( );
            }
            
            s += Sl( "]" );
            
            return s;
        }
    
        virtual size_t GetCount( void ) override
        {
            return mVector.size();
        }
    
        virtual JsonBase::Ref GetAt( JsonIntType i ) override
        {
            return mVector[i];
        }
    
    private:
    
        std::vector< JsonBase::Ref > mVector;
};

class Json
{
    public:
    
        Json( JsonBase::Ref refJson )
        {
            ASSERT( refJson.isValid() );

            mrefJson = refJson;
        }
    
        Json( Arg< std::string >::Type string )
        {
            mrefJson = new JsonString( string );
        }
    
        Json( Arg< JsonIntType >::Type integer )
    	{
        	mrefJson = new JsonInteger( integer );
        }
    
        Json( )
        {
            mrefJson = JsonNull::singleton();
        }

        inline JsonBase::Type GetType() const
        {
            return mrefJson->GetType();
        }
        
        inline bool IsObject() const
        {
            return GetType() == JsonBase::Type::Class;
        }

        inline bool IsString() const
        {
            return GetType() == JsonBase::Type::String;
        }

        inline bool IsBool() const
        {
            return GetType() == JsonBase::Type::Boolean;
        }

        inline void PushBack( Json json )
        {
        	mrefJson->push_back( json.mrefJson );
    	}
    
        inline const Json GetAt( JsonIntType i ) const
        {
            return Json( mrefJson->GetAt( i ) );
        }
        
        inline size_t GetCount( void ) const
        {
            return mrefJson->GetCount();
        }
    
        inline const std::string Dump( JsonIntType indent ) const
        {
            return mrefJson->Dump( indent );
        }
    
    	inline const std::string Stringify( ) const
    	{
        	return mrefJson->Stringify();
        }
    
        inline const Json GetValueForKey(CStr fieldName ) const
        {
            return Json( mrefJson->GetValueForKey(fieldName));
        }
    
        inline const void SetValueForKey(
            Arg< std::string >::Type key,
            Arg< Json >::Type json )
        {
            mrefJson->SetValueForKey( key, json.mrefJson );
        }
    
    	inline void SetIntegerValueForKey(
        	Arg< std::string >::Type key,
            Arg< JsonIntType >::Type intValue )
    	{
        	SetValueForKey( key, Json( new JsonInteger( intValue ) ) );
        }
    
        inline void SetStringValueForKey(
        	Arg< std::string >::Type key,
            Arg< std::string >::Type value )
    	{
        	SetValueForKey( key, Json( new JsonString( value ) ) );
        }
    
        template<typename T> T GetFloat() const
        {
            return static_cast<T>(GetFloat64());
        }

        inline const std::string& GetString( void ) const
        {
            return mrefJson->GetString();
        }
    
        inline const JsonFloatType GetFloat64( void ) const
        {
            return mrefJson->GetFloat64();
        }
    
        inline JsonIntType GetUInt( void ) const
        {
            return mrefJson->GetUInt();
        }
    
        template<typename T>
        inline T GetInteger(void) const
        {
            return static_cast<T>(GetUInt());
        }

        inline const bool GetBool( void ) const
        {
            return mrefJson->GetBool();
        }
    
        inline bool IsNull( void ) const
        {
        	return JsonBase::Type::Null == mrefJson->GetType();
    	}
    
        inline const Json operator[] ( CStr s ) const
        {
            return Json( mrefJson->GetValueForKey( s ));
        }
   
        inline const Json operator[] ( JsonIntType i ) const
        {
            return Json( mrefJson->GetAt( i ) );
        }

        inline bool operator==( Arg< std::string >::Type s ) const
        {
            return mrefJson->GetString() == s;
        }
    
        inline bool operator==( const char* pCString ) const
        {
            return mrefJson->GetString() == Ts( pCString );
        }

    
    private:
    
        JsonBase::Ref mrefJson;
};
#endif