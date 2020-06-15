#pragma once

#include <Component/Component.h>
#include <Entity/Entity.h>
#include <Entity/EntityMap.h>

#include <array>
#include <map>

namespace Starlight
{
	template<typename T>
	struct ComponentList
	{
		size_t nextId = 0;
		std::array<T, MAX_COMPONENTS>* data;
	};

	class IComponentManager
	{
	public:
		IComponentManager() = default;
		virtual ~IComponentManager() = default;
		IComponentManager(const IComponentManager&) = default;
		IComponentManager& operator=(const IComponentManager&) = default;
		IComponentManager(IComponentManager&&) = default;
		IComponentManager& operator=(IComponentManager&&) = default;
	};

	template<typename T>
	class ComponentManager : public IComponentManager
	{		
	private:
		ComponentList<T> m_componentList;
		EntityMap m_entityMap;

	public:
		ComponentManager()
		{
			m_componentList.data = static_cast<std::array<T, MAX_COMPONENTS>*>(malloc(sizeof(T) * MAX_COMPONENTS));
		}

		ComponentId AddComponent(Entity entity, T& component)
		{
			ComponentId id = m_componentList.nextId;
			m_componentList.data->at(id) = component;
			m_componentList.nextId++;
			m_entityMap.Add(entity, id);

			return id;
		}

		void RemoveComponent(Entity entity)
		{
			ComponentId id = m_entityMap.GetComponent(entity);

			ComponentId componentToMove = m_componentList.nextId--;
			m_componentList.data[id] = m_componentList.data[componentToMove];
			Entity entityToMove = m_entityMap.GetEntity(componentToMove);

			m_entityMap.Delete(entity);
			m_entityMap.Update(entityToMove, id);

			m_componentList.nextId--;
		}

		T* GetComponent(Entity entity)
		{
			ComponentId id = m_entityMap.GetComponent(entity);
			return &m_componentList.data->at(id);
		}
	};
}