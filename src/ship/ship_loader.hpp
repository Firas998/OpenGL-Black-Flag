
#include <cgp/cgp.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class ShipLoader {
public:
	ShipLoader();
	~ShipLoader();
	void loadObj();
	std::vector<int> textures;
	std::vector<cgp::mesh_drawable> meshes;
private:
	std::vector<cgp::mesh> rawMeshes;
	void processNode(aiNode* node, const aiScene* scene);
	void convertMesh(aiMesh* m, const aiScene* scene);
	void processMaterials(const aiScene* scene);
	void drawMeshes(const aiScene* scene);
};