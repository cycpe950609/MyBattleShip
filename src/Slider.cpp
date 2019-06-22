#include <algorithm>
#include <string>
#include <iostream>

#include "Point.hpp"
#include "Slider.hpp"
#include "Point.hpp"
#include "Resources.hpp"
#include "Collider.hpp"


Slider::Slider(float x, float y, float w, float h) :
	ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
	Bar("stage-select/bar.png", x, y, w, h),
	End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
	End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
	Position.x += w;
	Position.y += h / 2;
	Anchor = Engine::Point(0.5, 0.5);
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}
// TODO 4 (0/6): Finish the 6 functions below and ensure it can control both BGM and SFX volume.
//               The slider shouldn't be dragged outside the bar, and you should also keep the mouse-in / mouse-out effect.
void Slider::Draw() const {
	// TODO 4 (1/6): Draw all components.
	Bar.Draw();
	//End1.Draw();
	//End2.Draw();
	Engine::ImageButton::Draw();

}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
	// TODO 4 (2/6): Set the function pointer. Can imitate ImageButton's 'SetOnClickCallback'.
	OnValueChangedCallback = onValueChangedCallback;

}
void Slider::SetValue(float value) {
	// TODO 4 (3/6): Call 'OnValueChangedCallback' when value changed. Can imitate ImageButton's 'OnClickCallback'.
	//               Also move the slider along the bar, to the corresponding position.
	float new_value = value <= 1.0 ? value : 1.0;
	OnValueChangedCallback(new_value);
	Position.x = x + w*new_value;
}
void Slider::OnMouseDown(int button, int mx, int my) {
	// TODO 4 (4/6): Set 'Down' to true if mouse is in the slider.
	std::cout << button << std::endl;
	if(Engine::Collider::IsPointInBitmap(Engine::Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp))
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
	// TODO 4 (5/6): Set 'Down' to false.
	Down = false;
}

void Slider::OnMouseMove(int mx, int my) {
	// TODO 4 (6/6): Clamp the coordinates and calculate the value. Call 'SetValue' when you're done.
	mouseIn =  Engine::Collider::IsPointInBitmap(Engine::Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
    if (!mouseIn || !Enabled) bmp = imgOut;
    else bmp = imgIn;
    if(Down)
    {
        if(mx > x && mx < x + w)
        {
            SetValue((mx-x)/w);
        }
    }
}
