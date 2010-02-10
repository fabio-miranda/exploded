#include "keyboardEventHandler.h"

bool myKeyboardEventHandler::handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
{
	switch(ea.getEventType())
	{
		case(osgGA::GUIEventAdapter::KEYDOWN):
		{
			switch(ea.getKey())
			{
			
			case '0': m_explodedView->switchExplodeInplode(0); break;
			case '1': m_explodedView->switchExplodeInplode(1); break;
			case '2': m_explodedView->switchExplodeInplode(2); break;
			case '3': m_explodedView->switchExplodeInplode(3); break;
			case '4': m_explodedView->switchExplodeInplode(4); break;
			case '5': m_explodedView->switchExplodeInplode(5); break;
			case '6': m_explodedView->switchExplodeInplode(6); break;
			case '7': m_explodedView->switchExplodeInplode(7); break;
			case '8': m_explodedView->switchExplodeInplode(8); break;
			case '9': m_explodedView->switchExplodeInplode(9); break;
			
			default:
				return false;
			} 
		}
	default:
		return false;
	}
}