/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 05/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>


using namespace std;

const float Pi = 3.1416f;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();
int generateCircle(float radius, int nPoints);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;



// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pink Floyd - The Dark Side of the Moon", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação

	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//glViewport(0, 0, width, height);
	// define tamanho da tela de desenho (e nao da tela toda) (posiçao inicial de largura, posiçai inicial de altura, qtos pixels de largura, qtos pixels de altura)


	// Compilando e buildando o programa de shader que estão em arquivos

	Shader shader("../shaders/vs_ortho.vs", "../shaders/fs_ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();

	// Buffer da funçao de circulo
	GLuint nPoints = 10;
	//GLuint VAO = generateCircle(0.5, nPoints);



	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
	// que não está nos buffers

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	// Criando a matriz de projeção usando glm
	glm::mat4 projection = glm::mat4(1);       // matriz identidade      Sempre recomendado inicializar uma matriz como sendo identidade
	projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width / 2, height / 2);

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(5);
		glPointSize(5);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		//glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);   // esse é o comando propriamente dito de "drawcall" // 0 mostra a partir de onde vai ser o deslocamento do array e 3 eh o numero de vertices

		//glUseProgram(shader.ID);  // ???

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		// PONTOS - GL_POINTS
		glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		//glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints + 2);
		glDrawArrays(GL_LINE_LOOP, 0, 3);  // COMO DESENHAR A LINHA COM CORES DIFERENTES DO colors[]? USAR colorLoc OU NÃO?
		glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfwGetFramebufferSize(window, &width, &height);
		glViewport(width / 2, 0, width / 2, height / 2);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glUniform4f(colorLoc, 0.0f, 1.0f, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, height / 2, width / 2, height / 2);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//glfwGetFramebufferSize(window, &width, &height);
		glViewport(width / 2, height / 2, width / 2, height / 2);
		glDrawArrays(GL_LINE_LOOP, 0, 3);
		glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}



// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	GLfloat vertices[] = {
		-5.0f, -2.0f, 0.0f,
		5.0f, -2.0f, 0.0f,
		0.0f, 5.0f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	GLuint vVBO;

	//Definiçao buffer de vertices vVBO

	//Geração do identificador (quantidade) do VBO - como é apenas um VBO, o número 1 será seu identificador. Se fosse um vetor de VBOs como GLuint VBO[5]; seria de 1 ao 5
	glGenBuffers(1, &vVBO);

	//Faz a conexão (binding) do buffer como o seu tipo de buffer (buffer de array, para o VBO)
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);

	//Copia os dados dos vértices (array de floats) para a memória do buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// (TIPO DO BUFFER, tamanho da variável, variável, como queremos que a GPU gerencie os dados);


	GLuint cVBO;
	// Definiçao buffer de cores cVBO
	glGenBuffers(1, &cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint VAO;
	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int generateCircle(float radius, int nPoints)
{
	int totalSize = (nPoints + 2) * 3; // tamanho do array de floats que armazenará os vértices que forma o círculo (+1 é para o vértice do centro do circulo) *3 para coords xyz
	float* vertices = new float[totalSize];  // ponteiro para float - alocação dinamica

	//definindo o ponto central do círculo
	vertices[0] = 0.25;  // x
	vertices[1] = 0.0;  // y
	vertices[2] = 0.0; // z

	float angle = 0.0;
	float slice = 2 * Pi / (GLfloat)nPoints;

	// para um i que começa em 3 (quarta posição do array de vértices), enquanto i for menor que totalSize, i é incrementado de 3 em 3, pois
	// estaremos alimentando o array com 3 posições (coordenadas)

	for (int i = 3; i < totalSize; i += 3)
	{
		std::cout << i << endl;
		if ((i / 3) % 2 == 0)
		{
			float x = radius * cos(angle);
			float y = radius * sin(angle);
			float z = 0.0;
			vertices[i] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = z;
		}
		else {
			float x = 0.5 * radius * cos(angle);
			float y = 0.5 * radius * sin(angle);
			float z = 0.0;
			vertices[i] = x;
			vertices[i + 1] = y;
			vertices[i + 2] = z;
		}

		angle = angle + slice;  //incrementando o angulo ao passo que os pontos sao adicionados
	}

	GLuint VBO, VAO;

	//Geração do identificador do VBO - como é apenas um VBO, o número 1 será seu identificador. Se fosse um vetor de VBOs como GLuint VBO[5]; seria de 1 ao 5
	glGenBuffers(1, &VBO);

	//Faz a conexão (binding) do buffer como o seu tipo de buffer (buffer de array, para o VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copia os dados dos vértices (array de floats) para a memória do buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	// (TIPO DO BUFFER, tamanho da variável, variável, como queremos que a GPU gerencie os dados);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}