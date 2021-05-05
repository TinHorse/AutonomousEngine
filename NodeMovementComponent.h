#pragma once
#include "ECS.h"
#include "Components.h"
#include "Node.h"

enum PathStatus
{
	Idle,
	Moving,
	WaitingForReturn,
	Returning,
	ArrivedHome
};

class NodeMovementComponent : public Component
{
public:
	NodeMovementComponent() = default;
	NodeMovementComponent(TurretType type) : turretType(type) {}

	void Update() override
	{
		Accelerate();
		Move();
	}

	void Init() override
	{
		LinkComponentPointers();
	}

	void Accelerate()
	{
		if (speed < max_speed)
		{
			speed += acceleration;
		}
		else if (speed > max_speed)
		{
			speed -= deceleration;
		}
	}

	void Move()
	{
		if (Path.size() > 0)
		{
			if (Math::distance(cTransform->position, Path.top()->GetPosition()) > 5.f)
			{
				Vector2D Velocity = Path.top()->GetPosition() - cTransform->position;
				Velocity.Normalize();
				cTransform->SetViewTarget(Path.top()->GetPosition());

				Velocity * speed;
				cTransform->position += Velocity;
				
				return;
			}
			else if (TargetNode && Path.top() == TargetNode)
			{
				// if this is a transporter, set waiting for return
				if (turretType == tTransporter)
				{
					if (pathStatus == ArrivedHome)
					{
						Path.pop();
						speed = 0.f;
						return;
					}
					if (pathStatus == Returning)
					{
						pathStatus = ArrivedHome; 
						Path.pop();
						return;
					}
					pathStatus = WaitingForReturn; 
					speed = 0.f;
					Path.pop();
					return;
				}
				pathStatus = Idle; 
				speed = 0.f;
				Path.pop();
				return;
			}
			else if (Path.top()->bIsAccelerator)
			{
				speed *= 1.5f;
				speed = speed > 2.5f ? 2.5f : speed; // limit speed to 2.5
			}
			Path.pop();
		}
	}

	void SetPath(std::stack<Node*> p, Node* Target)
	{
		Path = p;
		TargetNode = Target;
	}

	void LinkComponentPointers() override
	{
		cTransform = &entity->GetComponent<TransformComponent>();
	}

	int GetPathStatus()
	{
		return pathStatus;
	}

	Node* GetTargetNode()
	{
		return TargetNode;
	}

	void setPathStatus(PathStatus status)
	{
		pathStatus = status;
	}

	bool Idle = true;

private:
	TransformComponent* cTransform = nullptr;
	float max_speed = 1.f;
	float acceleration = 0.05f;	
	float speed = 0.f;
	float deceleration = 0.0025f;
	

	std::stack<Node*> Path;
	Node* TargetNode = nullptr;
	int pathStatus = Moving;
	enum TurretType turretType = TurretType::tTransporter;
};