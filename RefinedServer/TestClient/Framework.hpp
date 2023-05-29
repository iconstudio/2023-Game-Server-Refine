#pragma once
import <memory>;
import <vector>;

import Game.Scene.Management;
import Game.Scene;
import Game.Camera;

class Framework
{
public:
	Framework() noexcept;
	~Framework() noexcept;

	void Awake();
	void Start();
	void Update();
	void UpdateOnce(const float& delta_time);
};
