#include "pch.h"

#include <Kore/IO/FileReader.h>
#include <Kore/Graphics/Graphics.h>
#include <Kore/Graphics/Graphics2.h>
#include <Kore/Graphics/Color.h>
#include <Kore/Graphics/Shader.h>
#include <Kore/System.h>
#include <limits>
#include <stdlib.h>
#include <stdio.h>

using namespace Kore;

namespace {
    Graphics2* g2;
    
    Texture* imageRed;
    Texture* imageGreen;
    Texture* imageBlue;
    
	Shader* vertexShader;
	Shader* fragmentShader;
	Program* program;
	VertexBuffer* vertices;
	IndexBuffer* indices;
    
    mat4 projectionMatrix;

	void update() {
        //printf("update\n");
		Graphics::begin();
		Graphics::clear(Kore::Graphics::ClearColorFlag);
        
        g2->begin();
        
        g2->drawImage(imageRed, 520, 50);
        g2->drawImage(imageGreen, 560, 50);
        g2->drawImage(imageBlue, 600, 50);
        
        g2->setColor(Color::Yellow);
        g2->fillTriangle(500, 300, 700, 300, 600, 200);
        g2->setColor(Color::Blue);
        g2->drawRect(500, 10, 150, 150, 3);
        g2->setColor(Color::White);

        g2->end();

		program->set();
		Graphics::setVertexBuffer(*vertices);
		Graphics::setIndexBuffer(*indices);
		Graphics::drawIndexedVertices();

		Graphics::end();
		Graphics::swapBuffers();
	}
}

int kore(int argc, char** argv) {
    int w = 1024;
    int h = 768;
    
    System::init("Shader", w, h);
    
    g2 = new Graphics2(w, h);
    
	Kore::System::setCallback(update);

	FileReader vs("shader.vert");
	FileReader fs("shader.frag");
	vertexShader = new Shader(vs.readAll(), vs.size(), VertexShader);
	fragmentShader = new Shader(fs.readAll(), fs.size(), FragmentShader);
	VertexStructure structure;
	structure.add("pos", Float3VertexData);
    
	program = new Program;
	program->setVertexShader(vertexShader);
	program->setFragmentShader(fragmentShader);
	program->link(structure);
    
    projectionMatrix = mat4::orthogonalProjection(0, w, h, 0, 0.1f, 1000);

	vertices = new VertexBuffer(3, structure);
	float* v = vertices->lock();
	v[0] = -1; v[1] = -1; v[2] = 0.5;
	v[3] = 1;  v[4] = -1; v[5] = 0.5;
	v[6] = -1; v[7] = 1;  v[8] = 0.5;
	vertices->unlock();

	indices = new IndexBuffer(3);
	int* i = indices->lock();
	i[0] = 0; i[1] = 1; i[2] = 2;
	indices->unlock();
    
    imageRed = new Texture("Textures/red.png");
    imageGreen = new Texture("Textures/green.png");
    imageBlue = new Texture("Textures/blue.png");

	Kore::System::start();

	return 0;
}
