# AssimpSceneImportExPlugin
基于Qt AssimpSceneImportPlugin插件，进行改进，支持glb格式

编译过程  
1、git clone --recursive https://github.com/1995zyl/AssimpSceneImportExPlugin.git  
2、mkdir build & cd build  
不要指定CMAKE_INSTALL_PREFIX  
4、cmake .. -DCMAKE_BUILD_TYPE=Release -DQT_SDK_DIR=D:/Qt/6.5.2/msvc2019_64  
将插件安装到qt目录中  
5、make -j4 & make install
