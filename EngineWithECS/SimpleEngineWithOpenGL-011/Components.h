#include "Entity.h"
#include <optional>
#include <utility>
#include <vector>

#include <memory>
#include "Assets.h"
#include "Renderer.h"

class ECSManager;
using std::vector;

enum class ComponentIndex {
	Transform2D = 0,
	Sprite = 1,
	Rigidbody2D = 2
};

struct Transform2D {
	explicit Transform2D(unsigned long long entityIdP) : entityId{ entityIdP } {}
	unsigned long long entityId;
	Vector2 pos{ 0.0f, 0.0f };
	float rotation{ 0.0f };
	Vector2 scale{ 0.0f, 0.0f };
};

struct Sprite {
	explicit Sprite(unsigned long long entityIdP, const string& textNameP, float width, float height, float rot) :
		entityId{ entityIdP },
		srcRect{ 0, 0, width, height },
		dstRect{ 0, 0, width, height },
		texName{ textNameP },
		rotation{rot},
		tex{ Assets::getTexture(textNameP)}
	{}
	unsigned long long entityId;
	unsigned char opacity{ 255 };
	float rotation;
	Rectangle srcRect{ 0, 0, 1, 1 };
	Rectangle dstRect{ 0, 0, 1, 1 };
	string texName;
	Texture tex;
};

struct Rigidbody2D {
	explicit Rigidbody2D(unsigned long long entityIdP, const Vector2& pos, const Rectangle& box) :
		entityId{ entityIdP },
		pos{ pos },

		boundingBox{ box }
	{}
	unsigned long long entityId;
	Vector2 pos{ 0.0f, 0.0f };
	Rectangle boundingBox{ 0, 0, 1, 1 };
	Vector2 velocity{ 0.0f, 0.0f };
	[[nodiscard]] Rectangle GetPositionedRectangle() const {
		return Rectangle{ pos.x + boundingBox.x, pos.y + boundingBox.y, boundingBox.width, boundingBox.height };
	}
	[[nodiscard]] float GetRealX() const {
		return pos.x + boundingBox.x;
	}
	[[nodiscard]] float GetRealY() const {
		return pos.y + boundingBox.y;
	}
};

struct Collision2D {
	Collision2D(unsigned long long entityId, Rectangle currentBox, Vector2 velocity,
		unsigned long long otherId, Rectangle otherCurrentBox, Vector2 otherVelocity) :
		entityId{ entityId }, currentBox{ currentBox }, velocity{ velocity },
		otherId{ otherId }, otherCurrentBox{ otherCurrentBox }, otherVelocity{ otherVelocity }
	{}
	unsigned long long entityId;
	Rectangle currentBox;
	Vector2 velocity;
	unsigned long long otherId;
	Rectangle otherCurrentBox;
	Vector2 otherVelocity;
};
