#pragma once
#include "Connection.h"

class Node
{
public:
	Node() = default;

	Node(float x, float y, Entity* entity)
	{
		posX = x;
		posY = y;
		_entity = entity;
	}

	~Node() {}

	void AddConnectedNode(Node* NewNode)
	{
		if (!IsConnectedTo(NewNode))
		{
			connected_nodes.push_back(NewNode);
		}
	}

	void Update(Vector2D cameraOffset)
	{
		posX -= cameraOffset.x;
		posY -= cameraOffset.y;
	}

	std::vector<Node*> GetConnectedNodes()
	{
		return connected_nodes;
	}

	const bool IsConnectedTo(Node* node) const
	{
		for (Node* n : connected_nodes)
		{
			if (n == node) return true;
		}
		return false;
	}

	Vector2D GetPosition() const
	{
		return Vector2D(posX, posY);
	}

	bool IsNotConnected()
	{
		return (connected_nodes.size() == 0);
	}

	Entity* GetEntity()
	{
		return _entity;
	}

	void SetEntity(Entity* entity)
	{
		_entity = entity;
	}	

	void AddConnection(Connection* c)
	{
		connections.push_back(c);
	}

	std::vector<Connection*>& GetConnections()
	{
		return connections;
	}


	float posX, posY;
	long int globalDist = LONG_MAX;
	int ID = 0;

	bool bIsGlobalResourceStorage = false;
	bool bIsAccelerator = false;

private:
	Entity* _entity;
	std::vector<Node*> connected_nodes;
	std::vector<Connection*> connections;
};

struct NodeCompare
{
	bool operator()(const Node* nA, const Node* nB) const
	{
		return nA->globalDist > nB->globalDist;
	}
};