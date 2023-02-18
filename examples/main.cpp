#include <Lyse/Lyse.hpp>

int main()
{
	spl::Window window(1000, 600, "Lyse Example", true);
	spl::Context* context = window.getContext();
	spl::Context::setCurrentContext(context);
	context->setClearColor(0.2f, 0.3f, 0.3f, 1.f);

	lys::CameraPerspective camera(window.getSize().x, window.getSize().y, 1.f, 0.01f, 10.f);
	camera.setTranslation({ 0.f, 0.f, 3.f });
	lys::Mesh<> mesh("examples/suzanne.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	lys::LightPoint light(3.f, 1.f, 1.f, 1.f, 1.f, 1.f, 20.f);
	lys::LightPoint light2(-3.f, 1.f, 1.f, 1.f, 0.8f, 0.7f, 20.f);
	lys::Scene scene(window.getSize().x, window.getSize().y);
	scene.setCamera(&camera);
	scene.addDrawable(&mesh);
	scene.addLight(&light);
	scene.addLight(&light2);

	// lys::CameraPerspective camera(window.getSize().x, window.getSize().y, 1.f, 0.01f, 10.f);
	// camera.setTranslation({ 0.f, 0.f, 3.f });
	// 
	// spl::Texture2D colorMap		("examples/color.png");
	// spl::Texture2D materialMap	("examples/material.png");
	// spl::Texture2D normalMap	("examples/normal.png", spl::TextureInternalFormat::RGB_ni8);
	// spl::Texture2D background	("examples/background.png");
	// 
	// lys::Material material;
	// material.setColorTexture(&colorMap);
	// material.setPropertiesTexture(&materialMap);
	// 
	// lys::Mesh<> mesh("examples/sphere.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	// mesh.setNormalMap(&normalMap);
	// mesh.setMaterial(&material);
	// 
	// lys::LightSun light(-3.f, -1.f, -1.f, 1.f, 1.f, 1.f, 10.f);
	// 
	// lys::Scene scene(window.getSize().x, window.getSize().y);
	// scene.setCamera(&camera);
	// scene.setBackgroundEquirectangular(&background);
	// scene.addDrawable(&mesh);
	// scene.addLight(&light);

	lys::VertexDefaultMesh screenVertices[] = {
		{ {-1.f,  1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 1.f, 0.f, 0.f} },
		{ {-1.f, -1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {0.f, 0.f, 0.f, 0.f} },
		{ { 1.f, -1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 0.f, 0.f, 0.f} },
		{ { 1.f,  1.f, 0.f, 1.f}, {0.f, 0.f, 1.f, 0.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f, 0.f, 0.f} }
	};
	uint32_t screenIndices[] = { 0, 1, 3, 1, 2, 3 };
	lys::Mesh<> screenMesh(screenVertices, 4, spl::BufferStorageFlags::None, screenIndices, 6, spl::BufferStorageFlags::None);

	spl::ShaderProgram screenShader("examples/screen.vert", "examples/screen.frag");
	spl::ShaderProgram::bind(screenShader);


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
		if (window.isKeyPressed(spl::KeyboardKey::A)) camera.move(camera.getLeftVector() * 0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::D)) camera.move(camera.getLeftVector() * -0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::Space)) camera.move(camera.getFrontVector() * 0.01f);
		if (window.isKeyPressed(spl::KeyboardKey::LeftShift)) camera.move(camera.getFrontVector() * -0.01f);

		camera.lookAt({ 0.f, 0.f, 0.f });
		mesh.rotate(0.f, 1.f, 0.f, 0.001f);

		scene.render();

		screenShader.setUniform("scene", 0, scene.getRenderTexture());
		screenMesh.draw();

		window.display();
		spl::Framebuffer::clear();
	}

	return 0;
}
