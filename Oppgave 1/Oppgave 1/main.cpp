#include <iostream>
#include <fstream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

//Generates 100 vertices (points on the graph) of the mathematical function x^2
const int numberOfVertices = 100;

//Is a string literal that contains the source code for a vertex shader. 
const char* vertexShaderSource = 
//Version 3,3 core
" #version 330 core\n"
//Represents the positon of the vertex. Locatioon 0 is the location of the data 
" layout (location = 0) in vec2 aPos;\n"
//Represents the color attributes of the vertex. Locatioon 1 is the location of the data 
" layout (location = 1) in vec3 aColor;\n"
//A variable called ourColor that will be used to pass color information from the vertex shader to the fragment shader. 
" out vec3 ourColor;\n"
//Calculates the final position of the vetex and assigns color to the vertex 
"void main() {\n"
"    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
"    ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
//Version 3,3 core
" #version 330 core\n"
//This variable recevies the color information passed from the vertex shader. 
" in vec3 ourColor;\n"
//The final color of the fragment
" out vec4 FragColor;\n"
//Sets the final color of the fragment. It takes the RBG values from the variable 'our color' and sets the transparency to 1.
"void main() {\n"
"    FragColor = vec4(ourColor, 1.0);\n"
"}\0";

// Function f(x)= x^2
double function( const double x)
{
    return pow(x, 2);
}

double differenceQuotient(double x)
{
    //Calculates the Newtons quotient
    const double h = 0.001;
    return (function(x + h) - function(x)) / h;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(void)
{
    // GLFW holds the information of a window (size, position etc.)
    // window is a pointer that stores adress where a 'GLFWwindow is located. 
    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW library" << endl;
        return -1;
    }
      

    //Specifies that the major and minor version of OpenGL is 3,3. 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Specifies that the core profile of OpenGL is used. 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Creates a window and checks if it was successful. 
    window = glfwCreateWindow(800, 600, "Graph of the function x^2", nullptr, nullptr);
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // OpenGL is a state machine and when creating a window with GLFW, it comes with its own OpenGL context.
    //This line of code spesifices that the attributes given to 'window' should be rendered 
    glfwMakeContextCurrent(window);
   
    //Checks if the loading of the function pointers using GLAD is successful.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failied to load openGL pointers!" << endl;
        glfwTerminate();
        return -1;
    }

    //Sets up a window with the size 800 in width, 600 in height and 0,0 (lower left corner)
    glViewport(0, 0, 800, 600);
    //This function is called automatecally when the window is resized. 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Definition quantity
    double a = -2.0; //The start of the definition quantity
    double b = 2.0; //The end of the definition quantitity 

    //Intervals n+1
    int n = 40;

    //Calculates the dissolution of h
    double h = (b - a) / n;

    // Opens the text file for writing
    ofstream file("Data.txt");

    // Kompiler shaderne
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Koble shaderne til et shaderprogram
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Slett shaderobjektene når de ikke lenger er nødvendige
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Generer verteksdata og indeksdata
    float vertices[numberOfVertices * 5];  // (x, y, r, g, b)
    int indices[numberOfVertices];

    double prev_y = function(a);
    for (int i = 0; i < numberOfVertices; ++i) {
        double x = a + i * h;
        double y = function(x);
        double deriv = differenceQuotient(x);

        vertices[i * 5] = static_cast<float>(x);
        vertices[i * 5 + 1] = static_cast<float>(y);
        vertices[i * 5 + 2] = (y > prev_y) ? 0.0f : 1.0f;  // R
        vertices[i * 5 + 3] = (y > prev_y) ? 1.0f : 0.0f;  // G
        vertices[i * 5 + 4] = 0.0f;  // B

        prev_y = y;
        indices[i] = i;

        file << x << " " << y << " " << deriv << std::endl;
    }

    // Opprett VAO, VBO og EBO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Kopier verteksdata til VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Kopier indeksdata til EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Spesifiser attributtene til verteksen
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Spesifiser attributtene til fargen
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
    glBindVertexArray(0);

    //Closes the text file
    file.close();

    cout<< "The data points has been created and saved in the file 'Data.txt'"<<endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        // Bind VAO
        glBindVertexArray(VAO);

        // Tegn grafen
        glDrawElements(GL_LINE_STRIP, numberOfVertices, GL_UNSIGNED_INT, 0);

        // Unbind VAO
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    // Rydd opp og avslutt
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//Closes the window when esc kay is pressed. 
void processInput(GLFWwindow* window)
{
    //Closes the window with the esc key
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

//This is a callback function that adjusts the dimentions it should render in when the window is resized.  
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cout << "The window size is: " << width << " x " << height << endl;
}

