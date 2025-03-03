#ifndef UNIFORMS_HPP
#define UNIFORMS_HPP

#include <glad/glad.h>
#include <glm/glm_custom.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <stdexcept>

class Uniforms
{
private :
    using RegularPair = std::pair<GLuint, std::string>;
    using ViewPair = std::pair<GLuint, std::string_view>;

    class _ShaderUniformPairHash
    {
    private :
        template <typename T1, typename T2>
        std::size_t _hashPair(const T1& key1, const T2& key2) const noexcept
        {
            std::hash<T1> hash1;
            std::hash<T2> hash2;

            std::size_t seed = hash1(key1);
            seed ^= hash2(key2) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }

    public :
        // enable heterogenous lookup
        using is_transparent = void;

        std::size_t operator()(const RegularPair& key) const noexcept;
        std::size_t operator()(const ViewPair& key) const noexcept;
    };

    struct _ShaderUniformPairEquals
    {
        // enable heterogenous lookup
        using is_transparent = void;

        bool operator()(const RegularPair& pair1, const RegularPair& pair2) const noexcept;
        bool operator()(const ViewPair& pair1, const ViewPair& pair2) const noexcept;
    };

    std::unordered_map<std::pair<GLuint, std::string>, GLint, _ShaderUniformPairHash, _ShaderUniformPairEquals> 
    _locations {};

public :
    Uniforms() = default;

    void addUniform(GLuint shaderId, std::string_view name);
    void setUniform(GLuint shaderId, std::string_view name, const glm::mat4& mat4);
};

#endif