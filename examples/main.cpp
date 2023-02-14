#include <Lyse/Lyse.hpp>

int main()
{
	spl::Window window(1000, 600, "Lyse Example", true);
	spl::Context* context = window.getContext();
	spl::Context::setCurrentContext(context);
	context->setClearColor(0.2f, 0.3f, 0.3f, 1.f);

	lys::CameraPerspective camera(window.getSize().x, window.getSize().y, 0.01f, 10.f, 1.f);
	camera.setTranslation({ 0.f, 0.f, 1.5f });

	lys::Mesh<> mesh("examples/suzanne.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	mesh.scale(0.5);

	lys::Scene scene(window.getSize().x, window.getSize().y);
	scene.addDrawable(&mesh);
	scene.setCamera(&camera);

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

		scene.render();

		screenShader.setUniform("scene", 0, scene.getRenderTexture());
		screenMesh.draw();

		window.display();
		spl::Framebuffer::clear();
	}

	return 0;
}
