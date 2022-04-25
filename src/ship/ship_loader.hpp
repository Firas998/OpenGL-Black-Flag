
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
	void processNode(aiNode* node, const aiScene* scene);
	void convertMesh(aiMesh* m, const aiScene* scene, cgp::mesh_drawable& out);
	void processMaterials(const aiScene* scene);
};