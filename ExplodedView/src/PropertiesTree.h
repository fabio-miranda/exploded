#ifndef PropertiesTree_H
#define PropertiesTree_H


#include <boost/intrusive/sg_set.hpp>

class PropertiesTree : public bs_set_base_hook<>
{
   double mProperty;

   public:

	   boost::intrusive::bs_set_member_hook<> member_hook_;

	   PropertiesTree(double prop)
		  :  mProperty(prop)
		  {}
	   friend bool operator< (const PropertiesTree &a, const PropertiesTree &b)
		  {  return a.mProperty < b.mProperty;  }
	   friend bool operator> (const PropertiesTree &a, const PropertiesTree &b)
		  {  return a.mProperty > b.mProperty;  }
	   friend bool operator== (const PropertiesTree &a, const PropertiesTree &b)
		  {  return a.mProperty < b.mProperty;  }
};

#endif
