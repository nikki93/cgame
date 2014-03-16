#version 150

in vec3 wmat1; // columns 1, 2, 3 of transform matrix
in vec3 wmat2;
in vec3 wmat3;

in vec2 bbmin;
in vec2 bbmax;
in float selected;

out mat3 wmat;
out vec2 bbmin_;
out vec2 bbmax_;
out float selected_;

void main()
{
    wmat = mat3(wmat1, wmat2, wmat3);
    bbmin_ = bbmin;
    bbmax_ = bbmax;
    selected_ = selected;
}

