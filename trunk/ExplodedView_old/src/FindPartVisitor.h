#ifndef FIND_NODE_VISITOR_H
#define FIND_NODE_VISITOR_H

#include "Part.h"

#include <osg/NodeVisitor>
#include <osg/Node>

#include <osgSim/DOFTransform>
#include <iostream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

class FindNamedPartVisitor : public osg::NodeVisitor
{
public:
    FindNamedPartVisitor(const std::string& name):
        osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN),
        _name(name) {}
    
	virtual void apply(osg::Node& node)
    {
		if (node.getName().find(_name) != string::npos)
        {
			Part* part = new Part();
			part->m_osgNode = &node;
            _foundParts.push_back(part);
        }
        traverse(node);
    }



    //TODO:
    //typedef std::vector< osg::ref_ptr< Part > > PartList;
	typedef std::vector< Part* > PartList;

    std::string _name;
    PartList _foundParts;

	Part* FindNamedPartVisitor::getFirst()
	{
	   return *(_foundParts.begin());
	}

	PartList& getPartList() { return _foundParts; }
};

#endif

