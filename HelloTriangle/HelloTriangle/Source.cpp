/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 05/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace std;

const float Pi = 3.1416;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
int generateCircle(float radius, int nPoints);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 800;



// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Pink Floyd - The Dark Side of the Moon", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader que est�o em arquivos

	Shader shader("../shaders/vs_hello.vs", "../shaders/fs_hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();

	// Buffer da fun�ao de circulo
	GLuint nPoints = 10;
	//GLuint VAO = generateCircle(0.5, nPoints);
	


	// Enviando a cor desejada (vec4) para o fragment shader
	// Utilizamos a vari�veis do tipo uniform em GLSL para armazenar esse tipo de info
	// que n�o est� nos buffers
	
	//GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	//assert(colorLoc > -1);

	glUseProgram(shader.ID);

	// Criando a matriz de proje��o usando glm
	glm::mat4 projection = glm::mat4(1);       // matriz identidade
	projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(10);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		//glUniform4f(colorLoc, 0.0f, 0.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);   // esse � o comando propriamente dito de "drawcall" // 0 mostra a partir de onde vai ser o deslocamento do array e 3 eh o numero de vertices
		glUseProgram(shader.ID);  // ???
		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		// PONTOS - GL_POINTS
		//glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f); //enviando cor para vari�vel uniform inputColor
		//glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints + 2);
		//glDrawArrays(GL_LINES, 0, 3);  // COMO DESENHAR A LINHA COM CORES DIFERENTES DO colors[]? USAR colorLoc OU N�O? 
		glDrawArrays(GL_POINTS, 0, 3);
	
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}



// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	GLfloat vertices[] = {
		-0.5f, -0.2f, 0.0f,
		0.5f, -0.2f, 0.0f, 
		0.0f, 0.5f, 0.0f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	
	GLuint vVBO;

	//Defini�ao buffer de vertices vVBO

	//Gera��o do identificador (quantidade) do VBO - como � apenas um VBO, o n�mero 1 ser� seu identificador. Se fosse um vetor de VBOs como GLuint VBO[5]; seria de 1 ao 5
	glGenBuffers(1, &vVBO);

	//Faz a conex�o (binding) do buffer como o seu tipo de buffer (buffer de array, para o VBO)
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);

	//Copia os dados dos v�rtices (array de floats) para a mem�ria do buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// (TIPO DO BUFFER, tamanho da vari�vel, vari�vel, como queremos que a GPU gerencie os dados);


	GLuint cVBO;
	// Defini�ao buffer de cores cVBO
	glGenBuffers(1, &cVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLuint VAO;
	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, vVBO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, cVBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int generateCircle(float radius, int nPoints)
{
	int totalSize = (nPoints + 2) * 3; // tamanho do array de floats que armazenar� os v�rtices que forma o c�rculo (+1 � para o v�rtice do centro do circulo) *3 para coords xyz
	float* vertices = new float[totalSize];  // ponteiro para float - aloca��o dinamica

	//definindo o ponto central do c�rculo
	vertices[0] = 0.25;  // x
	vertices[1] = 0.0;  // y
	vertices[2] = 0.0; // z

	float angle = 0.0;
	float slice = 2 * Pi / (GLfloat)nPoints;

	// para um i que come�a em 3 (quarta posi��o do array de v�rtices), enquanto i for menor que totalSize, i � incrementado de 3 em 3, pois
	// estaremos alimentando o array com 3 posi��es (coordenadas)

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
		}else {
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

	//Gera��o do identificador do VBO - como � apenas um VBO, o n�mero 1 ser� seu identificador. Se fosse um vetor de VBOs como GLuint VBO[5]; seria de 1 ao 5
	glGenBuffers(1, &VBO);

	//Faz a conex�o (binding) do buffer como o seu tipo de buffer (buffer de array, para o VBO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Copia os dados dos v�rtices (array de floats) para a mem�ria do buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	// (TIPO DO BUFFER, tamanho da vari�vel, vari�vel, como queremos que a GPU gerencie os dados);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

