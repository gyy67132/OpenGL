#include "PostProcessor.h"


PostProcessor::PostProcessor(Shader& shader, GLuint width, GLuint height)
	:PostProcessingShader(shader), Width(width), Height(height)
	, Confuse(false), ChaoS(false), Shake(false)
{
	glGenFramebuffers(1, &MSFBO);
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	Texture.Generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,Texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initRenderData();
	shader.use();
	shader.setInt("scene", 0);

	GLfloat offset = 1.0f / 300.0f;
	GLfloat offsets[9][2] = {
		{-offset, offset},
		{0.0, offset},
		{offset, offset},
		{-offset, 0.0},
		{0.0, 0.0},
		{offset, 0.0},
		{-offset, -offset},
		{0.0, -offset},
		{offset, -offset},
	};

	glUniform2fv(glGetUniformLocation(shader.ID, "offsets"), 9, (GLfloat*)offsets);

	GLint edge_kernel[9] = {
		-1,-1,-1,
		-1,8,-1,
		-1,-1,-1
	};
	glUniform1iv(glGetUniformLocation(shader.ID, "edge_kernel"), 9, edge_kernel);

	GLfloat blur_kernel[9] = {
		1.0/16, 2.0/16,1.0/16,
		2.0 / 16, 4.0 / 16, 2.0/16,
		1.0 / 16, 2.0 / 16,1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(shader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(GLfloat time)
{
	PostProcessingShader.use();
	PostProcessingShader.setFloat("time", time);
	PostProcessingShader.setInt("confuse", Confuse);
	PostProcessingShader.setInt("chaos", ChaoS);
	PostProcessingShader.setInt("shake", Shake);

	glActiveTexture(GL_TEXTURE0);
	Texture.Bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		-1.0, -1.0,  0.0, 0.0,
		1.0, 1.0, 1.0,1.0,
		-1.0,1.0, 0.0,1.0,

		-1.0, -1.0,  0.0, 0.0,
		1.0,-1.0, 1.0,0.0,
		1.0,1.0, 1.0,1.0
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, false, 4*sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}