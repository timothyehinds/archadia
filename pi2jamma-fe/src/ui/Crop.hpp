#pragma once

#include "core/Property.hpp"
#include "ui/CropMode.hpp"
#include "ui/HorizontalAlignment.hpp"
#include "ui/Size.hpp"
#include "ui/Rect.hpp"
#include "ui/VerticalAlignment.hpp"

namespace ui
{

Rect clipRect(const Rect& clipToRect, const Rect& drawRect);

struct FitRectResult
{
	FitRectResult(
		const Rect& targetRect,
		const Rect& sourceRect)
		: mTargetRect(targetRect)
		, mSourceRect(sourceRect) {}
	
	PROPERTY(Rect, TargetRect);
	PROPERTY(Rect, SourceRect);
};

FitRectResult clipAndCrop(
	const Rect& clipToRect,
	const Rect& targetRect,
	const Rect& sourceRect,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment);

FitRectResult fitRect(
	const Rect& sourceRect,
	const Rect& targetRect,
	CropMode cropMode,
	HorizontalAlignment clipHorizontalAlignment,
	VerticalAlignment clipVerticalAlignment,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment);

}



	




