
#include <osg/NodeVisitor>
#include <osg/Node>
#include <osg/Material>


class ColorVisitor : public osg::NodeVisitor
{
public:

   ColorVisitor (const osg::Vec4& pColor): osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), mColor(pColor)
   {

   }

   virtual void apply(osg::Geode& geode)
   {
      osg::StateSet* ss = geode.getOrCreateStateSet();

      unsigned pNumDrawables = geode.getNumDrawables();
      for(unsigned i = 0; i < pNumDrawables; ++i)
      {
         osg::Drawable* draw = geode.getDrawable(i);
		 //osg::Material* mat = new osg::Material();
		 osg::Material* mat = dynamic_cast<osg::Material*>(draw->getOrCreateStateSet()->getAttribute(osg::StateAttribute::MATERIAL));
         if(mat)
         {
            mat->setDiffuse(osg::Material::FRONT_AND_BACK, mColor);
            mat->setEmission(osg::Material::FRONT_AND_BACK, mColor);
         }
		 //draw->getOrCreateStateSet()->setAttributeAndModes(osg::StateAttribute::MATERIAL, mat);
      }
   }


private:

   osg::Vec4 mColor;
};