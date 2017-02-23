// Urho3D editor import functions

String importOptions = "-t";

class ParentAssignment
{
    uint childID;
    String parentName;
}

class AssetMapping
{
    String assetName;
    String fullAssetName;
}

Array<AssetMapping> assetMappings;

String assetImporterPath;

int ExecuteAssetImporter(Array<String>@ args)
{
    if (assetImporterPath.empty)
    {
        String exeSuffix = "";
        if (GetPlatform() == "Windows")
            exeSuffix = ".exe";
        // Try both with and without the tool directory; a packaged build may not have the tool directory
        assetImporterPath = fileSystem.programDir + "tool/bin/AssetImporter" + exeSuffix;
        if (!fileSystem.FileExists(assetImporterPath))
            assetImporterPath = fileSystem.programDir + "AssetImporter" + exeSuffix;
    }

    return fileSystem.SystemRun(assetImporterPath, args);
}

void ImportAnimation(const String&in fileName)
{
  if (fileName.empty)
      return;

  ui.cursor.shape = CS_BUSY;

  String modelName = "objects/" + GetFileName(fileName) + ".ani";
  String outFileName = sceneResourcePath + modelName;
  fileSystem.CreateDir(sceneResourcePath + "objects");

  Array<String> args;
  args.Push("anim");
  args.Push("\"" + fileName + "\"");
  args.Push("\"" + outFileName + "\"");
  args.Push("-p \"" + sceneResourcePath + "\"");
  Array<String> options = importOptions.Trimmed().Split(' ');
  for (uint i = 0; i < options.length; ++i)
      args.Push(options[i]);

  if (ExecuteAssetImporter(args) == 0)
  {

  }
  else
      log.Error("Failed to execute AssetImporter to import model");
}

void ImportModel(const String&in fileName)
{
    if (fileName.empty)
        return;

    ui.cursor.shape = CS_BUSY;

    String modelName = "objects/" + GetFileName(fileName) + ".mdl";
    String outFileName = sceneResourcePath + modelName;
    fileSystem.CreateDir(sceneResourcePath + "objects");

    Array<String> args;
    args.Push("model");
    args.Push("\"" + fileName + "\"");
    args.Push("\"" + outFileName + "\"");
    args.Push("-p \"" + sceneResourcePath + "\"");
    Array<String> options = importOptions.Trimmed().Split(' ');
    for (uint i = 0; i < options.length; ++i)
        args.Push(options[i]);
    // If material lists are to be applied, make sure the option to create them exists
    if (applyMaterialList)
        args.Push("-l");

    if (ExecuteAssetImporter(args) == 0)
    {
        Node@ newNode = editorScene.CreateChild(GetFileName(fileName));
        StaticModel@ newModel = newNode.CreateComponent("StaticModel");
        newNode.position = GetNewNodePosition();
        newModel.model = cache.GetResource("Model", modelName);
        newModel.ApplyMaterialList(); // Setup default materials if possible

        // Create an undo action for the create
        CreateNodeAction action;
        action.Define(newNode);
        SaveEditAction(action);
        SetSceneModified();

        FocusNode(newNode);
    }
    else
        log.Error("Failed to execute AssetImporter to import model");
}

void ImportScene(const String&in fileName)
{
    if (fileName.empty)
        return;

    ui.cursor.shape = CS_BUSY;

    // Export scene to a temp file, then load and delete it if successful
    Array<String> options = importOptions.Trimmed().Split(' ');
    bool isBinary = false;
    for (uint i = 0; i < options.length; ++i)
        if (options[i] == "-b")
            isBinary = true;
    String tempSceneName = sceneResourcePath + (isBinary ? TEMP_BINARY_SCENE_NAME : TEMP_SCENE_NAME);

    Array<String> args;
    args.Push("scene");
    args.Push("\"" + fileName + "\"");
    args.Push("\"" + tempSceneName + "\"");
    args.Push("-p \"" + sceneResourcePath + "\"");
    for (uint i = 0; i < options.length; ++i)
        args.Push(options[i]);
    if (applyMaterialList)
        args.Push("-l");

    if (ExecuteAssetImporter(args) == 0)
    {
        skipMruScene = true; // set to avoid adding tempscene to mru
        LoadScene(tempSceneName);
        fileSystem.Delete(tempSceneName);
        UpdateWindowTitle();
    }
    else
        log.Error("Failed to execute AssetImporter to import scene");
}

String GetFullAssetName(const String& assetName)
{
    for (uint i = 0; i < assetMappings.length; ++i)
    {
        if (assetMappings[i].assetName == assetName)
            return assetMappings[i].fullAssetName;
    }

    return assetName;
}

Quaternion GetTransformQuaternion(Vector3 rotEuler)
{
    // Convert rotation to lefthanded
    Quaternion rotateX(-rotEuler.x, Vector3(1, 0, 0));
    Quaternion rotateY(-rotEuler.y, Vector3(0, 1, 0));
    Quaternion rotateZ(-rotEuler.z, Vector3(0, 0, -1));
    return rotateZ * rotateY * rotateX;
}

String GetComponentAttribute(XMLElement compElem, const String&in name)
{
    XMLElement attrElem = compElem.GetChild("attribute");
    while (!attrElem.isNull)
    {
        if (attrElem.GetAttribute("name") == name)
            return attrElem.GetAttribute("value");

        attrElem = attrElem.GetNext("attribute");
    }

    return "";
}

