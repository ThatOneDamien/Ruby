#include "ruby_pch.h"

#include "ObjUtils.h"

#include <fstream>
#include <vector>

namespace Ruby
{

    namespace ObjUtils
    {

        ObjFilePayload readObjFile(const std::string& path)
        {
            std::ifstream is(path);
            
            RB_ENSURE_RET(is.is_open(), {}, "Unable to oben obj file.");

            std::string line;
            std::stringstream ss;
            std::vector<glm::vec3> positions;
            std::vector<glm::vec3> normals;
            std::vector<glm::vec2> uvs;
            
            std::vector<ObjVertex> vertices;
            std::vector<uint32_t> indices;
            
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
                    ObjVertex temp{glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec2{0.0f}};
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
                        vertices.push_back(temp);
                        count++;
                    }

                    for(size_t i = 1; i < count - 1; ++i)
                    {
                        uint32_t index = vertices.size() - count + i;
                        indices.push_back(vertices.size() - count);
                        indices.push_back(index);
                        indices.push_back(index + 1);
                    }
                }

            }
            
            ObjFilePayload res;
            res.Vertices = std::move(vertices);
            res.Indices = std::move(indices); 

            return res;
        }
    }

}
