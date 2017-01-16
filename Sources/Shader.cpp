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
    
    /*Kravur* font14;
    Kravur* font24;
    Kravur* font34;
    Kravur* font44;*/

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
        
        /*g2->setFont(font24);
        g2->setFontColor(Color::White);
        g2->setFontSize(24);
        char text[42];
        sprintf(text, "Hallo Shader");
        g2->drawString(text, 50, 10);*/

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
    
    /*font14 = Kravur::load("Fonts/arial", FontStyle(), 14);
    font24 = Kravur::load("Fonts/arial", FontStyle(), 24);
    font34 = Kravur::load("Fonts/arial", FontStyle(), 34);
    font44 = Kravur::load("Fonts/arial", FontStyle(), 44);*/

	Kore::System::start();

	return 0;
}
