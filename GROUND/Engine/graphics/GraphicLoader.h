#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <string>

namespace gr
{
    /**
     * @brief This function load a 3D model.
     * 
     * This only support OBJ file extension, and the model must be exported with Normals and with
     * Triangles
     * 
     * @deprecated See loadAssimp for 3D loading model
     */
    bool loadOBJ(
        const char *path,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals);

    /**
    * @brief load 3d models using assimp
    */
    bool loadAssimp(
        const char* path,
        std::vector<unsigned short>& indices,
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec2>& uvs,
        std::vector<glm::vec3>& normals
    );

    /**
     * @brief This function load a 2D texture
     */
    unsigned int LoadTexture2D(const char* file);
    /**
     * @brief This function load a cube map texture
     * 
     * @param files The files, the correct order of the cubemap is: right, left, top, bottom, front, back
     */
    unsigned int LoadCubeMap(std::vector<std::string> files);


    // static API
    void GetSupportExtensions();
    void WriteAllExtension();
    // public API
    bool CheckExtension(const std::string &extensionName);
}