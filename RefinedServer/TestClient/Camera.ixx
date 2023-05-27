module;
#include "pch.hpp"

export module Game.Camera;
import System.PipelineObject;

export extern "C++" namespace game
{
	// @brief Class representing a camera orbiting around its target.
	// @ingroup gl_utils
	class Camera : sys::PipelineObject
	{
	public:
		Camera() noexcept
		{}

		virtual ~Camera() noexcept
		{}

		void Awake()
		{}

		void Start()
		{}

		void Update(const float& delta_time)
		{}

		void LateUpdate(const float& delta_time)
		{}

		void SetCameraTarget(const glm::vec3& target);
		void SetCameraDistance(const float& distance);
		void SetCameraRotation(const float& rotation);
		void SetCameraElevation(const float& z);

		const glm::vec3& GetTarget() const;
		float GetDistance() const;
		float GetRotation() const;
		float GetElevation() const;

		glm::vec3 getPosition() const;
		glm::mat4 getViewMatrix() const;
	};
}
