osg::StateSet* createStateSet()
{
    osg::ref_ptr< osg::StateSet > ss = new osg::StateSet;

    // Create a vertex program that references the gl_InstanceID to
    // render each instance uniquely. gl_InstanceID will be in the range
    // 0 to numInstances-1 (1023 in our case).
    std::string vertexSource =
        "uniform sampler2D osgLogo; \n"
        "uniform float osg_SimulationTime; \n"

        "void main() \n"
        "{ \n"
            // Using the instance ID, generate "texture coords" for this instance.
            "vec2 tC; \n"
            "float r = float(gl_InstanceID) / 32.; \n"
            "tC.s = fract( r ); tC.t = floor( r ) / 32.; \n"
            // Get the color from the OSG logo.
            "gl_FrontColor = texture2D( osgLogo, tC ); \n"

            // Use the (scaled) tex coord to translate the position of the vertices.
            "vec4 pos = vec4( tC.s * 48., 0., tC.t * 48., 1. ); \n"

            // Compute a rotation angle from the instanceID and elapsed time.
            "float timeOffset = gl_InstanceID / (32. * 32.); \n"
            "float angle = ( osg_SimulationTime - timeOffset ) * 6.283; \n"
            "float sa = sin( angle ); \n"
            "float ca = cos( angle ); \n"
            // New orientation, rotate around z axis.
            "vec4 newX = vec4( ca, sa, 0., 0. ); \n"
            "vec4 newY = vec4( sa, ca, 0., 0. ); \n"
            "vec4 newZ = vec4( 0., 0., 1., 0. ); \n"
            "mat4 mV = mat4( newX, newY, newZ, pos ); \n"
            "gl_Position = ( gl_ModelViewProjectionMatrix * mV * gl_Vertex ); \n"
        "} \n";

    osg::ref_ptr< osg::Shader > vertexShader = new osg::Shader();
    vertexShader->setType( osg::Shader::VERTEX );
    vertexShader->setShaderSource( vertexSource );

    osg::ref_ptr< osg::Program > program = new osg::Program();
    program->addShader( vertexShader.get() );

    ss->setAttribute( program.get(),
        osg::StateAttribute::ON | osg::StateAttribute::PROTECTED );

    osg::ref_ptr< osg::Image> iLogo = osgDB::readImageFile( "Images/osg128.png" );
    if( !iLogo.valid() )
    {
        osg::notify( osg::ALWAYS ) << "Can't open image file osg128.png" << std::endl;
        return( NULL );
    }
    osg::Texture2D* texLogo = new osg::Texture2D( iLogo.get() );
    texLogo->setFilter( osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR );
    texLogo->setFilter( osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR );

    ss->setTextureAttribute( 0, texLogo );

    osg::ref_ptr< osg::Uniform > texLogoUniform =
        new osg::Uniform( "osgLogo", 0 );
    ss->addUniform( texLogoUniform.get() );

    return( ss.release() );
}