#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
using namespace std;

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 color;\n"
"void main(){\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   color = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"in vec3 color;\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(color, 1.0);\n"
"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

double function(double x, double y);
void calculateColor(double x, double& r, double& g, double& b);

int main() 
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW library" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "f(x,y)= 2x^2y", nullptr, nullptr);
    if (!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    unsigned int vertexShader, fragmentShader, shaderProgram;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to compile the vertex shader" << infoLog << endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to compile the fragment shader" << infoLog << endl;
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        cout << "Failed to link the shader program" << infoLog << endl;
    }
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Opens a textfile
    ofstream outfile("Data.txt");

    //Definition quantity
    double a_x = -2.0; //The start of the definition quantity x- axis 
    double b_x = 2.0; //The end of the definition quantity x-axis 
    double a_y = -2.0; //The start of the definition quantity y- axis 
    double b_y = 2.0; //The end of the definition quantity y-axis 

    //Number of data points (coordinates)
    int numberOfVertices_x = 10;
    int numberOfVertices_y = 10; 

    //Calculates the dissolution of h
    double h_x = (b_x - a_x) / (numberOfVertices_x-1);
    double h_y = (b_y - a_y) / (numberOfVertices_y-1);

    //Shows in the top of the text file how many lines of data points is in the text file 
    outfile << "Number of lines: " <<  numberOfVertices_x * numberOfVertices_y << endl;

    for (int i = 0; i < numberOfVertices_x; ++i) {
        for (int j = 0; j < numberOfVertices_y; ++j) {
            // Calculates the values for x and y coordinates 
            double x = a_x + i * h_x;
            double y = a_y + j * h_y;

            double z = function(x, y);

            //Calculates the rbg values based on the x value 
            double r;
            double g;
            double b;
            calculateColor(x, r, g, b);

            // Writes out the coordinates to the text file 
            outfile << "x: " << x << " y: " << y << " z: " << z << 
            " red: "<< r << " green: " << g << " blue: " << b<<  endl;
        }
    }

    // Closes the textfile 
    outfile.close();

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numberOfVertices_x * numberOfVertices_y * 6, nullptr, GL_STATIC_DRAW);

  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    cout << "The data points has been created and saved in the file 'Data.txt'" << endl;

    while (!glfwWindowShouldClose(window)) {

        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices_x * numberOfVertices_y); 

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cout << "The window size is: " << width << " x " << height << endl;
}

void processInput(GLFWwindow* window)
{
    //Closes the window with the esc key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

//Used a function from the lecture notes f(x,y)=2x^2y
double function(double x, double y)
{
    return 2.0 * pow(x, 2) * y;
}

//This function map the input value x from its original number to normalized 
//value from 0 to 1 that is used for color in openGL
void calculateColor(double x, double& r, double& g, double& b) {
    
    //This calculation do so the range of value x is between -0,5 and 0,5 
    r = (x + 1.0) / 2.0; 
    g = (x + 1.0) / 2.0;
    b = (x + 1.0) / 2.0;

    //Normalizes the RBG- values to be between 0 and 1, so it will read correctly in an OpenGL project 
    //This makes the values be between 0 and 1 and not -0,5 and 0,5 
    r = max(0.0, min(1.0, r));
    g = max(0.0, min(1.0, g));
    b = max(0.0, min(1.0, b));

}
