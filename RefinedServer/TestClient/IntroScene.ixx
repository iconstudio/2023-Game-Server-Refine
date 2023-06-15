export module Client.IntroScene;
import Game.Scene.Specialized;

export class IntroScene : public game::NamedScene<IntroScene>
{
public:
	using basis = game::NamedScene<IntroScene>;

	constexpr IntroScene() noexcept
		: basis(this, "Intro Scene")
	{}

	void Awake() override;

	void Start() noexcept override
	{
		basis::Start();
	}

	void Update(const float dt) override;

	void Destroy() override;

	const float maxPeriod = 6.0f;
	float myLoadTime = 0;
};
