# C3-对正方体加载纹理

### 一、实验效果

![result](Report\result.png)

![result](Report\result2.png)



### 二、实验环境

Windows 10， OpenGL 3.3

依赖库：GLFW 3.3.2，Glad， GLM 0.9.9.7



### 三、实验步骤

#### 3.1 启动程序

直接执行``Output``文件夹中的C3_cubic.exe**（确保glsl文件在``Code``文件夹中，纹理图片文件在``Code/texture``文件夹中，exe文件在``Output``文件夹中）**

#### 3.2 交互方式

WASD键控制摄像机的移动，鼠标控制视角的移动。

F1、F2、F3按键控制纹理加载方式：

1. F1为线性采样；
2. F2为Mipmap采样；
3. F3为最近点采样。



### 四、实验原理

#### 4.1 立方体

设置立方体6面的顶点的空间坐标和纹理坐标（其中，每个面由2个三角形面构成，共36个点）。

在绘制某个面的时候，激活该面的纹理，实现各个面的纹理不同。

```c++
for (int i = 0; i < 6; i++) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMaps[i]);

    glBindVertexArray(VAOs[i]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
```

#### 4.2 加载纹理

使用stb_image库将纹理图片加载到程序中。

之后根据采样方式设置纹理过滤参数。

```c++
// param为GL_LINEAR、GL_LINEAR_MIPMAP_LINEAR或GL_NEAREST
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
```

#### 4.3 交互

利用OpenGL函数对键盘按键的监听，在每一次渲染时处理按下的按键，从而实现摄像机上下左右移动；利用鼠标的回调机制，实现视角的上下左右移动。

如果按下的是F1、F2、F3按键，程序会根据对应的采样方式重新加载各个面的纹理，实现纹理过滤方式的切换。

```c++
if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
    cout << "TEXTURE::LINEAR" << endl;
    for (int i = 0; i < 6; i++) 
        diffuseMaps[i] = loadTexture(paths[i], GL_LINEAR);
}
if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
    cout << "TEXTURE::MIPMAP" << endl;
    for (int i = 0; i < 6; i++)
        diffuseMaps[i] = loadTexture(paths[i], GL_LINEAR_MIPMAP_LINEAR);
}
if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
    cout << "TEXTURE::NEAREST" << endl;
    for (int i = 0; i < 6; i++) 
        diffuseMaps[i] = loadTexture(paths[i], GL_NEAREST);
}
```

#### 