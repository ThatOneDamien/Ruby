#pragma once

// From Hazel repository by The Cherno for easy use

typedef unsigned char MouseCode;

namespace Ruby { 
	
	namespace Mouse {


		enum : MouseCode
		{
			// From glfw3.h
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,
	
			ButtonLast   = Button7,
			LeftClick    = Button0,
			RightClick   = Button1,
			ScrollButton = Button2
		};


	}
}