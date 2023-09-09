#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "input.h"
#include "Matrix.h"
#include "ImGuiManager.h"

class Player {
public:
	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void OnCollision();

	void Move();

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;

	XINPUT_STATE joyState_;

	Vector3 acce = {0.0f, -0.8f, 0.0f};
	Vector3 velocity = {0.0f, 0.0f, 0.0f};

	bool playerRotateLeftFlag = false;
	bool playerRotateRightFlag = false;
};
