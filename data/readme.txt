## data文件格式
```bash
data
├── Car24
├── biker
├── readme.txt
└── shaking
```

### 每一类目标中的目录结构
```bash
guiyangs-MBP:Car24 liuguiyang$ tree -L 1 ../Car24/
../Car24/
├── cfg.mat
├── framenum.txt
├── groundtruth_rect.txt
├── init.txt
└── source
```

- framenum.txt : source中文件的数量
- source : 存放的是原始的按照顺序存储的文件格式
- groundtruth_rect.txt : 是对应图片中目标的框的位置信息
- init.txt : 存储的是第一帧图像中目标点的位置