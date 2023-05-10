# 42_miniRT

# MiniRT - A Mini Ray Tracing Project at 42

![MiniRT](./images/miniRT_sample_image.jpg)

Welcome to **MiniRT**, a mini ray tracing project. This project is focused on creating a simple ray tracing engine, which will enable you to render beautiful 3D scenes with realistic lighting and shading.

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [Credits](#credits)

## Introduction

Ray tracing is a powerful technique used to render 3D scenes by simulating the behavior of light as it interacts with objects. This project aims to create a simple and efficient ray tracing engine that can render images with realistic lighting, shadows, and reflections.

## Features

- Basic primitives (sphere, plane, square, triangle, and cylinder)
- Multiple light sources
- Diffuse and specular shading
- Shadows and reflections
- Adjustable camera and viewport settings
- Anti-aliasing
- File format for defining scenes
- Save rendered images as BMP files

## Requirements

- GCC or Clang compiler
- GNU Make
- Basic understanding of linear algebra and C programming

## Installation

1. Clone the repository:

```sh
git clone https://github.com/yourusername/miniRT.git
```
2. Navigete to the project directory and build the project:
```sh
cd miniRT
make
```
## USAGE
1. Create a scene file with the .rt extension, following the scene file format provided in the documentation.
2. Run the miniRT executable with the path to the scene file:
```sh
./miniRT /path/to/scene/file.rt
```
## Examples
### Sphere
![Screenshot from 2023-05-10 16-03-48](https://github.com/IAmVictorG/42_miniRT/assets/112464371/e1e249ec-ef79-4688-a320-bd79db38d708)


