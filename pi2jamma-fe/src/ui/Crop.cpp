#include "ui/Crop.hpp"

#include "core/debug.hpp"
#include "ui/Point.hpp"

namespace ui
{

Rect clipRect(const Rect& clipToRect, const Rect& drawRect)
{
	UnitType l = drawRect.getLeft();
	UnitType t = drawRect.getTop();
	UnitType r = drawRect.getRightExclusive();
	UnitType b = drawRect.getBottomExclusive();

	UnitType clipL = clipToRect.getLeft();
	UnitType clipT = clipToRect.getTop();
	UnitType clipR = clipToRect.getRightExclusive();
	UnitType clipB = clipToRect.getBottomExclusive();

	if(l < clipL)
	{
		l = clipL;
	}

	if(r > clipR)
	{
		r = clipR;
	}

	if(t < clipT)
	{
		t = clipT;
	}

	if(b > clipB)
	{
		b = clipB;
	}

	return Rect(l, t, r - l, b - t);
}

FitRectResult clipAndCrop(
	const Rect& clipToRect,
	const Rect& targetRect,
	const Rect& sourceRect,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
{
	Rect clippedTargetRect
		= clipRect(clipToRect, targetRect);

	float widthRatio =
		static_cast<float>(clippedTargetRect.getWidth())
			/ static_cast<float>(targetRect.getWidth());

	float heightRatio =
		static_cast<float>(clippedTargetRect.getHeight())
			/ static_cast<float>(targetRect.getHeight());

	UnitType sourceWidth = sourceRect.getWidth() * widthRatio;
	UnitType x;

	if(HorizontalAlignment::Left == horizontalAlignment)
	{
		x = sourceRect.getX();
	}
	else if(HorizontalAlignment::Center == horizontalAlignment)
	{
		UnitType xOffset = static_cast<UnitType>(
			(clippedTargetRect.getX() - targetRect.getX())
				* widthRatio);

		x = sourceRect.getX() + xOffset;
	}
	else if(HorizontalAlignment::Right == horizontalAlignment)
	{
		x = sourceRect.getRightExclusive() - sourceWidth;
	}

	UnitType sourceHeight = sourceRect.getHeight() * heightRatio;
	UnitType y;

	if(VerticalAlignment::Top == verticalAlignment)
	{
	 	y = sourceRect.getY();
	}
	else if(VerticalAlignment::Center == verticalAlignment)
	{
		UnitType yOffset =
			static_cast<UnitType>(
				(clippedTargetRect.getTop() - targetRect.getTop()) * heightRatio);

		y = sourceRect.getTop() + yOffset;
	}
	else
	{
		y = sourceRect.getBottomExclusive() - sourceHeight;
	}

	Rect sourceRectOut(
		x,
		y,
		sourceWidth,
		sourceHeight);

	return
		FitRectResult(
			clippedTargetRect,
			sourceRectOut);
}

Rect getRectForSizeAlignedInRect(
	const Size& sourceSize,
	const Rect& targetRect,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
{
	Rect rect;
	rect.setSize(sourceSize);

	if(HorizontalAlignment::Center == horizontalAlignment)
	{
		rect.setX(targetRect.getX() + (targetRect.getWidth()-rect.getWidth())/2);
	}
	else if(HorizontalAlignment::Left == horizontalAlignment)
	{
		rect.setX(targetRect.getX());
	}
	else if(HorizontalAlignment::Right == horizontalAlignment)
	{
		rect.setX(targetRect.getRightExclusive() - sourceSize.getWidth());
	} 

	if(VerticalAlignment::Center == verticalAlignment)
	{
		rect.setY(targetRect.getY() + (targetRect.getHeight() - rect.getHeight()) / 2);
	}
	else if(VerticalAlignment::Top == verticalAlignment)
	{
		rect.setY(targetRect.getY());
	}
	else if(VerticalAlignment::Bottom == verticalAlignment)
	{
		rect.setY(targetRect.getBottomExclusive() - sourceSize.getHeight());
	}

	return rect;
}

FitRectResult fitRect(
	const Rect& sourceRect,
	const Rect& targetRect,
	CropMode cropMode,
	HorizontalAlignment cropHorizontalAlignment,
	VerticalAlignment cropVerticalAlignment,
	HorizontalAlignment horizontalAlignment,
	VerticalAlignment verticalAlignment)
{
	Size resultingSize;

	if(CropMode::AspectFit == cropMode)
	{
		float sourceAspectRatio = sourceRect.getAspectRatio();
		float rectToFitAspectRatio = targetRect.getAspectRatio();

		if(rectToFitAspectRatio < sourceAspectRatio)
		{
			resultingSize.setWidth(targetRect.getWidth());
			resultingSize.setHeight(resultingSize.getWidth() / sourceAspectRatio);
		}
		else
		{
			resultingSize.setHeight(targetRect.getHeight());
			resultingSize.setWidth(resultingSize.getHeight() * sourceAspectRatio);
		}
	}
	else if(CropMode::AspectFill == cropMode)
	{
		ASSERTMSG(false, "CropMode::AspectFill not implemented");
	}
	else if(CropMode::Stretch == cropMode)
	{
		resultingSize = targetRect.getSize();
	}
	else if(CropMode::None == cropMode)
	{
		resultingSize = sourceRect.getSize();
	}

	Rect sourceTargetRect =
		getRectForSizeAlignedInRect(
			resultingSize,
			targetRect,
			horizontalAlignment,
			verticalAlignment);

	auto result =
		clipAndCrop(
			targetRect,
			sourceTargetRect,
			sourceRect,
			cropHorizontalAlignment,
			cropVerticalAlignment);

	return result;
}

}

