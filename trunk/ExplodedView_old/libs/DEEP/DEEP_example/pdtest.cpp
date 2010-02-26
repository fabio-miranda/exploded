/*************************************************************************\

  Copyright 2002 The University of North Carolina at Chapel Hill.
  All Rights Reserved.

  Permission to use, copy, modify OR distribute this software and its
  documentation for educational, research and non-profit purposes, without
  fee, and without a written agreement is hereby granted, provided that the
  above copyright notice and the following three paragraphs appear in all
  copies.

  IN NO EVENT SHALL THE UNIVERSITY OF NORTH CAROLINA AT CHAPEL HILL BE
  LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR
  CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING OUT OF THE
  USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY
  OF NORTH CAROLINA HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH
  DAMAGES.

  THE UNIVERSITY OF NORTH CAROLINA SPECIFICALLY DISCLAIM ANY
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
  PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
  NORTH CAROLINA HAS NO OBLIGATIONS TO PROVIDE MAINTENANCE, SUPPORT,
  UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

  The authors may be contacted via:

  US Mail:             Y. Kim, M. Lin
                       Department of Computer Science
                       Sitterson Hall, CB #3175
                       University of N. Carolina
                       Chapel Hill, NC 27599-3175

  Phone:               (919) 962-1749

  EMail:               geom@cs.unc.edu
                       youngkim@cs.unc.edu
					   lin@cs.unc.edu

\**************************************************************************/

// This example demonstrates the usage of DEEP.
//
// Two convex polytopes, sphere100_xz.tri and sphere100.tri are given as an input.
// Then, DEEP computes the penetration depth between the two polytopes as
// sphere100_xz is both rotating around its center and revolving around sphere100.

#include <iostream.h>
#include <SWIFT.h>
#include <SWIFT_mesh.h>

const char* file1 = "sphere100_xz.tri";
const char* file2 = "sphere100.tri";
int  obj_id1;
int  obj_id2;

SWIFT_Real trans[] = {0, 0.5, 0};

void
main(int argc, char **argv)
{
	SWIFT_Scene* scene = new SWIFT_Scene(true, false);

	if (!scene->Add_General_Object(file1, obj_id1, false) ||
		!scene->Add_General_Object(file2, obj_id2, false) ) {
		cerr << "scene creation failed" << endl;
		exit(-1);
	}

	int   num_pairs;
	int* oids;
	int* num_contacts; 
	SWIFT_Real* distances; 
	SWIFT_Real* nearest_pts; 
	SWIFT_Real* normals; 
	int* feature_types; 
	int* feature_ids; 
	bool result;

	const SWIFT_Real inc = 0.5;
	for (SWIFT_Real angle=0.0; angle < 360.0; angle += inc) {
	
		SWIFT_Real rot_angle = angle*PI/180.0;
		SWIFT_Quaternion quat(SWIFT_Triple(0,0,1), rot_angle);
		SWIFT_Quaternion quat2(SWIFT_Triple(0,0,1), rot_angle*2);
		SWIFT_Triple quat_triple = quat.Rotate(trans);
		
		scene->Set_Object_Transformation(obj_id1, quat2.To_Matrix().Value(), quat_triple.Value());
		
		result = scene->Query_Contact_Determination( false, 2.0, num_pairs,
						     &oids, &num_contacts, &distances, &nearest_pts,
						     &normals, &feature_types,  &feature_ids );

		cout << "PD Dist = " << distances[0] << " PD Dir = ("
			<< normals[0] << ", " << normals[1] << ", " << normals[2] << ")" << endl;

	}

	delete scene;

}