Vector3 GetVector3FromStrings(Array<String>@ coords, uint startIndex)
{
    return Vector3(coords[startIndex].ToFloat(), coords[startIndex + 1].ToFloat(), coords[startIndex + 2].ToFloat());
}

void ProcessRef(String& ref)
{
    if (ref.StartsWith("local://"))
        ref = ref.Substring(8);
    if (ref.StartsWith("file://"))
        ref = ref.Substring(7);
}

String GetOutModelName(const String&in ref)
{
    return "objects/" + GetFullAssetName(ref).Replaced('/', '_').Replaced(".mesh", ".mdl");
}

String GetOutMaterialName(const String&in ref)
{
    return "materials/" + GetFullAssetName(ref).Replaced('/', '_').Replaced(".material", ".xml");
}

String GetOutTextureName(const String&in ref)
{
    return "textures/" + GetFullAssetName(ref).Replaced('/', '_');
}

void ConvertModel(const String&in modelName, const String&in filePath, Array<String>@ convertedModels)
{
    if (modelName.Trimmed().empty)
        return;

    for (uint i = 0; i < convertedModels.length; ++i)
    {
        if (convertedModels[i] == modelName)
            return;
    }

    String meshFileName = filePath + GetFullAssetName(modelName);
    String xmlFileName = filePath + GetFullAssetName(modelName) + ".xml";
    String outFileName = sceneResourcePath + GetOutModelName(modelName);

    // Convert .mesh to .mesh.xml
    String cmdLine = "ogrexmlconverter \"" + meshFileName + "\" \"" + xmlFileName + "\"";
    if (!fileSystem.FileExists(xmlFileName))
        fileSystem.SystemCommand(cmdLine.Replaced('/', '\\'));

    if (!fileSystem.FileExists(outFileName))
    {
        // Convert .mesh.xml to .mdl
        Array<String> args;
        args.Push("\"" + xmlFileName + "\"");
        args.Push("\"" + outFileName + "\"");
        args.Push("-a");
        fileSystem.SystemRun(fileSystem.programDir + "tool/OgreImporter", args);
    }

    convertedModels.Push(modelName);
}

void ConvertMaterial(const String&in materialName, const String&in filePath, Array<String>@ convertedMaterials)
{
    if (materialName.Trimmed().empty)
        return;

    for (uint i = 0; i < convertedMaterials.length; ++i)
    {
        if (convertedMaterials[i] == materialName)
            return;
    }

    String fileName = filePath + GetFullAssetName(materialName);
    String outFileName = sceneResourcePath + GetOutMaterialName(materialName);

    if (!fileSystem.FileExists(fileName))
        return;

    bool mask = false;
    bool twoSided = false;
    bool uvScaleSet = false;
    String textureName;
    Vector2 uvScale(1, 1);
    Color diffuse(1, 1, 1, 1);

    File file(fileName, FILE_READ);
    while (!file.eof)
    {
        String line = file.ReadLine().Trimmed();
        if (line.StartsWith("alpha_rejection") || line.StartsWith("scene_blend alpha_blend"))
            mask = true;
        if (line.StartsWith("cull_hardware none"))
            twoSided = true;
        // Todo: handle multiple textures per material
        if (textureName.empty && line.StartsWith("texture "))
        {
            textureName = line.Substring(8);
            ProcessRef(textureName);
        }
        if (!uvScaleSet && line.StartsWith("scale "))
        {
            uvScale = line.Substring(6).ToVector2();
            uvScaleSet = true;
        }
        if (line.StartsWith("diffuse "))
            diffuse = line.Substring(8).ToColor();
    }

    XMLFile outMat;
    XMLElement rootElem = outMat.CreateRoot("material");
    XMLElement techniqueElem = rootElem.CreateChild("technique");

    if (twoSided)
    {
        XMLElement cullElem = rootElem.CreateChild("cull");
        cullElem.SetAttribute("value", "none");
        XMLElement shadowCullElem = rootElem.CreateChild("shadowcull");
        shadowCullElem.SetAttribute("value", "none");
    }

    if (!textureName.empty)
    {
        techniqueElem.SetAttribute("name", mask ? "shaders/techniques/DiffAlphaMask.xml" : "shaders/techniques/Diff.xml");

        String outTextureName = GetOutTextureName(textureName);
        XMLElement textureElem = rootElem.CreateChild("texture");
        textureElem.SetAttribute("unit", "diffuse");
        textureElem.SetAttribute("name", outTextureName);

        fileSystem.Copy(filePath + GetFullAssetName(textureName), sceneResourcePath + outTextureName);
    }
    else
        techniqueElem.SetAttribute("name", "NoTexture.xml");

    if (uvScale != Vector2(1, 1))
    {
        XMLElement uScaleElem = rootElem.CreateChild("parameter");
        uScaleElem.SetAttribute("name", "UOffset");
        uScaleElem.SetVector3("value", Vector3(1 / uvScale.x, 0, 0));

        XMLElement vScaleElem = rootElem.CreateChild("parameter");
        vScaleElem.SetAttribute("name", "VOffset");
        vScaleElem.SetVector3("value", Vector3(0, 1 / uvScale.y, 0));
    }

    if (diffuse != Color(1, 1, 1, 1))
    {
        XMLElement diffuseElem = rootElem.CreateChild("parameter");
        diffuseElem.SetAttribute("name", "MatDiffColor");
        diffuseElem.SetColor("value", diffuse);
    }

    File outFile(outFileName, FILE_WRITE);
    outMat.Save(outFile);
    outFile.Close();

    convertedMaterials.Push(materialName);
}
