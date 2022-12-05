#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"
#include "VertexBufferLayou.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit() error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        //vertex data
        float positions[] = {
                    -0.5f, -0.5f, 0.0f, 0.0f,//0
                    0.5f, -0.5f, 1.0f, 0.0f,//1
                    0.5f, 0.5f, 1.0f, 1.0f,//2
                    -0.5f, 0.5f, 0.0f, 1.0f//3
        };

        //vertex indexs
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        //vetexbuffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        //vertexbuffer layout
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);

        //vetextArray
        VertexArray va;
        va.AddBuffer(vb, layout);

        //indexBuffer
        IndexBuffer ib(indices, 6);

        //shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);

        //texture
        Texture texture("res/textures/logo2.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        //reset state
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        Renderer render;
       
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            render.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
      
            render.Draw(va, ib, shader);

            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwSwapInterval(1);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();

    return 0;
}