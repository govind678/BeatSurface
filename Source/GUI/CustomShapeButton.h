/*
  ==============================================================================

    CustomShapeButton.h
    Created: 1 Mar 2014 2:13:02am
    Author:  Govinda Ram Pingali

  ==============================================================================
*/

#ifndef CUSTOMSHAPEBUTTON_H_INCLUDED
#define CUSTOMSHAPEBUTTON_H_INCLUDED

#include "JuceHeader.h"

/**
    Extended Juce ShapeButton with hit test and modifier keys
 
    @see ShapeButton
*/

class CustomShapeButton     :   public Button
{
public:
    
    /** Creates a CustomShapeButton.
     
     @param name             a name to give the component - see Component::setName()
     @param newShape         the path to fill the button
     */
    CustomShapeButton(const String& t, Colour n, Colour o, Colour d);
    
    /** Destructor. */
    ~CustomShapeButton();
    
    
    /** Sets the shape to use.
     
     @param newShape                 the shape to use
     @param resizeNowToFitThisShape  if true, the button will be resized to fit the shape's bounds
     @param maintainShapeProportions if true, the shape's proportions will be kept fixed when
     the button is resized
     @param hasDropShadow            if true, the button will be given a drop-shadow effect
     */
    void setShape (const Path& newShape,
                   const bool resizeNowToFitThisShape,
                   const bool maintainShapeProportions_);
    
    
    /** Set the colours to use for drawing the shape.
     
     @param normalColour     the colour to fill the shape with when the mouse isn't over
     @param overColour       the colour to use when the mouse is over the shape
     @param downColour       the colour to use when the button is in the pressed-down state
     */
    void setColours(Colour normalColour,
                    Colour overColour,
                    Colour downColour);
    
    
    /** Sets up an outline to draw around the shape.
     
     @param outlineColour        the colour to use
     @param outlineStrokeWidth   the thickness of line to draw
     */
    void setOutline (Colour newOutlineColour, const float newOutlineWidth);
    
    
    /** This lets you specify a border to be left around the edge of the button when
     drawing the shape.
     */
    void setBorderSize (BorderSize<int> newBorder);
    
    
    /** @internal */
    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    void resized();
    bool hitTest(int x, int y);
    void clicked(const ModifierKeys& modifier);
    

    
private:
    
    Path shape, test;
    Rectangle<float> bounds;
    Colour normalColour, overColour, downColour, outlineColour;

    BorderSize<int> border;
    bool maintainShapeProportions;
    float outlineWidth;
    float sizeDecreaseOnClick;
};



#endif  // CUSTOMSHAPEBUTTON_H_INCLUDED
