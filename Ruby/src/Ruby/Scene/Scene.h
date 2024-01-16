//#pragma once
//
//namespace Ruby {
//
//	class Entity;
//
//	class Scene
//	{
//	public:
//		Scene(const std::string& name);
//		//TODO: Make constructor for scene with filename
//		~Scene();
//
//		Entity createEntity();
//
//	private:
//		/*struct EntityEntry
//		{
//			Entity Self;
//			std::vector<uint32_t> Children;
//		};*/
//
//		std::string m_Name;
//		//std::unordered_map<uint32_t, EntityEntry> m_EntityList;
//		entt::registry m_Registry;
//
//		friend class Entity;
//	};
//
//}