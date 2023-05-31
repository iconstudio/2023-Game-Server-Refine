module;
#include <glm/glm.hpp>

export module Game.Camera;
import <memory>;
import System.PipelineObject;
import Game.Component;
import Game.GameObject;

export extern "C" namespace game
{
	// @brief Class representing a camera orbiting around its target.
	// @ingroup game
	class Camera final
		: public Component
	{
	public:
		constexpr Camera() noexcept
			: Component()
		{}

		~Camera() noexcept = default;

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
