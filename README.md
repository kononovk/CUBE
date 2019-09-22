# Fedora (установка библиотек и запуск программы)
sudo dnf install libGL-devel glew-devel glfw3-devel

cd CUBE

cc -g -o cube -l GLEW -l OpenGL -l glfw -l m main.c

./cube

# CUBE

Задание проекта:

отрисовка с помощью библиотеки GLEW(The OpenGL Extension Wrangler Library), GLFW и языка шейдеров GLSL кубик, состоящий из n * n * n маленьких кубиков, каждый из которых имеет свою текстуру (они, конечно, могут совпадать).

Формат входных данных:
- в файле textures.txt первым на вход подается число n (размер ребра куба в маленьких кубиках)
- далее идут названия текстур каждого кубика по очереди.


# Важно:
- Перед загрузкой текстуры нужно преобразовать в R8G8B8 формат
Преобразовать текстуры можно воспользовавшись сайтом https://convertio.co/ru/jpg-rgb,
который конвертирует любую картинку в *.rgb формат.
- Текстуры должны быть строго квадратными.


Поворот осуществляется с помощью мыши, передвижение с помощью клавиш:
W - вперед
A - влево
S - назад
D - вправо
Esc - выход

Список текстур, предлагаемый по умолчанию:
grass.rgb
PML30.rgb
brick.rgb

Также предлагается посмотреть скриншоты моей работы:

![alt text](https://github.com/kononovk/CUBE_Linux/blob/master/CUBE/screenshot1.JPG)
![alt text](https://github.com/kononovk/CUBE_Linux/blob/master/CUBE/screenshot2.JPG)
