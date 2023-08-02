// ON HOLD FOR NOW! Currently being worked on, as I want to develop my own ECS instead
// of using something like entt but right now I am working on sound and text rendering.

//#pragma once
//
//#include <stdint.h>
//typedef uint32_t EntityID;
//
//namespace Ruby {
//
//	inline constexpr uint32_t EntityIDMask = 0x003FFFFF;
//	inline constexpr uint32_t EntityIDMask = 0xFFC00000;
//	extern uint8_t ComponentIDCounter;
//
//	template<typename ComponentType>
//	inline uint8_t GetCompID()
//	{
//		static uint8_t id = Internal::ComponentIDCounter++;
//		return id;
//	}
//
//}