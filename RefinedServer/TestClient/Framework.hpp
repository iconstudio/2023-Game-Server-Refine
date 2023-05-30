#pragma once
#include <memory>
#include <chrono>
#include <vector>

import Utility;
import Utility.Constraints;
import Utility.Monad;
import Game.Camera;
import Game.GameObject;
import Game.Scene;
import Game.Scene.Specialized;

class Framework
{
public:
	Framework() noexcept
	{}

	~Framework() noexcept = default;

	template<typename S, typename... Args>
	constexpr void AddScene(Args&& ...args)
		noexcept(util::trivials<S>&& util::nothrow_constructibles<S, Args...>)
	{
		static_assert(util::hierachy<util::clean_t<S>, game::Scene>);

		everyScene.emplace_back(std::make_unique<S>(util::forward<Args>(args)...));
	}

	void Awake();
	void Start();
	void Update();

	void UpdateOnce(game::Scene* const& scene, const float delta_time);
	void LateUpdateOnce(game::Scene* const& scene, const float delta_time);

	constexpr util::Monad<game::Scene*> CurrentScene() const noexcept
	{
		if (roomIndex < everyScene.size())
		{
			return everyScene[roomIndex].get();
		}
		else
		{
			return util::nullopt;
		}
	}

	std::vector<std::unique_ptr<game::Scene>> everyScene{};
	size_t roomIndex = 0;
};
