#include "ObjectLoader.h"
#include "Graphics\Scene\SceneGraph.h"
#include "Graphics\VertexFormat.h"

void ObjectLoader::Clear() {
	m_objects.clear();
	m_materials.clear();
	m_model_name.clear();
}

void ObjectLoader::UpdateMinMaxXYZ(Vector3& minXYZ, Vector3& maxXYZ, const Vector3& xyz) {
	if (xyz.GetX() < minXYZ.GetX()) {
		minXYZ.SetX(xyz.GetX());
	} else if (xyz.GetX() > maxXYZ.GetX()) {
		maxXYZ.SetX(xyz.GetX());
	}

	if (xyz.GetY() < minXYZ.GetY()) {
		minXYZ.SetY(xyz.GetY());
	} else if (xyz.GetY() > maxXYZ.GetY()) {
		maxXYZ.SetY(xyz.GetY());
	}

	if (xyz.GetZ() < minXYZ.GetZ()) {
		minXYZ.SetZ(xyz.GetZ());
	}
	else if (xyz.GetZ() > maxXYZ.GetZ()) {
		maxXYZ.SetZ(xyz.GetZ());
	}
}

bool ObjectLoader::LoadMTL(const char* _filename_) {
	std::string placeholder = (std::string(MODEL_PATH) + m_model_name + std::string("/") + std::string(_filename_));
	const char* filename = placeholder.c_str();
	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		printf("Error in open the file !\n");
		return false;
	}

	char lineHeader[128], mtl_name[128], temps[128];
	float tempf[3];

	while (true) {
		if (strcmp(lineHeader, "newmtl") == 0) {
			fscanf(file, "%s\n", mtl_name);

			MTL mtl;
			mtl.material_name = mtl_name;

			while (fscanf(file, "%s", lineHeader) != EOF && strcmp(lineHeader, "newmtl") != 0) {
				if (strcmp(lineHeader, "Ns") == 0) {
					fscanf(file, "%f\n", &mtl.NS);
				}
				else if (strcmp(lineHeader, "Ka") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KA = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Kd") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KD = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ks") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KS = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ke") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					mtl.KE = Vector3(tempf[0], tempf[1], tempf[2]);
				}
				else if (strcmp(lineHeader, "Ni") == 0) {
					fscanf(file, "%f\n", &mtl.NI);
				}
				else if (strcmp(lineHeader, "d") == 0) {
					fscanf(file, "%f\n", &mtl.D);
				}
				else if (strcmp(lineHeader, "illum") == 0) {
					fscanf(file, "%f\n", &mtl.illum);
				}
				else if (strcmp(lineHeader, "map_Kd") == 0) {
					fscanf(file, "%s", temps);
					int found = std::string(temps).find_last_of("/\\");
					if (found == -1) {
						mtl.texture_filename = temps;
					}
					else {
						mtl.texture_filename = std::string(temps).substr(found + 1);
					}
				}
			}

			m_materials.push_back(mtl);
		}
		else if (fscanf(file, "%s", lineHeader) == EOF) {
			break;
		}
	}

	return m_materials.size() > 0;
}

