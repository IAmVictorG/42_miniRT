#include "../../includes/miniRT.h"

t_color color_add(t_color color1, t_color color2)
{
    t_color result;

    result.r = color1.r + color2.r;
    result.g = color1.g + color2.g;
    result.b = color1.b + color2.b;

    // Clamp 
    //result.r = result.r > 255.0f ? 255.0f : result.r;
    //result.g = result.g > 255.0f ? 255.0f : result.g;
    //result.b = result.b > 255.0f ? 255.0f : result.b;

    return result;
}

t_color color_multiply_scalar(t_color color, float scalar)
{
	t_color r;
	
	r.r = (int) roundf(color.r * scalar);
	r.g = (int) roundf(color.g * scalar);
	r.b = (int) roundf(color.b * scalar);
	return (r);
}
