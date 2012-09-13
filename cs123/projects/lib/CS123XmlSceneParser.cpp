/*
 * CS123 New Parser for XML
 */

#include "CS123XmlSceneParser.h"
#include "CS123SceneData.h"

#include <CS123Common.h>
#include <assert.h>
#include <string.h>
#include <string>

using namespace std;

#define ERROR_AT(e) "error at line " << e.lineNumber() << " col " << e.columnNumber() << ": "
#define PARSE_ERROR(e) cout << ERROR_AT(e) << "could not parse <" << e.tagName().toStdString() << ">" << endl
#define UNSUPPORTED_ELEMENT(e) cout << ERROR_AT(e) << "unsupported element <" << e.tagName().toStdString() << ">" << endl;

CS123XmlSceneParser::CS123XmlSceneParser(const std::string& name) 
{
    file_name = name;

    memset(&m_cameraData, 0, sizeof(CS123SceneCameraData));
    memset(&m_globalData, 0, sizeof(CS123SceneGlobalData));
    m_objects.clear();
    m_lights.clear();
    m_nodes.clear();
}

CS123XmlSceneParser::~CS123XmlSceneParser() 
{
    std::vector<CS123SceneLightData*>::iterator lights;
    for (lights = m_lights.begin(); lights != m_lights.end(); lights++) {
        delete *lights;
    }

    // Delete all Scene Nodes
    for (unsigned int node = 0; node < m_nodes.size(); node++) {
        for (size_t i = 0; i < (m_nodes[node])->transformations.size(); i++) {
            delete (m_nodes[node])->transformations[i];
        }
        for (size_t i = 0; i < (m_nodes[node])->primitives.size(); i++) {
            delete (m_nodes[node])->primitives[i]->material.textureMap;
            delete (m_nodes[node])->primitives[i]->material.bumpMap;
            delete (m_nodes[node])->primitives[i];
        }
        (m_nodes[node])->transformations.clear();
        (m_nodes[node])->primitives.clear();
        (m_nodes[node])->children.clear();
        delete m_nodes[node];
    }

    m_nodes.clear();
    m_lights.clear();
    m_objects.clear();
}

bool CS123XmlSceneParser::getGlobalData(CS123SceneGlobalData& data) const
{
    data = m_globalData;
    return true;
}

bool CS123XmlSceneParser::getCameraData(CS123SceneCameraData& data) const
{
    data = m_cameraData;
    return true;
}

int CS123XmlSceneParser::getNumLights() const
{
    return m_lights.size();
}

bool CS123XmlSceneParser::getLightData(int i, CS123SceneLightData& data) const
{
    if (i < 0 || (unsigned int)i >= m_lights.size())
    {
        cout << "invalid light index %d" << endl;
        return false;
    }
    data = *m_lights[i];
    return true;
}

CS123SceneNode* CS123XmlSceneParser::getRootNode() const
{
    map<std::string, CS123SceneNode*>::iterator node = m_objects.find("root");
    if (node == m_objects.end())
        return NULL;
    return m_objects["root"];
}