bool ObjectLoader::LoadWaveFrontObject(const char* model_name, const float scale) {
	m_model_name = model_name;
	std::string placeholder = std::string(MODEL_PATH) + std::string(m_model_name) + std::string("/") + std::string(m_model_name) + std::string(".obj");
	const char* filename = placeholder.c_str();
	FILE * file = fopen(filename, "r");

	if (file == NULL) {
		printf("Error in open the file !\n");
		return false;
	}

	std::vector<Vertex1P> tempVertices1P;
	std::vector<vertex1P1UV> tempVertices1P1UV;
	std::vector<Vector3> tempVertices;
	std::vector<Vector3> tempNormals;
	std::vector<std::vector<float>> tempUVs;

	char lineHeader[128], temps[128];
	float tempf[3];
	unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	unsigned int vertex_offset = 0; 
	unsigned int uv_offset = 0; 
	unsigned int normal_offset = 0;
	unsigned int vertexCount = 0;

	while (true) {
		if (strcmp(lineHeader, "mtllib") == 0) {
			fscanf(file, "%s\n", temps);
			if (LoadMTL(temps) == false) {
				break;
			}
			lineHeader[0] = '0';
		}
		else if (strcmp(lineHeader, "o") == 0) {
			fscanf(file, "%s\n", temps);
			Object object;
			object.setObjectName(temps);
			Vector3 minXYZ = Vector3(0.0f, 0.0f, 0.0f);
			Vector3 maxXYZ = Vector3(0.0f, 0.0f, 0.0f);

			while (fscanf(file, "%s", lineHeader) != EOF && strcmp(lineHeader, "o") != 0 ) {
				if (strcmp(lineHeader, "v") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					tempVertices.push_back(Vector3(tempf[0] * scale, tempf[1] * scale, tempf[2] * scale));
					UpdateMinMaxXYZ(minXYZ, maxXYZ, tempVertices.back());
				}
				else if (strcmp(lineHeader, "vt") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					std::vector<float> temp;
					temp.push_back(tempf[0]);
					temp.push_back(1.0f - tempf[1]);	//Transform the origin from bottom left to top left
					tempUVs.push_back(temp);
					object.setHasUV(true);
				}
				else if (strcmp(lineHeader, "vn") == 0) {
					fscanf(file, "%f %f %f\n", &tempf[0], &tempf[1], &tempf[2]);
					tempNormals.push_back(Vector3(tempf[0], tempf[1], tempf[2]));
				}
				else if (strcmp(lineHeader, "usemtl") == 0) {
					fscanf(file, "%s\n", temps);
					object.setMaterialName(temps);
				}
				else if (strcmp(lineHeader, "f") == 0) {
					bool is_pattern_match = false;
					if (object.hasUV() > 0) {
						is_pattern_match = (fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]) == 9);
					} else {
						is_pattern_match = (fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]) == 6);
					}

					if (!is_pattern_match) {
						printf("File can't be read by parser");
						return false;
					}

					//C++ indexing starts at 0 and OBJ indexing starts at 1
					
					for (int i = 0; i < 3; i++) {
						if (object.hasUV()) {
							float uv[2] = {
								tempUVs.at(uvIndex[i] - uv_offset - 1).at(0),
								tempUVs.at(uvIndex[i] - uv_offset - 1).at(1)
							};
							tempVertices1P1UV.push_back(vertex1P1UV{
								tempVertices.at(vertexIndex[i] - vertex_offset - 1),
								uv
							});
						}
						else {
							tempVertices1P.push_back(Vertex1P {
								tempVertices.at(vertexIndex[i] - vertex_offset - 1)
							});
						}

						object.addVertexIndex();
					}					
				}
			}
			
			if (object.hasUV()) {
				object.setVertices(tempVertices1P1UV);
			} else {
				object.setVertices(tempVertices1P);
			}

			object.setObjectDimension(Vector3(
				maxXYZ.GetX() - minXYZ.GetX(),
				maxXYZ.GetY() - minXYZ.GetY(),
				maxXYZ.GetZ() - minXYZ.GetZ()
			));

			vertex_offset += tempVertices.size();
			normal_offset += tempNormals.size();
			uv_offset += tempUVs.size();
			m_objects.push_back(object);
			
			//OutputDebugString((std::string(object.getObjectName()) + "\n").c_str());

			tempVertices1P1UV.clear();
			tempVertices1P.clear();
			tempVertices.clear();
			tempNormals.clear();
			tempUVs.clear();
			
		} else if (fscanf(file, "%s", lineHeader) == EOF) {
			break;
		}
	}

	return true;
}

std::vector<MeshComponent*> ObjectLoader::Draw() {

	std::vector<MeshComponent*> meshInstanceList;
	MeshComponent* meshComponent = NULL;
	std::string texture_path;

	for (Object object : m_objects) {
		for (MTL mtl : m_materials) {
			if (object.getMaterialName() == mtl.material_name) {
				texture_path = std::string(MODEL_PATH) + m_model_name + std::string("/Texture/") + std::string(mtl.texture_filename);

				meshComponent = new MeshComponent(
					object.getVertices(),
					object.getNumVertices(),
					object.getVertexIndices()->data(),
					object.getVertexIndices()->size(),
					object.getObjectDimension(),
					object.hasUV() ? eMeshType::V1P1UV : eMeshType::V1P,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					object.hasUV() ? texture_path.c_str() : NULL
				);

				meshInstanceList.push_back(meshComponent);
				SceneGraph::GetInstance()->AddComponent(meshComponent);
			}
		}
	}

	return meshInstanceList;
}