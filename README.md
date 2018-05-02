# liuhan_pipeline

I'm trying to implement own rendering pipeline.
by vs2017.

-----------------------------
1. 图片只支持32位bmp或dib。
2. 如果运行Release或Debug下的输出文件需要把res目录放于同级目录下(exe相于res需要../)。
3. x64没设置环境变量等，暂时只在x86下编译。

------------------------------
Pipeline_Process:
1. model = WVP*V;
2. backface_culling();
3. normalization(){postion/w, uv/w, color/w}
4. clip(){split_triangle()}
5. toscreen();
6. scanline(){postion, uv*w, color*w}


![image](https://github.com/xx8086/liuhan_pipeline/blob/master/lhpiple_static_uml.png)
![image](https://github.com/xx8086/liuhan_pipeline/blob/master/run_1.png)
![image](https://github.com/xx8086/liuhan_pipeline/blob/master/run_2.png)
![image](https://github.com/xx8086/liuhan_pipeline/blob/master/run_3.png)