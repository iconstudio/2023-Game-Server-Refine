export module Game.GameObject;
import Game.Object;

export namespace game
{
	class GameObject
		: public Object
		, public util::Classifier<GameObject>
	{

	};
}

export template<>
struct util::class_id<game::GameObject>
{
	static constexpr size_t id = 2;
};
