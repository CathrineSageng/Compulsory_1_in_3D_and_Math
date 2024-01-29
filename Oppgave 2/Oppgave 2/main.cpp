#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

//https://www.overleaf.com/project/65af96afd34739496ebf18ec

using namespace std;

//Parameters that are used to define the shape of the spiral 
float a = 0.1f; //Affetcs the distance between the circles in the spiral 
float b = 0.1f; //Affects the height of the circles in the spiral 

//Stores the coordinates x, y, z.
vector<float> verticesPositions;
//Stores the color coordinates for every single vertex 
vector<float> spiralColors;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

ofstream file("Data.txt");

void Spiral();

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

int main() {

    GLFWwindow* window;

    if (!glfwInit())
    {
        cout << "Failed to initialize GLFW library" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Spiral", nullptr, nullptr);
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

    Spiral();

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesPositions.size() * sizeof(float), verticesPositions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, spiralColors.size() * sizeof(float), spiralColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    file.close();

    cout << "The data points has been created and saved in the file 'Data.txt'" << endl;

    while (!glfwWindowShouldClose(window)) {
    
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, verticesPositions.size()/3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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

//Creates the data points for a 3D spiral, calculates the positions of the vertices and assigns 
//color to the vertices
void Spiral() 
{
    //Number of data points to be generated. 
    //The loop will iterate 50 times for genereating 50 data points 
     int numberOfDataPoints = 50;
    for (int i = 0; i < numberOfDataPoints; ++i) 
    {
        //t is used to control the position along the spiral 
        //Converts the loop variable 'i' from int to float with casting operation 
        float t = static_cast<float>(i) / numberOfDataPoints * 10.0f;

        //cos(t) and sin(t) are trigonometric functions. They create circular motions along the 
        //x- axis and y- axis 
        float x = a * t * cos(t);
        float y = a * t * sin(t);
        //Decides the height of the circle in the spiral and increase along the x-axis
        float z = b * t;

        //These three lines are used to store the calculated values of x,y and z in the end of the vector
        verticesPositions.push_back(x);
        verticesPositions.push_back(y);
        verticesPositions.push_back(z);

        //The color value to red varies from 0 to 1 were the loop variable 1 is 
        //divided by numberOfDataPoints. 
        //These tree calculations creates gradient colors 
        float red = static_cast<float>(i) / numberOfDataPoints;
        float green = 0.5f - red;
        float blue = 1.0f;

        //These three lines are used to store the calculated values for r, g, b and put in the end of the vector
        spiralColors.push_back(red);
        spiralColors.push_back(green);
        spiralColors.push_back(blue);
        
        file << "x: "<< x << " y: "<< y <<" z: " << z << " r: "
        << red<< " g: "<< green << " b: " << blue <<endl;
    }
}


