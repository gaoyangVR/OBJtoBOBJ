#include"general.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <string.h>
#include "zlib/zlib.h"
using namespace std;
extern int edg = 0, vetcnt = 0, frame = 1500;

void main(){
	


char name[100];
while (frame < 1501){

float *vectors = new float  [30000000];
int *facesindices = new int [90000000];
float *vetnormal = new float[30000000];

	sprintf(name, "./objsequence/bubble_%d.obj", frame);	//所有的bobj文件都叫做fluidsurface， 序号从0开始= ;
	countvectors(name, vectors, vetnormal, facesindices, vetcnt, edg);
	readObjFile(name, vectors, vetnormal, facesindices, vetcnt, edg);
	outputBOBJ(name, vectors, vetnormal, facesindices, vetcnt, edg);
	frame++;

delete[]vectors;
delete[]facesindices;
delete[]vetnormal;
}


}

void readObjFile(const std::string& name,float *vectors, float * vetnormal, int * facesindices, int cnt, int edgnum) {
	ifstream dataFile (name.c_str());
//	char num[5];
	//sprintf(num, "00037");
	int n = 0, j = 0, k = 0;

	if (!dataFile.good())
	printf("can't open file !" );



	while (dataFile.good() && !dataFile.eof()) {
		string id;
		dataFile >> id;

		if (id[0] == '#') {
			// comment
			getline(dataFile, id);
			continue;
		}
		if (id == "vt") {
			// tex coord, ignore            
		}
		else if (id == "v") {
			// vertex
			dataFile >> vectors[j] >> vectors[j+1] >> vectors[j+2];
			j +=3;
		//	printf("%d  %f ",j, vectors[j+1]); getchar();
		}
		else if (id == "vn") {
			//vector normal;
			dataFile >> vetnormal[n] >> vetnormal[n + 1] >> vetnormal[n + 2];
			n += 3;
			
		}
	
		else if (id == "g") {
			// group, ignore
		}
		else if (id == "f") {
			// face
		//	dataFile >> facesindices[k] >> facesindices[k + 1] >> facesindices[k + 2];
		
			// face
			string face;
			Triangle t;
			for (int i = 0; i < 3; i++) {
				dataFile >> face;
				if (face.find('/') != string::npos)
					face = face.substr(0, face.find('/')); // ignore other indices
				
				int idx = atoi(face.c_str()) - 1;
				if (idx < 0)
				{
					printf("invalid face encountered!"); getchar();
				}
			//	idx += cnt;
			//	printf("%d ", idx); getchar();
			facesindices[k + i] = idx;
			}
			k += 3;
		}
		
		else {
			// whatever, ignore
		}
		// kill rest of line
		getline(dataFile, id);
	}
	
	dataFile.close();

}

void outputBOBJ(const std::string& name, float *vectors, float * vetnormal, int * facesindices, int cnt,int edgnum)
{
	//filename
	static char filename[100];
	sprintf(filename, "BOBJdata\\fluidsurface_final_%04d.bobj.gz", frame);	//所有的bobj文件都叫做fluidsurface， 序号从0开始
	//FILE *fp = fopen(filename, "w");
	gzFile gzf = gzopen(filename, "wb1"); // do some compression
	if (gzf == NULL)
	{
		printf("cannot open bobj file waterBOBJdata for output here!!!\n");
		getchar();
		return;
	}
	printf("%d ", frame);
	//printf("%d", edgnum); getchar();

	//如果没有MC三角形，则只引用头文件且输出
	if (cnt == 0)
	{
		gzclose(gzf);
		return;
	}
	// vertex positions

	gzwrite(gzf, &cnt, sizeof(int));
	for (int i = 0; i < cnt*3; i+=3)
	{
		gzwrite(gzf, &vectors[i], sizeof(float));
		gzwrite(gzf, &vectors[i+1], sizeof(float));
		gzwrite(gzf, &vectors[i+2], sizeof(float));
	}

	//vertex normals
	gzwrite(gzf, &cnt, sizeof(int));
	for (int i = 0; i < cnt*3; i+=3)
	{
		gzwrite(gzf, &vetnormal[i], sizeof(float));
		gzwrite(gzf, &vetnormal[i + 1], sizeof(float));
		gzwrite(gzf, &vetnormal[i + 2], sizeof(float));
	}

	//face indices.
	
	gzwrite(gzf, &edgnum, sizeof(int));
	for (int i = 0; i < edgnum*3; i = i + 3)
		//for (int j = 0; j < 3; j++)
	{
		int trip = abs(facesindices[i]);
		gzwrite(gzf, &trip, sizeof(int));
		trip = abs(facesindices[i + 1]);
		gzwrite(gzf, &trip, sizeof(int));
		trip = abs(facesindices[i + 2]);
		gzwrite(gzf, &trip, sizeof(int));
		//fprintf(fp, "f  %d %d %d\n", hindices[i] + 1, hindices[i + 1] + 1, hindices[i + 2] + 1);	//obj indices from 1 !!!
	}

	gzclose(gzf);
}

void outputOBJ(const std::string& name, float *vectors, float * vetnormal, int * facesindices, int cnt, int edgnum)
{
	//filename
	static char filename[100];
	sprintf_s(filename, "OBJdata\\%05d.obj", 001);
	FILE *fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("cannot open obj file waterOBJdata for output here!!\n");
		getchar();
		return;
	}

	//如果没有MC三角形，则只引用头文件且输出
	if (cnt == 0)
	{
		fclose(fp);
		return;
	}

	fprintf(fp, "#obj model\n");
	fprintf(fp, "#vertex_vectors\n");
	//	fprintf(fp, "%d,\n", pnum);

	// vertex positions

	for (int i = 0; i < cnt*3; i+=3)
	{
		fprintf(fp, "v %f %f %f\n", vectors[i], vectors[i+1], vectors[i+2]);
	}
	//	fprintf(fp, "}\n ");

	fprintf(fp, "#normal_vectors\n");
	
	//vertex normals

	for (int i = 0; i < cnt*3; i+=3)
	{
		
		fprintf(fp, "vn %f %f %f\n", vetnormal[i], vetnormal[i+1], vetnormal[i+2]);
	}
	//	fprintf(fp, "}\n ");

	fprintf(fp, "#face_indices\n");
	//fprintf(fp, "%d,\n", indicesnum / 3);
	//face indices.
	
	for (int i = 0; i < edgnum*3; i += 3)
	{
		fprintf(fp, "f %d %d %d\n", facesindices[i]+1, facesindices[i + 1]+1, facesindices[i + 2]+1);	//obj indices from 1 !!!
	}//fprintf(fp, "}\n ");

	//	fprintf(fp, "inside_vector <0,0,1> }\n ");
	//	fprintf(fp, "object{ watermesh material{%s_material} }\n ", objectname);

	fclose(fp);
}

void countvectors(const std::string& name, float *vectors, float * vetnormal, int * facesindices, int cnt, int edgnum) {
	ifstream dataFile(name.c_str());
	//	char num[5];
	//sprintf(num, "00037");


	if (!dataFile.good())
		printf("can't open file !");

	while (dataFile.good() && !dataFile.eof()) {
		string id;
		dataFile >> id;
		if (id == "v")
			cnt++;
		else if (id == "f")
			edgnum++;
		getline(dataFile, id);
	}
	dataFile.close();
	vetcnt = cnt; edg = edgnum;
}