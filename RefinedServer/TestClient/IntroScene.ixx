export module Client.IntroScene;
import Game.Scene.Specialized;

export class IntroScene : public game::NamedScene<IntroScene>
{
public:
	using basis = game::NamedScene<IntroScene>;

	constexpr IntroScene() noexcept
		: basis(this, "Intro Scene")
	{}

	void Awake() override
	{
		basis::Awake();
		myLoadTime = 0;
	}

	void Start() noexcept override
	{
		basis::Start();
	}

	void Update(const float dt) override
	{
		basis::Update(dt);

		myLoadTime += dt;

		if (maxPeriod <= myLoadTime)
		{
			MakeCompletion();
		}
	}

	void Destroy() override
	{
		basis::Destroy();
		myLoadTime = 0;
	}

	const float maxPeriod = 6.0f;
	float myLoadTime = 0;
};
