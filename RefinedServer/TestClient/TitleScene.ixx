export module Client.TitleScene;
import Game.Scene.Specialized;

export class TitleScene : public game::NamedScene<TitleScene>
{
public:
	using basis = game::NamedScene<TitleScene>;

	constexpr TitleScene() noexcept
		: basis(this, "Title Scene")
	{}
};
