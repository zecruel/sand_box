#include <stddef.h>
#include <stdio.h>
#include <math.h>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define MAX_TRIANG 500
#define TOLERANCE 1e-6

#define GLSL(src) "#version 150 core\n" #src

/* mantain one unique element in a sorted array */
int unique (int n, const char **a) {
   int dst = 0, i;
   for (i = 1; i < n; ++i) {
       if (strcmp (a[dst], a[i]) != 0)
           a[++dst] = a[i];
   }

   return dst + 1;
}


struct Vertex {
    GLfloat pos[3];
    GLfloat uv[2];
    GLubyte col[4];
};

struct ogl {
	struct Vertex *verts;
	GLuint *elems;
	int vert_count;
	int elem_count;
};

int line_gl (struct ogl *gl_ctx, GLfloat p0[2], GLfloat p1[2], int thick){
	if (!gl_ctx) return 0;
	if (!gl_ctx->verts) return 0;
	if (!gl_ctx->elems) return 0;
	
	/* get polar parameters of line */
	float dx = p1[0] - p0[0];
	float dy = p1[1] - p0[1];
	float modulus = sqrt(pow(dx, 2) + pow(dy, 2));
	float cosine = 1.0;
	float sine = 0.0;
	
	if (modulus > TOLERANCE){
		cosine = dx/modulus;
		sine = dy/modulus;
	}
	
	int i = 0, j = 0;
	
	if (thick <= 0) thick = 1;
	
	float t = (float) thick / 600.0;
		
	j = gl_ctx->vert_count;
	
	gl_ctx->verts[j].pos[0] = p0[0] - sine * t;
	gl_ctx->verts[j].pos[1] = p0[1] + cosine * t;
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = 255;
	gl_ctx->verts[j].col[1] = 0;
	gl_ctx->verts[j].col[2] = 0;
	gl_ctx->verts[j].col[3]= 150;
	gl_ctx->verts[j].uv[0]= 0.0;
	gl_ctx->verts[j].uv[1]= 0.0;
	gl_ctx->vert_count ++;
	
	j = gl_ctx->vert_count;
	
	gl_ctx->verts[j].pos[0] = p0[0] + sine * t;
	gl_ctx->verts[j].pos[1] = p0[1] - cosine * t;
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = 255;
	gl_ctx->verts[j].col[1] = 0;
	gl_ctx->verts[j].col[2] = 0;
	gl_ctx->verts[j].col[3] = 150;
	gl_ctx->verts[j].uv[0]= 0.0;
	gl_ctx->verts[j].uv[1]= 1.0;
	gl_ctx->vert_count ++;
	
	j = gl_ctx->vert_count;
	
	gl_ctx->verts[j].pos[0] = p1[0] - sine * t;
	gl_ctx->verts[j].pos[1] = p1[1] + cosine * t;
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = 255;
	gl_ctx->verts[j].col[1] = 0;
	gl_ctx->verts[j].col[2] = 0;
	gl_ctx->verts[j].col[3] = 150;
	gl_ctx->verts[j].uv[0]= 1.0;
	gl_ctx->verts[j].uv[1]= 0.0;
	gl_ctx->vert_count ++;
	
	j = gl_ctx->vert_count;
	
	gl_ctx->verts[j].pos[0] = p1[0] + sine * t;
	gl_ctx->verts[j].pos[1] = p1[1] - cosine * t;
	gl_ctx->verts[j].pos[2] = 0.0;
	gl_ctx->verts[j].col[0] = 255;
	gl_ctx->verts[j].col[1] = 0;
	gl_ctx->verts[j].col[2] = 0;
	gl_ctx->verts[j].col[3] = 150;
	gl_ctx->verts[j].uv[0]= 1.0;
	gl_ctx->verts[j].uv[1]= 1.0;
	gl_ctx->vert_count ++;
	
	j = gl_ctx->elem_count * 3;
	
	gl_ctx->elems[j] = gl_ctx->vert_count - 4;
	gl_ctx->elems[j+1] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+2] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+3] = gl_ctx->vert_count - 3;
	gl_ctx->elems[j+4] = gl_ctx->vert_count - 2;
	gl_ctx->elems[j+5] = gl_ctx->vert_count - 1;
	
	gl_ctx->elem_count+= 2;
}

