#include "renderer/uniforms.hpp"

std::size_t Uniforms::_ShaderUniformPairHash::operator()(const RegularPair& key) const noexcept
{
    return _hashPair(key.first, key.second);
}

std::size_t Uniforms::_ShaderUniformPairHash::operator()(const ViewPair& key) const noexcept
{
    return _hashPair(key.first, key.second);
}

bool Uniforms::_ShaderUniformPairEquals::operator()(const RegularPair& pair1, const RegularPair& pair2) const noexcept
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

bool Uniforms::_ShaderUniformPairEquals::operator()(const ViewPair& pair1, const ViewPair& pair2) const noexcept
{
    return (pair1.first == pair2.first) && (pair1.second == pair2.second);
}

void Uniforms::addUniform(GLuint shaderId, std::string_view name)
{
    if(_locations.contains(std::make_pair(shaderId, name)))
    {
        return;
    }

    _locations.try_emplace(std::make_pair(shaderId, std::string {name}), glGetUniformLocation(shaderId, name.data()));
}

void Uniforms::setUniform(GLuint shaderId, std::string_view name, const glm::mat4& mat4)
{
    auto it {_locations.find(std::make_pair(shaderId, name))};

    if(it == _locations.end())
    {
        throw std::out_of_range {"This shader-id pair does not exist."};
    }

    glUniformMatrix4fv(it->second, 1, GL_FALSE, glm::value_ptr(mat4));
}