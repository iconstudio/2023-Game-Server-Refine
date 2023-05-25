#pragma once
import Net;
import Net.Socket;
import Net.EndPoint;
import Net.Promise;

// @brief Class representing a camera orbiting around its target.
// @ingroup gl_utils
class Camera
{
public:
	Camera() noexcept
	{}

	~Camera() noexcept
	{}

	void Awake();
	void Start();
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);

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

class System
{
public:
	System() noexcept;
	~System() noexcept;

	void Awake();
	void Start();
	void Update(const float& delta_time);
	void LateUpdate(const float& delta_time);

	net::Socket mySocket;
	net::EndPoint myAddress;
	net::EndPoint serverAddress;

	Camera myCamera;
};
