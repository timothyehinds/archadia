#pragma once

#include "Pi2Jamma/screens/ScreenThemeDescription.hpp"
#include "ui/BitmapFont.hpp"
#include "ui/Surface.hpp"

class ScreenTheme : public RefCounted
{
public:

    ScreenTheme(
        ScreenThemeDescription,
        CStr fillThemeDir);

    const ref<ui::Surface>& getBackgroundSurface() const;
    const ref<ui::BitmapFont>& getTitleBitmapFont() const;    
    const ref<ui::BitmapFont>& getSelectedBitmapFont() const;
    const ref<ui::BitmapFont> getUnselectedBitmapFont() const;
    const ScreenThemeDescription& getDescription() const;

private:
    ScreenThemeDescription m_description;

	ref<ui::Font> m_refFont;
    ref<ui::Surface> m_refBackgroundSurface;
    ref<ui::BitmapFont> m_refSelectedBitmapFont;
    ref<ui::BitmapFont> m_refUnselectedBitmapFont;
};

//------------------------------------------------------------------------------

inline const ref<ui::Surface>& ScreenTheme::getBackgroundSurface() const
{
    return m_refBackgroundSurface;
}

inline const ref<ui::BitmapFont>& ScreenTheme::getTitleBitmapFont() const
{
    // TODO: separate description.
    return m_refUnselectedBitmapFont;
}

inline const ref<ui::BitmapFont>& ScreenTheme::getSelectedBitmapFont() const
{
    return m_refSelectedBitmapFont;
}

inline const ref<ui::BitmapFont> ScreenTheme::getUnselectedBitmapFont() const
{
    return m_refUnselectedBitmapFont;
}

inline const ScreenThemeDescription& ScreenTheme::getDescription() const
{
    return m_description;
}
