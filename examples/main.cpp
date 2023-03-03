#include <Lyse/Lyse.hpp>

int main()
{
	constexpr scp::f32vec3 lightColor = { 1.f, 0.8f, 0.7f };
	const scp::f32vec3 lightDir = scp::normalize(scp::f32vec3(-3.f, -1.f, -1.f));

	spl::Window window(1000, 600, "Lyse Example", true);
	spl::Context* context = window.getContext();
	spl::Context::setCurrentContext(context);
	context->setClearColor(0.2f, 0.3f, 0.3f, 1.f);

	// lys::CameraPerspective camera(window.getSize().x, window.getSize().y, 1.f, 0.01f, 10.f);
	// camera.setTranslation({ 0.f, 0.f, 3.f });
	// lys::Mesh<> mesh("examples/assets/meshes/suzanne.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	// // lys::LightSun light(lightDir.x, lightDir.y, lightDir.z, lightColor.x, lightColor.y, lightColor.z, 10.f);
	// lys::LightPoint light(1.f, 2.f, 1.f, lightColor.x, lightColor.y, lightColor.z, 20.f);
	// lys::Scene scene(window.getSize().x, window.getSize().y);
	// scene.setCamera(&camera);
	// scene.addDrawable(&mesh);
	// scene.addLight(&light);

	lys::CameraPerspective camera(window.getSize().x, window.getSize().y, 1.f, 0.01f, 10.f);
	camera.setTranslation({ 0.f, 0.f, 3.f });
	
	spl::Texture2D colorMap				("examples/assets/images/color.png");
	spl::Texture2D moonColorMap			("examples/assets/images/moonColor.png");
	spl::Texture2D normalMap			("examples/assets/images/normal.png", spl::TextureInternalFormat::RGB_ni8);
	spl::Texture2D moonNormalMap		("examples/assets/images/moonNormal.png", spl::TextureInternalFormat::RGB_ni8);
	spl::Texture2D materialMap			("examples/assets/images/material.png");
	spl::Texture2D background			("examples/assets/images/background.png");
	
	lys::Material material(&colorMap, &materialMap);
	lys::Mesh<> mesh("examples/assets/meshes/sphere.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	mesh.setNormalMap(&normalMap);
	mesh.setMaterial(&material);
	
	lys::Material moonMaterial(&moonColorMap, 0.1f, 0.0f, 1.f);
	lys::Mesh<> moonMesh("examples/assets/meshes/sphere.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	moonMesh.setNormalMap(&moonNormalMap);
	moonMesh.setMaterial(&moonMaterial);
	moonMesh.setScale(0.1);
	moonMesh.rotate(0.f, 1.f, 0.f, std::numbers::pi);
	moonMesh.move(2.f, 1.f, 1.f);
	
	lys::LightSun light(lightDir.x, lightDir.y, lightDir.z, lightColor.x, lightColor.y, lightColor.z, 10.f);
	light.setCastShadows(true);
	
	lys::Scene scene(window.getSize().x, window.getSize().y);
	scene.setCamera(&camera);
	scene.setBackgroundEquirectangular(&background);
	scene.addDrawable(&mesh);
	scene.addDrawable(&moonMesh);
	scene.addLight(&light);
	
	
	static constexpr float screenVboData[] = { -1.f,  -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
	spl::Buffer screenVbo(sizeof(screenVboData), spl::BufferStorageFlags::None, screenVboData);
	spl::VertexArray screenVao;
	screenVao.setAttributeFormat(0, spl::GlslType::FloatVec2, 0);
	screenVao.setAttributeEnabled(0, true);
	screenVao.setAttributeBinding(0, 0);
	screenVao.setBindingDivisor(0, 0);
	screenVao.bindArrayBuffer(&screenVbo, 0, sizeof(float) * 2);
	
	spl::ShaderProgram screenShader("examples/assets/shaders/screen.vert", "examples/assets/shaders/screen.frag");
	spl::ShaderProgram::bind(&screenShader);

	while (!window.shouldClose())
	{
		spl::Event* rawEvent = nullptr;
		while (window.pollEvent(rawEvent))
		{
			switch (rawEvent->type)
			{
				case spl::EventType::ResizeEvent:
				{
					spl::ResizeEvent event = rawEvent->specialize<spl::EventType::ResizeEvent>();
					context->setViewport(0, 0, event.size.x, event.size.y);
					camera.setAspect(event.size.x, event.size.y);
					scene.resize(event.size.x, event.size.y);
					break;
				}
			}
		}

		spl::DebugMessage* message = nullptr;
		while (context->pollDebugMessage(message))
		{
			std::cout << "#\n" << message->descr << std::endl;
		}

		if (window.isKeyPressed(spl::KeyboardKey::W)) camera.move(camera.getUpVector() * 0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::S)) camera.move(camera.getUpVector() * -0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::A)) camera.move(camera.getRightVector() * -0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::D)) camera.move(camera.getRightVector() * 0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::Space)) camera.move(camera.getFrontVector() * 0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::LeftShift)) camera.move(camera.getFrontVector() * -0.01f);

		camera.lookAt({ 0.f, 0.f, 0.f });
		mesh.rotate(0.f, 1.f, 0.f, 0.001f);

		scene.render();

		screenShader.setUniform("u_scene", 0, scene.getRenderTexture());
		screenVao.drawArrays(spl::PrimitiveType::TriangleStrips, 0, 4);

		window.display();
		spl::Framebuffer::clear();
	}

	return 0;
}
