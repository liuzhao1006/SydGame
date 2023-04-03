//
// Created by aosp on 2023/2/25.
//

#include "scene.h"

void Scene::OnStartGraphics() {}

void Scene::OnKillGraphics() {}

void Scene::OnInstall() {}

void Scene::DoFrame() {}

void Scene::OnUninstall() {}

void Scene::OnPointerDown(int pointerId, const struct PointerCoords *coords) {}

void Scene::OnPointerUp(int pointerId, const struct PointerCoords *coords) {}

void Scene::OnPointerMove(int pointerId, const struct PointerCoords *coords) {}

bool Scene::OnBackKeyPressed() { return false; }

void Scene::OnKeyDown(int ourKeyCode) {}

void Scene::OnKeyUp(int ourKeyCode) {}

void Scene::OnJoy(float joyX, float joyY) {}

void Scene::OnScreenResized(int width, int height) {}

void Scene::OnPause() {}

void Scene::OnResume() {}

Scene::~Scene() = default;
