#pragma once
import <memory>;
import <vector>;

import Game.Scene;
import Game.Camera;

class Framework
{
public:
	Framework() noexcept
		: everyScene()
	{

	}

	~Framework() noexcept
	{

	}

	void Awake()
	{

	}

	void Start()
	{

	}

	void Update()
	{

	}

	void UpdateOnce(const float& delta_time)
	{

	}

	void LateUpdateOnce(const float& delta_time)
	{

	}

	std::vector<std::unique_ptr<game::Scene>> everyScene;
};
