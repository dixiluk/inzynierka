uniform mat4 MVP;   //Macierz modelu * macierz widoku * macierz projekcji 

void main()
{
	//gl_TexCoord[0].st = inTexcoord;  //Wysyłanie koordynantów tekstury do programu fragmentu (.ps)
	gl_Position = MVP * gl_Vertex;  //Mnożenie pozycji wierzchołków przez macież MVP  (Ustawianie ich na ekranie)
}
