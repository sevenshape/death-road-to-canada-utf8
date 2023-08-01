# death-road-to-canada-utf8
让drtc支持显示utf8字符的插件  

## TODO
1. 目前文本的显示逻辑和原生不一样导致了utf8字符和原字符显示不一致, 所以需要修改sprite_batch_plot函数并支持sdl2_ttf
2. 跨平台支持  
**因为游戏中对纹理的处理使用的函数不是标准opengl函数加上我对opengl不熟, 我目前卡在了第一步, 如果你有什么头绪欢迎通过电子邮箱联系我**

## INSTALL
参考Releases  
效果:
![U4OTYSE)8FGR`D9 SLRM70L](https://github.com/sevenshape/death-road-to-canada-utf8/assets/47808940/24042aa8-2aa7-4e03-8d97-b41933dc979c)

## BUILD
基于msys2和cmake构建  
msys2官网: https://www.msys2.org  
cmake官网: https://cmake.org

### BUILD in CLion 2023
1. 安装msys2后运行msys2 mingw32安装SDL2, SDL2_ttf, Gettext, libiconv   
```pacman -S mingw-w64-i686-SDL2 mingw-w64-i686-SDL2_ttf mingw-w64-i686-gettext mingw-w64-i686-libiconv```
2. 配置Clion 文件/设置/构建、执行、部署/CMake, 将工具链指向MSYS2 MinGW32, 生成器改为Ninja  
3. 修改CMakeLists.txt中的D:/program/Steam/steamapps/common/DeathRoadToCanada为本机drtc目录  
4. 构建SDL2_mixer

## Contact
如果有任何问题以及建议，通过电子邮件联系我：[sevenshape@foxmail.com](mailto:sevenshape@foxmail.com)
