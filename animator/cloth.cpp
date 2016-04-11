#include "cloth.h"
#include <FL/gl.h>
#include <math.h>
#include "constraint.h"



Cloth::Cloth(Vec3f origin, float width, float height, int x, int y) {

	// Create grid: (0,0,0)->(width*x_num, -height*y_num, 0)
	x_num = x;
	y_num = y;

	//cpList = new ClothParticle[x_num * y_num];

	for (int i = 0; i < x_num; i++) {

		for (int j = 0; j < y_num; j++) {

			Vec3f pos = Vec3f(width * i + origin[0],
						-height * j + origin[1],
						0); // reset z

			if ((i == 0 && j == 0) || (j == 0 && i == x_num - 1) || (i == 0 && j == y_num - 1) || (i ==x_num - 1 && j == y_num - 1)) {
				cpList.push_back(new ClothParticle(pos, true));
			} else {
				cpList.push_back(new ClothParticle(pos, false));
			}

		}
	}
}

Cloth::~Cloth() {

	//delete [] cpList;
}

void Cloth::drawCloth() {
	for (int i = 0; i < x_num - 1; ++i) {
		for (int j = 0; j < y_num - 1; ++j) {
			int index = j*x_num + i;

			Vec3f p1 = cpList[index].getPosition();

			index = (j+1)*x_num+i;

			Vec3f p2 = cpList[index].getPosition();

			index = (j+1)*x_num+i+1;

			Vec3f p3 = cpList[index].getPosition();

			index = j*x_num+i+1;
			
			Vec3f p4 = cpList[index].getPosition();

			Vec3f n1 = Vec3f(0.0);
			Vec3f n2 = Vec3f(0.0);
			Vec3f n3 = Vec3f(0.0);
			Vec3f n4 = Vec3f(0.0);

			Vec3f normal = findNormal(p3, p1, p2);

			n3 += normal;
			n1 += normal;
			n2 += normal;

			normal = findNormal(p4, p3, p2);

			n4 += normal;
			n3 += normal;
			n2 += normal;

			const GLfloat vertices[] = {
			 	p1[0], p1[1], p1[2],    
			 	p2[0], p2[1], p2[2],
				p3[0], p3[1], p3[2],
				p4[0], p4[1], p4[2]
			};

			const GLfloat normals[] = {
				n1[0], n1[1], n1[2],
				n2[0], n2[1], n2[2],
				n3[0], n3[1], n3[2],
				n4[0], n4[1], n4[2],
			};

			const GLubyte indices[] = {0, 1, 2, 0, 2, 3};
			const GLfloat colors[] = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0 };

			glEnableClientState(GL_NORMAL_ARRAY);
			glEnableClientState(GL_COLOR_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);
			
			glNormalPointer( GL_FLOAT, 0, normals);
			glColorPointer(3, GL_FLOAT, 0, colors);
			glVertexPointer(3, GL_FLOAT, 0, vertices);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indices);

			glDisableClientState(GL_NORMAL_ARRAY);
			glDisableClientState(GL_COLOR_ARRAY);
			glDisableClientState(GL_VERTEX_ARRAY);

		}
	}


}

Vec3f Cloth::findNormal(Vec3f p1, Vec3f p2, Vec3f p3){

		Vec3f v1 = p2-p1;
		Vec3f v2 = p3-p1;

		return v1 ^ v2;

}


//Cloth c1(10, 10, 4, 4);