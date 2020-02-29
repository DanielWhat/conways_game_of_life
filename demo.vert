#version 330

layout (location = 0) in vec4 position; //even though we only used 2d coordinates, openGL will set the 3rd coordinate to 0 (and the 4th coordinate to 1 to indicate this is a point and not a vector)
layout (location = 1) in vec4 colour;

out vec4 out_colour; //colour should be output to the fragment shader

void main()
{
	gl_Position = position;  //generally, if this was the last shader in the pipeline, we would multiply by the model-view-projection matrix 
	                         //to put it into clip coordinates, but we're just doing 2d stuff for this simple example and you'll notice we
	                         //intentionally made the vertex coordinates in the .cpp file between 0 and 1, so we're already in clip coordinates
	out_colour = colour;
}
