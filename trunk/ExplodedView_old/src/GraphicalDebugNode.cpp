class GraphicalDebugNode
   {
    
   public:
     
      GraphicalDebugNode();
   
      void RemoveDrawable(osg::Drawable* d) 
      {
         mDebugGeode->removeDrawable(d);
      }
      
      osg::Drawable* DrawLine(osg::Vec3 start, osg::Vec3 end, osg::Vec4 color) const;

      void EnableDebugGeometry(bool enable);
      bool IsDebugGeometryEnabled() const { return mRenderDebugGeometry; }
   
      osg::Geode* GetDebugGeode() const { return mDebugGeode.get(); }

   private:
      bool mRenderDebugGeometry;

      /** OSG node that will contain debug geometry */
      dtCore::RefPtr<osg::Geode> mDebugGeode;     
   };



GraphicalDebugNode::GraphicalDebugNode()
: mRenderDebugGeometry(false)
, mDebugGeode(new osg::Geode())
{
   mDebugGeode->setName("Debug");
   osg::StateSet *ss = mDebugGeode->getOrCreateStateSet();
   osg::LineWidth* linewidth = new osg::LineWidth(); 
   linewidth->setWidth(1); 
   ss->setAttributeAndModes(linewidth,osg::StateAttribute::ON); 
   ss->setMode(GL_LIGHTING,osg::StateAttribute::OFF); 
}

osg::Drawable* GraphicalDebugNode::DrawLine(osg::Vec3 start, osg::Vec3 end, osg::Vec4 color) const {

   if(!mRenderDebugGeometry) {
      LOG_ERROR("Debug drawing not enabled!\n");
      return NULL;
   }
   osg::Geometry* lineGeometry = new osg::Geometry();

   osg::Vec3Array* vArray = new osg::Vec3Array(2);
   osg::Vec4Array* colors = new osg::Vec4Array(1);

   (*vArray)[0] = start;
   (*vArray)[1] = end;
   (*colors)[0] = color;

   lineGeometry->setVertexArray(vArray);
   lineGeometry->setColorArray(colors);
   lineGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
   osg::DrawArrays* drawable = new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2);
   lineGeometry->addPrimitiveSet(drawable);
   mDebugGeode->addDrawable(lineGeometry);
   return lineGeometry;
}


void GraphicalDebugNode::EnableDebugGeometry(bool enable)
{
   mRenderDebugGeometry = enable;
  
   if(!enable)
   {
      mDebugGeode->removeDrawables(0, mDebugGeode->getNumDrawables());
      return;
   }
}