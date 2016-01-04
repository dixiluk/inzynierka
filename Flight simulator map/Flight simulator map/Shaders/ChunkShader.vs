
attribute vec3 inPosition;
attribute vec2 inTexcoord;

uniform mat4 MVP;   //Macierz modelu * macierz widoku * macierz projekcji 

void main()
{
	gl_TexCoord[0].st=inTexcoord;  //Wysyłanie koordynantów tekstury do programu fragmentu (.ps)
	gl_Position = MVP * vec4(inPosition,1.0);  //Mnożenie pozycji wierzchołków przez macież MVP  (Ustawianie ich na ekranie)
}
