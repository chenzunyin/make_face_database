# 本仓库用于实现构建模块内置人脸库功能

1. 目录说明

	database 用于存放生成的人脸数据库, 模块使用的数据库默认名称为tiny_face.db

	func 用于存放构建数据库代码

	images 用于存放注册用人脸图片

	inc 头文件

	lib 库文件

	lib-source 模块数据库构建API源文件, 编译成动态库使用

	output 编译后可执行文件存放那个目录

2. 功能说明

	add_single 每次添加一张人脸图片到人脸数据库

	cycle_add_person 遍历添加制定目录下所有人脸图片
