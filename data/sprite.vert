#version 150

in vec3 transform1;
in vec3 transform2;
in vec3 transform3;
in vec2 cell;
in vec2 size;

out mat3 transform;
out vec2 cell_;
out vec2 size_;

void main()
{
    transform = mat3(transform1, transform2, transform3);
    cell_ = cell;
    size_ = size;
}

