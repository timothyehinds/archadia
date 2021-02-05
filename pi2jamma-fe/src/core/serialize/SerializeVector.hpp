#pragma once

#include "core/serialize/Serializer.hpp"

#include <vector>

template<typename ElementType>
struct Serializer<std::vector<ElementType>>
{
	static Result<Success> load(
		std::vector<ElementType>& v,
		ObjectReadStream& readStream)
	{
		Result<Success> r = readStream.beginArray();
		if(!r) {
			return r;
		}
		while(true)
		{
			bool moreElements = false;
			r = readStream.nextArrayItem(moreElements);
			if(!r) {
				return r;
			}

			if(!moreElements) {
				break;
			}

			ElementType e;

			r = ::load(e, readStream);
			if(!r) {
				return r;
			}

			v.push_back(std::move(e));
		}

		r = readStream.endArray();
		if(!r) {
			return r;
		}

		return Result{Success{}};
	}

	static Result<Success> save(
		const std::vector<ElementType>& v,
		ObjectWriteStream& writeStream)
	{
		Result<Success> r = writeStream.beginArray();
		if(!r) {
			return r;
		}

		for(auto&& e: v) {
			r = writeStream.beginArrayItem();
			if(!r) {
				return r;
			}
			r = ::save(e, writeStream);
			if(!r) {
				return r;
			}
			r = writeStream.endArrayItem();
			if(!r){
				return r;
			}
		}
		r = writeStream.endArray();
		if(!r) {
			return r;
		}

		return Result{Success{}};
	}		
};