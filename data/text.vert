#version 150

in vec2 pos;
in vec2 cell;

out vec2 pos_;
out vec2 cell_;

void main()
{
    pos_ = pos;
    cell_ = cell;
}
