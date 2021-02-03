#pragma once

#include "ui/Surface.hpp"
#include "ui/elements/Element.hpp"

namespace ui {

class Image final : public Element
{
	public:

		Image(
			Element* pParent,
			const Rect& rect);

		Image(
			Element* pParent,
			const Rect& rect,
			CStr filePath);

		Image(
			Element* pParent,
			const Rect& rect,
			ref<Surface> refSurface);

		void loadFromFile(CStr filePath);
		void setSurface(ref<ui::Surface> refSurface);
		
	protected:

		virtual void render(RenderContext& renderContext) override;

	private:

		ref<ui::Surface> m_refSurface;
};

}