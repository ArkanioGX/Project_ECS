#pragma once
#include "Entity.h"
#include "Components.h"
#include <vector>
#include <variant>
#include <memory>
#include <numeric>
#include <complex>
#include <algorithm>
#include "Game.h"
using std::vector;
using std::shared_ptr;
class ECSManager {
public:
	void UpdateScene(float dt);
	void DrawScene();
	void PrepareDraw();
	unsigned long long CreateEntity();
	void RemoveEntity(unsigned long long entityId);
	Entity& FindEntity(unsigned long long entityId);
	Transform2D& CreateTransform2DComponent(unsigned long long entityId);
	Sprite& CreateSpriteComponent(unsigned long long entityId, const string& texName);
	Rigidbody2D& CreateRigidbody2DComponent(unsigned long long entityId, const Vector2& pos, const Rectangle& box);
	template<class T>
	T& GetComponent(unsigned long long entityId) {
		if constexpr (std::is_same_v<T, Transform2D>) {
			return transforms.at(FindEntityComponent(entityId, ComponentIndex::Transform2D));
		}
		else if constexpr (std::is_same_v<T, Sprite>) {
			return sprites.at(FindEntityComponent(entityId, ComponentIndex::Sprite));
		}
		else if constexpr (std::is_same_v<T, Rigidbody2D>) {
			return bodies.at(FindEntityComponent(entityId, ComponentIndex::Rigidbody2D));
		}
	}
private:
	static unsigned long long maxId;
	// Entities and components
	vector<unsigned long long> entityIds;
	vector<Entity> entities;
	vector<unsigned long long> entitiesToRemove{};
	vector<Transform2D> transforms;
	vector<Sprite> sprites;
	vector<Rigidbody2D> bodies;
	signed int FindEntityComponent(unsigned long long entityId, ComponentIndex componentIndex);
	void UpdateEntityWithComponent(unsigned long long entityId, int newComponentId, ComponentIndex componentIndex);
	void CleanRemovedEntities();
	void SystemPhysicsUpdate(float dt);
	void GameplayUpdate(float dt);
	void SystemSpriteDraw();
	template<class T>
	void RemoveComponent(vector<T>& components, Entity& removedEntity, ComponentIndex componentTypeIndex) {
		int typeIndex = static_cast<int>(componentTypeIndex);
		auto componentIndex = removedEntity.components.at(typeIndex);
		if (componentIndex != -1) {
			auto last = components.end() - 1;
			FindEntity(last->entityId).components[typeIndex] = componentIndex;
			auto removedComponent = components.begin() + componentIndex;
			std::iter_swap(removedComponent, last);
			components.pop_back();
		}
	}
	template<class T>
	void RemoveEntityComponent(unsigned long long entityId) {
		auto& removedEntity = FindEntity(entityId);
		if constexpr (std::is_same_v<T, Transform2D>) {
			RemoveComponent<Transform2D>(transforms, removedEntity, ComponentIndex::Transform2D);
		}
		else if constexpr (std::is_same_v<T, Sprite>) {
			RemoveComponent<Sprite>(sprites, removedEntity, ComponentIndex::Sprite);
		}
		else if constexpr (std::is_same_v<T, Rigidbody2D>) {
			RemoveComponent<Rigidbody2D>(bodies, removedEntity, ComponentIndex::Rigidbody2D);
		}
	}
};