#include "ChaseMove.h"
#include <raylib-cpp.hpp>
#include <raymath.h>
#include "../../../Graphics/Graphics.h"

void ChaseMove::DoInit()
{
	if (Owner != nullptr)
	{
		BulletOwner = (Bullet*)Owner;

		BulletOwner->SpriteScaled = Vector2Scale(BulletOwner->SpriteSize, Graphics::Get().GetFactorArea().y);
		BulletOwner->Position = Vector2Subtract(InitPosition, Vector2Scale(BulletOwner->SpriteScaled, 0.5f));

		Vector2 Orientation = Vector2Subtract(TargetPosition, BulletOwner->Position);
		Direction = Vector2Normalize(Orientation);

		BulletOwner->FrameRec = {	6 * BulletOwner->SpriteSize.x,
									2 * BulletOwner->SpriteSize.y,
									BulletOwner->SpriteSize.x,
									BulletOwner->SpriteSize.y };

		///----------------------------------------------------------------- ///

		const float CollisionSizeFactor = 0.25f;
		const float ColliderSide = BulletOwner->SpriteScaled.x * CollisionSizeFactor;

		BulletOwner->CollisionRec = {
			BulletOwner->Position.x - ColliderSide * 0.5f,
			BulletOwner->Position.y - ColliderSide * 0.5f,
			ColliderSide, ColliderSide
		};
	}
}

void ChaseMove::DoUpdate()
{
	if (!Tracking)
	{

		BulletOwner->Position.x += Direction.x * Speed * GetFrameTime();
		BulletOwner->Position.y += Direction.y * Speed * GetFrameTime();

		BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
		BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;

		return;
	}

	///--------------------------------------------------------------------
 
	/// Calcular el vector distancia (diferencia entre la posición del
	/// perseguidor y la posición del objetivo a perseguir )
	Vector2 Distance = Vector2Subtract( TargetPosition, BulletOwner->Position);

	/// Calcular la distancia que se movera el perseguidor teniendo en cuenta su velocidad máxima
	float displacement = Speed * GetFrameTime();

	/// Si la distancia que el perseguidor se va a desplazar es mayor o igual
	/// a la distancia que hay a su objetivo, se mueve solo hasta el punto objetivo
	/// ( para no pasarse ). Si no, se mueve la distancia establecida
	/// (para irse acercando poco a poco al objetivo )
	if (displacement >= Vector2Length(Distance))
	{
		BulletOwner->Position = TargetPosition;
	}
	else
	{
		/// La nueva posición del perseguidor será igual a su posición inicial 
		/// más el desplazamiento que debe hacer en la dirección indicada por
		/// vector distancia (para moverse así en dirección a su objetivo)
		Vector2 offset = Vector2Scale(Vector2Normalize(Distance), displacement);
		Vector2 deltaPosition = Vector2Add(BulletOwner->Position, offset);
		BulletOwner->Position = deltaPosition;
	}

	BulletOwner->CollisionRec.x = BulletOwner->Position.x - BulletOwner->CollisionRec.width * 0.5f;
	BulletOwner->CollisionRec.y = BulletOwner->Position.y - BulletOwner->CollisionRec.height * 0.5f;
	
}

void ChaseMove::DoRender()
{
	if (BulletOwner != nullptr)
	{
		//DrawCircle((int)BulletOwner->Position.x, (int)BulletOwner->Position.y, BulletOwner->Radius, RED);

		const float angleVar = std::atan2(-Direction.x, Direction.y);

		DrawTexturePro(GetAsset("Sprites"),
					   BulletOwner->FrameRec,
					   {
						   BulletOwner->Position.x, BulletOwner->Position.y,
						   BulletOwner->SpriteScaled.x, BulletOwner->SpriteScaled.y
					   },
					   Vector2Scale(BulletOwner->SpriteScaled, 0.5f), //BulletOwner->SpriteSize,
					   angleVar * RAD2DEG,
					   WHITE);
	}
}