// This is where it all goes down...
bool CS123XmlSceneParser::parse() 
{
    // Read the file
    QFile file(file_name.c_str());
    if (!file.open(QFile::ReadOnly))
    {
        cout << "could not open " << file_name << endl;
        return false;
    }

    // Load the XML document
    QDomDocument doc;
    QString errorMessage;
    int errorLine, errorColumn;
    if (!doc.setContent(&file, &errorMessage, &errorLine, &errorColumn))
    {
        cout << "parse error at line " << errorLine << " col " << errorColumn << ": " << errorMessage.toStdString() << endl;
        return false;
    }
    file.close();

    // Get the root element
    QDomElement scenefile = doc.documentElement();
    if (scenefile.tagName() != "scenefile")
    {
        cout << "missing <scenefile>" << endl;
        return false;
    }

    // Default camera
    m_cameraData.pos = Vector4(5, 5, 5, 1);
    m_cameraData.up = Vector4(0, 1, 0, 0);
    m_cameraData.look = Vector4(-1, -1, -1, 0);
    m_cameraData.heightAngle = 45;
    m_cameraData.aspectRatio = 1;

    // Default global data
    m_globalData.ka = 0.5f;
    m_globalData.kd = 0.5f;
    m_globalData.ks = 0.5f;

    // Iterate over child elements
    QDomNode childNode = scenefile.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "globaldata")
        {
            if (!parseGlobalData(e))
                return false;
        }
        else if (e.tagName() == "lightdata")
        {
            if (!parseLightData(e))
                return false;
        }
        else if (e.tagName() == "cameradata")
        {
            if (!parseCameraData(e))
                return false;
        }
        else if (e.tagName() == "object")
        {
            if (!parseObjectData(e))
                return false;
        }
        else if (!e.isNull())
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    cout << "finished parsing " << file_name << endl;
    return true;
}

/**
 * Helper function to parse a single value, the name of which is stored in
 * name.  For example, to parse <length v="0"/>, name would need to be "v".
 */
bool parseInt(const QDomElement &single, int &a, const char *name)
{
    if (!single.hasAttribute(name))
        return false;
    a = single.attribute(name).toInt();
    return true;
}

/**
 * Helper function to parse a single value, the name of which is stored in
 * name.  For example, to parse <length v="0"/>, name would need to be "v".
 */
template <typename T> bool parseSingle(const QDomElement &single, T &a, const QString &str)
{
    if (!single.hasAttribute(str))
        return false;
    a = single.attribute(str).toDouble();
    return true;
}

/**
 * Helper function to parse a triple.  Each attribute is assumed to take a
 * letter, which are stored in chars in order.  For example, to parse
 * <pos x="0" y="0" z="0"/>, chars would need to be "xyz".
 */
template <typename T> bool parseTriple(const QDomElement &triple, T &a, T &b, T &c, const QString &str_a, const QString &str_b, const QString &str_c)
{
    if (!triple.hasAttribute(str_a) ||
        !triple.hasAttribute(str_b) ||
        !triple.hasAttribute(str_c))
        return false;
    a = triple.attribute(str_a).toDouble();
    b = triple.attribute(str_b).toDouble();
    c = triple.attribute(str_c).toDouble();
    return true;
}

/**
 * Helper function to parse a quadruple.  Each attribute is assumed to take a
 * letter, which are stored in chars in order.  For example, to parse
 * <color r="0" g="0" b="0" a="0"/>, chars would need to be "rgba".
 */
template <typename T> bool parseQuadruple(const QDomElement &quadruple, T &a, T &b, T &c, T &d, const QString &str_a, const QString &str_b, const QString &str_c, const QString &str_d)
{
    if (!quadruple.hasAttribute(str_a) ||
        !quadruple.hasAttribute(str_b) ||
        !quadruple.hasAttribute(str_c) ||
        !quadruple.hasAttribute(str_d))
        return false;
    a = quadruple.attribute(str_a).toDouble();
    b = quadruple.attribute(str_b).toDouble();
    c = quadruple.attribute(str_c).toDouble();
    d = quadruple.attribute(str_d).toDouble();
    return true;
}

/**
 * Helper function to parse a matrix.  Example matrix:
 *
 * <matrix>
 *   <row a="1" b="0" c="0" d="0"/>
 *   <row a="0" b="1" c="0" d="0"/>
 *   <row a="0" b="0" c="1" d="0"/>
 *   <row a="0" b="0" c="0" d="1"/>
 * </matrix>
 */
