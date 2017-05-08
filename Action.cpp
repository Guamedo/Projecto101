#include "Action.h"


Action::Action()
{
}

Action::Action(std::string nombreAnimacion, char* sourceAnimacion) {
	nombre = nombreAnimacion;
	animacion = Animacion(sourceAnimacion);
}

Action::~Action()
{
}
