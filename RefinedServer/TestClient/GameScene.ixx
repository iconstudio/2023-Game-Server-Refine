module;
export module Client.GameScene;
import Game.Scene.Specialized;

export class GameScene : public game::NamedScene<GameScene>
{
public:
	using basis = game::NamedScene<GameScene>;

	constexpr GameScene() noexcept
		: basis(this, "Game Scene")
	{}
};
