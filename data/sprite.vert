#version 150

in vec3 wmat1; // columns 1, 2, 3 of transform matrix
in vec3 wmat2;
in vec3 wmat3;
in vec2 size;
in vec2 texcell;
in vec2 texsize;

out mat3 wmat;
out vec2 size_;
out vec2 texcell_;
out vec2 texsize_;

void main()
{
    wmat = mat3(wmat1, wmat2, wmat3);
    size_ = size;
    texcell_ = texcell;
    texsize_ = texsize;
}

