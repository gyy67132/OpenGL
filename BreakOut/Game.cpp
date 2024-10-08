#include "Game.h"

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "BallObject.h"

SpriteRenderer *spriteRenderer;

Game::Game(GLuint width, GLuint height)
	:State(GAME_ACTIVE),Keys(),Width(width),Height(height)
{

}
Game::~Game()
{

}

void Game::Init()
{
	Shader shader = ResourceManager::LoadShader("spriteVertShader.glsl", "spriteFragShader.glsl", "", "sprite");
	spriteRenderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
	shader.use();
	shader.setInt("image", 0);
	shader.setMat4("projection", projection);

	ResourceManager::LoadTexture("textures/background.jpg", false, "face");
	ResourceManager::LoadTexture("textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("textures/block.png", false, "block");
	ResourceManager::LoadTexture("textures/paddle.png", false, "paddle");
	ResourceManager::LoadTexture("textures/awesomeface.png", false, "ball");

	GameLevel gameLevel;
	gameLevel.Load("levels/one.lvl", 800, 300);
	Levels.push_back(gameLevel);
	GameLevel gameLevel2;
	gameLevel2.Load("levels/two.lvl", 800, 300);
	Levels.push_back(gameLevel2);
	GameLevel gameLevel3;
	gameLevel3.Load("levels/three.lvl", 800, 300);
	Levels.push_back(gameLevel3);
	GameLevel gameLevel4;
	gameLevel4.Load("levels/four.lvl", 800, 300);
	Levels.push_back(gameLevel4);

	Level = 0;
	glm::vec2  paddleSize = glm::vec2(100, 20);
	glm::vec2 paddlePos = glm::vec2((800 - 100) / 2, 0);
	paddle = GameObject(paddlePos, paddleSize, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPos = paddlePos + glm::vec2(paddleSize.x / 2, paddleSize.y);
	ballObject = BallObject(ballPos, 7.0, glm::vec2(350.0f, -350.0f), ResourceManager::GetTexture("ball"));

	Shader shader2 = ResourceManager::LoadShader("particleVertShader.glsl", "particleFragShader.glsl", "", "particle");
	shader2.use();
	shader2.setInt("sprite", 0);
	shader2.setMat4("projection", projection);
	Texture2D particleTexture = ResourceManager::LoadTexture("textures/particle.png", GL_TRUE, "particle");
	particleGenerator = new ParticleGenerator(shader2, particleTexture, 500);

	Shader postProcessorShader = ResourceManager::LoadShader("postprocessingVertShader.glsl", "postprocessingFragShader.glsl", "", "postprocessing");
	postProcessor = new PostProcessor(postProcessorShader, WindowsWidth, WindowsHeight);

	/*ResourceManager::LoadTexture("textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("textures/powerup_passthrough.png", true, "powerup_passthrough");
	ResourceManager::LoadTexture("textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("textures/powerup_confuse.png", true, "powerup_confuse");*/
}

void Game::ProcessInput(GLfloat dt)
{
	if (State == GAME_ACTIVE)
	{
		if (Keys[GLFW_KEY_A]) {
			paddle.Position.x -= 500 * dt;
			if (paddle.Position.x <= 0)
				paddle.Position.x = 0;
		}
		else if (Keys[GLFW_KEY_D]) {
			paddle.Position.x += 500 * dt;
			if (paddle.Position.x >= Width - 100)
				paddle.Position.x = Width - 100;
		}
		else if (Keys[GLFW_KEY_SPACE]) {
			ballObject.Stuck = false;
		}
	}
}

void Game::Update(GLfloat dt)
{
	if(ballObject.Stuck)
		ballObject.Position = paddle.Position + glm::vec2(paddle.Size.x / 2, paddle.Size.y);
	else
		ballObject.Move(dt, WindowsWidth, WindowsHeight);

	if (ballObject.Position.y <= 0)
	{
		for (GameObject& obj : Levels[Level].Bricks)
		{
			obj.Destroyed = false;
		}

		paddle.Position = glm::vec2((800 - 100) / 2, 0);
		ballObject.Stuck = true;
	}

	CheckCollision();

	particleGenerator->Update(dt, ballObject, 2, glm::vec2(ballObject.Radius / 2));

	if (ShakeTime > 0.0)
	{
		ShakeTime -= dt;
		if(ShakeTime < 0.0)
			postProcessor->Shake = GL_FALSE;
	}
}

void Game::Render()
{
	postProcessor->BeginRender();

	spriteRenderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(0.0, 0.0), glm::vec2(800, 600), 0.0, glm::vec3(1.0, 1.0, 1.0));

	Levels[Level].Draw(*spriteRenderer);

	paddle.Draw(*spriteRenderer);

	ballObject.Draw(*spriteRenderer);
	
	/*for (PowerUp& powerUp : PowerUps)
		if (!powerUp.Destroyed)
			powerUp.Draw(*spriteRenderer);*/

	particleGenerator->Draw();

	postProcessor->EndRender();
	postProcessor->Render(glfwGetTime());

}

void Game::CheckCollision()
{
	for (GameObject& obj : Levels[Level].Bricks)
	{
		if (!obj.Destroyed) {
			/*bool xCollision = false;
			if ((ballObject.Position.x + ballObject.Size.x) > obj.Position.x
				&& (obj.Position.x + obj.Size.x) > ballObject.Position.x)
				xCollision = true;
			bool yCollision = false;
			if ((ballObject.Position.y + ballObject.Size.y) > (600 - obj.Position.y - obj.Size.y)
				&& (600 - obj.Position.y ) > ballObject.Position.y)
				yCollision = true;
			obj.Destroyed = xCollision && yCollision;*/

			DoCollisions(obj);
		}
	}

	PaddleCollision();

	/*for (PowerUp& powerUp : PowerUps) {
		if (!powerUp.Destroyed)
		{
			if (powerUp.Position.y >= this->Height)
				powerUp.Destroyed = GL_TRUE;
			if (DoCollisions(paddle, powerUp))
			{
				ActivatePowerUp(powerUp);
				powerUp.Destroyed = GL_TRUE;
				powerUp.Activated = GL_TRUE;
			}
		}
	}*/
}

GLboolean Game::DoCollisions(GameObject& one, GameObject& obj)
{
	bool xCollision = false;
	if ((ballObject.Position.x + ballObject.Size.x) > obj.Position.x
		&& (obj.Position.x + obj.Size.x) > ballObject.Position.x)
		xCollision = true;
	bool yCollision = false;
	if ((ballObject.Position.y + ballObject.Size.y) > (600 - obj.Position.y - obj.Size.y)
		&& (600 - obj.Position.y ) > ballObject.Position.y)
		yCollision = true;
	obj.Destroyed = xCollision && yCollision;
	return obj.Destroyed;
}

GLboolean Game::DoCollisions(GameObject& obj)
{
	glm::vec2 centerA = ballObject.Position + ballObject.Size / 2.0f;
	glm::vec2 objPos = glm::vec2(obj.Position.x, 600 - obj.Position.y - obj.Size.y);
	glm::vec2 centerB = objPos + obj.Size / 2.0f;

	glm::vec2 BA = centerA - centerB;
	glm::vec2 BP = glm::clamp(BA, objPos - centerB, centerB - objPos);

	glm::vec2 P = centerB + BP;
	glm::vec2 PA = centerA - P;

	bool Collision = glm::length(PA) <= ballObject.Radius;
	if (!obj.IsSolid)
		obj.Destroyed = Collision;

	if (Collision) {
		ShakeTime = 0.05;
		postProcessor->Shake = GL_TRUE;
		Direction dir = VectorDirection(PA);
		if (dir == Direction::UP || dir == Direction::DOWN) {
			ballObject.Velocity.y = -ballObject.Velocity.y;
			if (dir == Direction::UP)
			{
				ballObject.Position.y += 7.0;
			}
			else {
				ballObject.Position.y -= 7.0;
			}
		}
		else if (dir == Direction::RIGHT || dir == Direction::LEFT) {
			ballObject.Velocity.x = -ballObject.Velocity.x;
			if (dir == Direction::RIGHT)
			{
				ballObject.Position.x += 7.0;
			}
			else {
				ballObject.Position.x -= 7.0;
			}
		}
	}
	return Collision;
}

Direction Game::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0,1.0),
		glm::vec2(1.0,0.0),
		glm::vec2(0.0,-1.0),
		glm::vec2(-1.0,0.0)
	};

	GLfloat max = 0.0f;
	GLuint match = 0;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(target, compass[i]);
		if (dot_product > max)
		{
			match = i;
			max = dot_product;
		}
	}
	
	return (Direction)match;
}

