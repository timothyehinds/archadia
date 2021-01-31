#pragma once

#include "core/serialize/Serializer.hpp"

#include <vector>

template<typename ElementType>
struct Serializer<std::vector<ElementType>>
{
	static Result load(
		std::vector<ElementType>& v,
		ObjectReadStream& readStream)
	{
		Result r = readStream.beginArray();
		if(r.peekFailed()) {
			return r;
		}
		while(true)
		{
			bool moreElements = false;
			r = readStream.nextArrayItem(moreElements);
			if(r.peekFailed()) {
				return r;
			}

			if(!moreElements) {
				break;
			}

			ElementType e;

			r = ::load(e, readStream);
			if(r.peekFailed()) {
				return r;
			}

			v.push_back(std::move(e));
		}

		r = readStream.endArray();
		if(r.peekFailed()) {
			return r;
		}

		return Result::makeSuccess();
	}

	static Result save(
		const std::vector<ElementType>& v,
		ObjectWriteStream& writeStream)
	{
		Result r = writeStream.beginArray();
		if(r.peekFailed()) {
			return r;
		}

		for(auto&& e: v) {
			r = writeStream.beginArrayItem();
			if(r.peekFailed()) {
				return r;
			}
			r = ::save(e, writeStream);
			if(r.peekFailed()) {
				return r;
			}
			r = writeStream.endArrayItem();
			if(r.peekFailed()){
				return r;
			}
		}
		r = writeStream.endArray();
		if(r.peekFailed()) {
			return r;
		}

		return Result::makeSuccess();
	}		
};