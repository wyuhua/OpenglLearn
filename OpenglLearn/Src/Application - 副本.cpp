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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
                    -50.0f, -50.0f, 0.0f, 0.0f,//0
                    50.0f, -50.0f, 1.0f, 0.0f,//1
                    50.0f, 50.0f, 1.0f, 1.0f,//2
                    -50.0f, 50.0f, 0.0f, 1.0f//3
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

        //mvp
        glm::mat4 proj = glm::ortho(0.0f, 960.f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

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

        //ImGui
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        const char* glsl_version = "#version 150"; //notice
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(200.0f, 200.0f, 0.0f);
        glm::vec3 translationB(400.0f, 200.0f, 0.0f);
        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            render.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            {
                glm::mat4 view = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                render.Draw(va, ib, shader);
            }

            {
                glm::mat4 view = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                render.Draw(va, ib, shader);
            }
         
            if (r > 1.0f)
            {
                increment = -0.05f;
            }
            else if (r < 0.0f)
            {
                increment = 0.05f;
            }

            r += increment;

            {
                ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 960.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            // Rendering
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwSwapInterval(1);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