void Game::PaddleCollision()
{
	glm::vec2 centerA = ballObject.Position + ballObject.Size / 2.0f;
	glm::vec2 objPos = paddle.Position;
	glm::vec2 centerB = objPos + paddle.Size / 2.0f;

	glm::vec2 BA = centerA - centerB;
	glm::vec2 BP = glm::clamp(BA, objPos - centerB, centerB - objPos);

	glm::vec2 P = centerB + BP;
	glm::vec2 PA = centerA - P;

	bool Collision = glm::length(PA) <= ballObject.Radius;

	if (Collision) {
		Direction dir = VectorDirection(PA);
		if (dir == Direction::UP || dir == Direction::DOWN) {
			ballObject.Velocity.y = -ballObject.Velocity.y;
			if (dir == Direction::UP)
			{
				ballObject.Position.y += 7.0;
			}
			else {
				ballObject.Position.y -= 7.0;
			}
		}
		else if (dir == Direction::RIGHT || dir == Direction::LEFT) {
			ballObject.Velocity.x = -ballObject.Velocity.x;
			if (dir == Direction::RIGHT)
			{
				ballObject.Position.x += 7.0;
			}
			else {
				ballObject.Position.x -= 7.0;
			}
		}
	}
}

GLboolean ShouldSpawn(GLuint chance)
{
	GLuint random = rand() % chance;
	return random == 0;
}
void Game::SpawnPowerUps(GameObject& block)
{
	ResourceManager::LoadTexture("textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("textures/powerup_passthrough.png", true, "powerup_passthrough");
	ResourceManager::LoadTexture("textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("textures/powerup_confuse.png", true, "powerup_confuse");
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("speed", glm::vec3(0.5,0.5,1.0),0.0,block.Position, ResourceManager::GetTexture("powerup_speed")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0, 0.5, 1.0), 20.0, block.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5, 1.0, 0.5), 10.0, block.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (ShouldSpawn(75))
		PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0, 0.6, 0.4), 0.0, block.Position, ResourceManager::GetTexture("powerup_increase")));
	if (ShouldSpawn(15))
		PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0, 0.3, 0.3), 15.0, block.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (ShouldSpawn(15))
		PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9, 0.25, 0.25), 15.0, block.Position, ResourceManager::GetTexture("powerup_chaos")));
}

