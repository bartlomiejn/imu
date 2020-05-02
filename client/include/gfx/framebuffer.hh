#ifndef IMU_FRAMEBUFFER_H
#define IMU_FRAMEBUFFER_H

#include <gfx/texture.hh>

class Framebuffer
{
public:
	Framebuffer(Texture &depth_texture);
	~Framebuffer();
	void try_load();
	void use();
	void set_draw(bool is_enabled);
	void set_read(bool is_enabled);
	const Texture& get_depth_texture();
private:
	Texture &depth_texture;
	unsigned int id;
};

class FramebufferGenerationFailure {};

#endif //IMU_FRAMEBUFFER_H