int main(int argc, char *argv[])
{
	struct ogl gl_ctx;
	gl_ctx.vert_count = 0;
	gl_ctx.elem_count = 0;
	gl_ctx.verts = NULL;
	gl_ctx.elems = NULL;
	
	/* buffer setup */
        GLsizei vs = sizeof(struct Vertex);
        size_t vp = offsetof(struct Vertex, pos);
        size_t vt = offsetof(struct Vertex, uv);
        size_t vc = offsetof(struct Vertex, col);
	
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 600, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	
	/* Init GLEW */
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	/* Create Vertex Array Object */
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/* Create a Vertex Buffer Object and copy the vertex data to it */
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_TRIANG*3*vs, NULL, GL_STATIC_DRAW);
	
	/* Create a Element Buffer Object */
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_TRIANG*3*sizeof(GLuint), NULL,  GL_STATIC_DRAW);
	
	/* Create and compile the vertex shader */
	const char* vertexSource = GLSL(
		in vec3 position;
		in vec2 uv;
		in vec4 color;
		out vec4 vertexColor;
		out vec2 texcoord;
	
		void main() {
			gl_Position = vec4(position, 1.0);
			vertexColor = color;
			texcoord = uv;
		}
	); /* =========== vertex shader */

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	/* Create and compile the fragment shader */
	const char* fragmentSource = GLSL(
		in vec4 vertexColor;
		in vec2 texcoord;
		
		out vec4 outColor;
		
		uniform sampler2D tex;

		void main() {
			outColor = texture(tex, texcoord) * vertexColor;
		}
	); /* ========== fragment shader */

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	/* Link the vertex and fragment shader into a shader program */
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
		
	/*texture */
	GLuint textures[2];
	glGenTextures(2, textures);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	/* blank texture (default) */
	GLubyte blank[] = {255,255,255,255};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, blank);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	// Black/white checkerboard
	GLubyte pixels[] = {
		0,0,0,255,    255,255,255,255,
		255,255,255,255,    0,0,0,255
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	glUniform1i(glGetUniformLocation(shaderProgram, "tex"), 1);

	/* Specify the layout of the vertex data */
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	GLint uvAttrib = glGetAttribLocation(shaderProgram, "uv");
	GLint colorAttrib = glGetAttribLocation(shaderProgram, "color");
	
	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(uvAttrib);
	glEnableVertexAttribArray(colorAttrib);
	
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer(colorAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
	
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	/* load vertices/elements directly into vertex/element buffer */
        gl_ctx.verts = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        gl_ctx.elems = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	
	
	//int line_gl (struct ogl *gl_ctx, GLfloat p0[2], GLfloat p1[2], int thick)
	line_gl(&gl_ctx, (GLfloat[]){0.0, 0.0}, (GLfloat[]){0.5, 0.7}, 10);
	line_gl(&gl_ctx, (GLfloat[]){0.0, 0.0}, (GLfloat[]){-0.5, 0.7}, 5);
	line_gl(&gl_ctx, (GLfloat[]){-0.5, 0.7}, (GLfloat[]){-0.7, 0.0}, 0);
	line_gl(&gl_ctx, (GLfloat[]){-0.5, 0.7}, (GLfloat[]){0.0, 0.7}, 0);
	
	line_gl(&gl_ctx, (GLfloat[]){-0.5, -0.7}, (GLfloat[]){0.5, -0.7}, 30);
	line_gl(&gl_ctx, (GLfloat[]){0.1, -0.7}, (GLfloat[]){0.1, 0.7}, 30);
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	int i = 0;
	SDL_Event windowEvent;
	while (1) {
		
		if (SDL_PollEvent(&windowEvent)) {
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_MOUSEBUTTONDOWN) {
				i = !i;
			}
		}
		
		//i = !i;
		glUniform1i(glGetUniformLocation(shaderProgram, "tex"), i);
		
		/* Clear the screen to black */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Draw a triangle from the 3 vertices */
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, gl_ctx.elem_count*3, GL_UNSIGNED_INT, 0);

		/* Swap buffers */
		SDL_GL_SwapWindow(window);
	}
	
	/* Delete allocated resources */
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
	SDL_GL_DeleteContext(context);
	SDL_Quit();
	return 0;
}