GLboolean IsOtherPowerUpActive(std::vector<PowerUp> &powerUps, std::string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.Activated)
			if (powerUp.Type == type)
				return GL_TRUE;
	}
	return GL_FALSE;
}

void Game::UpdatePowerUps(GLfloat dt)
{
	for (PowerUp& powerUp : PowerUps)
	{
		powerUp.Position += powerUp.Velocity * dt;
		if (powerUp.Activated)
		{
			powerUp.Duration -= dt;
			if (powerUp.Duration <= 0.0)
			{
				powerUp.Activated = GL_FALSE;
				if (powerUp.Type == "sticky")
				{
					if (!IsOtherPowerUpActive(PowerUps, "sticky")) {
						paddle.Color = glm::vec3(1.0);
					}
				}else if (powerUp.Type == "pass-through")
				{
					if (!IsOtherPowerUpActive(PowerUps, "pass-through")) {
						ballObject.Color = glm::vec3(1.0);
					}
				}
				else if (powerUp.Type == "confuse")
				{
					if (!IsOtherPowerUpActive(PowerUps, "confuse")) {
						postProcessor->Confuse = GL_FALSE;
					}
				}
				else if (powerUp.Type == "chaos")
				{
					if (!IsOtherPowerUpActive(PowerUps, "chaos")) {
						postProcessor->ChaoS = GL_FALSE;
					}
				}
			}
		}
	}

	PowerUps.erase(std::remove_if(PowerUps.begin(), PowerUps.end(), [](const PowerUp& powerUp) {
		return powerUp.Destroyed && !powerUp.Activated; }), PowerUps.end());
}

void Game::ActivatePowerUp(PowerUp& powerUp)
{
	if (powerUp.Type == "speed")
	{
		ballObject.Velocity *= 1.2;
	}else if (powerUp.Type == "sticky")
	{
		ballObject.Sticky = GL_TRUE;
		paddle.Color = glm::vec3(1.0, 0.5, 1.0);
	}
	else if (powerUp.Type == "pass-through")
	{
		ballObject.PassThrough = GL_TRUE;
		ballObject.Color = glm::vec3(1.0, 0.5, 0.5);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		paddle.Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!postProcessor->ChaoS)
			postProcessor->Confuse = GL_TRUE;
	}
	else if (powerUp.Type == "chaos")
	{
		if (!postProcessor->Confuse)
			postProcessor->ChaoS = GL_TRUE;
	}
}