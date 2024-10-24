#include "ruby_pch.h"

#include "ObjUtils.h"

#include <fstream>
#include <vector>

namespace Ruby
{

    namespace ObjUtils
    {

        void readMatLib(MatLib& lib, const std::string& path)
        {
            std::ifstream is(path);
            
            RB_ENSURE_RET_VOID(is.is_open(), "Unable to oben obj file at path %s.", path.c_str());

            std::string line;
            std::stringstream ss;
            
            size_t index = -1;
            

            while(std::getline(is, line))
            {
                ss.clear();
                ss.str(line);
                std::string cmd;
                ss >> cmd;
                if(cmd == "newmtl")
                {
                    ss >> cmd;
                    if(!lib.NameToIndex.count(cmd))
                    {
                        index = lib.Materials.size();
                        lib.NameToIndex.emplace(cmd, index);
                        lib.Materials.push_back({});
                    }
                    else
                        index = lib.NameToIndex[cmd];
                    
                }
                else if(cmd == "Ka")
                {
                    RB_ASSERT(index < lib.Materials.size(), "Invalid mtl file.");
                    glm::vec4& amb = lib.Materials[index].Ambient;
                    ss >> amb.x >> amb.y >> amb.z;
                }
                else if(cmd == "Kd")
                {
                    RB_ASSERT(index < lib.Materials.size(), "Invalid mtl file.");
                    glm::vec4& diff = lib.Materials[index].Diffuse;
                    ss >> diff.x >> diff.y >> diff.z;
                }
                else if(cmd == "Ks")
                {
                    RB_ASSERT(index < lib.Materials.size(), "Invalid mtl file.");
                    glm::vec4& spec = lib.Materials[index].Specular;
                    ss >> spec.x >> spec.y >> spec.z;
                }
                else if(cmd == "illum")
                {
                    RB_ASSERT(index < lib.Materials.size(), "Invalid mtl file.");
                    ss >> lib.Materials[index].LightModel;
                }

            }
        }

        ObjFilePayload readObjFile(const std::string& path)
        {
            std::ifstream is(path);
            
            RB_ENSURE_RET(is.is_open(), {}, "Unable to oben obj file.");

            std::string line;
            std::stringstream ss;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uvs;

            size_t currentMaterialIndex = -1;
            
            ObjFilePayload res;

            
            while(std::getline(is, line))
            {
                ss.clear();
                ss.str(line);
                std::string cmd;
                ss >> cmd;
                if(cmd == "v") // Positions
                {
                    glm::vec3 temp;
                    ss >> temp.x >> temp.y >> temp.z;
                    positions.push_back(temp);
                }
                else if(cmd == "vn")
                {
                    glm::vec3 temp;
                    ss >> temp.x >> temp.y >> temp.z;
                    normals.push_back(temp);
                }
                else if(cmd == "vt")
                {
                    glm::vec2 temp;
                    ss >> temp.x >> temp.y;
                    uvs.push_back(temp);
                }
                else if(cmd == "f")
                {
                    size_t count = 0;
                    ObjVertex temp{glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec2{0.0f}, (int)currentMaterialIndex};
                    size_t index;
                    while(ss >> index)
                    {
                        temp.Position = positions[index - 1];
                        ss.ignore(1);
                        if(ss.peek() != '/')
                        {
                            ss >> index;
                            temp.UV = uvs[index - 1];
                        }
                        ss.ignore(1);
                        if(ss.peek() != ' ')
                        {
                            ss >> index;
                            temp.Normal = normals[index - 1];
                        }
                        res.Vertices.push_back(temp);
                        count++;
                    }

                    for(size_t i = 1; i < count - 1; ++i)
                    {
                        uint32_t index = res.Vertices.size() - count + i;
                        res.Indices.push_back(res.Vertices.size() - count);
                        res.Indices.push_back(index);
                        res.Indices.push_back(index + 1);
                    }
                }
                else if(cmd == "mtllib")
                {
                    ss >> cmd;
                    readMatLib(res.Materials, path.substr(0, path.find_last_of('/') + 1) + cmd);
                }
                else if(cmd == "usemtl")
                {
                    ss >> cmd;
                    RB_ASSERT(res.Materials.NameToIndex.count(cmd), "Material name %s doesn't exist.", cmd.c_str());
                    currentMaterialIndex = res.Materials.NameToIndex[cmd];
                }

            }
            

            return res;
        }
    }

}
