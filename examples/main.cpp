#include <Lyse/Lyse.hpp>

int main()
{
	lys::Vertex<int, float, scp::f32vec4, bool> x;

	spl::Window window(1000, 600, "SPL Example", true);
	spl::Context* context = window.getContext();
	spl::ContextManager::setCurrentContext(context);
	context->setIsDepthTestEnabled(true);
	context->setClearColor(0.2f, 0.3f, 0.3f, 1.f);

	lys::CameraPerspective camera(1000, 600, 0.1f, 100.f, 1.f);
	camera.setTranslation({ 0.f, 0.f, 2.f });

	lys::Mesh<> mesh("P:/SplayLibrary/examples/basicPhong/resources/meshes/suzanne.obj", spl::BufferStorageFlags::None, spl::BufferStorageFlags::None);
	mesh.scale(0.5);

	lys::Scene scene(1000, 600);
	scene.addDrawable(&mesh);
	scene.setCamera(&camera);


	lys::DefaultVertex screenVertices[] = {
		{ {-1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 1.f} },
		{ {-1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f} },
		{ { 1.f, -1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 0.f} },
		{ { 1.f,  1.f, 0.f}, {0.f, 0.f, 1.f}, {1.f, 1.f} }
	};
	uint32_t screenIndices[] = { 0, 1, 3, 1, 2, 3 };
	lys::Mesh<> screenMesh(screenVertices, 4, spl::BufferUsage::StaticDraw, screenIndices, 6, spl::BufferUsage::StaticDraw);

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

		screenShader.setUniform("scene", 0, scene.getTexture());
		screenMesh.draw();

		window.display();
		spl::Framebuffer::clear();
	}

	return 0;
}
