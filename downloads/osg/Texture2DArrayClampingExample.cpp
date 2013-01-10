#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osg/Texture2DArray>
#include <osgDB/ReadFile>
#include <string>

using namespace osg;

void applyShader(Geode* geode)
{
	ref_ptr<Program> program = new Program;
	program->setName("TexturesTest");
	ref_ptr<Shader> vertObj = new Shader(Shader::VERTEX);
	ref_ptr<Shader> fragObj = new Shader(Shader::FRAGMENT);

	program->addShader(vertObj.get());
	program->addShader(fragObj.get());

	ref_ptr<StateSet> stateSet = new osg::StateSet;
	ref_ptr<Uniform> texturesUniform = new Uniform(Uniform::SAMPLER_2D_ARRAY, "textures", 0);
	stateSet->addUniform(texturesUniform.get());
	stateSet->setAttributeAndModes(program.get(), StateAttribute::ON);

	ref_ptr<Texture2DArray> textureArray = new Texture2DArray;
	textureArray->setFilter(osg::Texture2DArray::MIN_FILTER, osg::Texture2DArray::NEAREST);
    textureArray->setFilter(osg::Texture2DArray::MAG_FILTER, osg::Texture2DArray::NEAREST);
	textureArray->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
	textureArray->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
	textureArray->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_BORDER);



	textureArray->setTextureDepth(2);
	Image* image = osgDB::readImageFile("src/foo2.jpg");
	textureArray->setImage(0, image);
	textureArray->setImage(1, image);
	stateSet->setTextureAttribute(0, textureArray.get(), osg::StateAttribute::ON);

	std::string vertSource =
		"void main()"
		"{"
			"gl_TexCoord[0].st = gl_MultiTexCoord0.st;"
			"gl_Position = ftransform();"
		"}";
	vertObj->setShaderSource(vertSource);

	std::string fragSource =
		"#version 120\n"
		"#extension GL_EXT_gpu_shader4 : enable\n"
		""
		"uniform sampler2DArray textures;"
		""
		"void main()"
		"{"
			"gl_FragColor = texture2DArray(textures, vec3(gl_TexCoord[0].xy*vec2(1.5,1.5), 0));"
		"}";
	fragObj->setShaderSource(fragSource);

	geode->setStateSet(stateSet.get());
}

int main(int argc, char** argv)
{
	setNotifyLevel(osg::DEBUG_INFO);

	// Create a cube
	ref_ptr<Group> rootNode = new Group;
	Box* cube = new Box(Vec3(0,0,0), 1.0f);
	ShapeDrawable* drawable = new ShapeDrawable(cube);
	Geode* geode = new Geode();
	geode->addDrawable(drawable);
	rootNode->addChild(geode);

	// Apply our shader to this cube
	applyShader(geode);

	osgViewer::Viewer viewer;
	viewer.setSceneData(rootNode.get());
	return viewer.run();
}