bool parseMatrix(const QDomElement &matrix, double m[16])
{
    QDomNode childNode = matrix.firstChild();
    int row = 0;

    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.isElement())
        {
            float a, b, c, d;
            if (!parseQuadruple(e, a, b, c, d, "a", "b", "c", "d") && !parseQuadruple(e, a, b, c, d, "v1", "v2", "v3", "v4"))
            {
                PARSE_ERROR(e);
                return false;
            }
            m[row*4 + 0] = a;
            m[row*4 + 1] = b;
            m[row*4 + 2] = c;
            m[row*4 + 3] = d;
            if (++row == 4) break;
        }
        childNode = childNode.nextSibling();
    }

    return (row == 4);
}

/**
 * Helper function to parse a color.  Will parse an element with r, g, b, and
 * a attributes (the a attribute is optional and defaults to 1).
 */
bool parseColor(const QDomElement &color, CS123SceneColor& c)
{
    c.a = 1;
    return
            parseQuadruple(color, c.r, c.g, c.b, c.a, "r", "g", "b", "a") ||
            parseQuadruple(color, c.r, c.g, c.b, c.a, "x", "y", "z", "w") ||
            parseTriple(color, c.r, c.g, c.b, "r", "g", "b") ||
            parseTriple(color, c.r, c.g, c.b, "x", "y", "z");
}

/**
 * Helper function to parse a texture map tag.  Example texture map tag:
 * <texture file="/course/cs123/data/image/andyVanDam.jpg" u="1" v="1"/>
 */
bool parseMap(const QDomElement &e, CS123SceneFileMap* map)
{
    if (!e.hasAttribute("file"))
        return false;
    map->filename = e.attribute("file").toStdString();
    map->repeatU = e.hasAttribute("u") ? e.attribute("u").toFloat() : 1;
    map->repeatV = e.hasAttribute("v") ? e.attribute("v").toFloat() : 1;
    map->isUsed = true;
    return true;
}

/**
 * Parse a <globaldata> tag and fill in m_globalData.
 */
