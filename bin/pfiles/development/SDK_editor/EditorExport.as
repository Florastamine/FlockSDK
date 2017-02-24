void RemoveDebugDrawables(Array<Drawable@>@ drawables)
{
    for (uint i = 0; i < drawables.length;)
    {
        if (drawables[i].node !is null && (drawables[i].node.name == "EditorGizmo" || drawables[i].node.name == "DebugIconsContainer"
            || drawables[i].node.name == "EditorGrid"))
            drawables.Erase(i);
        else
            ++i;
    }
}
