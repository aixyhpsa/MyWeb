#include <upload.h>

int main()
{
    dya::Upload up("/home/wucz/test/MyWeb/resource/upload");
    up.write("hbtx.txt", "1234567\n1234566\n");
}