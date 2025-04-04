#include "ECSManager.h"

unsigned long long ECSManager::maxId = 0;

void ECSManager::UpdateScene(float dt) {
	GameplayUpdate(dt);
	SystemPhysicsUpdate(dt);
}
void ECSManager::DrawScene() {
	PrepareDraw();
	SystemSpriteDraw();
	CleanRemovedEntities();
}
unsigned long long ECSManager::CreateEntity() {
	unsigned long long newId = maxId++;
	entityIds.emplace_back(newId);
	entities.emplace_back(newId);
	return newId;
}
Transform2D& ECSManager::CreateTransform2DComponent(unsigned long long entityId) {
	int newComponentId = static_cast<int>(transforms.size());
	transforms.emplace_back(entityId);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Transform2D);
	return transforms.back();
}
Sprite& ECSManager::CreateSpriteComponent(unsigned long long entityId, const string& texName) {
	int newComponentId = static_cast<int>(sprites.size());
	const Texture& tex = Assets::getTexture(texName);
	sprites.emplace_back(entityId, texName, static_cast<float>(tex.getWidth()), static_cast<float>(tex.getHeight()),0);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Sprite);
	return sprites.back();
}
Rigidbody2D& ECSManager::CreateRigidbody2DComponent(unsigned long long entityId, const Vector2& pos,
	const Rectangle& box) {
	int newComponentId = static_cast<int>(bodies.size());
	bodies.emplace_back(entityId, pos, box);
	UpdateEntityWithComponent(entityId, newComponentId, ComponentIndex::Rigidbody2D);
	return bodies.back();
}

void ECSManager::UpdateEntityWithComponent(unsigned long long entityId, int newComponentId, ComponentIndex componentIndex) {
	int iComponentIndex = static_cast<int>(componentIndex);
	FindEntity(entityId).components[iComponentIndex] = newComponentId;
}
void ECSManager::SystemSpriteDraw() {
	for (auto& sprite : sprites) {
		SDL_Rect dstRect = sprite.dstRect.toSDLRect();
		SDL_Rect srcSDL = sprite.srcRect.toSDLRect();

		SDL_RenderCopyEx(
			Game::instance().getRenderer().toSDLRenderer(),
			sprite.tex.toSDLTexture(),
			&srcSDL,
			&dstRect,
			sprite.rotation,
			nullptr,		// rotation point, center by default
			SDL_FLIP_NONE);
	}
#ifdef GDEBUG
	for (auto& body : bodies) {
		body.DrawDebug();
	}
#endif
}
void ECSManager::RemoveEntity(unsigned long long entityId) {
	entitiesToRemove.push_back(entityId);
}
void ECSManager::CleanRemovedEntities() {
	for (auto entityId : entitiesToRemove) {
		// Transform
		RemoveEntityComponent<Transform2D>(entityId);
		// Sprites
		RemoveEntityComponent<Sprite>(entityId);
		// Rigidbodies
		RemoveEntityComponent<Rigidbody2D>(entityId);
		std::erase(entityIds, entityId);
		std::erase_if(entities, [=](Entity entity) {
			return entity.id == entityId;
			});
	}
	entitiesToRemove.clear();
}
Entity& ECSManager::FindEntity(unsigned long long entityId) {
	auto itr = std::lower_bound(entityIds.begin(), entityIds.end(), entityId);
	return entities.at(std::distance(entityIds.begin(), itr));
}
int ECSManager::FindEntityComponent(unsigned long long entityId, ComponentIndex componentIndex) {
	return FindEntity(entityId).components.at(static_cast<int>(componentIndex));
}

void ECSManager::PrepareDraw() {
	// Update sprite positions
	for (auto& sprite : sprites) {
		const auto& transform = GetComponent<Transform2D>(sprite.entityId);
		sprite.rotation = transform.rotation;
		sprite.dstRect = { transform.pos.x, transform.pos.y,
		static_cast<float>(sprite.tex.getWidth()), static_cast<float>(sprite.tex.getHeight()) };
	}
}

void ECSManager::SystemPhysicsUpdate(float dt) {

}

void ECSManager::GameplayUpdate(float dt) {

}