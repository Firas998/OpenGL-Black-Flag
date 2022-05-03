#pragma once
#include <iostream>
#include <vector>


class Cubemap {
public: 
	void initialize();
	void draw();
private:
	unsigned int cubemap;
	unsigned int shader;
	unsigned int loadCubemap(std::vector<std::string> faces);
};