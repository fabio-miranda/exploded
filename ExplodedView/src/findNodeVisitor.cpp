#include "findNodeVisitor.h"



    
virtual void apply(osg::Node& node)
{
    if (node.getName()==_name)
    {
        _foundNodes.push_back(&node);
    }
    traverse(node);
}

osg::Node* findNodeVisitor::getFirst()
{
   return *(_foundNodes.begin());
}