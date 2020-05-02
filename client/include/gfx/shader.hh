#ifndef IMU_SHADER_HH
#define IMU_SHADER_HH

#include <glm/glm.hpp>

/// Class representing an OpenGL program which combines a vertex and fragment
/// shader.
class Shader
{
public:
	unsigned int id;

	/// Initializes a Shader instance.
	Shader(const char* vert_filename, const char* frag_filename);
	
	/// Destroys a Shader instance.
	virtual ~Shader() = default;
	
	/// Tries to create and link vertex and fragment programs. Can throw an
	/// error.
	void try_create_and_link(void);
	
	/// Sets the `name` uniform to provided integer value.
	void set_uniform(const char *name, int value);
	
	/// Sets the `name` uniform to provided float value.
	void set_uniform(const char *name, float value);
	
	/// Sets the `name` uniform to provided glm::mat4 value.
	void set_uniform(const char *name, glm::mat4 matrix);
	
	/// Sets the `name` uniform to provided glm::vec3 value.
	void set_uniform(const char *name, glm::vec3 vec);
	
	/// Uses the shader program
	void use(void) const;
	
	/// Retrieves the location for the specified uniform.
	/// \param uniform
	/// \return
	unsigned int get_uniform_location(const char *uniform) const;

private:
	const char *vert_filename; ///< Vertex shader filename.
	const char *frag_filename; ///< Fragment shader filename.
};

class ShaderCompileFailure {};
class ShaderLinkFailure {};

#endif //IMU_SHADER_HH
