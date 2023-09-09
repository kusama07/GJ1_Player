#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model) { 
	assert(model);

	model_ = model;

	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

void Player::Update() { 
	Move();

}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}

void Player::OnCollision() {}

void Player::Move() {
	worldTransform_.TransferMatrix();

#pragma region キーボード操作

	Vector3 Keyboardmove = {0, 0, 0};
	const float kPlayerSpeed = 0.3f;

	if (input_->PushKey(DIK_LEFT))
	{
		Keyboardmove.x -= kPlayerSpeed;
	} else if (input_->PushKey(DIK_RIGHT))
	{
		Keyboardmove.x += kPlayerSpeed;
	}

	#pragma region ジャンプ

	acce.y = -0.08f;

	if ((input_->TriggerKey(DIK_SPACE) || joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
	    worldTransform_.translation_.y == worldTransform_.scale_.y) {
		velocity.y = 1.0f;
	}

	velocity.y += acce.y;
	worldTransform_.translation_.y += velocity.y;

	if (worldTransform_.translation_.y <= worldTransform_.scale_.y) {
		worldTransform_.translation_.y = worldTransform_.scale_.y;
	}

	#pragma endregion

	#pragma region 回転

	ImGui::Begin("Camera");
	ImGui::DragFloat3("rotation", &worldTransform_.rotation_.x, 1.0f);
	ImGui::Text("Flag = %d", playerRotateLeftFlag);
	ImGui::End();

	if (input_->TriggerKey(DIK_LEFT)) {
		playerRotateLeftFlag = true;
	}
	
	if (input_->TriggerKey(DIK_RIGHT)) {
		playerRotateRightFlag = true;
	}

	//左
	if (playerRotateLeftFlag == 1 ) {
		worldTransform_.rotation_.y = 3.0f;

		playerRotateLeftFlag = false;

	}

	//右
	if (playerRotateRightFlag == 1 ) {
		worldTransform_.rotation_.y = 6.0f;

		playerRotateRightFlag = false;

	} 
	
	#pragma endregion

#pragma endregion

#pragma region コントローラ
	if (Input::GetInstance()->GetJoystickState(0, joyState_)) {

		Vector3 move = {
		    (float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		    (float)joyState_.Gamepad.sThumbLY / SHRT_MAX};

		move = Multiply(kPlayerSpeed, Normalize(move));

		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
	}

#pragma endregion


	worldTransform_.translation_ = Add(worldTransform_.translation_, Keyboardmove);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.UpdateMatrix();

}
