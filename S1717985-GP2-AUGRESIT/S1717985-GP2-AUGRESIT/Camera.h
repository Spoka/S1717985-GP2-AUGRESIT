#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct Camera
{
public:
	Camera()
	{
	}

	void initialiseCamera(const glm::vec3& position, float fov, float aspect, float nearClip, float farClip)
	{
		this->position = position;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	inline glm::mat4 GetViewProjection() const
	{
		return projection * glm::lookAt(position, position + forward, up);
	}

	inline glm::mat4 GetView() const
	{
		return glm::lookAt(position, position + forward, up);
	}

	inline glm::mat4 GetProjection() const
	{
		return projection;
	}

	void rotateCamera(float x, float y, float z)
	{
		
	}

	void MoveForward(float amt)
	{
		position += forward * amt;
	}

	void MoveBackward(float amt)
	{
		position -= forward * amt;
	}

	void MoveRight(float amt)
	{
		position += glm::cross(up, forward) * amt;
	}

	void MoveLeft(float amt)
	{
		position -= glm::cross(up, forward) * amt;
	}

protected:
private:
	glm::mat4 projection;
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 up;
};