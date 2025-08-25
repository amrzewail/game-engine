#include <Moths/Moths.h>
#include <iostream>

using namespace MothsEngine;

void main()
{
	Screen::SetResolution(1280, 720);

	Moths::Initialize();

	Camera& camera = *new Camera();
	camera.transform->position = Vector(-0.9 * 1.2, 1.5 * 1.2, 1.5 * 1.2);
	camera.transform->set_EulerAngles(Vector(20, 150, 0));

	DirectionalLight* directionalLight = new DirectionalLight();
	directionalLight->intensity = 0.5;
	directionalLight->direction = Vector(0, -1, -1);
	directionalLight->color = Color(1, 1, 1, 1);

	GameObjectAsset& xbotAsset = Assets::Load<GameObjectAsset>("models/xbot_Tpose.fbx");
	GameObject& xbotGameObject = xbotAsset.GetGameObject();

	GameObjectAsset& floorAsset = Assets::Load<GameObjectAsset>("models/cube.obj");
	GameObject& floorGO = floorAsset.GetGameObject();
	floorGO.transform->localScale = Vector(50, 0.01, 50);

	Shader& triplanar = *Assets::LoadInternal<ShaderAsset>("data/shaders/triplanar.shader").GetShader();
	floorGO.GetComponent<MeshRenderer>()->SetMaterial(new Material("triplanar", triplanar, Assets::Load<TextureAsset>("textures/grid.png").GetTexture()));
	//
	Moths::Run();
}