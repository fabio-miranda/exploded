char volume_iso_frag[] = "uniform sampler3D baseTexture;\n"
                         "uniform float SampleDensityValue;\n"
                         "uniform float TransparencyValue;\n"
                         "uniform float IsoSurfaceValue;\n"
                         "\n"
                         "varying vec4 cameraPos;\n"
                         "varying vec4 vertexPos;\n"
                         "varying mat4 texgen;\n"
                         "\n"
                         "void main(void)\n"
                         "{ \n"
                         "\n"
                         "    vec3 t0 = (texgen * vertexPos).xyz;\n"
                         "    vec3 te = (texgen * cameraPos).xyz;\n"
                         "\n"
                         "    vec3 eyeDirection = normalize(te-t0);\n"
                         "\n"
                         "    if (te.x>=0.0 && te.x<=1.0 &&\n"
                         "        te.y>=0.0 && te.y<=1.0 &&\n"
                         "        te.z>=0.0 && te.z<=1.0)\n"
                         "    {\n"
                         "        // do nothing... te inside volume\n"
                         "    }\n"
                         "    else\n"
                         "    {\n"
                         "        if (te.x<0.0)\n"
                         "        {\n"
                         "            float r = -te.x / (t0.x-te.x);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.x>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.x) / (t0.x-te.x);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.y<0.0)\n"
                         "        {\n"
                         "            float r = -te.y / (t0.y-te.y);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.y>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.y) / (t0.y-te.y);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.z<0.0)\n"
                         "        {\n"
                         "            float r = -te.z / (t0.z-te.z);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "\n"
                         "        if (te.z>1.0)\n"
                         "        {\n"
                         "            float r = (1.0-te.z) / (t0.z-te.z);\n"
                         "            te = te + (t0-te)*r;\n"
                         "        }\n"
                         "    }\n"
                         "\n"
                         "    const float max_iteratrions = 2048.0;\n"
                         "    float num_iterations = ceil(length(te-t0)/SampleDensityValue);\n"
                         "    if (num_iterations<2.0) num_iterations = 2.0;\n"
                         "\n"
                         "    if (num_iterations>max_iteratrions) \n"
                         "    {\n"
                         "        num_iterations = max_iteratrions;\n"
                         "    }\n"
                         "\n"
                         "    vec3 deltaTexCoord=(t0-te)/float(num_iterations-1.0);\n"
                         "    vec3 texcoord = te;\n"
                         "\n"
                         "    vec4 previousColor = texture3D( baseTexture, texcoord);\n"
                         "    \n"
                         "    float normalSampleDistance = 1.0/512.0;\n"
                         "    vec3 deltaX = vec3(normalSampleDistance, 0.0, 0.0);\n"
                         "    vec3 deltaY = vec3(0.0, normalSampleDistance, 0.0);\n"
                         "    vec3 deltaZ = vec3(0.0, 0.0, normalSampleDistance);\n"
                         "    \n"
                         "    while(num_iterations>0.0)\n"
                         "    {\n"
                         "        vec4 color = texture3D( baseTexture, texcoord);\n"
                         "\n"
                         "        float m = (previousColor.a-IsoSurfaceValue) * (color.a-IsoSurfaceValue);\n"
                         "        if (m <= 0.0)\n"
                         "        {\n"
                         "            float r = (IsoSurfaceValue-color.a)/(previousColor.a-color.a);\n"
                         "            texcoord = texcoord - r*deltaTexCoord;\n"
                         "            \n"
                         "            float a = color.a;\n"
                         "            float px = texture3D( baseTexture, texcoord + deltaX).a;\n"
                         "            float py = texture3D( baseTexture, texcoord + deltaY).a;\n"
                         "            float pz = texture3D( baseTexture, texcoord + deltaZ).a;\n"
                         "\n"
                         "            float nx = texture3D( baseTexture, texcoord - deltaX).a;\n"
                         "            float ny = texture3D( baseTexture, texcoord - deltaY).a;\n"
                         "            float nz = texture3D( baseTexture, texcoord - deltaZ).a;\n"
                         "            \n"
                         "            vec3 grad = vec3(px-nx, py-ny, pz-nz);\n"
                         "            if (grad.x!=0.0 || grad.y!=0.0 || grad.z!=0.0)\n"
                         "            {\n"
                         "                vec3 normal = normalize(grad);\n"
                         "                float lightScale = 0.1 +  abs(dot(normal.xyz, eyeDirection))*0.9;\n"
                         "\n"
                         "                color.x = lightScale;\n"
                         "                color.y = lightScale;\n"
                         "                color.z = lightScale;\n"
                         "            }\n"
                         "\n"
                         "            color.a = 1.0;\n"
                         "\n"
                         "            gl_FragColor = color;\n"
                         "            \n"
                         "            return;\n"
                         "        }\n"
                         "        \n"
                         "        previousColor = color;\n"
                         "        \n"
                         "        texcoord += deltaTexCoord; \n"
                         "\n"
                         "        --num_iterations;\n"
                         "    }\n"
                         "\n"
                         "    // we didn't find an intersection so just discard fragment\n"
                         "    discard;\n"
                         "}\n"
                         "\n";