#include "EnemyManager.h"
#include "../../Utility/Utils.h"
#include "../Graphics/Graphics.h"
#include "raylib-cpp.hpp"
#include <functional>



#define ROAMER  (Vector2(0,1))
#define BUNGIE  (Vector2One())
#define RAZOR   (Vector2(2,1))
#define VOLBO   (Vector2(3,1))
#define SEEKER  (Vector2(4,1))


#define PADDING 50
#define POS_CENTER ((Graphics::Get().GetScreenCenter())
