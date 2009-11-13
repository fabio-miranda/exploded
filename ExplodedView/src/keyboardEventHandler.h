#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include "ExplodedView.h"

#include <osgGA/GUIEventHandler>

class ExplodedView;

class myKeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
	myKeyboardEventHandler(ExplodedView* explodedView)
	{
	  m_explodedView = explodedView;
	}


	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&);
	virtual void accept(osgGA::GUIEventHandlerVisitor& v)   { v.visit(*this); };

protected:
       ExplodedView* m_explodedView;
};


#endif