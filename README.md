# Hello Triangle in OpenGL

A simple OpenGL program that renders a blue triangle using modern OpenGL (version 3.3 core profile).

## Features

- Renders a blue triangle using OpenGL 3.3 core profile
- Uses GLAD for OpenGL function loading
- GLFW for window management and input handling
- Proper vertex and fragment shader setup
- Clean resource management

## Prerequisites

- OpenGL 3.3 compatible graphics card
- GLAD library
- GLFW library
- C++23 compatible compiler (uses `std::print`)

## Building

```bash
# Make sure you have GLAD and GLFW installed
make
```

## Usage

```bash
./main
```

**Controls:**
- Press `ESC` to exit

## What it does

1. Creates an 800x600 window
2. Sets up OpenGL context with GLAD
3. Defines triangle vertices in normalized device coordinates
4. Creates and compiles vertex and fragment shaders
5. Renders a blue triangle on white background
6. Handles window resizing and input

## Code Structure

- **Vertex Shader**: Passes vertex positions through without transformation
- **Fragment Shader**: Colors all pixels with a blue color `(0.0, 0.4745, 0.7215, 1.0)`
- **VAO/VBO**: Manages vertex data on GPU
- **Render Loop**: Continuously draws the triangle until window is closed

## Learning Points

This program demonstrates:
- Modern OpenGL pipeline setup
- Shader compilation and linking
- Vertex buffer objects and vertex array objects
- Basic render loop structure
- Error checking for shaders

Perfect starting point for learning OpenGL graphics programming!
