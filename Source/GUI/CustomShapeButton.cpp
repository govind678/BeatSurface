/*
  ==============================================================================

    CustomShapeButton.cpp
    Created: 1 Mar 2014 2:13:02am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#include "CustomShapeButton.h"

CustomShapeButton::CustomShapeButton (const String& t, Colour n, Colour o, Colour d)
                                    :   Button (t),
                                        normalColour (n), overColour (o), downColour (d),
                                        maintainShapeProportions (false),
                                        outlineWidth (0.0f),
                                        sizeDecreaseOnClick(10.0f)
{
}

CustomShapeButton::~CustomShapeButton() {}

void CustomShapeButton::setColours (Colour newNormalColour, Colour newOverColour, Colour newDownColour)
{
    normalColour = newNormalColour;
    overColour = newOverColour;
    downColour = newDownColour;
}

void CustomShapeButton::setOutline (Colour newOutlineColour, const float newOutlineWidth)
{
    outlineColour = newOutlineColour;
    outlineWidth = newOutlineWidth;
}

void CustomShapeButton::setBorderSize (BorderSize<int> newBorder)
{
    border = newBorder;
}

void CustomShapeButton::setShape (const Path& newShape,
                            const bool resizeNowToFitThisShape,
                            const bool maintainShapeProportions_)
{
    shape = newShape;
    maintainShapeProportions = maintainShapeProportions_;
    
    if (resizeNowToFitThisShape)
    {
        Rectangle<float> newBounds (shape.getBounds());
        
        setSize (1 + (int) (newBounds.getWidth()  + outlineWidth) + border.getLeftAndRight(),
                 1 + (int) (newBounds.getHeight() + outlineWidth) + border.getTopAndBottom());
    }
    
    repaint();
}

void CustomShapeButton::paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    if (! isEnabled())
    {
        isMouseOverButton = false;
        isButtonDown = false;
    }
    
    
    if (isButtonDown)
    {
        // Reduce Path Size here
    }
    
    else
    {
        
    }
    
    g.setColour (isButtonDown ? downColour
                 : isMouseOverButton ? overColour
                 : normalColour);
    
    g.fillPath(shape);
}


bool CustomShapeButton::hitTest(int x, int y)
{
    return shape.contains(x, y);
}

void CustomShapeButton::resized()
{
    
}


void CustomShapeButton::clicked(const juce::ModifierKeys &modifier)
{
    
}