bool CS123XmlSceneParser::parseGlobalData(const QDomElement &globaldata)
{
    // Iterate over child elements
    QDomNode childNode = globaldata.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "ambientcoeff")
        {
            if (!parseSingle(e, m_globalData.ka, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "diffusecoeff")
        {
            if (!parseSingle(e, m_globalData.kd, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "specularcoeff")
        {
            if (!parseSingle(e, m_globalData.ks, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "transparentcoeff")
        {
            if (!parseSingle(e, m_globalData.kt, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        childNode = childNode.nextSibling();
    }

    return true;
}

/**
 * Parse a <lightdata> tag and add a new CS123SceneLightData to m_lights.
 */
bool CS123XmlSceneParser::parseLightData(const QDomElement &lightdata)
{
    // Create a default light
    CS123SceneLightData* light = new CS123SceneLightData();
    m_lights.push_back(light);
    memset(light, 0, sizeof(CS123SceneLightData));
    light->pos = Vector4(3, 3, 3, 0);
    light->dir = Vector4(0, 0, 0, 0);
    light->color.r = light->color.g = light->color.b = 1;
    light->function = Vector3(1, 0, 0);

    // Iterate over child elements
    QDomNode childNode = lightdata.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "id")
        {
            if (!parseInt(e, light->id, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "type")
        {
            if (!e.hasAttribute("v"))
            {
                PARSE_ERROR(e);
                return false;
            }
            if (e.attribute("v") == "directional") light->type = LIGHT_DIRECTIONAL;
            else if (e.attribute("v") == "point") light->type = LIGHT_POINT;
            else if (e.attribute("v") == "spot") light->type = LIGHT_SPOT;
            else if (e.attribute("v") == "area") light->type = LIGHT_AREA;
            else
            {
                cout << ERROR_AT(e) << "unknown light type " << e.attribute("v").toStdString() << endl;
                return false;
            }
        }
        else if (e.tagName() == "color")
        {
            if (!parseColor(e, light->color))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "function")
        {
            if (!parseTriple(e, light->function.x, light->function.y, light->function.z, "a", "b", "c") &&
                !parseTriple(e, light->function.x, light->function.y, light->function.z, "x", "y", "z") &&
                !parseTriple(e, light->function.x, light->function.y, light->function.z, "v1", "v2", "v3"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "position")
        {
            if (light->type == LIGHT_DIRECTIONAL)
            {
                cout << ERROR_AT(e) << "position is not applicable to directional lights" << endl;
                return false;
            }
            if (!parseTriple(e, light->pos.x, light->pos.y, light->pos.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            light->pos.w = 1;
        }
        else if (e.tagName() == "direction")
        {
            if (light->type == LIGHT_POINT)
            {
                cout << ERROR_AT(e) << "direction is not applicable to point lights" << endl;
                return false;
            }
            if (!parseTriple(e, light->dir.x, light->dir.y, light->dir.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            light->dir.w = 0;
        }
        else if (e.tagName() == "radius")
        {
            if (light->type != LIGHT_SPOT)
            {
                cout << ERROR_AT(e) << "radius is only applicable to spot lights" << endl;
                return false;
            }
            if (!parseSingle(e, light->radius, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "penumbra")
        {
            if (light->type != LIGHT_SPOT)
            {
                cout << ERROR_AT(e) << "penumbra is only applicable to spot lights" << endl;
                return false;
            }
            if (!parseSingle(e, light->penumbra, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "angle")
        {
            if (light->type != LIGHT_SPOT)
            {
                cout << ERROR_AT(e) << "angle is only applicable to spot lights" << endl;
                return false;
            }
            if (!parseSingle(e, light->angle, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "width")
        {
            if (light->type != LIGHT_AREA)
            {
                cout << ERROR_AT(e) << "width is only applicable to area lights" << endl;
                return false;
            }
            if (!parseSingle(e, light->width, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "height")
        {
            if (light->type != LIGHT_AREA)
            {
                cout << ERROR_AT(e) << "height is only applicable to area lights" << endl;
                return false;
            }
            if (!parseSingle(e, light->height, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (!e.isNull())
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    return true;
}

/**
 * Parse a <cameradata> tag and fill in m_cameraData.
 */
bool CS123XmlSceneParser::parseCameraData(const QDomElement &cameradata)
{
    bool focusFound = false;
    bool lookFound = false;

    // Iterate over child elements
    QDomNode childNode = cameradata.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "pos")
        {
            if (!parseTriple(e, m_cameraData.pos.x, m_cameraData.pos.y, m_cameraData.pos.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            m_cameraData.pos.w = 1;
        }
        else if (e.tagName() == "look" || e.tagName() == "focus")
        {
            if (!parseTriple(e, m_cameraData.look.x, m_cameraData.look.y, m_cameraData.look.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }

            if (e.tagName() == "focus")
            {
                // Store the focus point in the look vector (we will later subtract
                // the camera position from this to get the actual look vector)
                m_cameraData.look.w = 1;
                focusFound = true;
            }
            else
            {
                // Just store the look vector
                m_cameraData.look.w = 0;
                lookFound = true;
            }
        }
        else if (e.tagName() == "up")
        {
            if (!parseTriple(e, m_cameraData.up.x, m_cameraData.up.y, m_cameraData.up.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            m_cameraData.up.w = 0;
        }
        else if (e.tagName() == "heightangle")
        {
            if (!parseSingle(e, m_cameraData.heightAngle, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "aspectratio")
        {
            if (!parseSingle(e, m_cameraData.aspectRatio, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "aperture")
        {
            if (!parseSingle(e, m_cameraData.aperture, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "focallength")
        {
            if (!parseSingle(e, m_cameraData.focalLength, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (!e.isNull())
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    if (focusFound && lookFound)
    {
        cout << ERROR_AT(cameradata) << "camera can not have both look and focus" << endl;
        return false;
    }

    if (focusFound)
    {
        // Convert the focus point (stored in the look vector) into a
        // look vector from the camera position to that focus point.
        m_cameraData.look -= m_cameraData.pos;
    }

    return true;
}

/**
 * Parse an <object> tag and create a new CS123SceneNode in m_nodes.
 */
bool CS123XmlSceneParser::parseObjectData(const QDomElement &object)
{
    if (!object.hasAttribute("name"))
    {
        PARSE_ERROR(object);
        return false;
    }

    if (object.attribute("type") != "tree")
    {
        cout << "top-level <object> elements must be of type tree" << endl;
        return false;
    }

    string name = object.attribute("name").toStdString();

    // Check that this object does not exist
    if (m_objects[name] != NULL)
    {
        cout << ERROR_AT(object) << "two objects with the same name: " << name << endl;
        return false;
    }

    // Create the object and add to the map
    CS123SceneNode *node = new CS123SceneNode;
    m_nodes.push_back(node);
    m_objects[name] = node;

    // Iterate over child elements
    QDomNode childNode = object.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "transblock")
        {
            CS123SceneNode *child = new CS123SceneNode;
            m_nodes.push_back(child);
            if (!parseTransBlock(e, child))
            {
                PARSE_ERROR(e);
                return false;
            }
            node->children.push_back(child);
        }
        else if (!e.isNull())
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    return true;
}

/**
 * Parse a <transblock> tag into node, which consists of any number of
 * <translate>, <rotate>, <scale>, or <matrix> elements followed by one
 * <object> element.  That <object> element is either a master reference,
 * a subtree, or a primitive.  If it's a master reference, we handle it
 * here, otherwise we will call other methods.  Example <transblock>:
 *
 * <transblock>
 *   <translate x="1" y="2" z="3"/>
 *   <rotate x="0" y="1" z="0" a="90"/>
 *   <scale x="1" y="2" z="1"/>
 *   <object type="primitive" name="sphere"/>
 * </transblock>
 */
bool CS123XmlSceneParser::parseTransBlock(const QDomElement &transblock, CS123SceneNode* node)
{
    // Iterate over child elements
    QDomNode childNode = transblock.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "translate")
        {
            CS123SceneTransformation *t = new CS123SceneTransformation();
            node->transformations.push_back(t);
            t->type = TRANSFORMATION_TRANSLATE;

            if (!parseTriple(e, t->translate.x, t->translate.y, t->translate.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            t->translate.w = 0;
        }
        else if (e.tagName() == "rotate")
        {
            CS123SceneTransformation *t = new CS123SceneTransformation();
            node->transformations.push_back(t);
            t->type = TRANSFORMATION_ROTATE;

            REAL angle;
            if (!parseQuadruple(e, t->rotate.x, t->rotate.y, t->rotate.z, angle, "x", "y", "z", "angle"))
            {
                PARSE_ERROR(e);
                return false;
            }
            t->rotate.w = 0;

            // Convert to radians
            t->angle = angle * M_PI / 180;
        }
        else if (e.tagName() == "scale")
        {
            CS123SceneTransformation *t = new CS123SceneTransformation();
            node->transformations.push_back(t);
            t->type = TRANSFORMATION_SCALE;

            if (!parseTriple(e, t->scale.x, t->scale.y, t->scale.z, "x", "y", "z"))
            {
                PARSE_ERROR(e);
                return false;
            }
            t->scale.w = 0;
        }
        else if (e.tagName() == "matrix")
        {
            CS123SceneTransformation* t = new CS123SceneTransformation();
            node->transformations.push_back(t);
            t->type = TRANSFORMATION_MATRIX;

            if (!parseMatrix(e, t->matrix.data))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "object")
        {
            if (e.attribute("type") == "master")
            {
                string masterName = e.attribute("name").toStdString();
                if (!m_objects[masterName])
                {
                    cout << ERROR_AT(e) << "invalid master object reference: " << masterName << endl;
                    return false;
                }
                node->children.push_back(m_objects[masterName]);
            }
            else if (e.attribute("type") == "tree")
            {
                QDomNode subNode = e.firstChild();
                while (!subNode.isNull())
                {
                    QDomElement e = subNode.toElement();
                    if (e.tagName() == "transblock")
                    {
                        CS123SceneNode* n = new CS123SceneNode;
                        m_nodes.push_back(n);
                        node->children.push_back(n);
                        if (!parseTransBlock(e, n))
                        {
                            PARSE_ERROR(e);
                            return false;
                        }
                    }
                    else if (!e.isNull())
                    {
                        UNSUPPORTED_ELEMENT(e);
                        return false;
                    }
                    subNode = subNode.nextSibling();
                }
            }
            else if (e.attribute("type") == "primitive")
            {
                if (!parsePrimitive(e, node))
                {
                    PARSE_ERROR(e);
                    return false;
                }
            }
            else
            {
                cout << ERROR_AT(e) << "invalid object type: " << e.attribute("type").toStdString() << endl;
                return false;
            }
        }
        else if (!e.isNull())
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    return true;
}

/**
 * Parse an <object type="primitive"> tag into node.
 */
bool CS123XmlSceneParser::parsePrimitive(const QDomElement &prim, CS123SceneNode* node)
{
    // Default primitive
    CS123ScenePrimitive* primitive = new CS123ScenePrimitive();
    CS123SceneMaterial& mat = primitive->material;
    memset(&mat, 0, sizeof(CS123SceneMaterial));
    mat.textureMap = new CS123SceneFileMap;
    mat.bumpMap = new CS123SceneFileMap;
    primitive->type = PRIMITIVE_CUBE;
    mat.textureMap->isUsed = false;
    mat.bumpMap->isUsed = false;
    mat.cDiffuse.r = mat.cDiffuse.g = mat.cDiffuse.b = 1;
    node->primitives.push_back(primitive);

    // Parse primitive type
    string primType = prim.attribute("name").toStdString();
    if (primType == "sphere") primitive->type = PRIMITIVE_SPHERE;
    else if (primType == "cube") primitive->type = PRIMITIVE_CUBE;
    else if (primType == "cylinder") primitive->type = PRIMITIVE_CYLINDER;
    else if (primType == "cone") primitive->type = PRIMITIVE_CONE;
    else if (primType == "torus") primitive->type = PRIMITIVE_TORUS;
    else if (primType == "mesh")
    {
        primitive->type = PRIMITIVE_MESH;
        if (prim.hasAttribute("meshfile"))
        {
            primitive->meshfile = prim.attribute("meshfile").toStdString();
        }
        else if (prim.hasAttribute("filename"))
        {
            primitive->meshfile = prim.attribute("filename").toStdString();
        }
        else
        {
            cout << "mesh object must specify filename" << endl;
            return false;
        }
    }

    // Iterate over child elements
    QDomNode childNode = prim.firstChild();
    while (!childNode.isNull())
    {
        QDomElement e = childNode.toElement();
        if (e.tagName() == "diffuse")
        {
            if (!parseColor(e, mat.cDiffuse))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "ambient")
        {
            if (!parseColor(e, mat.cAmbient))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "reflective")
        {
            if (!parseColor(e, mat.cReflective))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "specular")
        {
            if (!parseColor(e, mat.cSpecular))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "emissive")
        {
            if (!parseColor(e, mat.cEmissive))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "transparent")
        {
            if (!parseColor(e, mat.cTransparent))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "shininess")
        {
            if (!parseSingle(e, mat.shininess, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "ior")
        {
            if (!parseSingle(e, mat.ior, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "texture")
        {
            if (!parseMap(e, mat.textureMap))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "bumpmap")
        {
            if (!parseMap(e, mat.bumpMap))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else if (e.tagName() == "blend")
        {
            if (!parseSingle(e, mat.blend, "v"))
            {
                PARSE_ERROR(e);
                return false;
            }
        }
        else
        {
            UNSUPPORTED_ELEMENT(e);
            return false;
        }
        childNode = childNode.nextSibling();
    }

    return true;
}

